#define _CRT_SECURE_NO_WARNINGS
#pragma once

// Qt modules
#include <QtWidgets/QMainWindow>
#include "ui_AICoacherQT.h"
#include <QMessagebox>
#include <QInputdialog>
#include <QFiledialog>

// Standard libraries
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <semaphore>

// Third party libraries
#include <opencv2/opencv.hpp>

// Classes inside this project
#include "Coacher.h"

class AICoacherQT : public QMainWindow
{
    Q_OBJECT

public:
    AICoacherQT(QWidget *parent = nullptr);
    ~AICoacherQT();

private:
    Ui::AICoacherQTClass ui;

    // AICoacher
    Coacher coacher = Coacher();

    // Handles for returning data asynchronously
    std::binary_semaphore* handle_frame_refresh_ptr = new std::binary_semaphore(0);
    std::binary_semaphore* handle_action_refresh_ptr = new std::binary_semaphore(0);
    std::binary_semaphore* handle_course_end_ptr = new std::binary_semaphore(0);
    std::binary_semaphore* handle_count_refresh_ptr = new std::binary_semaphore(0);
    std::binary_semaphore* handle_time_refresh_ptr = new std::binary_semaphore(0);

    // Respond to internal events
    void frameRefreshed();
    void actionRefreshed();
    void courseEnded();
    void countRefreshed();
    void timeRefreshed();

    // Threads for respond to internal events
    std::thread thread_frame_refreshed;
    std::thread thread_action_refreshed;
    std::thread thread_course_ended;
    std::thread thread_count_refreshed;
    std::thread thread_time_refreshed;

    // Controller to stop handler threads
    bool is_training = false;

    // Video source information
    std::string video_file_name = "";
    int camera_port = 0;
    bool is_file_loaded = false;
    bool is_camera_loaded = false;

    // Count limit / time limit of an action
    int count_limit = 0;
    int time_limit = 0;

    // Obtaining all course schedules
    int obtainCourseList();
    std::string COURSE_FILE_DIR = "../configs/health_course/";
    std::vector<std::string> course_file_list;
    std::vector<std::string> course_name_list;
    int selected_course_index = 0;

    // Convert OpenCV images to QT images
    QPixmap imageFitQt(cv::Mat cv_image, QSize qt_size);

    // Generate logs with system time
    int logWithTime(std::string log_content);

// Respond to UI actions
private Q_SLOTS:
    void pushButtonStartClicked();   // start training
    void pushButtonSuspendClicked();//suspend training
    void pushButtonEndClicked();   // end training
    void comboBoxCourseChanged();   // course selection changed
    void menuFileLoadClicked();    // load file by clicking the menu
    void menuCameraLoadClicked();    // load webcam by clicking the menu
    void menuExitClicked();   // exit
    void menuAboutClicked();    // About dialog

};
