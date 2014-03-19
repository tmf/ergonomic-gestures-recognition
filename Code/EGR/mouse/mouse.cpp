#include "mouse.h"
#include <iostream>
#include <QCursor>
#include <QPropertyAnimation>
#include <QFileDialog>

mouse::mouse(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	connect(&timer, SIGNAL(timeout()), this, SLOT(frame()));
	connect(ui.btnTrackerConfiguration, SIGNAL(clicked()), this, SLOT(tracker()));
	//connect(ui.toolButton, SIGNAL(clicked()), this, SLOT(test()));

	trackerVisible = false;
	visible = false;
	
	QDesktopWidget *desktop = QApplication::desktop();
	int w =  desktop->screenGeometry().width();
	int h = desktop->screenGeometry().height();

	

	ui.lblBG->setGeometry(0, 0, width(), h);
	ui.lblLogo->setGeometry(0, 0, width(), h);
	
	minActionSpace = cvPoint(w, h);
	maxActionSpace = cvPoint(0, 0);
	pMouse = cvPoint(0,0);
	pMouseVelocity = cvPoint(0,0);

	zoomDistance = 0;
	zoomVelocity = 0;
	maxZoomDistance = 0;
	minZoomDistance = 9999;
	
	hand = createEGRHand();
	timer.setSingleShot(false);
	buffer = cvCreateImage(cvSize(160, 120), IPL_DEPTH_8U, 4);
	QString trackerConfigFile("c:\\egr\\tracker.cfg");
	if(!QFile::exists(trackerConfigFile)){
		trackerConfigFile = QFileDialog::getOpenFileName ( this, tr("Please select a tracker configuration file"), tr("c:\\egr"), tr("EGR Tracker Configuration Files (*.cfg)"));
	}
	hand->init(trackerConfigFile.toStdString(), "blobs_5", "blobs_6");
	classifier df;
	QString classifierFile("c:\\egr\\3gest.egrdf");
	if(!QFile::exists(classifierFile)){
		classifierFile = QFileDialog::getOpenFileName ( this, tr("Please select a trained decision function file"), tr("c:\\egr"), tr("EGR Decision Function Files (*.egrdf)"));
	}
	ifstream fin(classifierFile.toStdString().c_str(), ios::binary);
    deserialize(df, fin);
	hand->setClassifier(df);
	hand->start();
	timer.start(35);
	
	
}
void mouse::tracker(){
	hand->trackerConfiguration(!trackerVisible);
	trackerVisible = !trackerVisible;
}
void mouse::frame(){
	
		IplImage *tracker_frame = (IplImage *) hand->getTrackerImage("Screen 8");
		string posture = hand->getClassifiedPosture();
		if(tracker_frame!=0){
			ui.fps->setText(QString::number(hand->getFPS(), 10, 2));
			
			handleGesture(posture);
			cvResize(tracker_frame, buffer);
			hand->drawFingers(buffer,(float) buffer->width/tracker_frame->width);
			Mat xy = Mat(buffer);
			ui.video->sendImage(&xy);
		}
	
}
string mouse::filterPostures(string newposture, int window_size){
	if(postures.size()>=HISTORY_SIZE){
		postures.pop_front();
	}
	postures.push_back(newposture);
	map<string, int> histogram;
	int j=0;
	for(std::list<string>::iterator i=postures.end(); i!=postures.begin() && j<window_size;j++){
		i--;
		histogram[(*i)] = 0;
	}
	j=0;
	for(std::list<string>::iterator i=postures.end(); i!=postures.begin() && j<window_size;j++){
		i--;
		histogram[(*i)] = histogram[(*i)] + 1;
	}
	int max=0;
	string robustPosture = "";
	for(map<string, int>::iterator i=histogram.begin(); i!=histogram.end(); i++){
		if(i->second>max){
			max = i->second;
			robustPosture = i->first;
		}
	}
	
	return robustPosture;
}
void mouse::handleGesture(string newposture){
	
	string posture = filterPostures(newposture, 10);
	if(posture.compare("")==0){
		ui.lblZoom->setAutoFillBackground (false);
	}
	ui.posture->setText(QString::fromStdString(posture));
	if(posture.compare(string("POINT"))==0){
		if(newposture.compare(posture)==0){
			cv::Point p;
			p.y = 9999;
			list<EGRPoint> fingers = hand->getFingerPositions();
			for(list<EGRPoint>::iterator i = fingers.begin(); i!=fingers.end(); i++){
				if((*i).y < p.y){
					p.x = i->x;
					p.y = i->y;
					
				}
			}
			
			updateMousePointer(p);
		}
	}else if(posture.compare(string("SWIPELEFT"))==0){
		if(newposture.compare(string("SWIPERIGHT"))==0){
			slide(false);
		}
	}else if(posture.compare(string("SWIPERIGHT"))==0){
		if(newposture.compare(string("SWIPELEFT"))==0){
			slide(true);
		}
	}else if(posture.compare(string("ZOOM"))==0){
		if(newposture.compare(posture)==0){
			int d=0;

			cv::Point p1;
			cv::Point p2;
			p1.y = 9999;
			p2.y = 9999;
			list<EGRPoint> fingers = hand->getFingerPositions();
			for(list<EGRPoint>::iterator i = fingers.begin(); i!=fingers.end(); i++){
				if((*i).y < p2.y && (*i).y > p1.y){
					p2.x = i->x;
					p2.y = i->y;
					
					
				}
				if((*i).y < p1.y){
					
					p1.x = i->x;
					p1.y = i->y;
					
				}
			}
			/*
			list<EGRPoint> fingers = hand->getFingerPositions();
			list<EGRPoint>::iterator indexFinger = fingers.begin();
			list<EGRPoint>::iterator thumbFinger = fingers.end();
			thumbFinger--;
			int dx = indexFinger->x - thumbFinger->x;
			int dy = indexFinger->y - thumbFinger->y;*/
			int dx = p2.x - p1.x;
			int dy = p2.y - p1.y;
			d = sqrt((double)(dx*dx + dy*dy));
			updateZoom(d);
		}
	}
}
void mouse::updateZoom(int d){
	ui.lblZoom->setAutoFillBackground (true);
	float D = 0.4;
	float friction = 0.4;
	int dd =   d-zoomDistance;
	zoomVelocity = ((float)dd*D + (float)zoomVelocity)*friction;
	zoomDistance = zoomDistance + zoomVelocity;
	if(zoomDistance > maxZoomDistance){
		maxZoomDistance = zoomDistance;
	}
	if(d<minZoomDistance){
		minZoomDistance = d;
	}
	if((maxZoomDistance)>0){
		float z = (float)(zoomDistance-minZoomDistance)/(maxZoomDistance);
		z = z>0.0 ?z:0;
		ui.lblZoom->setGeometry(ui.lblZoom->pos().x(), ui.lblZoom->pos().y(), 160*z, 20);
	}
}
void mouse::updateMousePointer(cv::Point p){
	if(p.x>0 && p.y>0){
		
		p.x  = p.x*2.25;
		p.y  = p.y * 1.875;

		float D = 0.4;
		float friction = 0.4;
		
		int dx = p.x - pMouse.x;
		int dy = p.y - pMouse.y;
		pMouseVelocity.x = ((float)dx*D + (float)pMouseVelocity.x)*friction;
		pMouseVelocity.y = ((float)dy*D + (float)pMouseVelocity.y)*friction;
		pMouse.x = pMouse.x + pMouseVelocity.x;
		pMouse.y = pMouse.y + pMouseVelocity.y;
		
		if(pMouse.x < minActionSpace.x)
			minActionSpace.x = pMouse.x;
		if(pMouse.y < minActionSpace.y)
			minActionSpace.y = pMouse.y;
		if(pMouse.x > maxActionSpace.x)
			maxActionSpace.x = pMouse.x;
		if(pMouse.y > maxActionSpace.y)
			maxActionSpace.y = pMouse.y;

		int actionSpaceWidth = maxActionSpace.x - minActionSpace.x;
		int actionSpaceHeight = maxActionSpace.y  - minActionSpace.y;

		if(actionSpaceWidth >0 && actionSpaceHeight>0){
		float x = (float)(pMouse.x - minActionSpace.x)/actionSpaceWidth;
		float y = (float)(pMouse.y - minActionSpace.y)/actionSpaceHeight;

		QDesktopWidget *desktop = QApplication::desktop();
		int w =  desktop->screenGeometry().width();
		int h = desktop->screenGeometry().height();
		
		QCursor::setPos(w- (float)(x*w), (float)(y*h));
		}
	}
}
void mouse::test(){
	slide(true);
}
void mouse::slide(bool hide){
	QDesktopWidget *desktop = QApplication::desktop();
	QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");
	animation->setDuration(800);
	animation->setEasingCurve(QEasingCurve::InOutQuad);
	if(hide){
		animation->setStartValue(this->pos());
		animation->setEndValue(QPoint(desktop->screenGeometry().width(),0));
	}else{
		animation->setStartValue(this->pos());
		animation->setEndValue(QPoint(desktop->screenGeometry().width() - this->width(), 0));
	}
	

	// to slide in call
	animation->start();
	
}
mouse::~mouse()
{

}
