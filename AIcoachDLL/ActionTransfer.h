/////////////////////////////////////////////////////////////////////////
//Class ActionTransfer: 
//Function: define the state of one health action, and the transfer relations between framewised pose 
//Author: Tie LIU
//Created at 2021-03-31
///////////////////////////////////////////////////////////////////////
#pragma once
#include "commonincludes.h"
#include "ActionRecognizerUtil.h"


struct AcionStatePara
{
	int stateIndex[DEFINED_NUMBER_ACTION_STATE];

};

class ActionTransfer
{
private:
	string m_actionName;
	int m_actionNo;
	int m_numStates;
	vector<AcionStatePara> m_actionStateList;
	int m_currentState;

public:
	void InitActionState(vector<AcionStatePara>& inActionStateList, string inActionName = "Testing", int inActionNo = 0);

	bool CompareNextActionState(AcionStatePara& inNextActionState);

	bool ExecuteActionStateTransfer();

	bool NextActionState(AcionStatePara& inNextActionState);

	bool InitActionStateFromFile(string inFileCSV);

	AcionStatePara ComputeActionStateFromPosePoints(vector<PersonPosePara>& in_pose_queue);

	bool ReaddActionStateFromStream(ifstream & inFstream);

	int GetActionType();

	void PrintInfo();

};

bool InitActionStateList(string inFilename, vector<ActionTransfer>& curActionTransferList);