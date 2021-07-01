#include "ActionRecognizer.h"

ActionRecognizer::ActionRecognizer()
{

}

ActionRecognizer::~ActionRecognizer()
{

}


bool ActionRecognizer::InitActionRecognizerPara()
{
	return true;
}

// 初始化输入流和输出流
/*bool ActionRecognizer::InitCamera()
{
	m_file_io.InitIOstream();

	return true;
}*/

bool ActionRecognizer::Init(VideoIO* inVideoIO)
{
	m_videoIO = inVideoIO;
	
	//抓取第一帧图像，获得图像size
	GetCurrentFrame();

	//初始化Detector
	m_curPoseDetector.InitModelPara(POSE_MODEL);

	//初始化Tracker
	m_currentPoseTracker.InitPoseTrackerPara();

	return true;
}

bool ActionRecognizer::ExecuteActionRecognize(int inActionType)
{
//跟踪器，当前只是将pose数据push进去序列里面【2021-03-19，Tie LIU】
	m_currentPoseTracker.PoseTracking(m_curPoseDetector.GetCurrentPersonPose());

	//比较跟踪结果与当前动作识别情况
	bool b_actionRecognized = false; 
	
	//识别当前动作，如果成功，返回true;
	switch (inActionType)
	{
	//case 0: //测试动作//【应浩聪检查动作计数代码】
		
	//	//判断是否完成一次动作计数
	//	b_actionRecognized = m_currentPoseTracker.CalculateActionCount(); //framez_size是否需要传进去？【应浩聪考虑下】
	//	
	//	break;

	//case 1:  //第一类动作
	//	//【应浩聪：先完成两类动作】
	//	b_actionRecognized = ActionRecognizerUtil::CalculateActionCount(m_currentPoseTracker.GetPoseQueue(), 1);

	//	break;

	//case 2:  //第二类动作
	//	//【应浩聪：先完成两类动作】	
	//	b_actionRecognized = ActionRecognizerUtil::CalculateActionCount(m_currentPoseTracker.GetPoseQueue(), 2);

	//	break;
	case 0: //测试动作//【应浩聪检查动作计数代码】

		//判断是否完成一次动作计数
		b_actionRecognized = ActionRecognizerUtil::CalculateActionCount(m_currentPoseTracker.GetPoseQueue(), 0, m_current_state, m_current_prompt); //framez_size是否需要传进去？【应浩聪考虑下】

		break;

	case 1:  //第一类动作
		//【应浩聪：先完成两类动作】
		b_actionRecognized = ActionRecognizerUtil::CalculateActionCount(m_currentPoseTracker.GetPoseQueue(), 1, m_current_state, m_current_prompt);
		break;

	case 2:  //第二类动作
		//【应浩聪：先完成两类动作】	
		b_actionRecognized = ActionRecognizerUtil::CalculateActionCount(m_currentPoseTracker.GetPoseQueue(), 2, m_current_state, m_current_prompt);

		cout << m_current_state << "\t" << m_current_prompt << endl;

		break;

	case 3:  //第三类动作
		break;

	case 4:  //第四类动作
		break;

	case 5:  //第五类动作
		break;

	case 6:  //第六类动作
		break;

	case 7:  //第七类动作
		break;

	case 8:  //第八类动作
		break;


	default:
		break;
	}
	//---------------------------------废弃的变量【YHC, 3.30】
	// get_current_state = m_current_state;

	return b_actionRecognized;
}


VideoIO* ActionRecognizer::GetCurrentVideoIO()
{
	return m_videoIO;
}

void ActionRecognizer::GetCurrentFrame()
{
	m_current_frame = m_videoIO->GetCurrentFrame();
	m_frame_size = m_current_frame.size();
}

bool ActionRecognizer::CalculatePersonPose()
{
	return m_curPoseDetector.CalculatePersonPose(m_current_frame);
}

PersonPosePara ActionRecognizer::GetCurrentPersonPose()
{
	return m_curPoseDetector.GetCurrentPersonPose();
}

// ----------------------获取当前动作状态，用于生成动作指南【YHC，3.30】
int ActionRecognizer::GetCurrentActionPrompt()
{
	return m_current_prompt;
}