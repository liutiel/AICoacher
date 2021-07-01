#include "HealthPerson.h"

HealthPerson::HealthPerson()
{
	m_pCurrentAIclass = NULL;
	m_pCurrentHealthAction = NULL;
}

HealthPerson::~HealthPerson()
{

}

void HealthPerson::Init(VideoIO* inVideoIO)
{
	m_curActionRecognizer.Init(inVideoIO);
}

void HealthPerson::SetCurrentAIclass(AIclass * pInputAIclass) 
{ 
	m_pCurrentAIclass = pInputAIclass;
}

void HealthPerson::SetCurrentHealthAction(HealthAction *pInputHealthAction) 
{ 
	m_pCurrentHealthAction = pInputHealthAction;
}

void HealthPerson::SetCurrentHealthAction(HealthAction &inputHealthAction)
{
	m_pCurrentHealthAction = &inputHealthAction;
}


HealthAction * HealthPerson::GetCurrentHealthAction() 
{
	return m_pCurrentHealthAction;
}

AIclass *HealthPerson::GetCurrentAIclass()
{
	return m_pCurrentAIclass;
}

ActionRecognizer * HealthPerson::GetCurrentActionRecognizer()
{
	return &m_curActionRecognizer;
}

//启动线程做动作识别；
bool HealthPerson::StartActionRecognizeThread()
{
	return true;
}


bool HealthPerson::SkeletonPointExtractOnce()
{
	return m_curActionRecognizer.CalculatePersonPose();
}

bool HealthPerson::ActionRecognizeOnce()
{
	////如果动作识别为真，计数器+1

	//if (true == m_curActionRecognizer.ExecuteActionRecognize(m_pCurrentHealthAction->m_paraAction.actionType))
	//{
	//	m_pCurrentHealthAction->m_actionCounter.CountOnce();
	//}
	int get_current_state = 0;
//	cout << "m_pCurrentHealthAction->m_paraAction.actionType:  " << m_pCurrentHealthAction->m_paraAction.actionType << endl;

	//if (true == m_curActionRecognizer.ExecuteActionRecognize(m_pCurrentHealthAction->m_paraAction.actionType, get_current_state))
//	if (true == m_curActionRecognizer.ExecuteActionRecognize(1, get_current_state))
	if (true == m_curActionRecognizer.ExecuteActionRecognize(m_pCurrentHealthAction->m_paraAction.actionType))
	{
		m_pCurrentHealthAction->m_actionCounter.CountOnce();
	}
	
	return true;
}

