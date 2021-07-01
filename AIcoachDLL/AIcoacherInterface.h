#pragma once
#include <Windows.h>
#ifndef _AICOACHERDLL_API_  
#define _AICOACHERDLL_API_ extern "C" _declspec(dllexport)  
#endif  

typedef void (*pFuncAIcoachCount)(int* nCount, char* cPrompt);

class  _declspec(dllexport) AIcoacherInterface
{
public:
	//创建AIcoacher
	//输入为回调函数（输出动作计数和动作提示），输出返回句柄（传输给后面四个函数）；
	static HANDLE* AIcoacherCreate(pFuncAIcoachCount pFunC);
	
	//启动初始化AIcoacher
	//输入为句，为video，课程配置文件；
	static bool AIcoacherInitialize(HANDLE* pCurAIcoacher,int inVideoIndex, char* inClassName=NULL);

	//AIcoacher更新课程
	////输入为句柄，更新课程
	//static bool AIcoacherUpdateClass(HANDLE* pCurAIcoacher, char* inClassName);

	//AIcoacher开始工作
	//输入为句柄，启动工作
	static bool AIcoacherStart(HANDLE* pCurAIcoacher);

	//AIcoacher暂停工作（停止和暂停是等同的，对新课程要启动新的AIcoacher
	//输入为句柄，暂停工作
	static bool AIcoacherSuspend(HANDLE* pCurAIcoacher);


	//退出AIcoacher
	//输入为句柄，退出相应资源
	static bool AIcoacherExit(HANDLE* pCurAIcoacher);


	//输出AIcoacher当前课程的报告【设计未完成，2021-04-08】
	//输入为句柄，退出报告格式待定【也许是pdf文档，或者是数据结构，在API外再生产pdf文档】
	static bool AIcoacherReport(HANDLE* pCurAIcoacher, char* outReportFileName);


	////////////////////////////////Callback Function
	//static void SetCallbackFunction(HANDLE* pCurAIcoacher,pFuncAIcoachCount & pFunC);
};

