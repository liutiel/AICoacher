#pragma once

// Strandard libraties
#include <stdexcept>
#include <string>
#include <malloc.h>
#include <array>

// OpenCV
#include <opencv2/opencv.hpp>

// TFLite
#include "tensorflow/lite/c/c_api.h"

class __declspec(dllexport) PoseDetector
{
public:
	// Empty class
	PoseDetector();

	// Initialization
	PoseDetector(int thread_num);

	// Static features of the model
	const static int DETECTION_MODEL_SIZE = 192;
	const static int DETECTION_MODEL_CHANNEL = 3;
	const static int DETECTION_MODEL_OUTPUT_SIZE = 96;
	const static int DETECTION_MODEL_OUTPUT_CHANNEL = 14;

	// Body parts
	const static int HEAD = 0;
	const static int NECK = 1;
	const static int SHOULDER_R = 2;
	const static int ELBOW_R = 3;
	const static int WRIST_R = 4;
	const static int SHOULDER_L = 5;
	const static int ELBOW_L = 6;
	const static int WRIST_L = 7;
	const static int HIP_R = 8;
	const static int KNEE_R = 9;
	const static int ANKLE_R = 10;
	const static int HIP_L = 11;
	const static int KNEE_L = 12;
	const static int ANKLE_L = 13;
	const static int POINT_X = 0;
	const static int POINT_Y = 1;

	// Whether the model is loaded properly
	bool is_model_working = false;

	// Calculated the human pose from the frame
	bool calculatePersonPose(cv::Mat srcImg, std::array<std::array<double, 2>, DETECTION_MODEL_OUTPUT_CHANNEL>& current_person_pose_ptr, int frame_height, int frame_width);

private:
	// Initialize the model parameters
	std::string inFileName = "../models/pose.tflite";
	int thread_num = 4;
	bool is_model_quantized = false;

	// Model input / processor / output
	TfLiteInterpreter* tflite_interpreter = nullptr;
	TfLiteTensor* tflite_input_tensor = nullptr;
	const TfLiteTensor* tflite_output_points = nullptr;

	// Initialize the tflite model
	bool InitPoseDetector();
};

