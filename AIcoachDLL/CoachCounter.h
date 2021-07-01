#pragma once

// Counting for the training


class CoachCounter
{
private:
	int m_startCount;
	int m_endCount;
	int m_currentCount;
	int m_LimitCount=0;

public:
	CoachCounter();
	~CoachCounter();

	int GetCurrentCount() ;
	int GetRemainCount() ;
	int GetLimtCount();
	void SetLimitCount(int inLimitCount) ;

	void CountOnce();
	void ResetCounter();
	bool CheckCountFinished();


};

