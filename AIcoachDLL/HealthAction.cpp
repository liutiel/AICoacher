#include "HealthAction.h"


HealthAction::HealthAction()
{
	//m_countAction = 0;
	//m_durationAction = 0;
	
	//m_countActionSetting = 0;
	//m_durationActionSetting = 0;

	InitHealthAction(1);
		

}
HealthAction::~HealthAction()
{
	//delete m_pActionCounter;
	//delete m_pActionTimer;
}

void HealthAction::InitHealthAction(int inPara)
{
	m_paraAction.actionType = inPara;
	m_actionCounter.ResetCounter();
}

void HealthAction::SetCountAction(int inCount) 
{
	m_actionCounter.SetLimitCount(inCount);
}

void HealthAction::SetDurationAction(double inDuration) 
{ 
	m_actionTimer.SetLimitTime(inDuration);
}

void HealthAction::StartCounter() 
{ 
	m_actionCounter.ResetCounter(); 
}

void HealthAction::StartTimer() 
{ 
	m_actionTimer.StartTimer(); 
}



bool HealthAction::CheckCountFinished()
{
	return m_actionCounter.CheckCountFinished();
}

bool HealthAction::CheckDurationFinished() 
{
	return m_actionTimer.CheckDurationFinished();
}

int HealthAction::GetCurrentCount()
{
	return m_actionCounter.GetCurrentCount();
}
void HealthAction::PrintInfo()
{
	cout << m_paraAction.actionName << "   " << m_paraAction.actionType << "   "  
		<< m_paraAction.actionLimitNum << "   " << m_paraAction.actionLimitSecond << "   " << endl;

}
