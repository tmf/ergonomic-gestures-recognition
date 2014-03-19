#ifndef RECORDER_H
#define RECORDER_H

#include <QtGui/QMainWindow>
#include <QTimer>
#include "ui_recorder.h"

class recorder : public QMainWindow
{
	Q_OBJECT

public:
	recorder(QWidget *parent = 0, Qt::WFlags flags = 0);
	~recorder();

private:
	Ui::recorderClass ui;
	QTimer timer;
	IplImage *buffer;
	bool recording;
	bool tracker;
public slots:
	void frame();
	void saveTracker();
	void recordTracker();
	void showTracker();
	void test();
};

#endif // RECORDER_H
