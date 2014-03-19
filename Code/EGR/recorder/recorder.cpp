#include "recorder.h"
#include "tracker.h"
#include "highgui.h"
#include "srt2label.h"

recorder::recorder(QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	EGRTrackerLoadConfiguration("c:\\egr\\tracker.cfg");
	buffer = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, 4);
	EGRTrackerStart();
	
	timer.setSingleShot(false);
	connect(&timer, SIGNAL(timeout()), this, SLOT(frame()));
	timer.start(25);
	connect(ui.saveTrackerConfig, SIGNAL(clicked()), this, SLOT(saveTracker()));
	connect(ui.recordAVI, SIGNAL(clicked()), this, SLOT(recordTracker()));
	connect(ui.showTracker, SIGNAL(clicked()), this, SLOT(showTracker()));
	recording = false;
	tracker=false;
}

void recorder::frame() {
	IplImage *tracker_frame = (IplImage *) EGRTrackerGetImage("Screen 9");
	if(tracker_frame!=0){
		cvResize(tracker_frame, buffer);
		Mat xy = Mat(buffer);
		ui.videoWidget->sendImage(&xy);
	}
	return;
}
void recorder::saveTracker(){
	EGRTrackerSaveConfiguration("c:\\egr\\tracker.cfg");
}
void recorder::recordTracker(){
	if(recording){
		ui.recordAVI->setText(QString("Record AVI"));
		recording = false;
		EGRTrackerStopRecording();
		test();
	}else{
		ui.recordAVI->setText(QString("Stop"));
		recording = true;
		EGRTrackerStartRecording(ui.aviFilename->text().toStdString());
	}
}
void recorder::showTracker(){
	if(tracker){
		ui.showTracker->setText(QString("Configure..."));
		tracker = false;
		EGRTrackerHide();
	}else{
		ui.showTracker->setText(QString("Hide Tracker"));
		tracker = true;
		EGRTrackerShow();
	}
}
void recorder::test(){
	vector<string> labels = srt2label("c:\\egr\\tracker_0103135033.avi");

}
recorder::~recorder()
{

}
