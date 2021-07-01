#pragma once
#include "PoseDetector.h"

//功能函数的集合

class ActionRecognizerUtil
{


public: //函数
	//static bool cosineCalc(double& cosResult, double central_x, double central_y, double pt1_x, double pt1_y, double pt2_x, double pt2_y);
	//static bool averageCosineCalc(int left_central, int left_pt1, int left_pt2, int right_central, int right_pt1, int right_pt2,
	//	vector<PersonPosePara>& in_pose_queue, int &current_state);
	static bool cosineCalc(double& cosResult, double central_x, double central_y, double pt1_x, double pt1_y, double pt2_x, double pt2_y);
	static double averageCosineCalc(int left_central, int left_pt1, int left_pt2, int right_central, int right_pt1, int right_pt2,
		vector<PersonPosePara>& in_pose_queue);

	// suki_01
	static bool isHandsOverHead(vector<PersonPosePara>& in_pose_queue);
	static bool isLegsOpen(vector<PersonPosePara>& in_pose_queue);

	// suki_02--------------------------------【YHC-3.30】
	static bool isHandsOverShoulders(vector<PersonPosePara>& in_pose_queue);
	static bool isElbowShoulderSameSide(int hand, int elbow, int shoulder, vector<PersonPosePara>& in_pose_queue);
	static bool isKneeRaised(int hip, int knee, int ankle, vector<PersonPosePara>& in_pose_queue);

	//计算动作次数【测试函数】
	static bool CalculateActionCount(vector<PersonPosePara> & in_pose_queue, int inActionType, int &current_state, int &current_prompt);

	// ------------------------------------【3.30 YHC：废弃】
	// static bool CalculateActionCount(vector<PersonPosePara>& in_pose_queue, int inActionType);


	static bool isKneeAnkleAligned(vector<PersonPosePara>& in_pose_queue);
	static bool isThighLongerThanShank(vector<PersonPosePara>& in_pose_queue);


};

