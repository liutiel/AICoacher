#include "PoseTracker.h"


PoseTracker::PoseTracker()
{

}
PoseTracker::~PoseTracker()
{
	
}

bool PoseTracker::InitPoseTrackerPara()
{
	//初始化跟踪器的参数


	return true;
}


bool PoseTracker::PoseTracking(PersonPosePara current_pose)
{

	// 将提取出来的关节点送入队列便于后续处理
	m_pose_queue.push_back(current_pose);
	if (m_pose_queue.size() > 5 )
	{
		m_pose_queue.erase(m_pose_queue.begin());
	}

	int keyinput = cv::waitKey(1);
	return true;
}

// 【3.30 YHC----------------------------废弃】
/*
bool PoseTracker::CalculateActionCount()
{
	return ActionRecognizerUtil::CalculateActionCount(m_pose_queue, 0, m_current_state);
}*/

vector<PersonPosePara>& PoseTracker::GetPoseQueue()
{
	return m_pose_queue;
}