#include "ActionRecognizerUtil.h"

// 结果输出到cosResult中。如果出现除0等异常情况，返回false。
bool ActionRecognizerUtil::cosineCalc(double& cosResult, double central_x, double central_y, 
	double pt1_x, double pt1_y, double pt2_x, double pt2_y)
{
	// 计算模长
	double dist1 = sqrt(pow((pt1_x - central_x), 2) + pow((pt1_y - central_y), 2));
	double dist2 = sqrt(pow((pt2_x - central_x), 2) + pow((pt2_y - central_y), 2));

	// 防止除0
	if (dist1 == 0 || dist2 == 0)
	{
		cosResult = 0;
		return false;
	}

	// 计算向量积
	double mul = (pt1_x - central_x) * (pt2_x - central_x) + (pt1_y - central_y) * (pt2_y - central_y);

	// 计算夹角余弦
	double cos = mul / (dist1 * dist2);
	cosResult = cos;
	return true;
}

// 前六个参数是指定使用每一侧使用哪三个点来计算夹角，in_pose_queue是最近5帧里面的关节点序列
// current_state为-1时标记当前为【手臂伸直】状态，current_state为1时标记当前为【手臂弯曲】状态
double ActionRecognizerUtil::averageCosineCalc(int left_central, 
	int left_pt1, int left_pt2, int right_central, int right_pt1, int right_pt2,
	vector<PersonPosePara>& in_pose_queue
	)
{
	double left_cos_sum = 0;
	double right_cos_sum = 0;
	for (int j = 0; j < in_pose_queue.size(); j++)
	{
		PersonPosePara pose = in_pose_queue[j];

		// 对最近5帧的需要求的夹角余弦求和
		double left_cos = 0;
		if(cosineCalc(left_cos,
			pose.pointPose[left_central*2], pose.pointPose[left_central*2+1],
			pose.pointPose[left_pt1*2], pose.pointPose[left_pt1*2+1],
			pose.pointPose[left_pt2*2], pose.pointPose[left_pt2*2+1]))
		{
			left_cos_sum += left_cos;
		}
		double right_cos = 0;
		if(cosineCalc(right_cos_sum,
			pose.pointPose[right_central*2], pose.pointPose[right_central*2+1],
			pose.pointPose[right_pt1*2], pose.pointPose[right_pt1*2+1],
			pose.pointPose[right_pt2*2], pose.pointPose[right_pt2*2+1]))
		{
			right_cos_sum += right_cos;
		}
	}

	// 对最近5帧的需要求的夹角余弦求平均
	double left_cos_average = left_cos_sum / in_pose_queue.size();
	double right_cos_average = right_cos_sum / in_pose_queue.size();
	// cout << left_cos_average + right_cos_average << endl;


	return left_cos_average + right_cos_average;
}
//// 结果输出到cosResult中。如果出现除0等异常情况，返回false。
//bool ActionRecognizerUtil::cosineCalc(double& cosResult, double central_x, double central_y, 
//	double pt1_x, double pt1_y, double pt2_x, double pt2_y)
//{
//	// 计算模长
//	double dist1 = sqrt(pow((pt1_x - central_x), 2) + pow((pt1_y - central_y), 2));
//	double dist2 = sqrt(pow((pt2_x - central_x), 2) + pow((pt2_y - central_y), 2));
//
//	// 防止除0
//	if (dist1 == 0 || dist2 == 0)
//	{
//		cosResult = 0;
//		return false;
//	}
//
//	// 计算向量积
//	double mul = (pt1_x - central_x) * (pt2_x - central_x) + (pt1_y - central_y) * (pt2_y - central_y);
//
//	// 计算夹角余弦
//	double cos = mul / (dist1 * dist2);
//	cosResult = cos;
//	return true;
//}
//
//// 前六个参数是指定使用每一侧使用哪三个点来计算夹角，in_pose_queue是最近5帧里面的关节点序列
//// current_state为-1时标记当前为【手臂伸直】状态，current_state为1时标记当前为【手臂弯曲】状态
//bool ActionRecognizerUtil::averageCosineCalc(int left_central, 
//	int left_pt1, int left_pt2, int right_central, int right_pt1, int right_pt2,
//	vector<PersonPosePara>& in_pose_queue, int &current_state
//	)
//{
//	double left_cos_sum = 0;
//	double right_cos_sum = 0;
//	for (int j = 0; j < in_pose_queue.size(); j++)
//	{
//		PersonPosePara pose = in_pose_queue[j];
//
//		// 对最近5帧的需要求的夹角余弦求和
//		double left_cos = 0;
//		if(cosineCalc(left_cos,
//			pose.pointPose[left_central*2], pose.pointPose[left_central*2+1],
//			pose.pointPose[left_pt1*2], pose.pointPose[left_pt1*2+1],
//			pose.pointPose[left_pt2*2], pose.pointPose[left_pt2*2+1]))
//		{
//			left_cos_sum += left_cos;
//		}
//		double right_cos = 0;
//		if(cosineCalc(right_cos_sum,
//			pose.pointPose[right_central*2], pose.pointPose[right_central*2+1],
//			pose.pointPose[right_pt1*2], pose.pointPose[right_pt1*2+1],
//			pose.pointPose[right_pt2*2], pose.pointPose[right_pt2*2+1]))
//		{
//			right_cos_sum += right_cos;
//		}
//	}
//
//	// 对最近5帧的需要求的夹角余弦求平均
//	double left_cos_average = left_cos_sum / in_pose_queue.size();
//	double right_cos_average = right_cos_sum / in_pose_queue.size();
//	// cout << left_cos_average + right_cos_average << endl;
//
//	//如果达到某硬编码阈值，则标记为【伸直】/【放下】
//	if (left_cos_average + right_cos_average > 0.5)
//	{
//		current_state = 1;
//	}
//	if (left_cos_average + right_cos_average < 0.15)
//	{
//		if (current_state == 1)
//		{
//			current_state = -1;
//			return true;
//		}
//	}
//	return false;
//}

bool ActionRecognizerUtil::isHandsOverHead(vector<PersonPosePara>& in_pose_queue)
{
	double left_hand_sum = 0;
	double right_hand_sum = 0;
	double head_sum = 0;

	for (int j = 0; j < in_pose_queue.size(); j++)
	{
		PersonPosePara pose = in_pose_queue[j];

		left_hand_sum += pose.pointPose[WRIST_L * 2+1];
		right_hand_sum += pose.pointPose[WRIST_R * 2+1];
		head_sum += pose.pointPose[HEAD * 2];
	}

	double left_hand_average = left_hand_sum / in_pose_queue.size();
	double right_hand_average = right_hand_sum / in_pose_queue.size();
	double head_average = head_sum / in_pose_queue.size();

	if (left_hand_average < head_average && right_hand_average < head_average)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ActionRecognizerUtil::isHandsOverShoulders(vector<PersonPosePara>& in_pose_queue)
{
	double left_hand_sum = 0;
	double right_hand_sum = 0;
	double left_shoulder_sum = 0;
	double right_shoulder_sum = 0;

	for (int j = 0; j < in_pose_queue.size(); j++)
	{
		PersonPosePara pose = in_pose_queue[j];

		left_hand_sum += pose.pointPose[WRIST_L * 2 + 1];
		right_hand_sum += pose.pointPose[WRIST_R * 2 + 1];
		left_shoulder_sum += pose.pointPose[SHOULDER_L * 2 + 1];
		right_shoulder_sum += pose.pointPose[SHOULDER_R * 2 + 1];
	}

	double left_hand_average = left_hand_sum / in_pose_queue.size();
	double right_hand_average = right_hand_sum / in_pose_queue.size();
	double left_shoulder_average = left_shoulder_sum / in_pose_queue.size();
	double right_shoulder_average = right_shoulder_sum / in_pose_queue.size();

	if (left_hand_average >= left_shoulder_average && right_hand_average >= right_shoulder_average)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ActionRecognizerUtil::isLegsOpen(vector<PersonPosePara>& in_pose_queue)
{
	double left_shoulder_sum = 0;
	double right_shoulder_sum = 0;
	double left_ankle_sum = 0;
	double right_ankle_sum = 0;

	for (int j = 0; j < in_pose_queue.size(); j++)
	{
		PersonPosePara pose = in_pose_queue[j];

		left_shoulder_sum += pose.pointPose[SHOULDER_L * 2];
		right_shoulder_sum += pose.pointPose[SHOULDER_R * 2];
		left_ankle_sum += pose.pointPose[ANKLE_L * 2];
		right_ankle_sum += pose.pointPose[ANKLE_R * 2];
	}

	double left_shoulder_average = left_shoulder_sum / in_pose_queue.size();
	double right_shoulder_average = right_shoulder_sum / in_pose_queue.size();
	double left_ankle_average = left_ankle_sum / in_pose_queue.size();
	double right_ankle_average = right_ankle_sum / in_pose_queue.size();

	if (abs(right_shoulder_average - left_shoulder_average) < abs(right_ankle_average - left_ankle_average))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ActionRecognizerUtil::isElbowShoulderSameSide(int hand, int elbow, int shoulder, vector<PersonPosePara>& in_pose_queue)
{
	double hand_sum = 0;
	double elbow_sum = 0;
	double shoulder_sum = 0;

	for (int j = 0; j < in_pose_queue.size(); j++)
	{
		PersonPosePara pose = in_pose_queue[j];

		hand_sum += pose.pointPose[hand * 2];
		elbow_sum += pose.pointPose[elbow * 2];
		shoulder_sum += pose.pointPose[shoulder * 2];
	}

	double hand_average = hand_sum / in_pose_queue.size();
	double elbow_average = elbow_sum / in_pose_queue.size();
	double shoulder_average = shoulder_sum / in_pose_queue.size();

	if (hand == WRIST_L && elbow == ELBOW_L && shoulder == SHOULDER_L)
	{
		if (hand_average >= shoulder_average && elbow_average >= shoulder_average)
		{
			return true;
		}
	}
	else if(hand == WRIST_R && elbow == ELBOW_R && shoulder == SHOULDER_R)
	{
		if (hand_average <= shoulder_average && elbow_average <= shoulder_average)
		{
			return true;
		}
	}
	return false;
}

bool ActionRecognizerUtil::isKneeRaised(int hip, int knee, int ankle, vector<PersonPosePara>& in_pose_queue)
{
	double hip_sum = 0;
	double knee_sum = 0;
	double ankle_sum = 0;

	for (int j = 0; j < in_pose_queue.size(); j++)
	{
		PersonPosePara pose = in_pose_queue[j];

		hip_sum += pose.pointPose[hip * 2 + 1];
		knee_sum += pose.pointPose[knee * 2 + 1];
		ankle_sum += pose.pointPose[ankle * 2 + 1];
	}

	double hip_average = hip_sum / in_pose_queue.size();
	double knee_average = knee_sum / in_pose_queue.size();
	double ankle_average = ankle_sum / in_pose_queue.size();


	if (knee_average < hip_average && knee_average < ankle_average)
	{
		return true;
	}
	return false;
}

//bool ActionRecognizerUtil::CalculateActionCount(vector<PersonPosePara>& in_pose_queue, int inActionType, int &current_state)
//{
//	if (in_pose_queue.size() == 0)  
//	{
//		return false;
//	}
//
//	bool b_counted = averageCosineCalc(ELBOW_L, SHOULDER_L, WRIST_L, ELBOW_R, SHOULDER_R, WRIST_R,
//		in_pose_queue, current_state);
//
//	return b_counted; 
//}

bool ActionRecognizerUtil::CalculateActionCount(vector<PersonPosePara>& in_pose_queue, int inActionType, int& current_state, int& current_prompt)
{
	if (in_pose_queue.size() == 0)
	{
		return false;
	}
	bool b_counted = false;

	switch (inActionType)
	{
	case 0:
		/*current_state定义：
		-1——自然站立；
		1——举手；
		*/

		//如果达到某硬编码阈值，则标记为【伸直】/【放下】
		if (averageCosineCalc(ELBOW_L, SHOULDER_L, WRIST_L, ELBOW_R, SHOULDER_R, WRIST_R,
			in_pose_queue) > 0.5)
		{
			current_state = 1;
		}
		if (averageCosineCalc(ELBOW_L, SHOULDER_L, WRIST_L, ELBOW_R, SHOULDER_R, WRIST_R,
			in_pose_queue) < 0.15)
		{
			if (current_state == 1)
			{
				current_state = -1;
				return true;
			}
		}

		break;

	case 1: //【应浩聪完成：suki_01.mp4里面的动作】
		/*current_state定义：(备注3.30：正数为值得注意的关键帧，负数为需要提示用户之处；有微调)
		0——自然站立；
		1——跳起；
		*/
		/*current_prompt定义：
		0——无提示；
		1——应该举起双手；
		2——应该放下双手；
		3——应该张开双腿；
		4——应该闭合双腿；
		5——非站立姿势；
		10——未识别的非法姿势
		*/

		current_prompt = 10;

		if (isKneeRaised(HIP_L, KNEE_L, ANKLE_L, in_pose_queue)
			&& isKneeRaised(HIP_R, KNEE_R, ANKLE_R, in_pose_queue))
		{
			current_prompt = 5;
			return false;
		}

		if (isHandsOverHead(in_pose_queue) && isLegsOpen(in_pose_queue))
		{
			current_state = 1;
			current_prompt = 0;
		}
		if (!isHandsOverHead(in_pose_queue) && !isLegsOpen(in_pose_queue))
		{
			if (current_state == 1) b_counted = true;
			
			current_state = 0;
			current_prompt = 0;
		}
		if (!isHandsOverHead(in_pose_queue) && isLegsOpen(in_pose_queue))
		{
			if (current_state == 0) current_prompt = 1;
			if (current_state == 1) current_prompt = 4;
		}
		if (isHandsOverHead(in_pose_queue) && !isLegsOpen(in_pose_queue))
		{
			if (current_state == 0) current_prompt = 3;
			if (current_state == 1) current_prompt = 2;
		}
		// cout << current_state << endl;
		break;

	case 2://【应浩聪完成：suki_02.mp4里面的动作】
		/*current_state定义：(备注3.30：正数为值得注意的关键帧，负数为需要提示用户之处)
		0——抱头站立（一轮完成）；
		1——左侧接近；
		2——一侧接近后抱头站立；
		3——右侧接近；
		*/
		/*current_prompt定义：
		1——手不在双耳附近；
		2——非站立状态；
		3——膝盖没有抬起；
		4——肘关节没有移动；
		5——需要恢复站直状态；
		6——同手同脚；
		7——两侧手均弯曲；
		8——请换一侧；
		10——未识别的非法姿势*/

		current_prompt = 10;

		// 双手不在耳边
		if ((!isHandsOverHead(in_pose_queue)) && isHandsOverShoulders(in_pose_queue))
		{
			current_prompt = 1;
			return false;
		}

		// 蹲/坐在地上
		if (isKneeRaised(HIP_L, KNEE_L, ANKLE_L, in_pose_queue)
			&& isKneeRaised(HIP_R, KNEE_R, ANKLE_R, in_pose_queue))
		{
			current_prompt = 2;
			return false;
		}
		
		// 两侧手均弯曲
		if (!(isElbowShoulderSameSide(WRIST_L, ELBOW_L, SHOULDER_L, in_pose_queue))
			&& (!isElbowShoulderSameSide(WRIST_R, ELBOW_R, SHOULDER_R, in_pose_queue)))
		{
			current_prompt = 7;
			return false;
		}

		// 仅左膝弯曲
		if (isElbowShoulderSameSide(WRIST_L, ELBOW_L, SHOULDER_L, in_pose_queue)
			&& isElbowShoulderSameSide(WRIST_R, ELBOW_R, SHOULDER_R, in_pose_queue)
			&& isKneeRaised(HIP_L, KNEE_L, ANKLE_L, in_pose_queue)
			&& (!isKneeRaised(HIP_R, KNEE_R, ANKLE_R, in_pose_queue)))
		{
			switch (current_state)
			{
			case 0:
				current_prompt = 4;
				break;
			case 1:
				current_prompt = 5;
				break;
			case 2:
				current_prompt = 8;
				break;
			}
			return false;
		}

		// 仅右膝弯曲
		if (isElbowShoulderSameSide(WRIST_L, ELBOW_L, SHOULDER_L, in_pose_queue)
			&& isElbowShoulderSameSide(WRIST_R, ELBOW_R, SHOULDER_R, in_pose_queue)
			&& (!isKneeRaised(HIP_L, KNEE_L, ANKLE_L, in_pose_queue))
			&& (isKneeRaised(HIP_R, KNEE_R, ANKLE_R, in_pose_queue)))
		{
			switch (current_state)
			{
			case 0:
				current_prompt = 8;
				break;
			case 3:
				current_prompt = 5;
				break;
			case 2:
				current_prompt = 4;
				break;
			}
			return false;
		}

		// 仅左臂弯曲
		if ((!isElbowShoulderSameSide(WRIST_L, ELBOW_L, SHOULDER_L, in_pose_queue))
			&& isElbowShoulderSameSide(WRIST_R, ELBOW_R, SHOULDER_R, in_pose_queue)
			&& (!isKneeRaised(HIP_L, KNEE_L, ANKLE_L, in_pose_queue))
			&& (!isKneeRaised(HIP_R, KNEE_R, ANKLE_R, in_pose_queue)))
		{
			switch (current_state)
			{
			case 0:
				current_prompt = 8;
				break;
			case 3:
				current_prompt = 5;
				break;
			case 2:
				current_prompt = 3;
				break;
			}
			return false;
		}

		// 仅右臂弯曲
		if (isElbowShoulderSameSide(WRIST_L, ELBOW_L, SHOULDER_L, in_pose_queue)
			&& (!isElbowShoulderSameSide(WRIST_R, ELBOW_R, SHOULDER_R, in_pose_queue))
			&& (!isKneeRaised(HIP_L, KNEE_L, ANKLE_L, in_pose_queue))
			&& (!isKneeRaised(HIP_R, KNEE_R, ANKLE_R, in_pose_queue)))
		{
			switch (current_state)
			{
			case 0:
				current_prompt = 3;
				break;
			case 1:
				current_prompt = 5;
				break;
			case 2:
				current_prompt = 8;
				break;
			}
			return false;
		}

		// 右手和左膝弯曲
		if (isElbowShoulderSameSide(WRIST_L, ELBOW_L, SHOULDER_L, in_pose_queue) 
			&& (!isElbowShoulderSameSide(WRIST_R, ELBOW_R, SHOULDER_R, in_pose_queue))
			&& isKneeRaised(HIP_L, KNEE_L, ANKLE_L, in_pose_queue)
			&& (!isKneeRaised(HIP_R, KNEE_R, ANKLE_R, in_pose_queue)))
		{
			switch (current_state)
			{
			case 0:
			case 1:
				current_state = 1;
				current_prompt = 0;
				break;
			case 2:
				current_prompt = 8;
				break;
			}
		}

		// 左手和右膝弯曲
		if ((!isElbowShoulderSameSide(WRIST_L, ELBOW_L, SHOULDER_L, in_pose_queue))
			&& isElbowShoulderSameSide(WRIST_R, ELBOW_R, SHOULDER_R, in_pose_queue)
			&& (!isKneeRaised(HIP_L, KNEE_L, ANKLE_L, in_pose_queue))
			&& isKneeRaised(HIP_R, KNEE_R, ANKLE_R, in_pose_queue))
		{
			switch (current_state)
			{
			case 2:
			case 3:
				current_state = 3;
				current_prompt = 0;
				break;
			case 0:
				current_prompt = 8;
				break;
			}
		}

		// 站立状态
		if ((isElbowShoulderSameSide(WRIST_L, ELBOW_L, SHOULDER_L, in_pose_queue))
			&& isElbowShoulderSameSide(WRIST_R, ELBOW_R, SHOULDER_R, in_pose_queue)
			&& (!isKneeRaised(HIP_L, KNEE_L, ANKLE_L, in_pose_queue))
			&& (!isKneeRaised(HIP_R, KNEE_R, ANKLE_R, in_pose_queue)))
		{
			switch (current_state)
			{
			case 0:
				current_state = 0;
				current_prompt = 0;
				break;
			case 3:
				current_state = 0;
				current_prompt = 0;
				b_counted = true;
				break;
			case 1:
			case 2:
				current_state = 2;
				current_prompt = 0;
				break;
			}
		}
		break;
	default:
		break;
	}

	return b_counted;
}

// ------------------------------------【3.30 YHC：废弃】
/*
bool ActionRecognizerUtil::CalculateActionCount(vector<PersonPosePara>& in_pose_queue, int inActionType)
{
	if (in_pose_queue.size() == 0)
	{
		return false;
	}
	bool b_counted = false;

	switch (inActionType)
	{
	case 1: //【应浩聪完成：suki_01.mp4里面的动作】

		break;

	case 2://【应浩聪完成：suki_02.mp4里面的动作】
		
		break;
	default:
		break;
	}

	return b_counted;
}*/

bool ActionRecognizerUtil::isKneeAnkleAligned(vector<PersonPosePara>& in_pose_queue)
{
	double knee_sum_l = 0;
	double ankle_sum_l = 0;
	double knee_sum_r = 0;
	double ankle_sum_r = 0;
	double shoulder_sum_l = 0;
	double shoulder_sum_r = 0;
	double neck_sum = 0;

	for (int j = 0; j < in_pose_queue.size(); j++)
	{
		PersonPosePara pose = in_pose_queue[j];

		knee_sum_l += pose.pointPose[KNEE_L * 2 ];
		ankle_sum_l += pose.pointPose[ANKLE_L * 2 ];
		knee_sum_r += pose.pointPose[KNEE_R * 2 ];
		ankle_sum_r += pose.pointPose[ANKLE_R * 2 + 1];
		shoulder_sum_l += pose.pointPose[SHOULDER_L * 2 ];
		shoulder_sum_r += pose.pointPose[SHOULDER_R * 2 ];
		neck_sum += pose.pointPose[NECK * 2 + 1];
	}

	double shoulder_dist = (abs(neck_sum - shoulder_sum_r) + abs(neck_sum - shoulder_sum_l)) / 2;
	if (abs(knee_sum_l - ankle_sum_l) > shoulder_dist || abs(knee_sum_r - ankle_sum_r) > shoulder_dist)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool ActionRecognizerUtil::isThighLongerThanShank(vector<PersonPosePara>& in_pose_queue)
{
	double hip_sum = 0;
	double knee_sum = 0;
	double ankle_sum = 0;

	for (int j = 0; j < in_pose_queue.size(); j++)
	{
		PersonPosePara pose = in_pose_queue[j];

		hip_sum += pose.pointPose[HIP_L * 2+1] + pose.pointPose[HIP_R * 2 + 1];
		knee_sum += pose.pointPose[KNEE_L * 2 + 1] + pose.pointPose[KNEE_R * 2 + 1];
		ankle_sum += pose.pointPose[ANKLE_L * 2 + 1] + pose.pointPose[ANKLE_R * 2 + 1];
	}

	if (knee_sum - hip_sum > ankle_sum - knee_sum)
	{
		return true;
	}
	else
	{
		return false;
	}
}