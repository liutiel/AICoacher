#include "pch.h"
#include "VideoIO.h"

VideoIO::VideoIO()
{

}

VideoIO::VideoIO(int camera_port)
{
	this->stream_read = cv::VideoCapture(camera_port);

	this->original_frame_interval = 0.0;
}

VideoIO::VideoIO(int camera_port, std::string video_save_path)
{
	this->stream_read = cv::VideoCapture(camera_port);

	this->stream_write = cv::VideoWriter(video_save_path,
		cv::VideoWriter::fourcc('m', 'p', '4', 'v'),
		this->stream_read.get(cv::CAP_PROP_FPS),
		cv::Size(this->stream_read.get(cv::CAP_PROP_FRAME_WIDTH),
			this->stream_read.get(cv::CAP_PROP_FRAME_HEIGHT)));

	this->original_frame_interval = 0.0;
}

VideoIO::VideoIO(std::string video_path)
{
	this->stream_read = cv::VideoCapture(video_path);

	this->original_frame_interval = this->stream_read.get(cv::CAP_PROP_FPS);

	std::cout << this->original_frame_interval << std::endl;
}

VideoIO::VideoIO(std::string video_path, std::string video_save_path)
{
	this->stream_read = cv::VideoCapture(video_path);

	this->stream_write = cv::VideoWriter(video_save_path,
		cv::VideoWriter::fourcc('m', 'p', '4', 'v'),
		this->stream_read.get(cv::CAP_PROP_FPS),
		cv::Size(this->stream_read.get(cv::CAP_PROP_FRAME_WIDTH),
			this->stream_read.get(cv::CAP_PROP_FRAME_HEIGHT)));

	this->original_frame_interval = this->stream_read.get(cv::CAP_PROP_FPS);
}

bool VideoIO::refreshCurrentFrame()
{
	if (this->stream_read.isOpened() == false || this->stream_read.read(this->frame_original) == false)
	{
		this->is_frame_current_valid = false;
		this->stream_read.release();
		return false;
	}
	else
	{
		this->is_frame_current_valid = true;
		return true;
	}
}

bool VideoIO::obtainMonitorResolution(int* horizontal, int* vertical)
{
	*vertical = GetSystemMetrics(SM_CYSCREEN);
	*horizontal = GetSystemMetrics(SM_CXSCREEN);

	return true;
}

double VideoIO::obtainFrameResizeRatio(int horizontal_container, int vertical_container, int horizontal_stream, int vertical_stream)
{
	double resize_ratio = 1.0;

	double resize_ratio_X = double(horizontal_container) / double(horizontal_stream);
	double resize_ratio_Y = double(vertical_container) / double(vertical_stream);

	resize_ratio = fmin(fmin(resize_ratio_X, resize_ratio_Y), 1.0);

	return resize_ratio;
}

bool VideoIO::drawSkeleton(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose_points, cv::Mat* frame_to_draw, int point_size, cv::Scalar point_colour, int line_weight, cv::Scalar line_colour)
{
	cv::line(*frame_to_draw,
		cv::Point(pose_points[PoseDetector::HEAD][PoseDetector::POINT_X], pose_points[PoseDetector::HEAD][PoseDetector::POINT_Y]),
		cv::Point(pose_points[PoseDetector::NECK][PoseDetector::POINT_X], pose_points[PoseDetector::NECK][PoseDetector::POINT_Y]),
		line_colour, point_size);
	cv::line(*frame_to_draw,
		cv::Point(pose_points[PoseDetector::SHOULDER_L][PoseDetector::POINT_X], pose_points[PoseDetector::SHOULDER_L][PoseDetector::POINT_Y]),
		cv::Point(pose_points[PoseDetector::NECK][PoseDetector::POINT_X], pose_points[PoseDetector::NECK][PoseDetector::POINT_Y]),
		line_colour, point_size);
	cv::line(*frame_to_draw,
		cv::Point(pose_points[PoseDetector::SHOULDER_R][PoseDetector::POINT_X], pose_points[PoseDetector::SHOULDER_R][PoseDetector::POINT_Y]),
		cv::Point(pose_points[PoseDetector::NECK][PoseDetector::POINT_X], pose_points[PoseDetector::NECK][PoseDetector::POINT_Y]),
		line_colour, point_size);
	cv::line(*frame_to_draw,
		cv::Point(pose_points[PoseDetector::SHOULDER_L][PoseDetector::POINT_X], pose_points[PoseDetector::SHOULDER_L][PoseDetector::POINT_Y]),
		cv::Point(pose_points[PoseDetector::ELBOW_L][PoseDetector::POINT_X], pose_points[PoseDetector::ELBOW_L][PoseDetector::POINT_Y]),
		line_colour, point_size);
	cv::line(*frame_to_draw,
		cv::Point(pose_points[PoseDetector::SHOULDER_R][PoseDetector::POINT_X], pose_points[PoseDetector::SHOULDER_R][PoseDetector::POINT_Y]),
		cv::Point(pose_points[PoseDetector::ELBOW_R][PoseDetector::POINT_X], pose_points[PoseDetector::ELBOW_R][PoseDetector::POINT_Y]),
		line_colour, point_size);
	cv::line(*frame_to_draw,
		cv::Point(pose_points[PoseDetector::ELBOW_L][PoseDetector::POINT_X], pose_points[PoseDetector::ELBOW_L][PoseDetector::POINT_Y]),
		cv::Point(pose_points[PoseDetector::WRIST_L][PoseDetector::POINT_X], pose_points[PoseDetector::WRIST_L][PoseDetector::POINT_Y]),
		line_colour, point_size);
	cv::line(*frame_to_draw,
		cv::Point(pose_points[PoseDetector::ELBOW_R][PoseDetector::POINT_X], pose_points[PoseDetector::ELBOW_R][PoseDetector::POINT_Y]),
		cv::Point(pose_points[PoseDetector::WRIST_R][PoseDetector::POINT_X], pose_points[PoseDetector::WRIST_R][PoseDetector::POINT_Y]),
		line_colour, point_size);
	cv::line(*frame_to_draw,
		cv::Point(pose_points[PoseDetector::HIP_L][PoseDetector::POINT_X], pose_points[PoseDetector::HIP_L][PoseDetector::POINT_Y]),
		cv::Point(pose_points[PoseDetector::NECK][PoseDetector::POINT_X], pose_points[PoseDetector::NECK][PoseDetector::POINT_Y]),
		line_colour, point_size);
	cv::line(*frame_to_draw,
		cv::Point(pose_points[PoseDetector::HIP_R][PoseDetector::POINT_X], pose_points[PoseDetector::HIP_R][PoseDetector::POINT_Y]),
		cv::Point(pose_points[PoseDetector::NECK][PoseDetector::POINT_X], pose_points[PoseDetector::NECK][PoseDetector::POINT_Y]),
		line_colour, point_size);
	cv::line(*frame_to_draw,
		cv::Point(pose_points[PoseDetector::HIP_L][PoseDetector::POINT_X], pose_points[PoseDetector::HIP_L][PoseDetector::POINT_Y]),
		cv::Point(pose_points[PoseDetector::KNEE_L][PoseDetector::POINT_X], pose_points[PoseDetector::KNEE_L][PoseDetector::POINT_Y]),
		line_colour, point_size);
	cv::line(*frame_to_draw,
		cv::Point(pose_points[PoseDetector::HIP_R][PoseDetector::POINT_X], pose_points[PoseDetector::HIP_R][PoseDetector::POINT_Y]),
		cv::Point(pose_points[PoseDetector::KNEE_R][PoseDetector::POINT_X], pose_points[PoseDetector::KNEE_R][PoseDetector::POINT_Y]),
		line_colour, point_size);
	cv::line(*frame_to_draw,
		cv::Point(pose_points[PoseDetector::KNEE_L][PoseDetector::POINT_X], pose_points[PoseDetector::KNEE_L][PoseDetector::POINT_Y]),
		cv::Point(pose_points[PoseDetector::ANKLE_L][PoseDetector::POINT_X], pose_points[PoseDetector::ANKLE_L][PoseDetector::POINT_Y]),
		line_colour, point_size);
	cv::line(*frame_to_draw,
		cv::Point(pose_points[PoseDetector::KNEE_R][PoseDetector::POINT_X], pose_points[PoseDetector::KNEE_R][PoseDetector::POINT_Y]),
		cv::Point(pose_points[PoseDetector::ANKLE_R][PoseDetector::POINT_X], pose_points[PoseDetector::ANKLE_R][PoseDetector::POINT_Y]),
		line_colour, point_size);

	// Draw points with coordinates
	for (int i = 0; i < PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL; i++)
	{
		cv::Point current_point = cv::Point(int(pose_points[i][PoseDetector::POINT_X]), int(pose_points[i][PoseDetector::POINT_Y]));

		cv::circle(*frame_to_draw, current_point, point_size, point_colour, -1);
		std::stringstream output_text;
		output_text << int(pose_points[i][PoseDetector::POINT_X]) << ", " << int(pose_points[i][PoseDetector::POINT_Y]);

		cv::putText(*frame_to_draw, output_text.str(), current_point, cv::FONT_HERSHEY_SIMPLEX, 1, point_colour, 1, cv::LINE_8, false);

	}

	return true;
}

// Display original frames using OpenCV
int VideoIO::displayFrameOriginal()
{
	this->displayFrame(this->getFrameOriginal());
	return 0;
}

// Display skeleton frames using OpenCV
int VideoIO::displayFrameSkeleton(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose_points, int point_size, cv::Scalar point_colour, int line_weight, cv::Scalar line_colour)
{
	this->displayFrame(this->getFrameSkeleton(pose_points, point_size, point_colour, line_weight, line_colour));
	return 0;
}

// Display fusion (original + skeleton) frames using OpenCV
int VideoIO::displayFrameFusion(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose_points, int point_size, cv::Scalar point_colour, int line_weight, cv::Scalar line_colour)
{
	this->displayFrame(this->getFrameFusion(pose_points, point_size, point_colour, line_weight, line_colour));
	return 0;
}

// Display frames using OpenCV
int VideoIO::displayFrame(cv::Mat frame_input)
{
	int monitor_horizontal = 0;
	int monitor_vertical = 0;
	cv::Mat frame_display = cv::Mat();

	if (this->obtainMonitorResolution(&monitor_horizontal, &monitor_vertical))
	{
		double resize_ratio = this->obtainFrameResizeRatio(0.8 * monitor_horizontal, 0.8 * monitor_vertical, this->getFrameWidth(), this->getFrameHeight());
		cv::resize(frame_input, frame_display, cv::Size(0, 0), resize_ratio, resize_ratio, cv::INTER_LINEAR);
	}
	else
	{
		frame_display = frame_input;
	}

	cv::imshow("AICoacher - Orininal Stream", frame_display);
	cv::waitKey(1);

	return 0;
}

/*
* Getters
*/
// Get the original frame
cv::Mat VideoIO::getFrameOriginal() { return this->frame_original; }

// Get the pure skeleton frame
cv::Mat VideoIO::getFrameSkeleton(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose_points, int point_size, cv::Scalar point_colour, int line_weight, cv::Scalar line_colour)
{
	if (is_frame_current_valid)
	{
		cv::Mat frame_skeleton = cv::Mat::zeros(cv::Size(this->getFrameWidth(), this->getFrameHeight()), CV_8UC3);
		this->drawSkeleton(pose_points, &frame_skeleton, point_size, point_colour, line_weight, line_colour);
		return frame_skeleton;
	}
	else
	{
		return cv::Mat();
	}
}

// Get the frame with both original image and the skeleton
cv::Mat VideoIO::getFrameFusion(std::array<std::array<double, 2>, PoseDetector::DETECTION_MODEL_OUTPUT_CHANNEL> pose_points, int point_size, cv::Scalar point_colour, int line_weight, cv::Scalar line_colour)
{
	if (is_frame_current_valid)
	{
		cv::Mat frame_fusion = this->frame_original.clone();
		this->drawSkeleton(pose_points, &frame_fusion, point_size, point_colour, line_weight, line_colour);
		return frame_fusion;
	}
	else
	{
		return cv::Mat();
	}
}

// Get original frame time interval of a video file
double VideoIO::getOriginalFrameInterval() { return this->original_frame_interval; }

// Get frame height
int VideoIO::getFrameHeight() { return this->stream_read.get(cv::CAP_PROP_FRAME_HEIGHT); }

// Get frame width
int VideoIO::getFrameWidth()
{
	return this->stream_read.get(cv::CAP_PROP_FRAME_WIDTH);
}