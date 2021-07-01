/////////////////////////////////////////////////////////////////////////
//Class PoseChecker: 
//Function: check the pose points after the PoseDetector 
//Author: Tie LIU
//Created at 2021-03-31
///////////////////////////////////////////////////////////////////////

#pragma once
#include "commonincludes.h"


struct PersonPara
{
	//person body parameters, computed from the pose points at the beginning 

};
class PoseChecker
{
private:
	PersonPara* m_personPara;

public:
	void SetPersonPara(PersonPara& inPersonPara);

	bool PoseCheck(PersonPosePara& inPersonPose);

};

