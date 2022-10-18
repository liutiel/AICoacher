#pragma once

// Strandard libraties
#include <cmath>
#include <array>

// Project libraries
#include "PoseDetector.h"

class __declspec(dllexport) WorkoutParameters
{
public:
	void CalculateWorkoutPara(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose);

	// Calculating the cosine value of a joint
	static bool CosineCalc(double& cosResult, double central_x, double central_y, double pt1_x, double pt1_y, double pt2_x, double pt2_y);

	// Whether wrists are higher than shoulders
	//static bool IsHandOverShoulder(int dis, std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose);
	static bool IsHandOverShoulder(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose);

	// Whether legs are open
	//static bool IsLegsOpen(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose);

	/// Whether 
	static bool IsSide(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose);

	// Calculating the cosine value of knees
	static bool CosKnee(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose);

	// Whether right knee is raised
	static bool IsKneeRaisedR(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose);

	// Whether left knee is raised
	static bool IsKneeRaisedL(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose);

	// Whether right shoulder and right elbow are on the same side
	static bool IsSameSideShoulderElbowR(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose);

	// Whether left shoulder and left elbow are on the same side
	static bool IsSameSideShoulderElbowL(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose);

	// Judging whether right ankle is raised by calculating distance between ankles
	static bool IsAnkleRaisedR(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose);

	// Judging whether left ankle is raised by calculating distance between ankles
	static bool IsAnkleRaisedL(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose);

	// Whether knees are higher than the wrists
	static bool IsKneeOverHand(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose);

	// Whether arms are vertical enough
	static bool IsArmStraightV(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose);

	// Whether arms are horizontal enough
	static bool IsArmStraightL(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose);

	// Whether ankles are closed
	static bool IsAnkleClose(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose);

	// Whether legs are vertical enough
	static bool IsLegUpright(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose);

	// Whether both wrists are over head
	static bool IsHandOverHead(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose);

	// Whether right elbow is higher than the left one
	static bool IsElbowHigherR(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose);

	// Whether left elbow is higher than the right one
	static bool IsElbowHigherL(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose);

};

