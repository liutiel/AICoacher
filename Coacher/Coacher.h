#pragma once

// Standard libraries
#include <string>
#include <chrono>
#include <semaphore>
#include <array>

// Classes inside this project
#include "VideoIO.h"
#include "PoseDetector.h"
#include "HealthCourse.h"

// typedef
typedef std::map<std::string, std::map<std::string, double>> PersonPosePara;

class __declspec(dllexport) Coacher
{
public:
	// Empty class
	Coacher();
	
	// Capture frames from a webcam
	Coacher(double frame_rate, int camera_port, std::string course_config_name, int tflite_thread,
		std::binary_semaphore* handle_frame_refresh_ptr,
		std::binary_semaphore* handle_action_refresh_ptr,
		std::binary_semaphore* handle_course_end_ptr,
		std::binary_semaphore* handle_count_refresh_ptr,
		std::binary_semaphore* handle_time_refresh_ptr);
	// Capture frames from a video file
	Coacher(double frame_rate, std::string video_path, std::string course_config_name, int tflite_thread,
		std::binary_semaphore* handle_frame_refresh_ptr,
		std::binary_semaphore* handle_action_refresh_ptr,
		std::binary_semaphore* handle_course_end_ptr,
		std::binary_semaphore* handle_count_refresh_ptr,
		std::binary_semaphore* handle_time_refresh_ptr);

	// Capture the latest frame from the stream
	int frameRefreshing();

	// Process a frame according to the set FPS
	int frameProcessing();

	/* Thread control*/
	void startCoacher();	// Start Coacher
	void pauseCoacher();	// Pause Coacher
	void resumeCoacher();	// Resume Coacher
	void stopCoacher();	// Stop Coacher

	// Frame display mode
	static const int DISPLAY_ORIGINAL = 0;	// Display original frame
	static const int DISPLAY_SKELETON = 1;	// Display skeleton frame
	static const int DISPLAY_FUSION = 2;	// Display fusion (original + skeleton) frame

	// Handles for returning data asynchronously
	std::binary_semaphore* handle_frame_refresh_ptr;
	std::binary_semaphore* handle_action_refresh_ptr;
	std::binary_semaphore* handle_course_end_ptr;
	std::binary_semaphore* handle_count_refresh_ptr;
	std::binary_semaphore* handle_time_refresh_ptr;

	/* Extract information from the class */
	std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> getPosePoints();
	bool displayFrameCV(int display_mode);
	cv::Mat getCurrentFrame(int display_mode);
	int getActionCount();
	int getActionTime();
	int getActionCountLimit();
	int getActionTimeLimit();
	std::string getActionName();
	bool getActionPerfection();
	bool isThreadRunning();

private:
	// Current person pose
	/* FLAG: POSE_REFRESH
	double** person_pose;
	*/
	std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> person_pose;

	// The interval between frames which should be processed
	int frame_interval_in_ms = 0;

	// Initialize classes
	VideoIO video_io = VideoIO();	// The VideoIO class
	PoseDetector pose_detector = PoseDetector();	// The PoseDetector class
	HealthCourse health_course = HealthCourse();	// The HealthCourse class

	// Status
	bool is_thread_kept = true;	// Wheather the processing thread is started or not
	bool is_thread_running = false;	// Wheather the processing thread is running or not (paused)
	bool is_frame_available = false;
	int display_mode = 0;	// Display mode

	// Receive the signal about the end of a course
	std::binary_semaphore* singal_course_end_ptr = new std::binary_semaphore(0);

	// Coacher stop from internal
	int internalStopCoacher();

	// Threads
	std::thread thread_frame_refreshing;	// Thread for refreshing current frame
	std::thread thread_frame_processing;	// Thread for processing a frame
	std::thread thread_internal_stop;		// Coacher stop from internal
};

