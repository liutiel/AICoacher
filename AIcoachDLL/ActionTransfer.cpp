#include "ActionTransfer.h"
#include "WorkoutParameter.h"

//初始化状态列表：定义某个动作的关键状态
void ActionTransfer::InitActionState(vector<AcionStatePara>& inActionStateList,string inActionName,int inActionNo)
{
	m_actionName = inActionName;
	m_actionNo = inActionNo;

	m_numStates = inActionStateList.size();
	m_actionStateList.clear();
	for (int i = 0; i < m_numStates; i++)
	{
		AcionStatePara curActionState = inActionStateList[i];
		m_actionStateList.push_back(curActionState);
	}
	m_currentState = -1;
	m_currentPerfect = -1;
}

//比较进来的动作状态与下一个动作状态是否一样
bool ActionTransfer::CompareNextActionState(AcionStatePara& inNextActionState)
{
	//【YHC-0420-1807】修改计数循环方式以修正计数时间点不恰当的问题
	AcionStatePara currentState = m_actionStateList[(m_currentState + 1) % m_numStates];
	bool b_same = true; 
	for (int i = 0; i < DEFINED_NUMBER_ACTION_STATE; i++)
	{
		if (inNextActionState.stateIndex[i] != currentState.stateIndex[i])
		{
			b_same = false;
		}
	}

	return b_same;
}

//执行动作状态转移，并判断是否完成一次完成动作：true为计数一次；false不计数
int* ActionTransfer::ExecuteActionStateTransfer()
{
	int b_counted = 0;
	int perfect_code = -1;
	if (m_currentState == m_numStates - 1)
	{
		b_counted = 1;
		//【YHC-0420-1810】修改计数循环方式以修正计数时间点不恰当的问题
		m_currentState = -1;
		perfect_code = m_numStates - m_currentPerfect - 1;
		m_currentPerfect = -1;
	}
	else
	{
		m_currentState++;
	}
	int res[2];
	res[0] = b_counted;
	res[1] = perfect_code;
	// cout << res[0] << " " << res[1] << endl;
	return res;

}

int* ActionTransfer::NextActionState(AcionStatePara& inNextActionState)
{
	if (true == CompareNextActionState(inNextActionState))
	{
		int* exec_res = ExecuteActionStateTransfer();
		//cout << exec_res[0] << " " << exec_res[1] <<endl;
		nextAction = exec_res[0];
		perfectAction = exec_res[1];
		return exec_res;
	}
	else
	{
		int res[2] = {0, -1};
		return res;
	}

}

// Check whether the action meets the perfect standards
bool ActionTransfer::CompareNextPerfectState(AcionStatePara& inNextActionState)
{
	AcionStatePara currentState = m_actionStateList[(m_currentPerfect + 1) % m_numStates];
	bool b_same = true;
	for (int i = DEFINED_NUMBER_ACTION_STATE; i < DEFINED_NUMBER_ACTION_STATE + DEFINED_NUMBER_PERFECT_STATE; i++)
	{
		if (inNextActionState.stateIndex[i] != currentState.stateIndex[i])
		{
			b_same = false;
		}
	}

	return b_same;

}

void ActionTransfer::ExecutePerfectStateTransfer()
{
	if (m_currentPerfect < m_numStates - 1)
	{
		m_currentPerfect++;
	}

}

void ActionTransfer::NextPerfectState(AcionStatePara& inNextActionState)
{
	if (true == CompareNextPerfectState(inNextActionState))
	{
		ExecutePerfectStateTransfer();
	}

}

bool ActionTransfer::ReaddActionStateFromStream(ifstream & inFstream)
{
	inFstream >> m_actionName;
	inFstream >> m_actionNo;
	inFstream >> m_numStates;
	for (int i = 0; i < m_numStates; i++)
	{
		AcionStatePara curActionState;
		for (int j = 0; j < DEFINED_NUMBER_ACTION_STATE + DEFINED_NUMBER_PERFECT_STATE; j++)
		{
			inFstream >> curActionState.stateIndex[j];
		}
		m_actionStateList.push_back(curActionState);
	}
	return true;
}

//读取动作状态转移列表
bool ActionTransfer::InitActionStateFromFile(string inFileCSV)
{
	std::ifstream inF; 
	inF.open(inFileCSV, ios::in);
	//【需要检查读函数的写法是否正确】
	inF >> m_actionName;
	inF >> m_actionNo;
	inF >> m_numStates;
	for (int i = 0; i < m_numStates; i++)
	{
		AcionStatePara curActionState;
		for (int j = 0; j < DEFINED_NUMBER_ACTION_STATE + DEFINED_NUMBER_PERFECT_STATE; j++)
		{
			inF >> curActionState.stateIndex[j];
		}
		m_actionStateList.push_back(curActionState);
	}
	inF.close();

	m_currentState = 0; 
	m_currentPerfect = 0;

	//testing
	cout << m_actionName << "   "<< m_actionNo << "   " << m_numStates << "   " << endl;
	for (int i = 0; i < m_numStates; i++)
	{
		for (int j = 0; j < DEFINED_NUMBER_ACTION_STATE + DEFINED_NUMBER_PERFECT_STATE; j++)
		{
			cout << m_actionStateList[i].stateIndex[j] << " ";
		}
		cout << endl;
	}
	return true; 
}

AcionStatePara ActionTransfer::ComputeActionStateFromPosePoints(vector<PersonPosePara>& in_pose_queue)
{
	AcionStatePara curActionState;
	for (int i = 0; i < DEFINED_NUMBER_ACTION_STATE + DEFINED_NUMBER_PERFECT_STATE; i++)
	{
		curActionState.stateIndex[i] = 0;
	}
	switch (m_actionNo)
	{
///////////////////////////////////demo 519/////////////////////////////////////////////
	case 51901:	//哑铃侧平举
		//curActionState.stateIndex[0] = WorkoutParameter::IsHandOverShoulder(in_pose_queue);
		curActionState.stateIndex[10] = WorkoutParameter::IsArm_Vline(in_pose_queue);
		curActionState.stateIndex[11] = WorkoutParameter::IsArm_Lline(in_pose_queue);
		//curActionState.stateIndex[12] = WorkoutParameter::IsAnkleClose(in_pose_queue);

		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 0] = WorkoutParameter::IsHandOverShoulder(in_pose_queue);
		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 10] = WorkoutParameter::IsArm_Vline(in_pose_queue);
		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 11] = WorkoutParameter::IsArm_Lline(in_pose_queue);
		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 12] = WorkoutParameter::IsAnkleClose(in_pose_queue);
		break;

	case 51902:	//Y字伸展		  
		//curActionState.stateIndex[12] = WorkoutParameter::IsAnkleClose(in_pose_queue);
		curActionState.stateIndex[14] = WorkoutParameter::IsHandOverHead(in_pose_queue);

		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 12] = WorkoutParameter::IsAnkleClose(in_pose_queue);
		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 14] = WorkoutParameter::IsHandOverHead(in_pose_queue);
		break;

	case 51903:	//深蹲
		curActionState.stateIndex[2] = WorkoutParameter::Cos_Knee(in_pose_queue);
		curActionState.stateIndex[13] = WorkoutParameter::IsLegUpright(in_pose_queue);

		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 2] = WorkoutParameter::Cos_Knee(in_pose_queue);
		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 13] = WorkoutParameter::IsLegUpright(in_pose_queue);
		break;

	case 51904:	//后弓步蹲右
		curActionState.stateIndex[1] = WorkoutParameter::IsSide(in_pose_queue);
	    curActionState.stateIndex[2] = WorkoutParameter::Cos_Knee(in_pose_queue);
		curActionState.stateIndex[4] = WorkoutParameter::IsKnee_L_Raised(in_pose_queue);
		curActionState.stateIndex[5] = WorkoutParameter::IsSideShouldeElbow_R(in_pose_queue);

		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 1] = WorkoutParameter::IsSide(in_pose_queue);
		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 2] = WorkoutParameter::Cos_Knee(in_pose_queue);
		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 4] = WorkoutParameter::IsKnee_L_Raised(in_pose_queue);
		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 5] = WorkoutParameter::IsSideShouldeElbow_R(in_pose_queue);
		break;

	case 51905:	//后弓步蹲左
		curActionState.stateIndex[1] = WorkoutParameter::IsSide(in_pose_queue);
		curActionState.stateIndex[2] = WorkoutParameter::Cos_Knee(in_pose_queue);
		curActionState.stateIndex[3] = WorkoutParameter::IsKnee_R_Raised(in_pose_queue);
		curActionState.stateIndex[6] = WorkoutParameter::IsSideShouldeElbow_L(in_pose_queue);

		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 1] = WorkoutParameter::IsSide(in_pose_queue);
		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 2] = WorkoutParameter::Cos_Knee(in_pose_queue);
		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 3] = WorkoutParameter::IsKnee_R_Raised(in_pose_queue);
		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 6] = WorkoutParameter::IsSideShouldeElbow_L(in_pose_queue);
		break;

	case 51906:	//交替侧抬腿
		curActionState.stateIndex[7] = WorkoutParameter::IsAnkle_R_Raised(in_pose_queue);
		curActionState.stateIndex[8] = WorkoutParameter::IsAnkle_L_Raised(in_pose_queue);
		curActionState.stateIndex[12] = WorkoutParameter::IsAnkleClose(in_pose_queue);

		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 7] = WorkoutParameter::IsAnkle_R_Raised(in_pose_queue);
		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 8] = WorkoutParameter::IsAnkle_L_Raised(in_pose_queue);
		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 12] = WorkoutParameter::IsAnkleClose(in_pose_queue);
		break;

	case 51907:	//摆臂触地深蹲
		curActionState.stateIndex[2] = WorkoutParameter::Cos_Knee(in_pose_queue);
		curActionState.stateIndex[9] = WorkoutParameter::IsKneeOverHand(in_pose_queue);
		curActionState.stateIndex[14] = WorkoutParameter::IsHandOverHead(in_pose_queue);

		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 2] = WorkoutParameter::Cos_Knee(in_pose_queue);
		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 9] = WorkoutParameter::IsKneeOverHand(in_pose_queue);
		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 14] = WorkoutParameter::IsHandOverHead(in_pose_queue);
		break;

	case 51908://抱头提膝
		curActionState.stateIndex[3] = WorkoutParameter::IsKnee_R_Raised(in_pose_queue);
		curActionState.stateIndex[4] = WorkoutParameter::IsKnee_L_Raised(in_pose_queue);
		curActionState.stateIndex[15] = WorkoutParameter::IsElbow_R_Higher(in_pose_queue);
		curActionState.stateIndex[16] = WorkoutParameter::IsElbow_L_Higher(in_pose_queue);

		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 3] = WorkoutParameter::IsKnee_R_Raised(in_pose_queue);
		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 4] = WorkoutParameter::IsKnee_L_Raised(in_pose_queue);
		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 15] = WorkoutParameter::IsElbow_R_Higher(in_pose_queue);
		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 16] = WorkoutParameter::IsElbow_L_Higher(in_pose_queue);
		break;

	case 51909:	//站姿伐木右
		curActionState.stateIndex[5] = WorkoutParameter::IsSideShouldeElbow_R(in_pose_queue);
		curActionState.stateIndex[14] = WorkoutParameter::IsHandOverHead(in_pose_queue);

		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 5] = WorkoutParameter::IsSideShouldeElbow_R(in_pose_queue);
		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 14] = WorkoutParameter::IsHandOverHead(in_pose_queue);
		break;

	case 51910:	//站姿伐木左
		curActionState.stateIndex[6] = WorkoutParameter::IsSideShouldeElbow_L(in_pose_queue);
		curActionState.stateIndex[14] = WorkoutParameter::IsHandOverHead(in_pose_queue);

		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 6] = WorkoutParameter::IsSideShouldeElbow_L(in_pose_queue);
		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 14] = WorkoutParameter::IsHandOverHead(in_pose_queue);
		break;

	case 51911:	//交替膝击
		curActionState.stateIndex[3] = WorkoutParameter::IsKnee_R_Raised(in_pose_queue);
		curActionState.stateIndex[4] = WorkoutParameter::IsKnee_L_Raised(in_pose_queue);
		curActionState.stateIndex[14] = WorkoutParameter::IsHandOverHead(in_pose_queue);

		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 3] = WorkoutParameter::IsKnee_R_Raised(in_pose_queue);
		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 4] = WorkoutParameter::IsKnee_L_Raised(in_pose_queue);
		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 14] = WorkoutParameter::IsHandOverHead(in_pose_queue);
		break;

	case 51912:	//开合跳
		curActionState.stateIndex[12] = WorkoutParameter::IsAnkleClose(in_pose_queue);
		curActionState.stateIndex[13] = WorkoutParameter::IsLegUpright(in_pose_queue);
		curActionState.stateIndex[14] = WorkoutParameter::IsHandOverHead(in_pose_queue);

		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 12] = WorkoutParameter::IsAnkleClose(in_pose_queue);
		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 13] = WorkoutParameter::IsLegUpright(in_pose_queue);
		curActionState.stateIndex[DEFINED_NUMBER_ACTION_STATE + 14] = WorkoutParameter::IsHandOverHead(in_pose_queue);
		break;

	default:
		break;
	}



	return curActionState;

}

int ActionTransfer::GetActionType()
{
	return m_actionNo;
}

void ActionTransfer::PrintInfo()
{
	cout << m_actionName << "   " << m_actionNo << "   "
		<< m_numStates <<  endl;
	for (int i = 0; i < m_numStates; i++)
	{
		for (int j = 0; j < DEFINED_NUMBER_ACTION_STATE + DEFINED_NUMBER_PERFECT_STATE; j++)
			cout << m_actionStateList[i].stateIndex[j] << "   ";
		cout << endl;
	}
}
bool InitActionStateList(string inFilename, vector<ActionTransfer>& curActionTransferList)
{
	curActionTransferList.clear();

	std::ifstream inF;
	inF.open(inFilename, ios::in);
	//【需要检查读函数的写法是否正确】

	while (!inF.eof())
	{
		ActionTransfer curAT;
		if (true == curAT.ReaddActionStateFromStream(inF))
		{
			curActionTransferList.push_back(curAT);
		}
	} 

	inF.close();

	return true;
}