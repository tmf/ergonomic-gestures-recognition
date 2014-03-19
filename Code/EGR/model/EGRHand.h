#ifdef EGRHANDLIBRARY_EXPORT // inside DLL
#define DLL	__declspec(dllexport)
#define API	WINAPI
#else // outside DLL
#define DLL __declspec(dllimport)
#define API __stdcall
#endif  // EGRHANDLIBRARY_EXPORT
#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <list>
#include "cv.h"
#include "EGRPoint.h"
#include "EGRPosture.h"
#include "dlib/svm.h"

using namespace std;
using namespace dlib;

typedef matrix<double, 10, 1> sample_fingers_type;
typedef one_vs_one_trainer<any_trainer<sample_fingers_type>, string> ovo_trainer;
typedef polynomial_kernel<sample_fingers_type> poly_kernel; 
typedef one_vs_one_decision_function<ovo_trainer, decision_function<poly_kernel>> classifier;

#define MAX_FINGERS 5
#define MAX_ESTIMATED_FINGERS 3
#define HISTORY_SIZE 50
#define MODE_CLASSIFY 100
#define MODE_TRAIN 200

struct Point{
	int x;
	int y;
};

typedef void (*EGRCallback)(int oldGesture, int newGesture);

struct EGRHand {
	virtual void init(std::string trackerConfig, std::string fingerBlob, std::string wristBlob) = 0;
	virtual void log(std::string l) = 0;
	virtual EGRPoint getWristPosition() = 0;
	virtual std::list<EGRPoint> getFingerPositions() = 0;
	virtual bool start() = 0;
	virtual void startTracking() = 0;
	virtual void startRecognition() = 0;
	virtual void stop() = 0;
	virtual void stopTracking() = 0;
	virtual void stopRecognition() = 0;

	virtual void updateWristPosition(::Point p, int frame) = 0;
	virtual void updateFingerPosition(::Point p, int frame) = 0;
	virtual void adjust() = 0;

	virtual void setMode(int mode) = 0;
	virtual IplImage* getTrackerImage() = 0;
	virtual IplImage* getTrackerImage(std::string screen) = 0;
	virtual void livePause(bool hand) = 0;
	virtual void frameForward() = 0;
	virtual std::string currentLabel() = 0;
	virtual float getFPS() = 0;
	virtual void drawFingers(IplImage *img, float scale)=0;
	virtual void setFinishedCallback(void (*callback)(void)) = 0;
	virtual void setAVISource(std::string avi) = 0;
	virtual void pause(bool pauseTracker) = 0;
	virtual std::vector<EGRPosture> &getPostures() = 0;
	virtual void addPosture(EGRPosture p) = 0;
	virtual void removePosture(int index) = 0;
    virtual void clearPostures() = 0;

	virtual void setClassifier(classifier &c) = 0;
	virtual classifier getClassifier() = 0;
	virtual string getClassifiedPosture() = 0;
	virtual int getMode()=0;
	virtual void trackerConfiguration(bool show) = 0;
};


extern "C" DLL EGRHand* API createEGRHand(void);