#include "CoachTimer.h"


CoachTimer::CoachTimer()
{
	b_currentStatus = 0;

}

CoachTimer::~CoachTimer()
{

}


double CoachTimer::GetCurTimer()
{
	m_currentTime = clock();
	return (m_currentTime- m_startTime) * 1000.0f / CLOCKS_PER_SEC; //毫秒为单位
}

double CoachTimer::GetRemainTime() 
{
	m_currentTime = clock();
	return  m_LimitTime - (m_currentTime - m_startTime) * 1000.0f / CLOCKS_PER_SEC; //毫秒为单位

}

double CoachTimer::GetLimtTime() 
{ 
	return m_LimitTime;
}

void CoachTimer::SetLimitTime(double inLimitTime) 
{ 
	m_LimitTime = inLimitTime; 
}


void CoachTimer::StartTimer()
{
	m_startTime = clock();
	m_durationTime = 0;
	
	b_currentStatus = 1;
}

void CoachTimer::EndTimer()
{
	m_currentTime = clock();

	m_durationTime = (m_currentTime - m_startTime) * 1000.0f / CLOCKS_PER_SEC; //毫秒为单位

	b_currentStatus = 0;
}

void CoachTimer::SuspendTimer()
{
	b_currentStatus = 0;
}

void CoachTimer::ResumeTimer()
{
	b_currentStatus = 0;
}

bool CoachTimer::CheckDurationFinished()
{
	m_currentTime = clock();
	m_durationTime = (m_currentTime - m_startTime) * 1000.0f / CLOCKS_PER_SEC;

	if (m_durationTime >= m_LimitTime && m_LimitTime>0)
	{
		return true;
	}
	else
	{
		return false;
	}
}