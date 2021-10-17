#include "AIcoacherTest.h"
#define NO_DISPLAY_DEBUG_INFO
//【YHC-0421-1614】定义结构体和文件读取方法用于从文件读取测试视频路径
typedef struct TestPaths {

	string input_stream;

	string output_stream;

	string output_image_prefix;

}TestPaths;

TestPaths GetTestPath()
{
	std::ifstream inF;
	inF.open(PATH_IO, ios::in);

	TestPaths m_test_path;

	getline(inF, m_test_path.input_stream);
	getline(inF, m_test_path.output_stream);
	getline(inF, m_test_path.output_image_prefix);

	return m_test_path;
}

string GetActionTips(int actionNo, int tipNo)
{
	//cout << "tipno: " << tipNo << endl;

	switch(actionNo)
	{
	case 51901:
		switch (tipNo)
		{
		case 0:
			return "Perfect";
		case 1:
		case 2:
			return "Rise high";
		case 3:
		case 4:
			return "Close legs";
		default:
			return "";
		}
		break;
	case 51902:
		switch (tipNo)
		{
		case 0:
			return "Perfect";
		case 1:
		case 2:
		case 3:
		case 4:
			return "Close legs";
		default:
			return "";
		}
		break;
	case 51903:
		switch (tipNo)
		{
		default:
			return "";
		}
		break;
	default:
		return "";
	}
}

bool TestForActionTransfer()
{
	const int num_pose_queue = 5;
	
	//【YHC-0421-1618】从文件获取测试视频路径 
//	string videoName = "../../data/video/suki/002-suki_01.mp4";
	string videoName = GetTestPath().input_stream;

	vector<PersonPosePara> cur_pose_queue;

	VideoIO curVideo;
	if (curVideo.InitInputStream(videoName))
	{
		cout << "read video from file: " << videoName << endl;
	}

	//关节点识别器
	PoseDetector curPoseDetector;
	//初始化Detector
	curPoseDetector.InitModelPara(POSE_MODEL);


	//initial first fames;
	for (int i = 0; i < num_pose_queue; i++)
	{
		if (!curVideo.CaptureNextFrame())
		{
			curVideo.DestroyInputStream();
			return false;
		}
		curPoseDetector.CalculatePersonPose(curVideo.GetCurrentFrame());

		cur_pose_queue.push_back(curPoseDetector.GetCurrentPersonPose());
	}


	string inFileName = "../../AIcoachDLL/models/action_category/action01.txt";
	ActionTransfer curActionTransfer;
	curActionTransfer.InitActionStateFromFile(inFileName);


	//begin to process 
	int count = 0;
	int index = 0; 
	while (curVideo.CaptureNextFrame())
	{
		cout << "index: " << index++ << endl;
		curPoseDetector.CalculatePersonPose(curVideo.GetCurrentFrame());

		PersonPosePara cur_pose = curPoseDetector.GetCurrentPersonPose();
		cur_pose_queue.push_back(cur_pose);

		if (cur_pose_queue.size() > num_pose_queue)
		{
			cur_pose_queue.erase(cur_pose_queue.begin());
		}


		AcionStatePara curActionState;
		curActionState = curActionTransfer.ComputeActionStateFromPosePoints(cur_pose_queue);
		for (int j = 0; j < DEFINED_NUMBER_ACTION_STATE + DEFINED_NUMBER_PERFECT_STATE; j++)
		{
			cout << curActionState.stateIndex[j] << " ";
		}
		cout << endl;

		if (1 == curActionTransfer.NextActionState(curActionState)[0])
		{
			count++; 

			cout << "Count: " << count << endl;
		}
		curVideo.PutPointInfo(curPoseDetector.GetCurrentPersonPose());
		curVideo.PutCountInfo(count);
		cv::Mat curImg = curVideo.GetCurrentFrame();
		cv::imshow("TestForActionTransfer", curImg);
		
		waitKey(0);
	}
	curVideo.DestroyInputStream();
	return true;
}

bool TestForAIClass()
{
	const int num_pose_queue = 5;

	string videoName = "D:/GitHub/210203_AIcoach/AIcoachData/video/0519/1cepingjv.mp4"; //"../../data/video/suki/002-suki_01.mp4"; 

	//"../../../data/video/YHC/yhc-01.mp4"; //

	vector<PersonPosePara> cur_pose_queue;

	VideoIO curVideo;
	if (curVideo.InitInputStream(videoName))
	{
		cout << "read video from file: " << videoName << endl;
	}
	//读取action state transfer
	string inFileName = "../../AIcoachDLL/models/action_category/action01.txt";
	//ActionTransfer curActionTransfer;
	//curActionTransfer.InitActionStateFromFile(inFileName);
	std::vector<ActionTransfer> curActionTransferList;	
	InitActionStateList(inFileName, curActionTransferList);
	cout << "InitActionStateList(inFileName, curActionTransferList): " << endl;

	//读取课程信息
	string inClassName = "../../AIcoachDLL/models/class_editor/class01.txt";
	AIclass curClass;
	curClass.InitClassFromFile(inClassName);
	cout << "curClass.InitClassFromFile(inClassName): " << inClassName << endl;
	curClass.PrintInfo();

	//关节点识别器
	PoseDetector curPoseDetector;
	//初始化Detector
	curPoseDetector.InitModelPara(POSE_MODEL);
	cout << "InitActionStateList(inFileName, curActionTransferList): " << POSE_MODEL << endl;

	//开始课程
	for (int indClass = 0; indClass < curClass.m_numAction; indClass++)
	{

		//获取当前动作参数
		HealthAction & curHealthAction = curClass.m_curHealthActionList[indClass];
		curHealthAction.PrintInfo();
		waitKey(0);

		//获取当前动作actiontransfer
		int indexAction = curHealthAction.m_paraAction.actionType;
		ActionTransfer curActionTransfer;
		for (size_t indAT = 0; indAT < curActionTransferList.size(); indAT++)
		{
			if (indexAction == curActionTransferList[indAT].GetActionType())
			{
				curActionTransfer = curActionTransferList[indAT];
				break;
			}
		}

		//初始化，获得初始序列
		while (curVideo.CaptureNextFrame())
		{
			curPoseDetector.CalculatePersonPose(curVideo.GetCurrentFrame());

			cur_pose_queue.push_back(curPoseDetector.GetCurrentPersonPose());

			if (cur_pose_queue.size() >= num_pose_queue )
			{
				//检测第一个状态：如果检测到了break出去，如果没有检测到，继续循环；【不需要】

				break;
			}

		}

		//begin to process for each heathaction
		int count = 0;
		int index = 0;
		string tips = "";
		while (curVideo.CaptureNextFrame())
		{
			//cout << "index: " << index++;// << endl;
			curPoseDetector.CalculatePersonPose(curVideo.GetCurrentFrame());

			PersonPosePara cur_pose = curPoseDetector.GetCurrentPersonPose();
			cur_pose_queue.push_back(cur_pose);

			if (cur_pose_queue.size() > num_pose_queue)
			{
				cur_pose_queue.erase(cur_pose_queue.begin());
			}


			AcionStatePara curActionState;
			curActionState = curActionTransfer.ComputeActionStateFromPosePoints(cur_pose_queue);
			for (int j = 0; j < DEFINED_NUMBER_ACTION_STATE + DEFINED_NUMBER_PERFECT_STATE; j++)
			{
				//cout << curActionState.stateIndex[j] << " ";
			}

			int* nextAction = curActionTransfer.NextActionState(curActionState);
			curActionTransfer.NextPerfectState(curActionState);
			if (1 == nextAction[0])
			{
				count++;

				//cout << "Count: " << count;// << endl;

				tips = GetActionTips(indexAction, nextAction[1]);
			}
			cout << endl;

			if (count >= curHealthAction.m_paraAction.actionLimitNum)
			{
				cout << "Change the Health Action" << endl;
				break;
			}			
			
#ifndef NO_DISPLAY_DEBUG_INFO

			curVideo.PutPointInfo(curPoseDetector.GetCurrentPersonPose());

#endif
			curVideo.PutCountInfo(count);
			curVideo.PutTipsInfo(tips);
			cv::Mat curImg = curVideo.GetCurrentFrame();

			std::stringstream output_text;
			output_text.str("");
			for (int j = 0; j < DEFINED_NUMBER_ACTION_STATE + DEFINED_NUMBER_PERFECT_STATE; j++)
			{
				output_text << curActionState.stateIndex[j] << " ";
			}
#ifndef NO_DISPLAY_DEBUG_INFO

			cv::putText(curImg, output_text.str(), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(244, 186, 57), 1, cv::LINE_8, false);

#endif
			cv::resize(curImg, curImg, cv::Size(0, 0), 0.65, 0.65);
			cv::imshow("AICoacher Demo", curImg);
			
			// 指定图像保存目录！
			curVideo.SaveFrame("../../../data/results/text/output_", true);

			waitKey(1);
		}

	}
	
	curVideo.DestroyInputStream();
	return true;
}


void TestAIcoacherThread()
{
	AIcoacher curAIcoach;

	string inFileName = "../../AIcoachDLL/models/class_editor/class01.txt";

	cout << "curAIcoach.AIcoacherInitialize(-1, inFileName.c_str());" << endl;
	curAIcoach.AIcoacherInitialize(-1, inFileName.c_str());

	cout << "curAIcoach.AIcoacherStart():" << endl;
	curAIcoach.AIcoacherStart();

	cout << "11" << endl;
	int index = 0;
	while (true)
	{
		cout << "22" << endl;
		waitKey(5000);

		cout << "curAIcoach.AIcoacherSuspend():" << endl;
		curAIcoach.AIcoacherSuspend();

		cout << "33" << endl;
		waitKey(2000);

		cout << "curAIcoach.AIcoacherStart():" << endl;
		curAIcoach.AIcoacherStart();

		cout << "44" << endl;
		if (index++ >= 3)
		{
			break;
		}
		cout << "************************ " << index << endl << endl;
	}


	cout << "curAIcoach.AIcoacherExit():" << endl;
	curAIcoach.AIcoacherExit();

	waitKey(0);

}

bool TestForAIClassSchedule()
{

	AIcoacher curCoacher;

	const int num_pose_queue = 5;

	//输入编号为519的新的测试视频
	string videoName = "../../../data/videocut/4.mp4";// actiondemo519.mp4"; 
	curCoacher.Init(519, videoName);

	vector<PersonPosePara> cur_pose_queue;

	//读取action state transfer
	curCoacher.InitActionStateList("../../AIcoachDLL/models/action_category/actiondemo519.txt");
	cout << "InitActionStateList(inFileName, curActionTransferList): " << endl;

	//读取课程信息
	string inClassName = "../../AIcoachDLL/models/class_editor/classdemo519.txt";
	curCoacher.m_curClass.InitClassFromFile(inClassName);
	cout << "curClass.InitClassFromFile(inClassName): " << inClassName << endl;
	curCoacher.m_curClass.PrintInfo();

	curCoacher.m_curClass.ReadClassScheduleFromFile("../../AIcoachDLL/models/class_editor/classschedule519testing.txt");


	//关节点识别器
	PoseDetector curPoseDetector;
	//初始化Detector
	curPoseDetector.InitModelPara(POSE_MODEL);
	cout << "InitActionStateList(inFileName, curActionTransferList): " << POSE_MODEL << endl;


	AIclass& curClass = curCoacher.m_curClass;
	std::vector<ActionTransfer>& curActionTransferList = curCoacher.m_ActionTransferList;
	VideoIO& curVideo = curCoacher.m_videoIO;

	curCoacher.m_aicoachTimer.SetLimitTime((double)(curCoacher.m_curClass.m_durationClassSetting)); //毫秒为单位
	curCoacher.m_aicoachTimer.StartTimer();

	//开始课程
	for (int indClass = 0; indClass < curClass.m_numAction; indClass++)
	{
		if (curCoacher.m_aicoachTimer.CheckDurationFinished())
			break;

		//获取当前动作参数
		HealthAction& curHealthAction = curClass.m_curHealthActionList[indClass];
		curHealthAction.PrintInfo();
		//获取当前动作actiontransfer
		int indexAction = curHealthAction.m_paraAction.actionType;
		ActionTransfer curActionTransfer = curActionTransferList[0];
		for (size_t indAT = 0; indAT < curActionTransferList.size(); indAT++)
		{
			if (indexAction == curActionTransferList[indAT].GetActionType())
			{
				curActionTransfer = curActionTransferList[indAT];
				break;
			}
		}
		curActionTransfer.PrintInfo();

		//等待Schedule
		double curStartMillisecond = curCoacher.m_curClass.m_classScheduleList[indClass].startMilliSecond;
		while (curCoacher.m_aicoachTimer.GetCurTimer() < curStartMillisecond)
		{
			cout << "Waiting: " << curCoacher.m_aicoachTimer.GetCurTimer() << "  To: " << curStartMillisecond << endl;
			waitKey(100);
		}



		//初始化，获得初始序列
		while (curVideo.CaptureNextFrame())
		{
			curPoseDetector.CalculatePersonPose(curVideo.GetCurrentFrame());

			cur_pose_queue.push_back(curPoseDetector.GetCurrentPersonPose());

			if (cur_pose_queue.size() >= num_pose_queue)
			{
				//检测第一个状态：如果检测到了break出去，如果没有检测到，继续循环；【不需要】

				break;
			}

		}

		//begin to process for each heathaction
		int count = 0;
		int index = 0;
		curHealthAction.m_actionTimer.StartTimer();
		curHealthAction.PrintInfo();
		cout << "Starting...: " << curHealthAction.m_actionTimer.GetLimtTime() << endl;
		while (curVideo.CaptureNextFrame())
		{
			cout << "index: " << index++;// << endl;
			curPoseDetector.CalculatePersonPose(curVideo.GetCurrentFrame());

			PersonPosePara cur_pose = curPoseDetector.GetCurrentPersonPose();
			cur_pose_queue.push_back(cur_pose);

			if (cur_pose_queue.size() > num_pose_queue)
			{
				cur_pose_queue.erase(cur_pose_queue.begin());
			}


			AcionStatePara curActionState;
			curActionState = curActionTransfer.ComputeActionStateFromPosePoints(cur_pose_queue);
			for (int j = 0; j < DEFINED_NUMBER_ACTION_STATE + DEFINED_NUMBER_PERFECT_STATE; j++)
			{
				cout << curActionState.stateIndex[j] << " ";
			}


			if (1 == curActionTransfer.NextActionState(curActionState)[0])
			{
				count++;

				cout << "Count: " << count;// << endl;
			}
			cout << endl;

			if(curHealthAction.CheckDurationFinished())
			{
				cout << "Change the Health Action" << endl;
				break;
			}

			curVideo.PutPointInfo(curPoseDetector.GetCurrentPersonPose());
			curVideo.PutCountInfo(count);
			cv::Mat curImg = curVideo.GetCurrentFrame();
			cv::resize(curImg, curImg, cv::Size(0, 0), 0.75, 0.75);
			cv::imshow("TestForActionTransfer", curImg);

			waitKey(1);
		}

	}

	curVideo.DestroyInputStream();
	return true;
}

void OutputPointsHeadNeck()
{

	// Init VideoIO
	VideoIO curVideoIO = VideoIO();
	curVideoIO.InitInputStream("C:/Users/Admin/Desktop/l/data/video1/ThreeActions.mp4");

	// Init PoseDetector
	PoseDetector curPoseDetector = PoseDetector();
	curPoseDetector.InitModelPara(POSE_MODEL);

	// Capture frames from then video
	while (true == curVideoIO.CaptureNextFrame())
	{

		curPoseDetector.CalculatePersonPose(curVideoIO.GetCurrentFrame());
		
		cout << curPoseDetector.GetCurrentPersonPose().pointPose[3] - curPoseDetector.GetCurrentPersonPose().pointPose[1] << endl;

	}


}

void main()
{
	system("chcp 65001");

//	TestForActionTransfer();

	//TestForAIClass();
	
	//OutputPointsHeadNeck();
	
	// TestAIcoacherThread();

	//【5/4测试，需要准备输入视频文件actiondemo519.mp4和动作识别文件actiondemo519.txt】
	//TestForAIClassSchedule();

	//return; 
	
	AIcoacher curAIcoach;
//	cout << "AIcoacher curAIcoach;" << endl;
	cout << "开始测试" << endl;

	//【YHC-0421-1625】从文件获取测试视频路径
	curAIcoach.Init(0, "D:/GitHub/210203_AIcoach/AIcoachData/video/107/k/1001.mp4");
//	curAIcoach.Init(2, INPUT_STREAM);
//	cout << "curAIcoach.Init(0);" << endl;


	////////////////testing for AIcoachDemo////////////ok//////////////

	// curAIcoach.StartHealthClassDemo();

	// 统一使用一套代码，方便在Qt界面故障时通过cmd界面输出调试信息【YHC, 3.28】
	bool b_processing = true;
	curAIcoach.StartHealthClass();
	do
	{
		b_processing = curAIcoach.AIcoacherFrameProcessing();

		curAIcoach.GetCurrentVideoIO().PutPointInfo(curAIcoach.GetCurrentPose());
		curAIcoach.GetCurrentVideoIO().PutCountInfo(curAIcoach.GetCurrentCount());
		curAIcoach.GetCurrentVideoIO().PutFPSInfo(curAIcoach.GetCurrentFPS());

		//【YHC-0421-1626】从文件读取视频输出路径并输入函数。不可hardcode
//		curAIcoach.GetCurrentVideoIO().ShowAndSaveFrame();
		curAIcoach.GetCurrentVideoIO().ShowAndSaveFrame(GetTestPath().output_image_prefix);

	} while (b_processing);


	curAIcoach.EndHealthClass();
	cout << "curAIcoach.EndHealthClass();" << endl;


	return;
}