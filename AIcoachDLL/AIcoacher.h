#pragma once
/**************************************************/
//AICoacher, manages the following information:

//1. Course selection (AIClass)

//2. Fitness personnel management;

//3. Real time management of fitness course
/**************************************************/
#include <vector>
#include "AIclass.h"
#include "HealthPerson.h"
#include "VideoIO.h"
#include "ActionTransfer.h"
#include <queue>
#include <thread>

typedef enum {
	AICOACHER_INITIALIZE,
	AICOACHER_SUSPEND,  // SUSPEND 
	AICOACHER_RUNNING,
	AICOACHER_EXIT,
}eAIcoacherThreadStatus;

class AIcoacher
{

public:

	AIclass m_curClass;
	std::vector<AIclass> m_preClass;
	VideoIO m_videoIO;
	std::vector<ActionTransfer> m_ActionTransferList;
	PoseDetector m_PoseDetector;
	CoachTimer m_aicoachTimer;
	vector<PersonPosePara> m_pose_queue; //【replace the pose queque definition in PoseTracker】
	ActionTransfer m_current_transfer;
	HealthAction m_HealthAction;


	/***********           legency design           ******/
	int m_curClassID;
	float m_durationStart;
	float m_durationEnd;
	float m_durationCurrent;
	double m_curfps;
	double m_curTimer;




public:
	AIcoacher();
	~AIcoacher();

	int m_pose_queue_num = 1;

	bool Init2(int inClassID, std::string input_stream); // Init the class setting

	bool StartHealthClass();

	bool SuspendHealthClass();// pause training

	bool EndHealthClass();// stop training

	bool AIcoacherFrameProcessing();
	bool AIcoacherFrameProcessing2();

	VideoIO GetCurrentVideoIO();

	// Get the data
	PersonPosePara GetCurrentPose();

	int GetCurrentCount();
	double GetCurTimer();
	double GetCurrentFPS();

	// Get the limits
	int GetCountLimit();
	double GetTimeLimit();

	bool IsActionFinished();
	int GetCurClassID();
	int GetActionPrompt();

	//for AIcoacherInterface
	bool AIcoacherInitialize(int inVideoIndex = -1, const char* inClassName = NULL);

	bool AIcoacherUpdateClass(const char* inClassName);
	
	bool AIcoacherStart();
	
	bool AIcoacherSuspend();
	
	bool AIcoacherExit();

	// Init course and state transfer from file
	bool AIcoacherActionTransferInit(HealthAction curHealthAction, ActionTransfer& curActionTransfer);



	//thread design at 2021-04-21 by Tie LIU
private:
	std::thread m_VideoCaptureThread; //video capture
	std::thread m_AICoacherThread; //coacher
	std::thread m_PoseDetectorThread; //posedetector

	std::mutex              m_Mutex;
	std::mutex              m_Mutex4Queue;
	std::mutex              m_Mutex4ThreadsStatus;


	std::queue<cv::Mat>      m_DataQueue;    // Queue for communication between threads   
	std::condition_variable m_ConVar;//not used 

	eAIcoacherThreadStatus  m_AIcoacherThreadStatus;
	eAIcoacherThreadStatus getThreadsStatus();
	void setThreadsStatus(eAIcoacherThreadStatus status);

	void VideoCaptureThread();
	void AICoacherThread();
	bool InitializePoseQueue(); 
	bool GetActionTransfer(int indexAction, ActionTransfer& out_action_transfer);


	bool isDataQueueEmpty();
	bool isDataQueueNotEmpty();
	void pushDataToQueue(const cv::Mat& inData);
	bool popDataFromQueue(cv::Mat& outData);

public:
	//////////////////////////////Callback Function
//	static int SetCallbackFunction(pFuncAIcoachCount pFunC);

	 void (*pCallBackAIcoachCount)(int* nCount, char* cPrompt);

public:
	bool InitActionStateList(string inFilename, vector<ActionTransfer>& curActionTransferList);


};

