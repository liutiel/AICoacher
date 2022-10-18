#pragma once

// Standard libraries
#include <malloc.h>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <semaphore>
#include <thread>
#include <array>

// Classes inside this project
#include "HealthAction.h"

// Information of health actions
typedef struct HealthActionInfomation
{
	std::string name = "";
	int limit_count = 0;
	int limit_time = 0;
};

class __declspec(dllexport) HealthCourse
{
public:
	// Empty class
	HealthCourse();

	// Initialize health course from .txt configuration file
	HealthCourse(std::binary_semaphore* singal_course_end_ptr, std::string course_config_name,
		std::binary_semaphore* handle_action_refresh_ptr,
		std::binary_semaphore* handle_count_refresh_ptr,
		std::binary_semaphore* handle_time_refresh_ptr);

	// Set current health action by action index
	bool startHealthAction(int new_action_index);

	// Shift to the next health action
	bool nextHealthAction();

	// Shift to the next health action when an action is ended
	bool internalNextHealthAction();

	// Receive the current frame and judge whether to transfer to the next key frame
	bool actionTransferCheck(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> person_pose);

	// Pause or resume course
	void setCourseActive(bool input_action_state);
	void stopCourse();

	// Getters
	std::string getCourseName();
	std::string getActionName();
	// HealthAction* getCurrentHealthActionPtr();
	int getActionCount();
	int getActionTime();
	int getActionCountLimit();
	int getActionTimeLimit();
	int getActionPerfection();

private:
	// Release the signal about the end of a course
	std::binary_semaphore* singal_course_end_ptr;

	// Receive the signal about the end of an action
	std::binary_semaphore* singal_action_end_ptr = new std::binary_semaphore(0);

	// Handles for sending out information
	std::binary_semaphore* handle_action_refresh_ptr;
	std::binary_semaphore* handle_count_refresh_ptr;
	std::binary_semaphore* handle_time_refresh_ptr;

	// Course information
	std::string COURSE_FILE_DIR = "../configs/health_course/";
	std::string course_name = "";
	bool is_course_started = false;
	bool is_course_ended = false;

	// Action information
	std::vector<HealthActionInfomation> health_action_list;
	HealthAction* current_health_action = new HealthAction();
	int current_health_action_index = -1;

	// Threads
	std::thread thread_internal_action_shift;		// AICoacher stop from internal
};

