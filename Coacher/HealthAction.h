#pragma once

// Strandard libraties
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <thread>
#include <semaphore>
#include <cmath>
#include <array>

// Classes inside this project
#include "WorkoutParameters.h"

class __declspec(dllexport) HealthAction
{
public:
	// Empty class
	HealthAction();

	// Initialize health course from .txt configuration file
	HealthAction(std::binary_semaphore* singal_action_end_ptr, std::string action_config_name, int action_count_limit, int action_time_limit, 
		std::binary_semaphore* handle_count_refresh_ptr,
		std::binary_semaphore* handle_time_refresh_ptr);

	// Receive the current frame and judge whether to transfer to the next key frame
	bool actionTransferCheck(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> person_pose);

	// Start health action timer
	void startActionTimer();

	// Get the current action vector
	std::vector<int> getCurrentActionState(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> person_pose);

	// Compare the vectors
	bool actionVectorCompare(std::vector<int>key_frame_vector, std::vector<int> current_vector);

	// Pause or resume action
	void setActionActive(bool input_action_state);
	void stopAction();

	/*Getters*/
	std::string getActionName();
	int getActionCount();
	int getActionTime();
	int getActionCountLimit();
	int getActionTimeLimit();
	int getActionPerfection();

private:
	// Release the signal about the end of an action
	std::binary_semaphore* singal_action_end_ptr;

	// Handles for sending out information
	std::binary_semaphore* handle_count_refresh_ptr;
	std::binary_semaphore* handle_time_refresh_ptr;

	int count_limit = 0;
	int time_limit = 0;
	int current_count = 0;
	int current_time = 0;
	bool current_perfection = false;

	int current_key_frame_index = 0;
	int current_perfect_frame_index = 0;

	void secondTimer();
	void actionCountOnce();

	// Action information
	std::string ACTION_FILE_DIR = "../configs/health_action/";
	std::string action_name = "";
	bool isActionEnded = false;

	// Key frames
	std::vector<std::vector<int>> key_frame_list_complete;
	std::vector<std::vector<int>> key_frame_list_perfect;

	// Check time limit
	std::thread action_timer;

	// Pause or resume action
	bool is_action_active = false;
};

