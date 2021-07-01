#include "CoachCounter.h"


CoachCounter::CoachCounter()
{
	m_currentCount = 0;
}
CoachCounter::~CoachCounter()
{

}

int CoachCounter::GetCurrentCount() 
{ 
	return m_currentCount; 
}

int CoachCounter::GetRemainCount()
{ 
	return m_LimitCount - m_currentCount; 
}

int CoachCounter::GetLimtCount()
{ 
	return m_LimitCount; 
}

void CoachCounter::SetLimitCount(int inLimitCount) 
{ 
	m_LimitCount = inLimitCount; 
}

void CoachCounter::CountOnce() 
{ 
	m_currentCount++; 
}

void CoachCounter::ResetCounter() 
{ 
	m_currentCount = 0; 
}

bool CoachCounter::CheckCountFinished()
{
	if (m_currentCount >= m_LimitCount && m_LimitCount>0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

