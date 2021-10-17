#include "VideoIO.h"

VideoIO::VideoIO()
{
	//
}

VideoIO::~VideoIO()
{
	//
}

bool VideoIO::InitInputStream(std::string input_stream)
{
	m_instream = cv::VideoCapture(input_stream);
	if (m_instream.isOpened()) return true;
	else return false;
}

bool VideoIO::InitInputStream(int input_stream)
{
	m_instream = cv::VideoCapture(input_stream);
	if (m_instream.isOpened()) return true;
	return false;
}

bool VideoIO::InitOutputStream(std::string output_stream)
{
	m_outstream = cv::VideoWriter(output_stream,
		cv::VideoWriter::fourcc('m', 'p', '4', 'v'),
		m_instream.get(cv::CAP_PROP_FPS),
		cv::Size(m_instream.get(cv::CAP_PROP_FRAME_WIDTH), m_instream.get(cv::CAP_PROP_FRAME_HEIGHT)));

	return true;
}

bool VideoIO::DestroyInputStream()
{
	m_instream.release();
	return true;
}

bool VideoIO::DestroyOutputStream()
{
	m_outstream.release();
	cv::destroyAllWindows();
	return true;
}

bool VideoIO::CaptureNextFrame()
{
	if (m_instream.isOpened() && m_instream.read(m_current_frame))
	{
		m_current_frame.copyTo(m_original_frame);
		
		m_skeleton_frame = Mat::ones(m_current_frame.size(), CV_8UC3); 

		m_frame_count++;
		return true;
	}
	else
	{
		return false;
	}
}

void VideoIO::connect_keypoints(PersonPosePara pose, int pt1, int pt2, cv::Mat* pInImage)
{
	cv::Mat current_frame = (NULL == pInImage) ? m_current_frame : *pInImage;

	cv::line(current_frame,
			cv::Point(int(pose.pointPose[pt1 * 2 ]), int(pose.pointPose[pt1 * 2+ 1])),
			cv::Point(int(pose.pointPose[pt2 * 2]), int(pose.pointPose[pt2 * 2 + 1])),
		cv::Scalar(255, 255, 255), 5);//cv::Scalar(89, 90, 90), 3);

	return;
}

void VideoIO::PutPointInfo(PersonPosePara pose, cv::Mat* pInImage)
{
	cv::Mat current_frame = (NULL == pInImage) ? m_current_frame : *pInImage;
	for (int i = 0; i < OUTPUT_CNLS; i++)
	{
		pose.pointPose[i * 2] = pose.pointPose[i * 2] / RESULT_IMAGE_SIZE * m_current_frame.cols;
		pose.pointPose[i * 2 + 1] = pose.pointPose[i * 2 + 1] / RESULT_IMAGE_SIZE * m_current_frame.rows;
	}

	connect_keypoints(pose, HEAD, NECK, pInImage);
	connect_keypoints(pose, SHOULDER_L, NECK, pInImage);
	connect_keypoints(pose, SHOULDER_R, NECK, pInImage);
	connect_keypoints(pose, SHOULDER_L, ELBOW_L, pInImage);
	connect_keypoints(pose, SHOULDER_R, ELBOW_R, pInImage);
	connect_keypoints(pose, ELBOW_L, WRIST_L, pInImage);
	connect_keypoints(pose, ELBOW_R, WRIST_R, pInImage);
	connect_keypoints(pose, HIP_L, NECK, pInImage);
	connect_keypoints(pose, HIP_R, NECK, pInImage);
	connect_keypoints(pose, HIP_L, KNEE_L, pInImage);
	connect_keypoints(pose, HIP_R, KNEE_R, pInImage);
	connect_keypoints(pose, KNEE_L, ANKLE_L, pInImage);
	connect_keypoints(pose, KNEE_R, ANKLE_R, pInImage);

	// Draw points
	for (int i = 0; i < OUTPUT_CNLS; i++)
	{
		cv::circle(current_frame,
			cv::Point(int(pose.pointPose[i * 2 ]), int(pose.pointPose[i * 2+ 1])),
			8, cv::Scalar(244, 186, 57), -1);
		std::stringstream output_text;
		output_text << pose.pointPose[i * 2 ] << ", " << pose.pointPose[i * 2+ 1];
		/*
		cv::putText(current_frame, output_text.str(), 
			cv::Point(int(pose.pointPose[i * 2 ]), int(pose.pointPose[i * 2+ 1])), 
			cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(244, 186, 57), 1, cv::LINE_8, false);
		*/
	}
}

void VideoIO::PutCountInfo(int action_count, cv::Mat* pInImage)
{
	cv::Mat current_frame = (NULL == pInImage) ? m_current_frame : *pInImage;
	std::stringstream output_text;
		
	output_text.str("");
		
	output_text << "Count: " << action_count;
		
	cv::putText(current_frame, output_text.str(), cv::Point(10, 70), cv::FONT_HERSHEY_SIMPLEX, 2.5, cv::Scalar(0, 0, 255), 6, cv::LINE_8, false);

}

void VideoIO::PutTipsInfo(string tips, cv::Mat* pInImage)
{
	cv::Mat current_frame = (NULL == pInImage) ? m_current_frame : *pInImage;
	std::stringstream output_text;

	output_text.str("");

	output_text << tips;

	cv::putText(current_frame, output_text.str(), cv::Point(10, 140), cv::FONT_HERSHEY_SIMPLEX, 2.5, cv::Scalar(0, 0, 255), 6, cv::LINE_8, false);

}

void VideoIO::PutFPSInfo(double fps,cv::Mat* pInImage)
{
	cv::Mat current_frame = (NULL == pInImage) ? m_current_frame : *pInImage;


	std::stringstream output_text;
	
	output_text.str("");

	output_text << "FPS: " << std::setiosflags(std::ios::fixed) << std::setprecision(2) << fps;
	
	cv::putText(current_frame, output_text.str(), cv::Point(10, 60), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(244, 186, 57), 3, cv::LINE_8, false);

}

cv::Mat VideoIO::GetCurrentFrame()
{
	return m_current_frame;
}

cv::Mat VideoIO::GetOriginalFrame()
{
	return m_original_frame;
}
cv::Mat VideoIO::GetSkeletonFrame()
{
	return m_skeleton_frame;
}
cv::Mat* VideoIO::GetSkeletonFramePr()
{
	return &m_skeleton_frame;
}

void VideoIO::ShowAndSaveFrame(std::string output_image_prefix, bool b_saveImage)
{
//	if (!isRunningOnQt)
	{
		cv::imshow("Inference", m_current_frame);

		if (true == b_saveImage)
		{
			std::stringstream filename;
			filename.str("");

			filename << output_image_prefix << m_frame_count << ".jpg";
			cv::imwrite(filename.str(), m_current_frame);
		}

		m_outstream.write(m_current_frame);
	}
}

void VideoIO::SaveFrame(std::string output_image_prefix, bool b_saveImage)
{


		if (true == b_saveImage)
		{
			std::stringstream filename;
			filename.str("");

			filename << output_image_prefix << m_frame_count << ".jpg";

			cv::imwrite(filename.str(), m_current_frame);
		}

		m_outstream.write(m_current_frame);

}

int VideoIO::GetFrameCount()
{
	return m_frame_count;
}