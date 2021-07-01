#include "AIcoachDemo.h"

AIcoachDemo::AIcoachDemo(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // Style
    ui.video_original->setStyleSheet("QLabel{background-color:rgb(128,128,128);}");
    ui.video_processed->setStyleSheet("QLabel{background-color:rgb(128,128,128);}");

    // Helper connects the ui and the library
    m_aicoacher_helper = new AIcoacherHelper();

    // Initialize and start a thread for processsing
    m_thread_processing = new QThread;
    m_thread_processing->start();
    m_aicoacher_helper->moveToThread(m_thread_processing);

    // Connect signal and slot
    connect(this, SIGNAL(coach_init_signal(int, int)), m_aicoacher_helper, SLOT(coach_init_slot(int, int)));
    connect(this, SIGNAL(coach_init_signal(int, QString)), m_aicoacher_helper, SLOT(coach_init_slot(int, QString)));
    connect(m_aicoacher_helper, SIGNAL(coach_init_result_signal(bool, int)), this, SLOT(coach_init_result_slot(bool, int)));
    connect(m_aicoacher_helper, SIGNAL(coach_init_result_signal(bool, QString)), this, SLOT(coach_init_result_slot(bool, QString)));
    connect(this, SIGNAL(coach_start_signal()), m_aicoacher_helper, SLOT(coach_start_slot()));
    connect(m_aicoacher_helper, SIGNAL(coach_one_frame_signal()), this, SLOT(coach_one_frame_slot()));
    connect(m_aicoacher_helper, SIGNAL(frame_finished_signal()), this, SLOT(frame_finished_slot()));

    // Status bar
    m_fps_label = new QLabel("FPS: 0\t\t");
    //m_drop_label = new QLabel("Dropped frames: 0\t\t");

    ui.statusBar->addWidget(m_fps_label);
    //ui.statusBar->addWidget(m_drop_label);

    // Default curriculum
    ui.comboBox_class->setCurrentIndex(0);
}

// Start training
void AIcoachDemo::pushButtonStartClicked()
{

    emit coach_init_signal(ui.comboBox_class->currentIndex(), m_file_path);

    m_aicoacher_helper->m_besuspended = false;
    m_aicoacher_helper->m_beended = false;
}

// Webcam
void AIcoachDemo::coach_init_result_slot(bool isSuccessful, int input_stream)
{
    stringstream log_string_stream;

    if (isSuccessful)
    {
        // Curriculum log
        log_string_stream.str("");
        log_string_stream << "Class \"" << ui.comboBox_class->currentText().toStdString() << "\" Started！" << endl;
        ui.textBrowser_log->setText(QString::fromStdString(log_string_stream.str()));

        ui.pushButton_start->setEnabled(false);
     //   ui.pushButton_suspend->setEnabled(true);
        ui.pushButton_stop->setEnabled(true);

        emit coach_start_signal();
    }
    else
    {
        ui.textBrowser_log->append("Camera initialization failed！\n");
        return;
    }
}

// Check whether file loaded successfully
void AIcoachDemo::coach_init_result_slot(bool isSuccessful, QString input_stream)
{
    // Initialize the log stream
    stringstream log_string_stream;

    if (isSuccessful)
    {
        log_string_stream.str("");
        log_string_stream
            << "Training started! " << endl
            << "Current Class: \"" << ui.comboBox_class->currentText().toStdString() << "\"，" << endl
            << "Current Video: \"" << m_file_path.toStdString() << "\"" << endl << endl;

 //       log_string_stream << "current frame count: "<< m_aicoacher_helper->GetAICoacherPr()->m_videoIO.GetFrameCount() << endl;

        ui.textBrowser_log->append(QString::fromStdString(log_string_stream.str()));
        
        
        // Log of the curriculum
        log_string_stream.str("");
        log_string_stream << "Health class \"" << ui.comboBox_class->currentText().toStdString() << "\" started! " << endl;
        ui.textBrowser_log->append(QString::fromStdString(log_string_stream.str()));

        ui.pushButton_start->setEnabled(false);

        ui.pushButton_suspend->setEnabled(true);
        
        ui.pushButton_stop->setEnabled(true);

        // Limit info
        if (m_aicoacher_helper->GetAICoacherPr()->GetCountLimit() != 0)
        {
            stringstream ui_string_stream;
            ui_string_stream.str("");
            ui_string_stream << "/ " << m_aicoacher_helper->GetAICoacherPr()->GetCountLimit();
            
            ui.label_count_max->setText(QString::fromStdString(ui_string_stream.str()));
        }
        else
        {
            ui.label_count_max->setText("");
        }

        emit coach_start_signal();


    }
    else
    {
        ui.textBrowser_log->append("Video initialization failed! Please reload the video! \n");
        return;
    }
}

// Stop
void AIcoachDemo::pushButtonEndClicked()
{
    // emit coach_end_signal();
    m_aicoacher_helper->m_beended = true;
}

// Suspend
void AIcoachDemo::pushButtonSuspendClicked()
{
    m_aicoacher_helper->m_besuspended = !m_aicoacher_helper->m_besuspended;
    if (false == m_aicoacher_helper->m_besuspended)
    {
        emit coach_start_signal();

        ui.textBrowser_log->append("Resume training！\n");
    }
    else
    {
        ui.textBrowser_log->append("Pause training！\n");
    }
}
// Load file
void AIcoachDemo::menuFileLoadClicked()
{
    m_file_path = QFileDialog::getOpenFileName(this, "Explore video files", "../../data/video/suki", "*.*");    // Choose a video file to open

    if (strcmp(m_file_path.toStdString().c_str(), "") != 0)
    {
        stringstream log_string_stream;
        log_string_stream.str("");
        log_string_stream << "Set the input video file：\"" << m_file_path.toStdString() << "\"" << endl << endl;

        ui.textBrowser_log->append(QString::fromStdString(log_string_stream.str()));
    }

    // log_string_stream << "current frame count: "<< m_aicoacher_helper->GetAICoacherPr()->m_videoIO.GetFrameCount() << endl;
}

void AIcoachDemo::menuCameraLoadClicked()
{
    //
}

// Click "Exit" to close the window
void AIcoachDemo::menuExitClicked()
{
    this->close();
}

// Enable/disable buttons
void AIcoachDemo::frame_finished_slot()
{
    m_aicoacher_helper->m_beended = true;
    m_aicoacher_helper->GetAICoacherPr()->GetCurrentVideoIO().DestroyInputStream();

    stringstream log_string_stream;
    log_string_stream.str("");
    log_string_stream
        << "Training finished! " << endl
        << "Action " << ui.comboBox_class->itemText(m_aicoacher_helper->GetAICoacherPr()->GetCurClassID()).toStdString() << " has been accomplised for "
        << m_aicoacher_helper->GetAICoacherPr()->GetCurrentCount() << " times in "
        << std::setiosflags(std::ios::fixed) << std::setprecision(1) << m_aicoacher_helper->GetAICoacherPr()->GetCurTimer() << "seconds. " << endl << endl;

    ui.textBrowser_log->append(QString::fromStdString(log_string_stream.str()));

    ui.pushButton_start->setEnabled(true);
    ui.pushButton_suspend->setEnabled(false);
    ui.pushButton_stop->setEnabled(false);
}

// The "About" dialog
void AIcoachDemo::menuAboutClicked()
{
    QString dlgTitle = "About AIcoachDemo";

    QString strInfo = "AIcoachDemo v0.1";

    QMessageBox::information(this, dlgTitle, strInfo, QMessageBox::Ok, QMessageBox::NoButton);
}

////////////////////updated at 2021-03-25 by Tie LIU////////////////////////////////////////
void AIcoachDemo::DisplayMat(cv::Mat image,QLabel* in_pLabel)
{
    cv::Mat rgb;
    QImage img;
    if (image.channels() == 3)
    {
        cvtColor(image, rgb, COLOR_BGR2RGB);
        img = QImage((const unsigned char*)(rgb.data),
            rgb.cols, rgb.rows, rgb.cols * rgb.channels(),
            QImage::Format_RGB888);
    }
    else
    {
        img = QImage((const unsigned char*)(image.data),
            image.cols, image.rows, rgb.cols * image.channels(),
            QImage::Format_RGB888);
    }
 
    in_pLabel->setPixmap(QPixmap::fromImage(img).scaled(in_pLabel->size()));//setPixelmap(QPixmap::fromImage(img));


}

void AIcoachDemo::coach_one_frame_slot()
{
    cv::Mat current_image = m_aicoacher_helper->GetAICoacherPr()->GetCurrentVideoIO().GetSkeletonFrame();
    DisplayMat(current_image, ui.video_processed);
    cv::Mat original_image = m_aicoacher_helper->GetAICoacherPr()->GetCurrentVideoIO().GetOriginalFrame();
    DisplayMat(original_image, ui.video_original);

    //fps
    stringstream output_text_stream;
    output_text_stream.str("");
    output_text_stream << "FPS: "<< std::setiosflags(std::ios::fixed) << std::setprecision(0) << m_aicoacher_helper->GetAICoacherPr()->GetCurrentFPS() << "\t\t";
    m_fps_label->setText(QString::fromLocal8Bit(output_text_stream.str().c_str()));

    //Count number
    output_text_stream.str("");
    output_text_stream << m_aicoacher_helper->GetAICoacherPr()->GetCurrentCount();
    ui.label_count_num->setText(QString::fromLocal8Bit(output_text_stream.str().c_str()));

    // Action tips
    PutPromptText();

    // Time
    output_text_stream.str("");
    output_text_stream << std::setiosflags(std::ios::fixed) << std::setprecision(0) << m_aicoacher_helper->GetAICoacherPr()->GetCurTimer();
    ui.label_time_num->setText(QString::fromLocal8Bit(output_text_stream.str().c_str()));
}

// Action tips
void AIcoachDemo::PutPromptText()
{
    string correction_string="";

    switch (m_aicoacher_helper->GetAICoacherPr()->GetCurClassID())
    {
    case 1:
        switch (m_aicoacher_helper->GetAICoacherPr()->GetActionPrompt())
        {
        case 0:
            correction_string = "";
            break;
        case 1:
            correction_string = "Raise hands over head! ";
            break;
        case 2:
            correction_string = "Put down your hands! ";
            break;
        case 3:
            correction_string = "Open your legs! ";
            break;
        case 4:
            correction_string = "Close your legs! ";
            break;
        case 5:
            correction_string = "Stand straight! ";
            break;
        case 10:
            correction_string = "Illegal pose！";
            break;
        default:
            correction_string = "";
            break;
        }
        break;
    case 2:
        switch (m_aicoacher_helper->GetAICoacherPr()->GetActionPrompt())
        {
        case 0:
            correction_string = "";
            break;
        case 1:
            correction_string = "Put your hands beside your ears! ";
            break;
        case 2:
            correction_string = "Stand straight! ";
            break;
        case 3:
            correction_string = "Raise your knees! ";
            break;
        case 4:
            correction_string = "Bend your elbow! ";
            break;
        case 5:
            correction_string = "Resume standing with hands holding head. ";
            break;
        case 6:
            correction_string = "Do not train the hand and the leg at the same side! ";
            break;
        case 7:
            correction_string = "Do not bend both elbows! ";
            break;
        case 8:
            correction_string = "Train the other side! ";
            break;
        case 10:
            correction_string = "Illegal pose! ";
            break;
        default:
            correction_string = "";
            break;
        }
    }

    std::stringstream prompt_text;
    prompt_text.str(correction_string);

    // Show action tips in the log
    if (m_current_prompt_id != m_aicoacher_helper->GetAICoacherPr()->GetActionPrompt() && m_aicoacher_helper->GetAICoacherPr()->GetActionPrompt() != 0)
    {
        stringstream log_string_stream;
        log_string_stream.str("");
        log_string_stream
            << "Action tips：" << prompt_text.str() << endl
            << "[" << std::setiosflags(std::ios::fixed) << std::setprecision(1) << m_aicoacher_helper->GetAICoacherPr()->GetCurTimer() << "s] ";
        ui.textBrowser_log->append(QString::fromStdString(log_string_stream.str()));
    }

    m_current_prompt_id = m_aicoacher_helper->GetAICoacherPr()->GetActionPrompt();
}