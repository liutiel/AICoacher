#pragma once
//Timing for the training
//Designed tat 2021-03-16 by Tie LIU

#include<stdio.h>
#include<time.h>


class CoachTimer
{
private:
	double m_startTime;
	double m_endTime;
	double m_currentTime;

	//status: 1: timing；0：suspending；
	int b_currentStatus; 

	double m_durationTime; 
	double m_remainTime;
	double m_LimitTime=0.0; 


public:
	CoachTimer();
	~CoachTimer();

	double GetCurTimer();
	double GetRemainTime();
	double GetLimtTime();
	void SetLimitTime(double inLimitTime);
	
	void StartTimer();
	void EndTimer();
	void SuspendTimer();
	void ResumeTimer();

	bool CheckDurationFinished();
};

