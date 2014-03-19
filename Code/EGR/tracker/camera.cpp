#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include "cleyecameracapture.h"
#include "global.h"

bool useAVIasSource=false;

CvVideoWriter *avi=0;
bool recordingAVI = false;
bool doRecordAVI = false;
bool stopRecordingAVI = false;
std::string logFile;
std::string aviFile;
//std::string logLine = "";
int frame_counter=0;

bool ready(CLEyeCameraCapture *cam){
	
	return cam!=NULL && cam->pCapImage!=NULL;
}

void camera_setup(){
	int i,j, n = CLEyeGetCameraCount();
	StorageObject *so_cameras = getStorageObject(std::string("cameras"));
	if(so_cameras->data == 0)
		so_cameras->data = new std::vector<CLEyeCameraCapture *>();
	std::vector<CLEyeCameraCapture *> *cameras = (std::vector<CLEyeCameraCapture *> *)so_cameras->data;

	if(cameras->size()==0){
		for(i=0, j=0; i<n; i++){
			GUID guid;
			guid = CLEyeGetCameraUUID(i);
			
			//if(guid!=GUID_NULL){//GUID_NULL
				CLEyeCameraCapture *cam = new CLEyeCameraCapture(guid, CLEYE_COLOR_RAW, CLEYE_VGA , 60);
				cam->StartCapture();
				cameras->push_back(cam);
				//char *name = new char[32];
				//sprintf(name, "camera_%c", 'A' + j);
				std::stringstream str;
				str<<"camera_"<< ('A' + j);
				std::string name(str.str());
				strSourceList.push_back(name);
				StorageObject *so_camera = getStorageObject(name);
				if(so_camera->data == 0)
					so_camera->data = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 4);
				j++;

			//}
		}
	}
	/*
	int ps3_eye = 0;
	ps3_eye_left = NULL;
	//ps3_eye_right=NULL;
	GUID guid1 = CLEyeGetCameraUUID(ps3_eye);
	GUID guid2 = CLEyeGetCameraUUID(ps3_eye+1);
	ps3_eye_left = new CLEyeCameraCapture(guid1, CLEYE_COLOR, CLEYE_VGA , 60);
	//ps3_eye_right = new CLEyeCameraCapture(guid2, CLEYE_COLOR_RAW, CLEYE_VGA , 60);
	ps3_eye_left->StartCapture();
	//ps3_eye_right->StartCapture();
*/
}
void camera_stop(){
	StorageObject *so_cameras = getStorageObject(std::string("cameras"));
	if(so_cameras->data == 0)
		return;
	std::vector<CLEyeCameraCapture *> *cameras = (std::vector<CLEyeCameraCapture *> *)so_cameras->data;
	for(int i=0; i < cameras->size();i++){
		cameras->at(i)->StopCapture();
		//CLEyeCameraCapture *cam = (CLEyeCameraCapture*)cameras[i];
		//cam->StopCapture();
	}
}

void saveScreenshot(IplImage *img){
	char *fname = new char[256];
	char *filterName = new char[256];
	
	if(screenSteps[currentScreen-1]->flt != 0){
		sprintf(filterName, "%s", screenSteps[currentScreen-1]->flt->name.c_str());
		for(int i=0;i<256;i++){
			if(filterName[i]==' ')
				filterName[i]='_';
			if(filterName[i]=='\0')
				break;
		}
		time_t rawtime;
		struct tm * timeinfo;
		char strTime [13];
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		strftime (strTime,13,"%m%d%H%M%S",timeinfo);

		sprintf(fname, "c:\\egr\\%s_%s.jpg", filterName, strTime);
		cvSaveImage(fname, img);
	}
	delete fname;
	delete filterName;

}
void prepareAVI(int fps){
	
	if(avi == 0){
		char *tmp = new char[256];
		char *fname = new char[256];
		char *filterName = new char[256];
		frame_counter = 0;
		time_t rawtime;
		struct tm * timeinfo;
		char strTime [13];
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		strftime (strTime,13,"%m%d%H%M%S",timeinfo);
		sprintf(tmp, "c:\\egr\\%s", aviFile.c_str());
		sprintf(fname, tmp, strTime);
		logFile = std::string("c:\\egr\\tracker_").append(std::string(strTime)).append(".log");
		
		avi = cvCreateVideoWriter(fname, CV_FOURCC('M', 'P', '4', '2'), fps, cvSize(WIDTH, HEIGHT));
		delete fname;
		delete filterName;
	}else{
		cvReleaseVideoWriter(&avi);
		
	}
	
}

void writeAVI(IplImage *img){
	if(avi!=0){
		IplImage *tmp;
		if(img->nChannels==4){
			tmp = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
			cvCvtColor(img, tmp, CV_BGRA2BGR);
		}else{
			tmp = img;
		}
		/*LONGLONG g_Frequency, g_CurentCount, g_LastCount;
		QueryPerformanceFrequency((LARGE_INTEGER*)&g_Frequency);
		QueryPerformanceCounter((LARGE_INTEGER*)&g_CurentCount);*/
		cvWriteFrame(avi, tmp);
		frame_counter++;
		/*QueryPerformanceCounter((LARGE_INTEGER*)&g_LastCount);

		double t = 1000 * (((double)(g_LastCount-g_CurentCount))/((double)g_Frequency));
		printf("frame: %f s", t);*/
	
	}
}
