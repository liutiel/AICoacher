#pragma once
#include "FaceDetector.h"
#include "ActionRecognizer.h"
#include "AIclass.h"
#include "HealthAction.h"


struct HealthPersonStatus
{

};

class HealthPerson
{
private:
	int m_curPersonID;//按某种规整分配一个ID；
	//存储人脸模型做人脸比对；【可以放在第二顺位做】；

	//当前的动作识别器
	ActionRecognizer m_curActionRecognizer;

private:
	//当前的健身课程
	AIclass* m_pCurrentAIclass;

	//当前的健身动作
	HealthAction* m_pCurrentHealthAction;

public:
	HealthPerson();
	~HealthPerson();

	void SetCurrentAIclass(AIclass* pInputAIclass);
	void SetCurrentHealthAction(HealthAction* pInputHealthAction);
	void SetCurrentHealthAction(HealthAction& inputHealthAction);
	
	//视频IO保存在动作识别器里面
	void Init(VideoIO* inVideoIO);

	HealthAction * GetCurrentHealthAction();

	AIclass *GetCurrentAIclass();

	ActionRecognizer * GetCurrentActionRecognizer();

public:
	//还没有实现
	bool StartActionRecognizeThread();

	//执行一次骨骼点提取
	bool SkeletonPointExtractOnce();

	//执行一次动作识别
	bool ActionRecognizeOnce();
};

