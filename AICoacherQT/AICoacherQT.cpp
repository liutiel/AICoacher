#include "AICoacherQT.h"

AICoacherQT::AICoacherQT(QWidget *parent)
    : QMainWindow(parent)
{
    this->ui.setupUi(this);

    // Style
    this->ui.video_original->setStyleSheet("QLabel{background-color:rgb(0,0,0);}");
    this->ui.video_processed->setStyleSheet("QLabel{background-color:rgb(0,0,0);}");

    // Connecting singals and slots
    connect(this, SIGNAL(signalUIChangeCourseEnded()), this, SLOT(slotUIChangeCourseEnded()));

    // Obtaining Courses
    if (this->obtainCourseList() > 0)
    {
        std::vector<std::string>::iterator iter_name = this->course_name_list.begin();
        while (iter_name != this->course_name_list.end())
        {
            this->ui.comboBox_class->addItem(QString::fromStdString(*iter_name));
            iter_name++;
        }

        this->logWithTime(std::to_string(this->course_name_list.size()) + " course configurations loaded! ");
    }
    else
    {
        this->ui.comboBox_class->setEnabled(false);
    }

    // Count limit and time limit set as invisible by default
    this->ui.label_count_num->setText(QString::fromStdString("0"));
    this->ui.label_time_num->setText(QString::fromStdString("0"));

    this->ui.statusBar->showMessage(tr("Welcome to AICoacher! "));
}

AICoacherQT::~AICoacherQT()
{
    //
}


/**
    Clicking the buttons.
**/
// Start training
void AICoacherQT::pushButtonStartClicked()
{
    // Controller to stop handler threads
    this->is_training = true;

    // Initializing threads for respond to internal events
    this->thread_frame_refreshed = std::thread(&AICoacherQT::frameRefreshed, this);
    this->thread_frame_refreshed.detach();
    this->thread_action_refreshed = std::thread(&AICoacherQT::actionRefreshed, this);
    this->thread_action_refreshed.detach();
    this->thread_course_ended = std::thread(&AICoacherQT::courseEnded, this);
    this->thread_course_ended.detach();
    this->thread_count_refreshed = std::thread(&AICoacherQT::countRefreshed, this);
    this->thread_count_refreshed.detach();
    this->thread_time_refreshed = std::thread(&AICoacherQT::timeRefreshed, this);
    this->thread_time_refreshed.detach();

    if (this->is_file_loaded)
    {
        this->coacher = Coacher(10.0, this->video_file_name, this->course_file_list[this->selected_course_index], 4,
            this->handle_frame_refresh_ptr,
            this->handle_action_refresh_ptr,
            this->handle_course_end_ptr,
            this->handle_count_refresh_ptr,
            this->handle_time_refresh_ptr);
    }
    else if (this->is_camera_loaded)
    {
        this->coacher = Coacher(10.0, this->camera_port, this->course_file_list[this->selected_course_index], 4,
            this->handle_frame_refresh_ptr,
            this->handle_action_refresh_ptr,
            this->handle_course_end_ptr,
            this->handle_count_refresh_ptr,
            this->handle_time_refresh_ptr);
    }
    else
    {
        return;
    }

    this->ui.pushButton_start->setEnabled(false);
    this->ui.pushButton_suspend->setEnabled(true);
    this->ui.pushButton_stop->setEnabled(true);
    this->ui.comboBox_class->setEnabled(false);

    // Writing the log
    this->logWithTime("Starting the training course: " + this->course_name_list[this->selected_course_index] + ". ");

    this->coacher.startCoacher();
}

// Stop
void AICoacherQT::pushButtonEndClicked()
{
    this->coacher.stopCoacher();
}

// Suspend
void AICoacherQT::pushButtonSuspendClicked()
{
    if (this->coacher.isThreadRunning())
    {
        this->ui.pushButton_suspend->setText("RESUME");
        this->coacher.pauseCoacher();
    }
    else
    {
        this->ui.pushButton_suspend->setText("PAUSE");
        this->coacher.resumeCoacher();
    }

}

// course selection changed
void AICoacherQT::comboBoxCourseChanged()
{
    this->selected_course_index = this->ui.comboBox_class->currentIndex();
}

// Load file
void AICoacherQT::menuFileLoadClicked()
{
    // Choose a video file to open
    std::string video_file_name = "";
    video_file_name = QFileDialog::getOpenFileName(this, "Explore video files", "/", "*.*").toStdString();

    // If video name is not empty
    if (video_file_name != "")
    {
        // Set current video file path of the entire programme
        this->video_file_name = video_file_name;
        this->is_file_loaded = true;
        this->is_camera_loaded = false;

        // Enabling starting a course
        this->ui.pushButton_start->setEnabled(true);

        // Writing the log
        this->logWithTime("Setting the input video file: \"" + this->video_file_name + "\". ");
    }
}

// Select camera
void AICoacherQT::menuCameraLoadClicked()
{
    QString dialog_title = "Set camera port";
    QString dialog_info = QString::fromStdString("Input camera port: ");
    
    // Set the camera port in the dialog
    bool camera_port_obtained = false;
    int temp_camera_port = 0;
    temp_camera_port = QInputDialog::getInt(this, dialog_title, dialog_info, 0, 0, 0x7fffffff, 1, &camera_port_obtained);

    if (camera_port_obtained)
    {
        this->camera_port = temp_camera_port;
        this->is_file_loaded = false;
        this->is_camera_loaded = true;

        // Enabling starting a course
        this->ui.pushButton_start->setEnabled(true);

        // Writing the log
        this->logWithTime("Setting the input camera: " + std::to_string(this->camera_port));
    }
}

// Click "Exit" to close the window
void AICoacherQT::menuExitClicked()
{
    this->close();
}

// The "About" dialog
void AICoacherQT::menuAboutClicked()
{
    QString dialog_title = "About AIcoacher";

    std::stringstream dialog_info_stream;
    dialog_info_stream << "AIcoacher version 1.1" << std::endl;
    dialog_info_stream << "Released by liutiel under MIT protocol. " << std::endl;
    dialog_info_stream << "GitHub: https://github.com/liutiel/AICoacher" << std::endl;
    QString dialog_info = QString::fromStdString(dialog_info_stream.str());

    QMessageBox::about(this, dialog_title, dialog_info);
}


/* Respond to internal events */
void AICoacherQT::frameRefreshed()
{
    do
    {
        this->handle_frame_refresh_ptr->acquire();

        cv::Mat original_frame_cv = this->coacher.getCurrentFrame(Coacher::DISPLAY_ORIGINAL);
        if (!original_frame_cv.empty())
        {
            QPixmap original_frame_qpixmap = this->imageFitQt(original_frame_cv, this->ui.video_original->size());
            this->ui.video_original->setPixmap(original_frame_qpixmap);
        }

        cv::Mat skeleton_frame_cv = this->coacher.getCurrentFrame(Coacher::DISPLAY_SKELETON);
        if (!skeleton_frame_cv.empty())
        {
            QPixmap skeleton_frame_qpixmap = this->imageFitQt(this->coacher.getCurrentFrame(Coacher::DISPLAY_SKELETON), this->ui.video_processed->size());
            this->ui.video_processed->setPixmap(skeleton_frame_qpixmap);
        }

    } while (this->is_training);

    // Clear the images
    this->ui.video_original->clear();
    this->ui.video_processed->clear();
}

void AICoacherQT::actionRefreshed()
{
    do
    {
        this->handle_action_refresh_ptr->acquire();

        this->ui.label_action_name_content->setText(QString::fromStdString(this->coacher.getActionName()));

        this->count_limit = this->coacher.getActionCountLimit();
        this->time_limit = this->coacher.getActionTimeLimit();
        if (this->count_limit > 0)
        {
            this->ui.label_count_num->setText(QString::fromStdString(std::to_string(this->coacher.getActionCount()) + " / " + std::to_string(this->count_limit)));
        }
        else
        {
            this->ui.label_count_num->setText(QString::fromStdString(std::to_string(this->coacher.getActionCount())));
        }

        if (this->coacher.getActionTimeLimit() != 0)
        {
            this->ui.label_time_num->setText(QString::fromStdString(std::to_string(this->coacher.getActionTime()) + " / " + std::to_string(this->count_limit)));
        }
        else
        {
            this->ui.label_time_num->setText(QString::fromStdString(std::to_string(this->coacher.getActionTime())));
        }

    } while (this->is_training);

    this->ui.label_action_name_content->setText(QString::fromStdString("N/A"));
}

void AICoacherQT::courseEnded()
{
    this->handle_course_end_ptr->acquire();

    this->is_training = false;
    this->coacher.~Coacher();

    // Writing the log
    this->logWithTime("Training course \"" + this->course_name_list[this->selected_course_index] + "\" ended. ");

    this->ui.label_action_name_content->setText(QString::fromStdString("N/A"));
    this->ui.label_count_num->setText(QString::fromStdString("0"));
    this->ui.label_time_num->setText(QString::fromStdString("0"));
    this->ui.video_original->clear();
    this->ui.video_processed->clear();

    emit signalUIChangeCourseEnded();

    this->thread_frame_refreshed.~thread();
    this->thread_action_refreshed.~thread();
    this->thread_count_refreshed.~thread();
    this->thread_time_refreshed.~thread();
}

void AICoacherQT::countRefreshed()
{
    do
    {
        this->handle_count_refresh_ptr->acquire();

        if (this->count_limit > 0)
        {
            this->ui.label_count_num->setText(QString::fromStdString(std::to_string(this->coacher.getActionCount()) + " / " + std::to_string(this->count_limit)));
        }
        else
        {
            this->ui.label_count_num->setText(QString::fromStdString(std::to_string(this->coacher.getActionCount())));
        }

        if (this->coacher.getActionPerfection())
        {
            this->ui.statusBar->showMessage(tr("Last count: perfect. "));
        }
        else
        {
            this->ui.statusBar->showMessage(tr("Last count: not perfect. "));
        }
        

    } while (this->is_training);

    this->ui.label_count_num->setText(QString::fromStdString("0"));
}

void AICoacherQT::timeRefreshed()
{
    do
    {
        this->handle_time_refresh_ptr->acquire();

        if (this->time_limit > 0)
        {
            this->ui.label_time_num->setText(QString::fromStdString(std::to_string(this->coacher.getActionTime()) + " / " + std::to_string(this->time_limit)));
        }
        else
        {
            this->ui.label_time_num->setText(QString::fromStdString(std::to_string(this->coacher.getActionTime())));
        }

    } while (this->is_training);

    this->ui.label_time_num->setText(QString::fromStdString("0"));
}

// Obtaining all course schedules
int AICoacherQT::obtainCourseList()
{
    if (!std::filesystem::exists(this->COURSE_FILE_DIR)) 
    {
        return 0;
    }

    // Obtaining every .txt file in the "config file directory"
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(this->COURSE_FILE_DIR)) 
    {

        if (entry.is_regular_file() and entry.path().extension() == ".txt") 
        {

            // Read course configuration from .txt file
            std::ifstream course_config_reader(entry.path().string(), std::ifstream::in);

            // Read the course configuration line by line
            char course_config_line[256];
            
            while (course_config_reader.good())
            {
                course_config_reader.getline(course_config_line, 256);
                std::string course_config_line_string = course_config_line;
                course_config_line_string.erase(0, course_config_line_string.find_first_not_of(" "));

                if (course_config_line_string.length() == 0)
                {
                    continue;
                }

                // Notes starting with '//'
                if (course_config_line_string.substr(0, 2) == "//")
                {
                    continue;
                }

                // Saving course name and course file path
                this->course_file_list.push_back(entry.path().stem().string());
                this->course_name_list.push_back(course_config_line_string);
                break;
            }

        }
    }
    return this->course_file_list.size();
}

// Convert OpenCV images to QT images
QPixmap AICoacherQT::imageFitQt(cv::Mat cv_image, QSize qt_size)
{
    double qt_height = qt_size.height();
    double qt_width = qt_size.width();

    double src_height = cv_image.rows;
    double src_width = cv_image.cols;

    double scale_ratio = min(qt_height / src_height, qt_width / src_width);

    cv::Mat cv_image_resized;
    cv::resize(cv_image, cv_image_resized, cv::Size(0, 0), scale_ratio, scale_ratio, cv::INTER_LINEAR);

    QImage qimage = QImage((const unsigned char*)(cv_image_resized.data), cv_image_resized.cols, cv_image_resized.rows, cv_image_resized.cols * cv_image_resized.channels(), QImage::Format_BGR888);
    QPixmap qpixmap = QPixmap::fromImage(qimage);

    return qpixmap;
}

// Generate logs with system time
int AICoacherQT::logWithTime(std::string log_content)
{
    time_t tt = time(NULL);
    tm* t = localtime(&tt);

    std::string time_string = "[";
    time_string.append(std::to_string(t->tm_hour));
    time_string.append(":");
    if (t->tm_min >= 10)
    {
        time_string.append(std::to_string(t->tm_min));
    }
    else
    {
        time_string.append("0" + std::to_string(t->tm_min));
    }
    time_string.append(":");
    if (t->tm_sec >= 10)
    {
        time_string.append(std::to_string(t->tm_sec));
    }
    else
    {
        time_string.append("0" + std::to_string(t->tm_sec));
    }
    time_string.append("] ");

    // Writing the log
    ui.textBrowser_log->append(QString::fromStdString(time_string + log_content + "\n"));
    
    return 0;
}

// UI adjustment after internal events
void AICoacherQT::slotUIChangeCourseEnded()
{
    this->ui.pushButton_start->setEnabled(true);
    this->ui.pushButton_suspend->setText("PAUSE");
    this->ui.pushButton_suspend->setEnabled(false);
    this->ui.pushButton_stop->setEnabled(false);
    this->ui.comboBox_class->setEnabled(true);
}