#pragma once

#include "ActionRecognizerUtil.h"
#include "PoseDetector.h"

//构建跟踪器，调用PoseDetector进行跟踪；
//输入输出都是cv::Mat格式的数据





struct PoseTrackerPara
{

};

class PoseTracker
{
private:
	PoseTrackerPara m_curPoseTracker;

	// 用于标记之前是处于举起/放下状态，当状态切换时计数  【需要考虑这个变量是否需要在这里定义】
	int m_current_state = 0;  

	//人体姿势序列
	vector<PersonPosePara> m_pose_queue;


public:
	PoseTracker();
	~PoseTracker();

	//初始化跟踪器的参数
	bool InitPoseTrackerPara();

	//跟踪器，目前只是实现了pose数据的输入存储
	bool PoseTracking(PersonPosePara current_pose);

	//计算动作次数【测试函数】【3.30 YHC----------------------------废弃】
	// bool CalculateActionCount();

	//获取动作序列
	vector<PersonPosePara>& GetPoseQueue();
};
