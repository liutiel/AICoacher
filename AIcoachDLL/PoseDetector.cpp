#include "PoseDetector.h"
PoseDetector::PoseDetector()
{
	m_modelQuantized = false;

	m_model = nullptr;
	m_interpreter = nullptr;
	m_input_tensor = nullptr;
	m_output_points = nullptr;
}


PoseDetector::~PoseDetector()
{
	if (m_model != nullptr)
	{
		TfLiteModelDelete(m_model);
	}
	if (m_interpreter != nullptr)
	{

	}

	if (m_input_tensor != nullptr)
	{

	}

	if (m_output_points != nullptr)
	{

	}
}
PersonPosePara PoseDetector::GetCurrentPersonPose()
{
	return m_currentPersonPose;
}



bool PoseDetector::InitModelPara()
{
	return true;
}

bool PoseDetector::InitModelPara(const char* inFileName)
{
	m_model = TfLiteModelCreateFromFile(inFileName);
	if (m_model == nullptr) 
	{
		cout << "Failed to load model" <<endl;
		return false;
	}

	// Build the interpreter
	TfLiteInterpreterOptions* options = TfLiteInterpreterOptionsCreate();
	TfLiteInterpreterOptionsSetNumThreads(options, THREAD_NUM);
	cout << "Number of thread(s): " << THREAD_NUM << endl;

	// Create the interpreter.
	m_interpreter = TfLiteInterpreterCreate(m_model, options);
	if (m_interpreter == nullptr) 
	{
		cout << "Failed to create interpreter" <<endl;
		return false;
	}

	// Allocate tensor buffers.
	if (TfLiteInterpreterAllocateTensors(m_interpreter) != kTfLiteOk) 
	{
		cout << "Failed to allocate tensors!" << endl;
		return false;
	}

	// Find input tensors.
	if (TfLiteInterpreterGetInputTensorCount(m_interpreter) != 1) 
	{
		cout << "Detection model graph needs to have 1 and only 1 input!" << endl;
		return false;
	}

	m_input_tensor = TfLiteInterpreterGetInputTensor(m_interpreter, 0);
	if (m_modelQuantized && m_input_tensor->type != kTfLiteUInt8) 
	{
		cout << "Detection model input should be kTfLiteUInt8!";
		return false;
	}

	if (!m_modelQuantized && m_input_tensor->type != kTfLiteFloat32) 
	{
		cout << "Detection model input should be kTfLiteFloat32!";
		return false;
	}

	if (m_input_tensor->dims->data[0] != 1 
		|| m_input_tensor->dims->data[1] != DETECTION_MODEL_SIZE 
		|| m_input_tensor->dims->data[2] != DETECTION_MODEL_SIZE 
		|| m_input_tensor->dims->data[3] != DETECTION_MODEL_CNLS) 
	{
		printf("Detection model must have input dims of 1x%ix%ix%i", DETECTION_MODEL_SIZE, DETECTION_MODEL_SIZE, DETECTION_MODEL_CNLS);
		return false;
	}

	// Find output tensors.
	if (TfLiteInterpreterGetOutputTensorCount(m_interpreter) != 1) 
	{
		cout << "Detection model graph needs to have 1 and only 1 output!";
		return false;
	}

	m_output_points = TfLiteInterpreterGetOutputTensor(m_interpreter, 0);

	return true;
}

bool PoseDetector::CalculatePersonPose(cv::Mat srcImg)
{
	cv::Mat srccpy;
	cv::resize(srcImg, srccpy, cv::Size(DETECTION_MODEL_SIZE, DETECTION_MODEL_SIZE), 0, 0, cv::INTER_NEAREST);

	srccpy.convertTo(srccpy, CV_32FC3);

	float inference_result[OUTPUT_SIZE * OUTPUT_SIZE][OUTPUT_CNLS];
	if (m_model == nullptr) 
	{
		return false;
	}

	// Copy image into input tensor
	float* dst = m_input_tensor->data.f;
	memcpy(dst, srccpy.data, sizeof(float) * DETECTION_MODEL_SIZE * DETECTION_MODEL_SIZE * DETECTION_MODEL_CNLS);
	
	if (TfLiteInterpreterInvoke(m_interpreter) != kTfLiteOk) 
	{
		printf("Error invoking detection model");
		return false;
	}
	const float* tensor_extract = m_output_points->data.f;
	for (int x = 0; x < OUTPUT_SIZE * OUTPUT_SIZE; x++)
	{
		for (int y = 0; y < OUTPUT_CNLS; y++)
		{
			int idx = x * OUTPUT_CNLS + y;
			inference_result[x][y] = tensor_extract[idx];
		}
	}

	for (int i = 0; i < OUTPUT_CNLS; i++)
	{
		double max_val = 0;
		int max_idx = 0;
		for (int j = 0; j < OUTPUT_SIZE * OUTPUT_SIZE; j++)
		{
			if (inference_result[j][i] > max_val) 
			{
				max_val = inference_result[j][i];
				max_idx = j;
			}
		}
	
	//将结果写入m_currentPersonPose中		
		m_currentPersonPose.pointPose[i * 2 + 1] = max_idx / (DETECTION_MODEL_SIZE / 2);
		m_currentPersonPose.pointPose[i * 2] = max_idx % (DETECTION_MODEL_SIZE / 2);
	}

	return true;
}