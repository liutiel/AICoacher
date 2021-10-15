#pragma once

#include <QObject>
#include "commonincludes.h"
#include "AIcoacher.h"

class AIcoacherHelper : public QObject
{
	Q_OBJECT

public:
	explicit AIcoacherHelper();
	~AIcoacherHelper();

signals:
	void coach_init_result_signal(bool isSuccessful, int input_stream);
	void coach_init_result_signal(bool isSuccessful, QString input_stream);
	void coach_one_frame_signal();
	void frame_finished_signal();

public slots:
	void coach_init_slot(int classID, int input_stream);	// For webcam
	void coach_init_slot(int classID, QString input_stream);	// For files
	void coach_start_slot();	// Start proccessing frames

public:
	bool m_besuspended;
	bool m_beended;
	AIcoacher* GetAICoacherPr();	// Pointer of AICoacher library

private:
	AIcoacher m_aicoacher;

};

