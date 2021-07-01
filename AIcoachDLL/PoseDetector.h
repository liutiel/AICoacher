#pragma once
//捕获当前摄像头范围内的人体姿势参数；
//输入输出都是cv::Mat格式的数据

#include "commonincludes.h"
// tflite
#include "tensorflow/lite/c/c_api.h"

template<class ForwardIterator>
inline size_t argmax(ForwardIterator first, ForwardIterator last)
{
	return std::distance(first, std::max_element(first, last));
}

class PoseDetector
{
private:
	//当前人体姿势参数；
	PersonPosePara m_currentPersonPose;


	bool m_modelQuantized = false;
	TfLiteModel* m_model = nullptr;
	TfLiteInterpreter* m_interpreter =nullptr;
	TfLiteTensor* m_input_tensor = nullptr;
	const TfLiteTensor* m_output_points = nullptr;


public:
	PoseDetector();
	~PoseDetector();

	//初始化模型参数，读取默认路径下的模型参数
	bool InitModelPara();

	//初始化模型参数，读取指定路径下的模型参数
	bool InitModelPara(const char* inFileName);

	//计算人体姿势；——修改此函数增加输入图像作为参数
	bool CalculatePersonPose(cv::Mat srcImg);

	//获得人体姿势数据
	PersonPosePara GetCurrentPersonPose();
};

