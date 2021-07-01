#pragma once
#include "commonincludes.h"

class VideoIO
{
public:
	VideoIO();
	~VideoIO();

	// 定义运行环境
//	bool isRunningOnQt = false;

	// 初始化文件输入输出流
	bool InitInputStream(std::string input_stream);	// 从文件读取
	bool InitInputStream(int input_stream);	//从摄像头读取

	//【YHC-0421-1652】从外部获取输出路径，不要hardcode
//	bool InitOutputStream(); 
	bool InitOutputStream(std::string output_stream);

	bool DestroyInputStream();
	bool DestroyOutputStream();

	cv::Mat GetCurrentFrame();
	cv::Mat GetOriginalFrame();
	cv::Mat GetSkeletonFrame();
	cv::Mat* GetSkeletonFramePr();
	bool CaptureNextFrame();

	void PutPointInfo(PersonPosePara pose, cv::Mat* pInImage=NULL);
	void PutCountInfo(int count, cv::Mat* pInImage = NULL);
	void PutFPSInfo(double fps, cv::Mat* pInImage = NULL);

	//【YHC-0421-1654】从外部获取输出路径，不要hardcode   
//	void ShowAndSaveFrame(bool b_saveImage= false);
	void ShowAndSaveFrame(std::string output_image_prefix, bool b_saveImage = false);
	//	void SaveFrame(bool b_saveImage = true);
	void SaveFrame(std::string output_image_prefix, bool b_saveImage = true);

	int GetFrameCount();

private:
	//摄像头或者视频文件参数的定义：
	cv::VideoCapture m_instream;

	cv::VideoWriter m_outstream;

	cv::Mat m_current_frame;
	cv::Mat m_original_frame;

	cv::Mat m_skeleton_frame; //存储骨骼点

	int m_frame_count = 0;

	bool m_stream_terminated = false;

	void connect_keypoints(PersonPosePara pose, int pt1, int pt2, cv::Mat* pInImage=NULL);

};

