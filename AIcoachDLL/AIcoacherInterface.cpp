#include "AIcoacherInterface.h"
#include "AIcoacher.h"
#include <thread>
//
//std::thread* pAIcoacherThread = NULL;

HANDLE* AIcoacherInterface::AIcoacherCreate( pFuncAIcoachCount pFunC)
{
	AIcoacher* pAIcoacher = new AIcoacher();

	cout << "Test for callback function settings!" << endl;

	pAIcoacher->pCallBackAIcoachCount = pFunC;
	
	return (HANDLE*)pAIcoacher;
}

bool AIcoacherInterface::AIcoacherInitialize(HANDLE* pCurAIcoacher, int inVideoIndex, char* inClassName)
{
	AIcoacher* pAIcoacher = (AIcoacher*)pCurAIcoacher;

	pAIcoacher->AIcoacherInitialize(inVideoIndex,inClassName);

	//	pAIcoacherThread = new thread(&AIcoacher::AIcoacherInitialize, pAIcoacher, inVideoIndex, (const char*)inClassName);

	return true;
}


//bool AIcoacherInterface::AIcoacherUpdateClass(HANDLE* pCurAIcoacher, char* inClassName)
//{
//	AIcoacher* pAIcoacher = (AIcoacher*)pCurAIcoacher;
//
//	pAIcoacher->AIcoacherUpdateClass((const char*)inClassName);
//
//	return true;
//}


//AIcoacher开始工作
//输入为句柄，启动工作
bool AIcoacherInterface::AIcoacherStart(HANDLE* pCurAIcoacher)
{
	AIcoacher* pAIcoacher = (AIcoacher*)pCurAIcoacher;

	pAIcoacher->AIcoacherStart();

	return true;
}


//AIcoacher暂停工作（停止和暂停是等同的，对新课程要启动新的AIcoacher
//输入为句柄，暂停工作
bool AIcoacherInterface::AIcoacherSuspend(HANDLE* pCurAIcoacher)
{
	AIcoacher* pAIcoacher = (AIcoacher*)pCurAIcoacher;

	pAIcoacher->AIcoacherSuspend();

	return true;
}



//退出AIcoacher
//输入为句柄，退出相应资源
bool AIcoacherInterface::AIcoacherExit(HANDLE* pCurAIcoacher)
{
	AIcoacher* pAIcoacher = (AIcoacher*)pCurAIcoacher;

	//释放AIcoacher资源
	pAIcoacher->AIcoacherExit();

	delete pAIcoacher;

	return true;
}



//输出AIcoacher当前课程的报告【设计未完成，2021-04-08】
//输入为句柄，退出报告格式待定【也许是pdf文档，或者是数据结构，在API外再生产pdf文档】
bool AIcoacherInterface::AIcoacherReport(HANDLE* pCurAIcoacher, char* outReportFileName)
{
	AIcoacher* pAIcoacher = (AIcoacher*)pCurAIcoacher;

	//not designed 

	return true;
}

//void AIcoacherInterface::SetCallbackFunction(HANDLE* pCurAIcoacher, pFuncAIcoachCount & pFunC)
//{
//	cout << "****************************************callback function**************************************" << endl;
//
//
//	AIcoacher* pAIcoacher = (AIcoacher*)pCurAIcoacher;
//
////	pAIcoacher->SetCallbackFunction(pFunC);
//
////	pAIcoacher->pCallBackAIcoachCount = pFunC;
//
//	pFuncAIcoachCC = pFunC;
//
////	return 0;
//}