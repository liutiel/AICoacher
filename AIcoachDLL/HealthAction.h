#pragma once
#include "HealthActionUnit.h"
#include "CoachTimer.h"
#include "CoachCounter.h"

struct HealthActionPara
{
	string actionName;
	
	int actionType;
	
	int actionLimitNum;

	float actionLimitSecond;
};

class HealthAction
{
public:
	CoachCounter m_actionCounter;

	CoachTimer m_actionTimer; 

	HealthActionUnit m_unitAction;  

	HealthActionPara m_paraAction;  

public:
	HealthAction();
	~HealthAction();

	void InitHealthAction(int inPara);

	void SetCountAction(int inCount);

	void SetDurationAction(double inDuration) ;

	void StartCounter();
	void StartTimer();

	bool CheckCountFinished();

	bool CheckDurationFinished();

	int GetCurrentCount();

	void PrintInfo();
};

