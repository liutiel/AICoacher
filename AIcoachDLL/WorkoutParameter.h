#pragma once
#include "commonincludes.h"

class WorkoutParameter
{
	public:
		void CalculateWorkoutPara(vector<PersonPosePara>& in_pose_queue);

		//计算关节点的cos值
		static bool CosineCalc(double& cosResult, double central_x, double central_y, double pt1_x, double pt1_y, double pt2_x, double pt2_y);

		//手腕是否超过肩部
		//static bool IsHandOverShoulder(int dis, vector<PersonPosePara>& in_pose_queue);
		static bool IsHandOverShoulder(vector<PersonPosePara>& in_pose_queue);

		//判断双腿是否打开
		//static bool IsLegsOpen(vector<PersonPosePara>& in_pose_queue);
		/////////////新增函数       判断侧身    
		static bool IsSide(vector<PersonPosePara>& in_pose_queue);

		//计算膝盖的cos值
		static bool Cos_Knee(vector<PersonPosePara>& in_pose_queue);

		//判断右膝盖是否抬起
		static bool IsKnee_R_Raised(vector<PersonPosePara>& in_pose_queue);

		//判断左膝盖是否抬起
		static bool IsKnee_L_Raised(vector<PersonPosePara>& in_pose_queue);

		//判断右肩膀和右手肘不在一侧
		static bool IsSideShouldeElbow_R(vector<PersonPosePara>& in_pose_queue);

		//判断左肩膀和左手肘不在一侧
		static bool IsSideShouldeElbow_L(vector<PersonPosePara>& in_pose_queue);

		//判断脚踝的距离
		static bool IsAnkle_R_Raised(vector<PersonPosePara>& in_pose_queue);

		//判断脚踝的距离
		static bool IsAnkle_L_Raised(vector<PersonPosePara>& in_pose_queue);

		//判断膝盖是否高于手腕
		static bool IsKneeOverHand(vector<PersonPosePara>& in_pose_queue);

		//////////////////////////////////////新添加函数///////////////////////
		//手臂是否处于竖直平面上
		static bool IsArm_Vline(vector<PersonPosePara>& in_pose_queue);

		//手臂是否处于水平平面上
		static bool IsArm_Lline(vector<PersonPosePara>& in_pose_queue);

		//判断脚是否并拢
		static bool IsAnkleClose(vector<PersonPosePara>& in_pose_queue);

		//判断腿直立
		static bool IsLegUpright(vector<PersonPosePara>& in_pose_queue);

		static bool IsHandOverHead(vector<PersonPosePara>& in_pose_queue);

		//右手肘高于左手肘
		static bool IsElbow_R_Higher(vector<PersonPosePara>& in_pose_queue);

		//左手肘高于右手肘
		static bool IsElbow_L_Higher(vector<PersonPosePara>& in_pose_queue);

};


