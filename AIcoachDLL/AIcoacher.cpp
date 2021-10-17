#include "AIcoacher.h"

//pFuncAIcoachCount pCallBackAIcoachCount = NULL;


AIcoacher::AIcoacher()
{
//	pFuncAIcoachCC = NULL;

}

AIcoacher::~AIcoacher()
{
}

bool AIcoacher::Init(int inClassID, std::string input_stream)
{
	m_curClassID = inClassID;

	if (m_videoIO.InitInputStream(input_stream))
	{
		cout << "read video from file: " << input_stream << endl;
	}
	// Reading action state transfer
	string inFileName = "../../AIcoachDLL/models/action_category/action01.txt";
	InitActionStateList(inFileName);
	cout << "InitActionStateList(inFileName, curActionTransferList): " << endl;

	// Reading course info
	string inClassName = "../../AIcoachDLL/models/class_editor/class01.txt";
	m_curClass.InitClassFromFile(inClassName);
	cout << "curClass.InitClassFromFile(inClassName): " << inClassName << endl;
	m_curClass.PrintInfo();
	m_HealthAction = m_curClass.m_curHealthActionList[inClassID];

	//Initializing pose detector
	m_PoseDetector.InitModelPara(POSE_MODEL);
	cout << "InitActionStateList(inFileName, curActionTransferList): " << POSE_MODEL << endl;

	// Obtain action parameters
	HealthAction& curHealthAction = m_curClass.m_curHealthActionList[inClassID];
	curHealthAction.PrintInfo();
	waitKey(0);

	// Obtain actiontransfer
	int indexAction = curHealthAction.m_paraAction.actionType;

	for (size_t indAT = 0; indAT < m_ActionTransferList.size(); indAT++)
	{
		if (indexAction == m_ActionTransferList[indAT].GetActionType())
		{
			m_current_transfer = m_ActionTransferList[indAT];
			break;
		}
	}

	return true;
}

VideoIO AIcoacher::GetCurrentVideoIO()
{
	return m_videoIO;
}

PersonPosePara AIcoacher::GetCurrentPose()
{
	return m_PoseDetector.GetCurrentPersonPose();
}

int AIcoacher::GetCurrentCount()
{
	return m_HealthAction.GetCurrentCount();
}

double AIcoacher::GetCurrentFPS()
{
	return m_curfps;
}

bool AIcoacher::IsActionFinished()
{
	return m_HealthAction.CheckCountFinished();
}

// Get the limits
int AIcoacher::GetCountLimit()
{
	return m_HealthAction.m_paraAction.actionLimitNum;
}

double AIcoacher::GetTimeLimit()
{
	return m_HealthAction.m_paraAction.actionLimitSecond;
}

int AIcoacher::GetCurClassID()
{
	return m_curClassID;
}

int AIcoacher::GetActionPrompt()
{
	return m_perfection;
}

double AIcoacher::GetCurTimer()
{
	return m_curTimer;
}

bool AIcoacher::AIcoacherFrameProcessing()
{
	// Obtain the frame, or exit
	if (!m_videoIO.CaptureNextFrame())
	{
		m_videoIO.DestroyInputStream();
		return false;
	}

	// Start timing
	CoachTimer curTimer;
	curTimer.StartTimer();

	m_PoseDetector.CalculatePersonPose(m_videoIO.GetCurrentFrame());

	// Recognize actions
	PersonPosePara cur_pose = m_PoseDetector.GetCurrentPersonPose();
	m_pose_queue.push_back(cur_pose);


	if (m_pose_queue.size() > m_pose_queue_num)
	{
		m_pose_queue.erase(m_pose_queue.begin());
	}


	AcionStatePara curActionState;
	curActionState = m_current_transfer.ComputeActionStateFromPosePoints(m_pose_queue);
	for (int j = 0; j < DEFINED_NUMBER_ACTION_STATE + DEFINED_NUMBER_PERFECT_STATE; j++)
	{
		cout << curActionState.stateIndex[j] << " ";
	}
	cout << endl;

	m_current_transfer.NextActionState(curActionState);
	m_current_transfer.NextPerfectState(curActionState);
	cout << m_current_transfer.nextAction << " " << m_current_transfer.perfectAction << endl;
	m_perfection = m_current_transfer.perfectAction;

	if (1 == m_current_transfer.nextAction)
	{
		m_HealthAction.m_actionCounter.CountOnce();
	}

	if (m_HealthAction.CheckDurationFinished())
	{
		cout << "Change the Health Action" << endl;
		return false;
	}


	// Calculate FPS
	m_curfps = 1000.0 / curTimer.GetCurTimer();

	m_curTimer += curTimer.GetCurTimer() / 1000.0;

	// Draw points
	cv::Mat* curPImage = m_videoIO.GetSkeletonFramePr();//NULL;//
	m_videoIO.PutPointInfo(m_PoseDetector.GetCurrentPersonPose(), curPImage);

	return true;
}

bool AIcoacher::StartHealthClass()
{
	return true;
}

bool AIcoacher::SuspendHealthClass()
{
	return true;
}

bool AIcoacher::EndHealthClass()
{
	return true;
}

//for AIcoacherInterface
bool AIcoacher::AIcoacherInitialize(int inVideoIndex, const char* inClassName)
{

	string inFileName = "../../AIcoachDLL/models/action_category/action01.txt";
	InitActionStateList(inFileName);

	m_PoseDetector.InitModelPara(POSE_MODEL);

	if (-1 == inVideoIndex) 
	{
		string videoName = "D:/GitHub/210203_AIcoach/AIcoachData/video/107/k/1001.mp4";
		m_videoIO.InitInputStream(videoName);
	}
	else
	{
		m_videoIO.InitInputStream(inVideoIndex);
	}

	AIcoacherUpdateClass(inClassName);
	m_curClass.PrintInfo();

	m_VideoCaptureThread = std::thread(std::bind(&AIcoacher::VideoCaptureThread, this));
	m_VideoCaptureThread.detach();

	m_AICoacherThread = std::thread(std::bind(&AIcoacher::AICoacherThread, this));
	m_AICoacherThread.detach();

	return true;
}

bool AIcoacher::AIcoacherUpdateClass(const char* inClassName)
{
	if (NULL != inClassName)
	{	
		m_curClass.InitClassFromFile(inClassName);
	
		m_HealthAction = m_curClass.m_curHealthActionList[0];

		AIcoacherActionTransferInit(m_HealthAction, m_current_transfer);

		return true;
	}
	else
	{
		cout << "Please input the class information! " << endl;
		return false;
	}
}

bool AIcoacher::AIcoacherStart()
{
	//Capturing frames per 100ms and push into the queue
	setThreadsStatus(AICOACHER_RUNNING);

	return true;
}

bool AIcoacher::AIcoacherSuspend()
{
	setThreadsStatus(AICOACHER_SUSPEND);

	return true;
}

bool AIcoacher::AIcoacherExit()
{
	setThreadsStatus(AICOACHER_EXIT);

	m_VideoCaptureThread.join();   

	m_AICoacherThread.join();

	std::cout << "AIcoacherThread deconstruction" << std::endl;

	return true;
}

void AIcoacher::VideoCaptureThread()
{
	cout << "VideoCaptureThread():" << endl;
	int index = 0;
	while (true)
	{
		if (AICOACHER_EXIT == m_AIcoacherThreadStatus) //Thread exit
			break;

		//AIcoacher functions implementations 
		m_videoIO.CaptureNextFrame();

		pushDataToQueue(m_videoIO.GetCurrentFrame());

//		cout << index++ << "  " << m_DataQueue.size() << endl;

		if (m_DataQueue.size() > 2)
		{
			m_DataQueue.pop();
		}

		Sleep(100);
	}
}
void AIcoacher::AICoacherThread()
{
	cout << "AICoacherThread():" << endl;
//	setThreadsStatus(AICOACHER_INITIALIZE);

//	InitializePoseQueue();
	
	setThreadsStatus(AICOACHER_RUNNING);

	//Start training
	for (int indClass = 0; indClass < m_curClass.m_numAction; indClass++)
	{
		if (AICOACHER_EXIT == m_AIcoacherThreadStatus) //Thread exit
			break;

		cout << "indClass: " << indClass << " m_curClass.m_numAction: " << m_curClass.m_numAction << endl;

		HealthAction& curHealthAction = m_curClass.m_curHealthActionList[indClass];
		curHealthAction.PrintInfo();

		ActionTransfer curActionTransfer;
		GetActionTransfer(curHealthAction.m_paraAction.actionType, curActionTransfer);
		curActionTransfer.PrintInfo();

		InitializePoseQueue();

		//begin to process for each heathaction
#ifdef TESTING_AICOACH_TOOLBOX
		int index = 0;
#endif // TESTING_AICOACH_TOOLBOX


		while (true)
		{
			if (AICOACHER_EXIT == m_AIcoacherThreadStatus) //Thread exit
				break;

			if (AICOACHER_SUSPEND == m_AIcoacherThreadStatus) //Thread suspend
			{
				Sleep(1000);
				continue;
			}
			//AIcoacher functions implementations 
			cv::Mat curImage;
			if (!popDataFromQueue(curImage))
				continue;

			//calculate the pose points
			m_PoseDetector.CalculatePersonPose(curImage);

			//push to the pose queue
			m_pose_queue.push_back(m_PoseDetector.GetCurrentPersonPose());
			if (m_pose_queue.size() > NUM_POSE_QUEUE)
				m_pose_queue.erase(m_pose_queue.begin());

			//calculate the action state
			AcionStatePara curActionState = m_current_transfer.ComputeActionStateFromPosePoints(m_pose_queue);

			//judge whether the action is counted 
			if (1 == m_current_transfer.NextActionState(curActionState)[0])
			{
				m_HealthAction.m_actionCounter.CountOnce();

				//call back funtion 
				//cout << "************************************* " << "pCallBackAIcoachCount" <<"************************************ " << endl;
				int nCount = m_HealthAction.m_actionCounter.GetCurrentCount();
				pCallBackAIcoachCount(&nCount, (char*)&"Count Once!!");
			}
			//int nCount = m_HealthAction.m_actionCounter.GetCurrentCount();
			//char cPrompt[] = "Count Once!!";
			//pCallBackAIcoachCount(&nCount, (char*)cPrompt);

#ifdef TESTING_AICOACH_TOOLBOX   //debug information 
			cout << "index: " << index++ << "     ";
			for (int j = 0; j < DEFINED_NUMBER_ACTION_STATE; j++)
			{
				cout << curActionState.stateIndex[j] << " ";
			}
			cout << " Count: " << m_HealthAction.m_actionCounter.GetCurrentCount() << " " << m_HealthAction.m_actionCounter.GetLimtCount()<< endl;

#endif		
#ifdef _AICOACH_TOOLBOX_DISPLAY
			m_videoIO.PutPointInfo(m_PoseDetector.GetCurrentPersonPose());
			m_videoIO.PutCountInfo(m_HealthAction.m_actionCounter.GetCurrentCount());
			cv::Mat curImg = m_videoIO.GetCurrentFrame();
			cv::imshow("TestForActionTransfer", curImg);

			waitKey(1);
#endif
			
			// Shifting between actions
			if (m_HealthAction.CheckCountFinished())
			{
				break;
			}
		}

	}

}


bool AIcoacher::InitializePoseQueue()
{
	m_pose_queue.clear();
	while (m_pose_queue.size() < NUM_POSE_QUEUE)
	{
		cv::Mat curImage;
		if (!popDataFromQueue(curImage))
		{
			cout << "InitializePoseQueue: There is no data in the data queue! " << endl;
		}
		else
		{
			m_PoseDetector.CalculatePersonPose(curImage);

			m_pose_queue.push_back(m_PoseDetector.GetCurrentPersonPose());

		}

		Sleep(1000);
	}
	return true;
}

 bool AIcoacher::GetActionTransfer(int indexAction, ActionTransfer & out_action_transfer)
{
	for (size_t indAT = 0; indAT < m_ActionTransferList.size(); indAT++)
	{
		if (indexAction == m_ActionTransferList[indAT].GetActionType())
		{
			out_action_transfer= m_ActionTransferList[indAT];
			break;
		}
	}
	return true;
}

bool AIcoacher::isDataQueueEmpty()
{
	std::lock_guard<std::mutex> lockTmp(m_Mutex4Queue);
	std::cout << "AIcoacher::isDataQueueEmpty " << std::endl;
	return m_DataQueue.empty();
}

bool AIcoacher::isDataQueueNotEmpty()
{
	std::lock_guard<std::mutex> lockTmp(m_Mutex4Queue);
	std::cout << "AIcoacher::isDataQueueNotEmpty " << std::endl;
	return !m_DataQueue.empty();
}

void AIcoacher::pushDataToQueue(const cv::Mat& inData)
{
	std::lock_guard<std::mutex> lockTmp(m_Mutex4Queue);
//	std::cout << " pushDataToQueue " << std::endl;
	m_DataQueue.push(inData);
}

bool AIcoacher::popDataFromQueue(cv::Mat& outData)
{
	std::lock_guard<std::mutex> lockTmp(m_Mutex4Queue);
//	std::cout << " popDataFromQueue " << std::endl;
	if (m_DataQueue.size())
	{
		outData = m_DataQueue.front();
		m_DataQueue.pop();
		return true;
	}
	else
		return false;
}

eAIcoacherThreadStatus AIcoacher::getThreadsStatus()
{
	std::lock_guard<std::mutex> lockTmp(m_Mutex4ThreadsStatus);
	return m_AIcoacherThreadStatus;
}

void AIcoacher::setThreadsStatus(eAIcoacherThreadStatus status)
{
	std::lock_guard<std::mutex> lockTmp(m_Mutex4ThreadsStatus);
	m_AIcoacherThreadStatus = status;
}

// Initialize info of class and transfer from file
bool AIcoacher::AIcoacherActionTransferInit(HealthAction curHealthAction, ActionTransfer& curActionTransfer)
{
	int indexAction = curHealthAction.m_paraAction.actionType;

	for (size_t indAT = 0; indAT < m_ActionTransferList.size(); indAT++)
	{
		if (indexAction == m_ActionTransferList[indAT].GetActionType())
		{
			curActionTransfer = m_ActionTransferList[indAT];
			break;
		}
	}
	return true;
}

bool AIcoacher::InitActionStateList(string inFilename)
{
	m_ActionTransferList.clear();

	std::ifstream inF;
	inF.open(inFilename, ios::in);

	while (!inF.eof())
	{
		ActionTransfer curAT;
		if (true == curAT.ReaddActionStateFromStream(inF))
		{
			m_ActionTransferList.push_back(curAT);
		}
	}

	inF.close();

	return true;
}