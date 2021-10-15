#include "AIcoacherHelper.h"

AIcoacherHelper::AIcoacherHelper()
{
	qRegisterMetaType<string>("string");

	m_besuspended = false;
}

AIcoacherHelper::~AIcoacherHelper()
{
	//
}

// For webcam
void AIcoacherHelper::coach_init_slot(int classID, int input_stream)
{

}

// For files
void AIcoacherHelper::coach_init_slot(int classID, QString input_stream)
{
	if (m_aicoacher.Init(classID, input_stream.toStdString()))
	{
		emit coach_init_result_signal(true, input_stream);
	}
	else
	{
		emit coach_init_result_signal(false, input_stream);
	}
}

// Start proccessing frames
void AIcoacherHelper::coach_start_slot()
{
	bool b_processing = true; 
	do
	{
		if (false == m_besuspended && false == m_beended)
		{
			b_processing = m_aicoacher.AIcoacherFrameProcessing();

			emit coach_one_frame_signal();
		}
		else
		{
			break;
		}

		if (true == m_aicoacher.IsActionFinished())
		{
			emit frame_finished_signal();
			m_aicoacher.GetCurrentVideoIO().DestroyInputStream();
			break;
		}

	} while (b_processing);

	if ((!(true == m_besuspended && false == m_beended))|| !b_processing)
	{
		emit frame_finished_signal();
	}

}

// Pointer of AICoacher library
AIcoacher* AIcoacherHelper::GetAICoacherPr()
{
	return &m_aicoacher;
}
