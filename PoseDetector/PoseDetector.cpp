#include "pch.h"
#include "PoseDetector.h"


PoseDetector::PoseDetector()
{
	//
}

PoseDetector::PoseDetector(int thread_num)
{
	// Receive the parameters
	this->thread_num = thread_num;

	// Initialize the tflite model
	this->InitPoseDetector();
}

// Initialize the tflite model
bool PoseDetector::InitPoseDetector()
{
	TfLiteModel* model_file = TfLiteModelCreateFromFile(this->inFileName.c_str());
	if (model_file == nullptr)
	{
		std::cout << "Failed to load model" << std::endl;
		return false;
	}

	// Build the interpreter
	TfLiteInterpreterOptions* options = TfLiteInterpreterOptionsCreate();
	TfLiteInterpreterOptionsSetNumThreads(options, thread_num);
	std::cout << "Number of thread(s): " << thread_num << std::endl;

	// Create the interpreter.
	this->tflite_interpreter = TfLiteInterpreterCreate(model_file, options);
	if (this->tflite_interpreter == nullptr)
	{
		std::cout << "Failed to create interpreter" << std::endl;
		return false;
	}

	// Allocate tensor buffers.
	if (TfLiteInterpreterAllocateTensors(this->tflite_interpreter) != kTfLiteOk)
	{
		std::cout << "Failed to allocate tensors!" << std::endl;
		return false;
	}

	// Find input tensors.
	if (TfLiteInterpreterGetInputTensorCount(this->tflite_interpreter) != 1)
	{
		std::cout << "Detection model graph needs to have 1 and only 1 input!" << std::endl;
		return false;
	}

	this->tflite_input_tensor = TfLiteInterpreterGetInputTensor(this->tflite_interpreter, 0);
	if (this->is_model_quantized && this->tflite_input_tensor->type != kTfLiteUInt8)
	{
		std::cout << "Detection model input should be kTfLiteUInt8!" << std::endl;
		return false;
	}

	if (!this->is_model_quantized && this->tflite_input_tensor->type != kTfLiteFloat32)
	{
		std::cout << "Detection model input should be kTfLiteFloat32!" << std::endl;
		return false;
	}

	if (this->tflite_input_tensor->dims->data[0] != 1
		|| this->tflite_input_tensor->dims->data[1] != this->DETECTION_MODEL_SIZE
		|| this->tflite_input_tensor->dims->data[2] != this->DETECTION_MODEL_SIZE
		|| this->tflite_input_tensor->dims->data[3] != this->DETECTION_MODEL_CHANNEL)
	{
		std::printf("Detection model must have input dims of 1x%ix%ix%i", this->DETECTION_MODEL_SIZE, this->DETECTION_MODEL_SIZE, this->DETECTION_MODEL_CHANNEL);
		return false;
	}

	// Find output tensors.
	if (TfLiteInterpreterGetOutputTensorCount(this->tflite_interpreter) != 1)
	{
		std::cout << "Detection model graph needs to have 1 and only 1 output!" << std::endl;
		return false;
	}

	this->tflite_output_points = TfLiteInterpreterGetOutputTensor(this->tflite_interpreter, 0);
	return true;
}

bool PoseDetector::calculatePersonPose(cv::Mat srcImg, std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL>& current_person_pose_ptr, int frame_height, int frame_width)
{
	cv::Mat srccpy;
	cv::resize(srcImg, srccpy, cv::Size(this->DETECTION_MODEL_SIZE, this->DETECTION_MODEL_SIZE), 0, 0, cv::INTER_NEAREST);

	srccpy.convertTo(srccpy, CV_32FC3);

	// Initialize the matrix for storing inference result
	std::array<std::array<float, this->DETECTION_MODEL_OUTPUT_CHANNEL>, this->DETECTION_MODEL_OUTPUT_SIZE * this->DETECTION_MODEL_OUTPUT_SIZE> inference_result;

	// Copy image into input tensor
	float* dst = this->tflite_input_tensor->data.f;
	memcpy(dst, srccpy.data, sizeof(float) * this->DETECTION_MODEL_SIZE * this->DETECTION_MODEL_SIZE * this->DETECTION_MODEL_CHANNEL);

	if (TfLiteInterpreterInvoke(this->tflite_interpreter) != kTfLiteOk)
	{
		printf("Error invoking detection model");
		this->is_model_working = false;
		return this->is_model_working;
	}
	const float* tensor_extract = this->tflite_output_points->data.f;
	for (int x = 0; x < this->DETECTION_MODEL_OUTPUT_SIZE * this->DETECTION_MODEL_OUTPUT_SIZE; x++)
	{
		for (int y = 0; y < this->DETECTION_MODEL_OUTPUT_CHANNEL; y++)
		{
			int idx = x * this->DETECTION_MODEL_OUTPUT_CHANNEL + y;
			inference_result[x][y] = tensor_extract[idx];
		}
	}

	for (int i = 0; i < this->DETECTION_MODEL_OUTPUT_CHANNEL; i++)
	{
		double max_val = 0;
		int max_idx = 0;
		for (int j = 0; j < this->DETECTION_MODEL_OUTPUT_SIZE * this->DETECTION_MODEL_OUTPUT_SIZE; j++)
		{
			if (inference_result[j][i] > max_val)
			{
				max_val = inference_result[j][i];
				max_idx = j;
			}
		}

		// Write result into current_person_pose	
		current_person_pose_ptr[i][this->POINT_Y] = double(max_idx / (DETECTION_MODEL_SIZE / 2)) * (double(frame_height) / double(this->DETECTION_MODEL_OUTPUT_SIZE));
		current_person_pose_ptr[i][this->POINT_X] = double(max_idx % (DETECTION_MODEL_SIZE / 2)) * (double(frame_width) / double(this->DETECTION_MODEL_OUTPUT_SIZE));
	}

	this->is_model_working = true;
	return this->is_model_working;
}