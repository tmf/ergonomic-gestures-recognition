#ifndef MOUSE_H
#define MOUSE_H

#include <QtGui/QMainWindow>
#include "ui_mouse.h"
#include <QTimer>
#include "EGRHand.h"
#include "egr_dlib.h"
#include "dlib/svm.h"
#include "cv.h"

using namespace cv;

class mouse : public QMainWindow
{
	Q_OBJECT

public:
	mouse(QWidget *parent = 0, Qt::WFlags flags = 0);
	~mouse();
public slots:
	void frame();
	void tracker();
	void test();
private:
	Ui::mouseClass ui;
	EGRHand *hand;
	QTimer timer;
	IplImage *buffer;
	bool visible;
	bool trackerVisible;
	void handleGesture(string newposture);
	string filterPostures(string newposture, int window_size);
	void updateMousePointer(cv::Point p);
	void updateZoom(int d);
	void slide(bool hide);
	cv::Point pMouse;
	cv::Point pMouseVelocity;
	cv::Point minActionSpace;
	cv::Point maxActionSpace;

	int zoomDistance;
	int zoomVelocity;
	int maxZoomDistance;
	int minZoomDistance;

	list<string> postures;
};

#endif // MOUSE_H
