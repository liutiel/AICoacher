#include "WorkoutParameter.h"
bool WorkoutParameter::CosineCalc(double& cosResult, double central_x, double central_y,
	double pt1_x, double pt1_y, double pt2_x, double pt2_y)
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

bool WorkoutParameter::IsHandOverShoulder(vector<PersonPosePara>& in_pose_queue)
{
	PersonPosePara pose = in_pose_queue[0];
	int dis_Head_Neck = pose.pointPose[NECK * 2 + 1] - pose.pointPose[HEAD * 2 + 1];
	int nose= (pose.pointPose[NECK * 2 + 1] - pose.pointPose[HEAD * 2 + 1])/2;
	int dis_neck_hip_L = pose.pointPose[HIP_L * 2 + 1] - pose.pointPose[NECK * 2 + 1];
	int dis_neck_hip_R = pose.pointPose[HIP_R * 2 + 1] - pose.pointPose[NECK * 2 + 1];
	int dis_hip_ankle_L= pose.pointPose[ANKLE_L * 2 + 1] - pose.pointPose[HIP_L * 2 + 1];
	int dis_hip_ankle_R= pose.pointPose[ANKLE_R * 2 + 1] - pose.pointPose[HIP_R * 2 + 1];

	int ankle_knee_l= pose.pointPose[ANKLE_L * 2 + 1] - pose.pointPose[KNEE_L * 2 + 1];
	int ankle_knee_R = pose.pointPose[ANKLE_R * 2 + 1] - pose.pointPose[KNEE_R * 2 + 1];

	int knee_hip_l = pose.pointPose[KNEE_L * 2 + 1] - pose.pointPose[HIP_L * 2 + 1];
	int knee_hip_R = pose.pointPose[KNEE_R * 2 + 1] - pose.pointPose[HIP_R * 2 + 1];

	float shoulder= abs(pose.pointPose[SHOULDER_R * 2 ] - pose.pointPose[SHOULDER_L * 2]);

	cout <<  dis_Head_Neck;

	
	if (abs(pose.pointPose[WRIST_R * 2 + 1] - pose.pointPose[SHOULDER_R * 2 + 1]) < dis_Head_Neck
		&& abs(pose.pointPose[WRIST_L * 2 + 1] - pose.pointPose[SHOULDER_L * 2 + 1]) < dis_Head_Neck)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorkoutParameter::IsSide(vector<PersonPosePara>& in_pose_queue)
{
	PersonPosePara pose = in_pose_queue[0];
	int dis_Head_Neck = pose.pointPose[NECK * 2 + 1] - pose.pointPose[HEAD * 2 + 1];
	if (abs(pose.pointPose[SHOULDER_L * 2] - pose.pointPose[SHOULDER_R * 2]) < 1.2 * dis_Head_Neck)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorkoutParameter::Cos_Knee(vector<PersonPosePara>& in_pose_queue)
{

	PersonPosePara pose = in_pose_queue[0];

	double right_cos = 0;
	CosineCalc(right_cos,
		pose.pointPose[KNEE_R * 2], pose.pointPose[KNEE_R * 2 + 1],
		pose.pointPose[HIP_R * 2], pose.pointPose[HIP_R * 2 + 1],
		pose.pointPose[ANKLE_R * 2], pose.pointPose[ANKLE_R * 2 + 1]);

	double left_cos = 0;
	CosineCalc(left_cos,
		pose.pointPose[KNEE_L * 2], pose.pointPose[KNEE_L * 2 + 1],
		pose.pointPose[HIP_L * 2], pose.pointPose[HIP_L * 2 + 1],
		pose.pointPose[ANKLE_L * 2], pose.pointPose[ANKLE_L * 2 + 1]);

	if (right_cos - (-0.5) > 0 || left_cos - (-0.5) > 0)

	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorkoutParameter::IsKnee_R_Raised(vector<PersonPosePara>& in_pose_queue)
{
	PersonPosePara pose = in_pose_queue[0];
	int dis_Head_Neck = pose.pointPose[NECK * 2 + 1] - pose.pointPose[HEAD * 2 + 1];
	if (pose.pointPose[KNEE_L * 2 + 1] - pose.pointPose[KNEE_R * 2 + 1] >0.8* dis_Head_Neck
		|| pose.pointPose[ANKLE_L * 2 + 1] - pose.pointPose[ANKLE_R * 2 + 1] >0.8 * dis_Head_Neck)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorkoutParameter::IsKnee_L_Raised(vector<PersonPosePara>& in_pose_queue)
{
	PersonPosePara pose = in_pose_queue[0];
	int dis_Head_Neck = pose.pointPose[NECK * 2 + 1] - pose.pointPose[HEAD * 2 + 1];
	if (pose.pointPose[KNEE_R * 2 + 1] - pose.pointPose[KNEE_L * 2 + 1] > 0.8* dis_Head_Neck
		|| pose.pointPose[ANKLE_R * 2 + 1] - pose.pointPose[ANKLE_L * 2 + 1] >0.8* dis_Head_Neck)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorkoutParameter::IsSideShouldeElbow_R(vector<PersonPosePara>& in_pose_queue)
{
	PersonPosePara pose = in_pose_queue[0];
	if (pose.pointPose[ELBOW_R * 2] - pose.pointPose[SHOULDER_R * 2] > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorkoutParameter::IsSideShouldeElbow_L(vector<PersonPosePara>& in_pose_queue)
{
	PersonPosePara pose = in_pose_queue[0];
	if (pose.pointPose[ELBOW_L * 2] - pose.pointPose[SHOULDER_L * 2] < 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorkoutParameter::IsAnkle_R_Raised(vector<PersonPosePara>& in_pose_queue)
{
	PersonPosePara pose = in_pose_queue[0];
	int shank = pose.pointPose[ANKLE_L * 2 + 1] - pose.pointPose[KNEE_L * 2 + 1];
	if (pose.pointPose[ANKLE_L * 2 + 1] - pose.pointPose[ANKLE_R * 2 + 1] > 0.5 * shank)
	{
		return true;
	}
	else {
		return false;
	}

}

bool WorkoutParameter::IsAnkle_L_Raised(vector<PersonPosePara>& in_pose_queue)
{
	PersonPosePara pose = in_pose_queue[0];
	int shank = pose.pointPose[ANKLE_R * 2 + 1] - pose.pointPose[KNEE_R * 2 + 1];
	if (pose.pointPose[ANKLE_R * 2 + 1] - pose.pointPose[ANKLE_L * 2 + 1] > 0.5 * shank)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorkoutParameter::IsKneeOverHand(vector<PersonPosePara>& in_pose_queue)
{
	PersonPosePara pose = in_pose_queue[0];
	if (pose.pointPose[KNEE_R * 2 + 1] - pose.pointPose[WRIST_R * 2 + 1] < 0
		&& pose.pointPose[KNEE_L * 2 + 1] - pose.pointPose[WRIST_L * 2 + 1] < 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorkoutParameter::IsArm_Vline(vector<PersonPosePara>& in_pose_queue)
{
	PersonPosePara pose = in_pose_queue[0];
	int dis_Head_Neck = pose.pointPose[NECK * 2 + 1] - pose.pointPose[HEAD * 2 + 1];
	if (abs(pose.pointPose[SHOULDER_R * 2] - pose.pointPose[WRIST_R * 2]) < dis_Head_Neck
		&& abs(pose.pointPose[SHOULDER_L * 2] - pose.pointPose[WRIST_L * 2]) < dis_Head_Neck)
	{
		return true;
	}
	else {
		return false;
	}

}

bool WorkoutParameter::IsArm_Lline(vector<PersonPosePara>& in_pose_queue)
{
	PersonPosePara pose = in_pose_queue[0];
	int dis_Head_Neck = pose.pointPose[NECK * 2 + 1] - pose.pointPose[HEAD * 2 + 1];
	if (abs(pose.pointPose[SHOULDER_L * 2] - pose.pointPose[WRIST_L * 2]) > dis_Head_Neck
		&& abs(pose.pointPose[SHOULDER_R * 2] - pose.pointPose[WRIST_R * 2]) > dis_Head_Neck)
	{
		return true;

	}
	else {
		return false;
	}

}

bool WorkoutParameter::IsAnkleClose(vector<PersonPosePara>& in_pose_queue)
{
	PersonPosePara pose = in_pose_queue[0];
	int dis_Shoulder = abs(pose.pointPose[SHOULDER_R * 2] - pose.pointPose[SHOULDER_L * 2]);
	if ((pose.pointPose[ANKLE_L * 2] - pose.pointPose[ANKLE_R * 2]) <= dis_Shoulder)
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool WorkoutParameter::IsLegUpright(vector<PersonPosePara>& in_pose_queue)
{
	PersonPosePara pose = in_pose_queue[0];
	int dis_Head_Neck = pose.pointPose[NECK * 2 + 1] - pose.pointPose[HEAD * 2 + 1];
	if (abs(pose.pointPose[KNEE_L * 2 + 1] - pose.pointPose[HIP_L * 2 + 1]) >= 0.8 * dis_Head_Neck
		&& abs(pose.pointPose[KNEE_R * 2 + 1] - pose.pointPose[HIP_R * 2 + 1]) >= 0.8 * dis_Head_Neck)
	{
		return true;
	}
	else
	{
		return false;
	}

}
bool WorkoutParameter::IsHandOverHead(vector<PersonPosePara>& in_pose_queue)
{

	PersonPosePara pose = in_pose_queue[0];
	int nose = (pose.pointPose[NECK * 2 + 1] + pose.pointPose[HEAD * 2 + 1]) / 2;
	if (pose.pointPose[WRIST_R * 2 + 1] - nose < 0
		&& pose.pointPose[WRIST_L * 2 + 1] - nose < 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool WorkoutParameter::IsElbow_R_Higher(vector<PersonPosePara>& in_pose_queue)
{
	PersonPosePara pose = in_pose_queue[0];
	if (pose.pointPose[ELBOW_L * 2 + 1] - pose.pointPose[ELBOW_R * 2 + 1] > 0)
	{
		return true;
	}
	else {
		return false;
	}
}

bool WorkoutParameter::IsElbow_L_Higher(vector<PersonPosePara>& in_pose_queue)
{
	PersonPosePara pose = in_pose_queue[0];
	if (pose.pointPose[ELBOW_R * 2 + 1] - pose.pointPose[ELBOW_L * 2 + 1] > 0)
	{
		return true;
	}
	else {
		return false;
	}
}

