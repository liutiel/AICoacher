#pragma once

// standard libraries
#include <string>
#include <cmath>

// windows
#include <windows.h>

// opencv
#include <opencv2/opencv.hpp>

// Project libraries
#include "PoseDetector.h"

class __declspec(dllexport) VideoIO
{
public:
	// Empty class
	VideoIO();

	// Capture frames from a webcam
	VideoIO(int camera_port);
	// Capture frames from a webcam (with output)
	VideoIO(int camera_port, std::string video_save_path);

	// Capture frames from a video file
	VideoIO(std::string video_path);
	// Capture frames from a video file (with output)
	VideoIO(std::string video_path, std::string video_save_path);

	// Refresh the latest frame
	bool refreshCurrentFrame();

	// Getters
	int displayFrameOriginal();	// Display original frames using OpenCV
	int displayFrameSkeleton(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose_points, int point_size, cv::Scalar point_colour, int line_weight, cv::Scalar line_colour);	// Display skeleton frames using OpenCV
	int displayFrameFusion(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose_points, int point_size, cv::Scalar point_colour, int line_weight, cv::Scalar line_colour);	// Display fusion (original + skeleton) frames using OpenCV
	cv::Mat getFrameOriginal();	// Get the original frame
	cv::Mat getFrameSkeleton(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose_points, int point_size, cv::Scalar point_colour, int line_weight, cv::Scalar line_colour);	// Get the pure skeleton frame
	cv::Mat getFrameFusion(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose_points, int point_size, cv::Scalar point_colour, int line_weight, cv::Scalar line_colour);	// Get the frame with both original image and the skeleton
	double getOriginalFrameInterval();	// Get the original frame interval
	int getFrameHeight();	// Get frame height
	int getFrameWidth();	// Get frame width

private:
	// OpenCV APIs
	cv::VideoCapture stream_read = cv::VideoCapture();
	cv::VideoWriter stream_write = cv::VideoWriter();

	// Frames
	cv::Mat frame_original = cv::Mat();

	// Original frame interval
	double original_frame_interval = 0.0;

	// Whether current frame valid
	bool is_frame_current_valid = false;

	// Draw the skeleton
	bool drawSkeleton(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose_points, cv::Mat* frame_to_draw, int point_size, cv::Scalar point_colour, int line_weight, cv::Scalar line_colour);

	// Display frames using OpenCV
	int displayFrame(cv::Mat frame_input);

	// Get monitor width (Windows)
	bool obtainMonitorResolution(int* horizontal, int* vertical);

	// Get video resize ratio
	double obtainFrameResizeRatio(int horizontal_container, int vertical_container, int horizontal_stream, int vertical_stream);
};

