#pragma once
//Defining AI classes

#include "commonincludes.h"

#include "HealthAction.h"

struct AIClassSchedule
{
	string actionName;
	int actionType;
	int startMilliSecond;
};
class AIclass
{
private:
	string m_className;
	int m_classNo;
	int m_currentAction;


public:
	bool InitClassFromFile(string inClassFileName);

public:
	int m_numAction;  

	std::vector<HealthAction> m_curHealthActionList;

	float m_durationClassSetting;

	std::vector <AIClassSchedule> m_classScheduleList;

public:
	AIclass();
	~AIclass();
	bool ReadHealthActionFile(string inFileName);

	bool AddHealthAction(HealthAction inHealthAction);

	bool SetClass(int inClassID);

	bool GetHealthActionList(std::vector<HealthAction>& outHealthActionList);

	void PrintInfo();

	void ReadClassScheduleFromFile(string inClassScheduleFileName);
};

