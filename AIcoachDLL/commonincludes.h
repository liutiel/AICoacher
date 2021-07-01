#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <direct.h>
#include <windows.h> 
#include <filesystem>
#include <cstdlib>
#include <tchar.h>
#include <windows.h>
#include <io.h>
#include <stdlib.h> 
#include <stdio.h> 

#include <iostream>
#include <fstream>


// opencv
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv; //use OpenCV namespace

//#define TESTING_AICOACH_TOOLBOX

#define _AICOACH_TOOLBOX_DISPLAY

// -------------------------已经不需要【YHC-3.30】
//#define RUNNING_ON_OPENCV 0
//#define RUNNING_ON_QT 1

// T-CNU
/*
#define INPUT_STREAM "D:/2020/SPAX-AI/video/002-suki.mp4"
#define OUTPUT_STREAM "D:/2020/SPAX-AI/results/002-suki-output.mp4"
#define OUTPUT_IMAGES_PREFIX "D:/2020/SPAX-AI/results/images/002-suki-output_"
*/

// YHC
/*
#define INPUT_STREAM "D:/OneDrive - cnu.edu.cn/My_Programme/Projects/210125_Exhaustion/01_Data/alex/alex_01.mp4"
#define OUTPUT_STREAM "D:/user/Desktop/output.mp4"
#define OUTPUT_IMAGES_PREFIX "D:/user/Desktop/output_"
*/

//【YHC-0421-1240】从外部txt文件为各Test函数指定输入输出视频路径
//#define INPUT_STREAM "../../data/video/suki/002-suki_02.mp4"
//#define OUTPUT_STREAM "../../data/results/002-suki_02-output.mp4"
//#define OUTPUT_IMAGES_PREFIX "../../data/results/images/002-suki_02-output_"
#define PATH_IO "../../path.txt"

#define POSE_MODEL "../../AIcoachDLL/models/pose.tflite"
#define THREAD_NUM 6

// -----------------------------考虑到画面上的左右方向和人自身感知的左右方向正好相反，应该在这里交换左右【YHC, 3.30】
#define HEAD 0
#define NECK 1
#define SHOULDER_R 2
#define ELBOW_R 3
#define WRIST_R 4
#define SHOULDER_L 5
#define ELBOW_L 6
#define WRIST_L 7
#define HIP_R 8
#define KNEE_R 9
#define ANKLE_R 10
#define HIP_L 11
#define KNEE_L 12
#define ANKLE_L 13
/*
#define SHOULDER_L 2
#define ELBOW_L 3
#define WRIST_L 4
#define SHOULDER_R 5
#define ELBOW_R 6
#define WRIST_R 7
#define HIP_L 8
#define KNEE_L 9
#define ANKLE_L 10
#define HIP_R 11
#define KNEE_R 12
#define ANKLE_R 13*/

#define DETECTION_MODEL_SIZE 192
#define DETECTION_MODEL_CNLS 3

#define OUTPUT_SIZE 96
#define OUTPUT_CNLS 14
#define RESULT_IMAGE_SIZE 92.0
// 【YHC-0419-1438】统一管理队列中存储的帧数
#define NUM_POSE_QUEUE 1

//人体姿势的存储格式怎么定义合适？要思考一下
struct PersonPosePara
{
	int pointPose[OUTPUT_CNLS * 2];
};

//action state transfer
const static int DEFINED_NUMBER_ACTION_STATE = 19;
