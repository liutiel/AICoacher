#pragma once
#include "commonincludes.h"
//动作单元，比如跳跃运动，有很多个重复的HealthActionUnit组成，而HealthActionUnit包含一个完整的但不重复的动作

struct HealthActionType
{
	int HealthActionID;
	std::string HealActionName;
	int HealthActionCategory;
};



class HealthActionUnit
{

};

