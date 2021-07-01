#include <string>
#include <iostream>
#include <direct.h>
#include <windows.h> 
#include<filesystem>
#include <cstdlib>
#include <tchar.h>
#include <windows.h>
#include <io.h>
#include< stdlib.h > 
#include< stdio.h > 


#include "AIclass.h"

AIclass::AIclass()
{

}

AIclass::~AIclass()
{

}

bool AIclass::InitClassFromFile(string inClassFileName)
{
    std::ifstream inF;
    inF.open(inClassFileName, ios::in);

    //【需要检查读函数的写法是否正确】
    inF >> m_className;
    inF >> m_classNo;
    inF >> m_numAction;
    //课程时间，单位为分钟，后续使用要转换单位
    inF >> m_durationClassSetting;

    m_curHealthActionList.clear();
    for (int i = 0; i < m_numAction; i++)
    {
        HealthAction curAction;

        inF >> curAction.m_paraAction.actionName;
        inF >> curAction.m_paraAction.actionType;
        inF >> curAction.m_paraAction.actionLimitNum;
        inF >> curAction.m_paraAction.actionLimitSecond;

        curAction.m_actionCounter.SetLimitCount(curAction.m_paraAction.actionLimitNum);
        curAction.m_actionTimer.SetLimitTime(curAction.m_paraAction.actionLimitSecond);
        m_curHealthActionList.push_back(curAction);
    }
    inF.close();

    return true;
}

//读取健身课程设置
bool AIclass::ReadHealthActionFile(string inFileName)
{
    // 读文件  
    //std::ifstream inf;
    //inf.open(inFileName, ios::in); // 打开模式可省略  

    // //读取表头
    //char buffer[25600];
    //inf.getline(buffer, 256, '\n');//getline(char *,int,char) 表示该行字符达到256个或遇到换行就结束
    //cout << buffer << endl;
 
    //while (!inf.eof())
    //{

    //    HealthAction curAction;

    //    std::string buffTmp;
    //    while (std::getline(inf, buffTmp))
    //    {
    //        std::stringstream stringIn(buffTmp);//使用串流实现对string的输入输出操作

    //        std::string cell;
    //        //std::getline(stringIn, cell, ',');
    //        //std::getline(stringIn, cell, ',');
    //        //cout << atof(cell.c_str()) << std::endl;
    //        //std::getline(stringIn, cell, ',');
    //        //cout << atof(cell.c_str()) << std::endl;
    //        //std::getline(stringIn, cell, ',');
    //        //cout << atof(cell.c_str()) << std::endl;

    //        m_curHealthAction.push_back(curAction);

    //    }

    //}
    //inf.close();



	return true;
}

//增加健身课程；
bool AIclass::AddHealthAction(HealthAction inHealthAction)
{
    m_curHealthActionList.push_back(inHealthAction);
    
    m_numAction = (int)m_curHealthActionList.size();

	return true;
}

bool AIclass::SetClass(int inClassID)
{
// 【YHC-3.30：各课程的初始化代码可以统一，但是】
/*    switch (inClassID)
    {
        case 0: // 测试课程
        {
            m_numAction = 1;
            HealthAction simpleAction;
            simpleAction.InitHealthAction(0);
            m_curHealthAction.clear();
            m_curHealthAction.push_back(simpleAction);
            break;
        }
        case 1: // suki_01【开合跳】
        {*/
            cout << "inClassID:  " << inClassID << endl;
            m_numAction = inClassID;
            HealthAction curAction;
            curAction.InitHealthAction(inClassID);
            m_curHealthActionList.clear();
            m_curHealthActionList.push_back(curAction);
 /*           break;
        }
    }*/
    return true;
}

bool AIclass::GetHealthActionList(std::vector<HealthAction>& outHealthActionList)
{
    cout << "m_curHealthAction.size(): " << m_curHealthActionList.size() << endl;

    for (size_t i = 0; i < m_curHealthActionList.size(); i++)
    {
        outHealthActionList.push_back(m_curHealthActionList[i]);
    }
    return true;
}

void AIclass::PrintInfo()
{
    //testing
    cout << m_className << "   " << m_classNo << "   " << m_numAction << "   " << m_durationClassSetting << endl;
    for (int i = 0; i < m_numAction; i++)
    {
        m_curHealthActionList[i].PrintInfo();
    }
    cout << endl;
}

void AIclass::ReadClassScheduleFromFile(string inClassScheduleFileName)
{
    std::ifstream inF;
    inF.open(inClassScheduleFileName, ios::in);

    //【需要检查读函数的写法是否正确】
    inF >> m_className;
    inF >> m_classNo;
    inF >> m_numAction;
    //课程时间，单位为毫秒
    inF >> m_durationClassSetting;

   
    for (int i = 0; i < m_numAction; i++)
    {
        AIClassSchedule curSchedule;

        inF >> curSchedule.actionName;
        inF >> curSchedule.actionType;
        inF >> curSchedule.startMilliSecond;

        m_classScheduleList.push_back(curSchedule);
    }
    inF.close();
}