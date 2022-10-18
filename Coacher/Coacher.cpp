#include "pch.h"
#include "Coacher.h"

// Capture frames from a webcam
Coacher::Coacher()
{
	//
}

// Capture frames from a webcam
Coacher::Coacher(double frame_rate, int camera_port, std::string course_config_name, int tflite_thread,
	std::binary_semaphore* handle_frame_refresh_ptr,
	std::binary_semaphore* handle_action_refresh_ptr,
	std::binary_semaphore* handle_course_end_ptr,
	std::binary_semaphore* handle_count_refresh_ptr,
	std::binary_semaphore* handle_time_refresh_ptr)
{
	this->frame_interval_in_ms = int(1000 / frame_rate);

	this->handle_frame_refresh_ptr = handle_frame_refresh_ptr;
	this->handle_action_refresh_ptr = handle_action_refresh_ptr;
	this->handle_course_end_ptr = handle_course_end_ptr;
	this->handle_count_refresh_ptr = handle_count_refresh_ptr;
	this->handle_time_refresh_ptr = handle_time_refresh_ptr;

	this->video_io = VideoIO(camera_port);
	this->pose_detector = PoseDetector(4);
	this->health_course = HealthCourse(this->singal_course_end_ptr, course_config_name,
		this->handle_action_refresh_ptr,
		this->handle_count_refresh_ptr,
		this->handle_time_refresh_ptr);
}

// Capture frames from a video file
Coacher::Coacher(double frame_rate, std::string video_path, std::string course_config_name, int tflite_thread, 
	std::binary_semaphore* handle_frame_refresh_ptr,
	std::binary_semaphore* handle_action_refresh_ptr,
	std::binary_semaphore* handle_course_end_ptr,
	std::binary_semaphore* handle_count_refresh_ptr,
	std::binary_semaphore* handle_time_refresh_ptr)
{
	this->frame_interval_in_ms = int(1000 / frame_rate);

	this->handle_frame_refresh_ptr = handle_frame_refresh_ptr;
	this->handle_action_refresh_ptr = handle_action_refresh_ptr;
	this->handle_course_end_ptr = handle_course_end_ptr;
	this->handle_count_refresh_ptr = handle_count_refresh_ptr;
	this->handle_time_refresh_ptr = handle_time_refresh_ptr;

	this->video_io = VideoIO(video_path);
	this->pose_detector = PoseDetector(4);
	this->health_course = HealthCourse(this->singal_course_end_ptr, course_config_name, 
		this->handle_action_refresh_ptr,
		this->handle_count_refresh_ptr,
		this->handle_time_refresh_ptr);
}

// Capture the latest frame from the stream
int Coacher::frameRefreshing()
{
	while (this->is_thread_kept)
	{
		if (this->is_thread_running)
		{
			// Processing starting time
			std::chrono::steady_clock::time_point time_process_start = std::chrono::steady_clock::now();

			this->is_frame_available = this->video_io.refreshCurrentFrame();

			if (!this->is_frame_available)
			{
				this->stopCoacher();
			}

			// Processing time
			std::chrono::steady_clock::time_point time_process_end = std::chrono::steady_clock::now();
			std::chrono::duration<double, std::milli> timespan_process = std::chrono::duration<double, std::milli>(time_process_end - time_process_start);

			// Thread sleep for a while
			if (timespan_process.count() < this->video_io.getOriginalFrameInterval())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(int(this->video_io.getOriginalFrameInterval())) - timespan_process);
			}
		}
	}
	return 0;
}

// Process a frame according to the set FPS
int Coacher::frameProcessing()
{
	while (this->is_thread_kept)
	{
		if (this->is_thread_running)
		{
			if (this->is_frame_available)
			{
				// Processing starting time
				std::chrono::steady_clock::time_point time_process_start = std::chrono::steady_clock::now();

				// Calculate the keypoints
				this->pose_detector.calculatePersonPose(this->video_io.getFrameOriginal(), this->person_pose, this->video_io.getFrameHeight(), this->video_io.getFrameWidth());

				// Course stat
				this->health_course.actionTransferCheck(this->person_pose);

				// Processing time
				std::chrono::steady_clock::time_point time_process_end = std::chrono::steady_clock::now();
				std::chrono::duration<double, std::milli> timespan_process = std::chrono::duration<double, std::milli>(time_process_end - time_process_start);

				// Show the frames
				this->handle_frame_refresh_ptr->release();


				// Thread sleep for a while
				if (timespan_process.count() < this->frame_interval_in_ms)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(this->frame_interval_in_ms) - timespan_process);
				}
			}
		}
	}
	return 0;
}

/* Extract information from the class */
std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> Coacher::getPosePoints(){return this->person_pose;}	// Get pose point
bool Coacher::displayFrameCV(int display_mode)	// Display frames using VideoIO APIs
{
	switch (display_mode)
	{
	case this->DISPLAY_ORIGINAL:
		this->video_io.displayFrameOriginal();
	case this->DISPLAY_SKELETON:
		this->video_io.displayFrameSkeleton(this->person_pose, 8, cv::Scalar(255, 255, 255), 3, cv::Scalar(255, 255, 255));
	case this->DISPLAY_FUSION:
		this->video_io.displayFrameFusion(this->person_pose, 8, cv::Scalar(255, 255, 255), 3, cv::Scalar(255, 255, 255));
	default:
		break;
	}
	return true;
}

cv::Mat Coacher::getCurrentFrame(int display_mode)	// Return current frame
{
	cv::Mat frame_to_get;
	switch (display_mode)
	{
	case this->DISPLAY_ORIGINAL:
		frame_to_get = this->video_io.getFrameOriginal();
		break;
	case this->DISPLAY_SKELETON:
		frame_to_get = this->video_io.getFrameSkeleton(this->person_pose, 8, cv::Scalar(255, 255, 255), 3, cv::Scalar(255, 255, 255));
		break;
	case this->DISPLAY_FUSION:
		frame_to_get = this->video_io.getFrameFusion(this->person_pose, 8, cv::Scalar(255, 255, 255), 3, cv::Scalar(255, 255, 255));
		break;
	default:
		break;
	}
	return frame_to_get;
}

/* Getters */
int Coacher::getActionCount() { return this->health_course.getActionCount(); }	// Return current action count
int Coacher::getActionTime() { return this->health_course.getActionTime(); }	// Return current action time
int Coacher::getActionCountLimit() { return this->health_course.getActionCountLimit(); }	// Return current action count limit
int Coacher::getActionTimeLimit() { return this->health_course.getActionTimeLimit(); }	// Return current action time limit
std::string Coacher::getActionName() { return this->health_course.getActionName(); }	// Return current action name
bool Coacher::getActionPerfection() { return this->health_course.getActionPerfection(); }	// Whether the last count did perfectly
bool Coacher::isThreadRunning() { return this->is_thread_running; }

// Start Coacher
void Coacher::startCoacher()
{
	if (!(this->is_thread_kept && this->is_thread_running))
	{
		this->thread_frame_refreshing = std::thread(&Coacher::frameRefreshing, this);
		this->thread_frame_processing = std::thread(&Coacher::frameProcessing, this);
		this->thread_internal_stop = std::thread(&Coacher::internalStopCoacher, this);

		this->thread_frame_processing.detach();
		this->thread_frame_refreshing.detach();
		this->thread_internal_stop.detach();

		this->health_course.startHealthAction(0);
	}

	this->is_thread_kept = true;
	this->is_thread_running = true;
}

// Pause Coacher
void Coacher::pauseCoacher()
{
	this->is_thread_kept = true;
	this->is_thread_running = false;

	this->health_course.setCourseActive(false);
}

// Resume Coacher
void Coacher::resumeCoacher()
{
	this->is_thread_kept = true;
	this->is_thread_running = true;

	this->health_course.setCourseActive(true);
}

// Stop Coacher
void Coacher::stopCoacher()
{
	this->is_thread_kept = false;
	this->is_thread_running = false;

	this->health_course.stopCourse();
	this->handle_course_end_ptr->release();
}

// Internal stop Coacher
int Coacher::internalStopCoacher()
{
	this->singal_course_end_ptr->acquire();
	this->stopCoacher();

	return 0;
}