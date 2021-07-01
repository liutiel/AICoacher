#include "FileIO.h"


//bool FileIO::writePosePointInfo(, const std::string saveFileName)
//{
//
//    // 写文件  
//    ofstream outf;
//    outf.open(saveFileName, ios::out); // 打开模式可省略  
//
//    //write the 表头
////    outf << "单木编号" << ',' << "位置X坐标 " << ',' << "位置Y坐标" << ',' << "树高" << ',' << "冠幅" << endl;/    outf << "TREEID" << ',' << "TLX" << ',' << "TLY" << ',' << "TH" << ',' << "CROWND" << endl;
//    //outf << "TREEID" << ', ' << "TLX" << ', ' << "TLY" << ', ' << "TH" << ', ' << "CROWND" << endl;
//    outf << "TreeID" << ',' << "TreeLocationX" << ',' << "TreeLocationY" << ',' << "TreeHeight" << ',' << "CrownDiameter" << endl;
//
//    TreeInfo.resize(tree_center->size());
//
//    for (size_t i = 0; i < tree_center->size(); i++)
//    {
//
//        pcl::PointXYZ pnt = tree_center->points[i];
//        if (true == b_offsetadded)
//        {
//            pnt.x += m_xOffset;
//            pnt.y += m_yOffset;
//
//        }
//        //calculate 树冠
//        double Cw = 0.003164 * pow(pnt.z, 2) + 0.1311 * pnt.z + 1.708;
//
//        //将每棵树信息存入TreeInfo
//        TreeInfo[i].id = i + 1;
//        TreeInfo[i].pnt.x = pnt.x;
//        TreeInfo[i].pnt.y = pnt.y;
//        TreeInfo[i].pnt.z = pnt.z;
//        TreeInfo[i].Cw = Cw;
//
//        //write the data into the file 
//       //outf << i + 1 << ' ' << setiosflags(ios::fixed) << setprecision(3) << pnt.x << ' ' << pnt.y << ' ' <<  pnt.z << ' ' << Cw << endl;
//        //outf << i + 1 << ',' << setiosflags(ios::fixed) << setprecision(3) << pnt.x << ',' << pnt.y << ',' << pnt.z << ',' << Cw << endl;
//
//        //将树木信息写入CSV文件
//        outf << TreeInfo[i].id << ',' << setiosflags(ios::fixed) << setprecision(3) << TreeInfo[i].pnt.x << ',' << TreeInfo[i].pnt.y << ',' << TreeInfo[i].pnt.z << ',' << TreeInfo[i].Cw << endl;
//    }
//    outf.close();
//    return 0;
//
//}
///****************************** finished***************************/
//bool FileIO::readTreeInfo(pcl::PointCloud<pcl::PointXYZ>::Ptr tree_center, const std::string readFileName)
//{
//
//    // 读文件  
//    ifstream inf;
//    inf.open(readFileName, ios::in); // 打开模式可省略  
//
// //   tree_center->clear();
//
//    char buffer[25600];
//    //读取表头
//    inf.getline(buffer, 256, '\n');//getline(char *,int,char) 表示该行字符达到256个或遇到换行就结束
//    cout << buffer << endl;
//    while (!inf.eof())
//    {
//
//        pcl::PointXYZ pnt;
//        int  id;
//        //int index;
//        float Cw;
//        string buffTmp;
//        while (getline(inf, buffTmp))
//        {
//            //        inf >> index >> pnt.x >> pnt.y >> pnt.z >> Cw;
//            stringstream stringIn(buffTmp);//使用串流实现对string的输入输出操作
//            //while (stringIn >> dTmp)
//            //    dBeamsX.push_back(dTmp);
//           // stringIn >> index;
//            //stringIn >> pnt.x;
//            //stringIn >> pnt.y;
//            //stringIn >> pnt.z;
//            //stringIn >> Cw;
//
//
//            std::string cell;
//            std::getline(stringIn, cell, ',');
//            std::getline(stringIn, cell, ',');
//            pnt.x = atof(cell.c_str());
//            std::getline(stringIn, cell, ',');
//            pnt.y = atof(cell.c_str());
//            std::getline(stringIn, cell, ',');
//            pnt.z = atof(cell.c_str());
//            tree_center->push_back(pnt);
//
//            //    cout << buffTmp << endl;
//        }
//
//    }
//    inf.close();
//
//    return true;
//}