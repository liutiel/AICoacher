#pragma once
// #pragma execution_character_set("utf-8")
// The GUI

#include <QtWidgets/QMainWindow>
#include "ui_AIcoachDemo.h"
#include <QTextCodec>
#include <QThread>
#include "AIcoacherHelper.h"
#include "qfiledialog.h"
#include "commonincludes.h"

#include "qmessagebox.h"

class AIcoachDemo : public QMainWindow
{
    Q_OBJECT

public:
    AIcoachDemo(QWidget* parent = Q_NULLPTR);

    void DisplayMat(cv::Mat image, QLabel* in_pLabel);

private:
    Ui::AIcoachDemoClass ui;
    AIcoacherHelper* m_aicoacher_helper;
    QThread* m_thread_processing;

    // Output action tips
    void PutPromptText();

    //Status bar
    //QLabel* m_count_label;
    QLabel* m_fps_label;
    QLabel* m_drop_label;
    //QLabel* m_prompt_label;

    QString m_file_path;

    int m_current_prompt_id = 0;

    // Respond to UI actions
private Q_SLOTS:
    void pushButtonStartClicked();   // start training
    void pushButtonSuspendClicked();//suspend training
    void pushButtonEndClicked();   // end training
    void menuFileLoadClicked();    // load file by clicking the menu
    void menuCameraLoadClicked();    // load webcam by clicking the menu
    void menuExitClicked();   // exit
    // About dialog
    void menuAboutClicked();

public slots:
    // Initialization result
    void coach_init_result_slot(bool isSuccessful, int input_stream);
    void coach_init_result_slot(bool isSuccessful, QString input_stream);
    void coach_one_frame_slot();
    void frame_finished_slot();

signals:
    void coach_init_signal(int classID, int input_stream);
    void coach_init_signal(int classID, QString input_stream);
    void coach_start_signal();
    void coach_suspend_signal();
    // void coach_end_signal();
};