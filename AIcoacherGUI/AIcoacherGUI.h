#pragma once
// #pragma execution_character_set("utf-8")
// The GUI

#include <QtWidgets/QMainWindow>
#include "ui_AIcoacherGUI.h"
#include <QTextCodec>
#include <QThread>
#include "AIcoacherHelper.h"
#include "qfiledialog.h"
#include "commonincludes.h"

#include "qmessagebox.h"

class AIcoacherGUI : public QMainWindow
{
    Q_OBJECT

public:
    AIcoacherGUI(QWidget* parent = Q_NULLPTR);

// Respond to UI actions
private Q_SLOTS:
    void pushButtonStartClicked();   // start training
    void pushButtonSuspendClicked();//suspend training
    void pushButtonEndClicked();   // end training
    void menuFileLoadClicked();    // load file by clicking the menu
    void menuCameraLoadClicked();    // load webcam by clicking the menu
    void menuExitClicked();   // exit
    void menuAboutClicked();    // About dialog

public slots:
    // Initialization result
    void coach_init_result_slot(bool isSuccessful, int input_stream);   // Webcam
    void coach_init_result_slot(bool isSuccessful, QString input_stream);   //Video File
    
    void frame_finished_slot(); // Enable/disable buttons

    void coach_one_frame_slot();    // One frame processed

signals:
    void coach_init_signal(int classID, int input_stream);
    void coach_init_signal(int classID, QString input_stream);
    void coach_start_signal();
    void coach_suspend_signal();
    // void coach_end_signal();

public:
    void DisplayMat(cv::Mat image, QLabel* in_pLabel);  // Put OpenCV Mat into a Qt label

private:
    Ui::AIcoacherGUIClass ui;
    AIcoacherHelper* m_aicoacher_helper;
    QThread* m_thread_processing;

    //Status bar
    QLabel* m_fps_label;
    QLabel* m_drop_label;

    QString m_file_path;

    int m_current_prompt_id = 0;

    void PutPromptText();   // Output action tips
};