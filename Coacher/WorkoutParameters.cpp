#include "pch.h"
#include "WorkoutParameters.h"

bool WorkoutParameters::CosineCalc(double& cosResult, double central_x, double central_y,
	double pt1_x, double pt1_y, 
	double pt2_x, double pt2_y)
{
	double dist1 = sqrt(pow((pt1_x - central_x), 2) + pow((pt1_y - central_y), 2));
	double dist2 = sqrt(pow((pt2_x - central_x), 2) + pow((pt2_y - central_y), 2));

	if (dist1 == 0 || dist2 == 0)
	{
		cosResult = 0;
		return false;
	}

	double mul = (pt1_x - central_x) * (pt2_x - central_x) + (pt1_y - central_y) * (pt2_y - central_y);
	double cos = mul / (dist1 * dist2);
	cosResult = cos;
	return true;
}

bool WorkoutParameters::IsHandOverShoulder(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose)
{
	int dis_Head_Neck = pose[PoseDetector::NECK][PoseDetector::POINT_Y] - pose[PoseDetector::HEAD][PoseDetector::POINT_Y];
	int nose = (pose[PoseDetector::NECK][PoseDetector::POINT_Y] - pose[PoseDetector::HEAD][PoseDetector::POINT_Y]) / 2;
	int dis_neck_hip_L = pose[PoseDetector::HIP_L][PoseDetector::POINT_Y] - pose[PoseDetector::NECK][PoseDetector::POINT_Y];
	int dis_neck_hip_R = pose[PoseDetector::HIP_R][PoseDetector::POINT_Y] - pose[PoseDetector::NECK][PoseDetector::POINT_Y];
	int dis_hip_ankle_L = pose[PoseDetector::ANKLE_L][PoseDetector::POINT_Y] - pose[PoseDetector::HIP_L][PoseDetector::POINT_Y];
	int dis_hip_ankle_R = pose[PoseDetector::ANKLE_R][PoseDetector::POINT_Y] - pose[PoseDetector::HIP_R][PoseDetector::POINT_Y];

	int ankle_knee_l = pose[PoseDetector::ANKLE_L][PoseDetector::POINT_Y] - pose[PoseDetector::KNEE_L][PoseDetector::POINT_Y];
	int ankle_knee_R = pose[PoseDetector::ANKLE_R][PoseDetector::POINT_Y] - pose[PoseDetector::KNEE_R][PoseDetector::POINT_Y];

	int knee_hip_l = pose[PoseDetector::KNEE_L][PoseDetector::POINT_Y] - pose[PoseDetector::HIP_L][PoseDetector::POINT_Y];
	int knee_hip_R = pose[PoseDetector::KNEE_R][PoseDetector::POINT_Y] - pose[PoseDetector::HIP_R][PoseDetector::POINT_Y];

	float shoulder = abs(pose[PoseDetector::SHOULDER_R][PoseDetector::POINT_X] - pose[PoseDetector::SHOULDER_L][PoseDetector::POINT_X]);

	if (abs(pose[PoseDetector::WRIST_R][PoseDetector::POINT_Y] - pose[PoseDetector::SHOULDER_R][PoseDetector::POINT_Y]) < dis_Head_Neck
		&& abs(pose[PoseDetector::WRIST_L][PoseDetector::POINT_Y] - pose[PoseDetector::SHOULDER_L][PoseDetector::POINT_Y]) < dis_Head_Neck)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorkoutParameters::IsSide(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose)
{
	int dis_Head_Neck = pose[PoseDetector::NECK][PoseDetector::POINT_Y] - pose[PoseDetector::HEAD][PoseDetector::POINT_Y];
	if (abs(pose[PoseDetector::SHOULDER_L][PoseDetector::POINT_X] - pose[PoseDetector::SHOULDER_R][PoseDetector::POINT_X]) < 1.2 * dis_Head_Neck)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorkoutParameters::CosKnee(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose)
{
	double right_cos = 0;
	CosineCalc(right_cos,
		pose[PoseDetector::KNEE_R][PoseDetector::POINT_X], pose[PoseDetector::KNEE_R][PoseDetector::POINT_Y],
		pose[PoseDetector::HIP_R][PoseDetector::POINT_X], pose[PoseDetector::HIP_R][PoseDetector::POINT_Y],
		pose[PoseDetector::ANKLE_R][PoseDetector::POINT_X], pose[PoseDetector::ANKLE_R][PoseDetector::POINT_Y]);

	double left_cos = 0;
	CosineCalc(left_cos,
		pose[PoseDetector::KNEE_L][PoseDetector::POINT_X], pose[PoseDetector::KNEE_L][PoseDetector::POINT_Y],
		pose[PoseDetector::HIP_L][PoseDetector::POINT_X], pose[PoseDetector::HIP_L][PoseDetector::POINT_Y],
		pose[PoseDetector::ANKLE_L][PoseDetector::POINT_X], pose[PoseDetector::ANKLE_L][PoseDetector::POINT_Y]);

	if (right_cos - (-0.5) > 0 || left_cos - (-0.5) > 0)

	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorkoutParameters::IsKneeRaisedR(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose)
{
	int dis_Head_Neck = pose[PoseDetector::NECK][PoseDetector::POINT_Y] - pose[PoseDetector::HEAD][PoseDetector::POINT_Y];
	if (pose[PoseDetector::KNEE_L][PoseDetector::POINT_Y] - pose[PoseDetector::KNEE_R][PoseDetector::POINT_Y] > 0.8 * dis_Head_Neck
		|| pose[PoseDetector::ANKLE_L][PoseDetector::POINT_Y] - pose[PoseDetector::ANKLE_R][PoseDetector::POINT_Y] > 0.8 * dis_Head_Neck)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorkoutParameters::IsKneeRaisedL(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose)
{
	int dis_Head_Neck = pose[PoseDetector::NECK][PoseDetector::POINT_Y] - pose[PoseDetector::HEAD][PoseDetector::POINT_Y];
	if (pose[PoseDetector::KNEE_R][PoseDetector::POINT_Y] - pose[PoseDetector::KNEE_L][PoseDetector::POINT_Y] > 0.8 * dis_Head_Neck
		|| pose[PoseDetector::ANKLE_R][PoseDetector::POINT_Y] - pose[PoseDetector::ANKLE_L][PoseDetector::POINT_Y] > 0.8 * dis_Head_Neck)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorkoutParameters::IsSameSideShoulderElbowR(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose)
{
	if (pose[PoseDetector::ELBOW_R][PoseDetector::POINT_X] - pose[PoseDetector::SHOULDER_R][PoseDetector::POINT_X] > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorkoutParameters::IsSameSideShoulderElbowL(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose)
{
	if (pose[PoseDetector::ELBOW_L][PoseDetector::POINT_X] - pose[PoseDetector::SHOULDER_L][PoseDetector::POINT_X] < 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorkoutParameters::IsAnkleRaisedR(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose)
{
	int shank = pose[PoseDetector::ANKLE_L][PoseDetector::POINT_Y] - pose[PoseDetector::KNEE_L][PoseDetector::POINT_Y];
	if (pose[PoseDetector::ANKLE_L][PoseDetector::POINT_Y] - pose[PoseDetector::ANKLE_R][PoseDetector::POINT_Y] > 0.5 * shank)
	{
		return true;
	}
	else {
		return false;
	}

}

bool WorkoutParameters::IsAnkleRaisedL(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose)
{
	int shank = pose[PoseDetector::ANKLE_R][PoseDetector::POINT_Y] - pose[PoseDetector::KNEE_R][PoseDetector::POINT_Y];
	if (pose[PoseDetector::ANKLE_R][PoseDetector::POINT_Y] - pose[PoseDetector::ANKLE_L][PoseDetector::POINT_Y] > 0.5 * shank)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorkoutParameters::IsKneeOverHand(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose)
{
	if (pose[PoseDetector::KNEE_R][PoseDetector::POINT_Y] - pose[PoseDetector::WRIST_R][PoseDetector::POINT_Y] < 0
		&& pose[PoseDetector::KNEE_L][PoseDetector::POINT_Y] - pose[PoseDetector::WRIST_L][PoseDetector::POINT_Y] < 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorkoutParameters::IsArmStraightV(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose)
{
	int dis_Head_Neck = pose[PoseDetector::NECK][PoseDetector::POINT_Y] - pose[PoseDetector::HEAD][PoseDetector::POINT_Y];
	if (abs(pose[PoseDetector::SHOULDER_R][PoseDetector::POINT_X] - pose[PoseDetector::WRIST_R][PoseDetector::POINT_X]) < dis_Head_Neck
		&& abs(pose[PoseDetector::SHOULDER_L][PoseDetector::POINT_X] - pose[PoseDetector::WRIST_L][PoseDetector::POINT_X]) < dis_Head_Neck)
	{
		return true;
	}
	else {
		return false;
	}

}

bool WorkoutParameters::IsArmStraightL(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose)
{
	int dis_Head_Neck = pose[PoseDetector::NECK][PoseDetector::POINT_Y] - pose[PoseDetector::HEAD][PoseDetector::POINT_Y];
	if (abs(pose[PoseDetector::SHOULDER_L][PoseDetector::POINT_X] - pose[PoseDetector::WRIST_L][PoseDetector::POINT_X]) > dis_Head_Neck
		&& abs(pose[PoseDetector::SHOULDER_R][PoseDetector::POINT_X] - pose[PoseDetector::WRIST_R][PoseDetector::POINT_X]) > dis_Head_Neck)
	{
		return true;

	}
	else {
		return false;
	}

}

bool WorkoutParameters::IsAnkleClose(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose)
{
	int dis_Shoulder = abs(pose[PoseDetector::SHOULDER_R][PoseDetector::POINT_X] - pose[PoseDetector::SHOULDER_L][PoseDetector::POINT_X]);
	if ((pose[PoseDetector::ANKLE_L][PoseDetector::POINT_X] - pose[PoseDetector::ANKLE_R][PoseDetector::POINT_X]) <= dis_Shoulder)
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool WorkoutParameters::IsLegUpright(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose)
{
	int dis_Head_Neck = pose[PoseDetector::NECK][PoseDetector::POINT_Y] - pose[PoseDetector::HEAD][PoseDetector::POINT_Y];
	if (abs(pose[PoseDetector::KNEE_L][PoseDetector::POINT_Y] - pose[PoseDetector::HIP_L][PoseDetector::POINT_Y]) >= 0.8 * dis_Head_Neck
		&& abs(pose[PoseDetector::KNEE_R][PoseDetector::POINT_Y] - pose[PoseDetector::HIP_R][PoseDetector::POINT_Y]) >= 0.8 * dis_Head_Neck)
	{
		return true;
	}
	else
	{
		return false;
	}

}
bool WorkoutParameters::IsHandOverHead(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose)
{
	int nose = (pose[PoseDetector::NECK][PoseDetector::POINT_Y] + pose[PoseDetector::HEAD][PoseDetector::POINT_Y]) / 2;
	if (pose[PoseDetector::WRIST_R][PoseDetector::POINT_Y] - nose < 0
		&& pose[PoseDetector::WRIST_L][PoseDetector::POINT_Y] - nose < 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorkoutParameters::IsElbowHigherR(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose)
{
	if (pose[PoseDetector::ELBOW_L ][PoseDetector::POINT_Y] - pose[PoseDetector::ELBOW_R][PoseDetector::POINT_Y] > 0)
	{
		return true;
	}
	else {
		return false;
	}
}

bool WorkoutParameters::IsElbowHigherL(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose)
{
	if (pose[PoseDetector::ELBOW_R][PoseDetector::POINT_Y] - pose[PoseDetector::ELBOW_L ][PoseDetector::POINT_Y] > 0)
	{
		return true;
	}
	else {
		return false;
	}
}
