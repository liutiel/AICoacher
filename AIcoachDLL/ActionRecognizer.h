#pragma once
#include "HealthAction.h"
#include "PoseTracker.h"
#include "VideoIO.h"


//
struct ActionRecognizerPara
{

};

class ActionRecognizer
{
private:
	//还没有定义使用
	ActionRecognizerPara m_currentActionRecognizePara; 

	//关节点识别器
	PoseDetector m_curPoseDetector;

	//关节点跟踪器
	PoseTracker m_currentPoseTracker;

	//输入的视频接口
	VideoIO* m_videoIO;

	//当前帧图像
	cv::Mat m_current_frame;

	//当前帧图像的size
	cv::Size m_frame_size;

	// 用于标记之前是处于举起/放下状态，当状态切换时计数  【需要考虑这个变量是否需要在这里定义】
	int m_current_state = 0;
	int m_current_prompt = 0;		//-----------------分离状态标记和错误标记【YHC, 3.30】


public:
	ActionRecognizer();
	~ActionRecognizer();

	bool InitActionRecognizerPara();

	//初始化摄像头//not finished
	bool InitCamera(); 

	bool Init(VideoIO* inVideoIO);
	VideoIO* GetCurrentVideoIO();


	//获得当前帧图像，在执行关节点提取前需要先执行此函数
	void GetCurrentFrame();  
	
	//执行关节点提取
	bool CalculatePersonPose();

	//获得关节点数据
	PersonPosePara GetCurrentPersonPose();

	//执行动作识别，输入为动作类型
	bool ExecuteActionRecognize(int inActionType);

	// ----------------------获取当前动作状态，用于生成动作指南【YHC，3.30】
	int GetCurrentActionPrompt();
};

