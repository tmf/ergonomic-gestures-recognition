#include "stdafx.h"
#include "srt2label.h"
#include "cv.h"
#include "highgui.h"
#include "stdio.h"

using namespace std;

DLL vector<string> APIENTRY srt2label(string aviFile){
	vector<string> result;
	CvCapture *avi = cvCaptureFromAVI(aviFile.c_str());
	if(!avi) return result;
	int fps = ( int )cvGetCaptureProperty( avi, CV_CAP_PROP_FPS );
	int totalFrames = (int) cvGetCaptureProperty(avi, CV_CAP_PROP_FRAME_COUNT);
	float frameDuration = 1000/fps;
	if(fps<=0) return result;
	cvReleaseCapture(&avi);
	string srtFile = aviFile;
	int extensionPosition = srtFile.find(".avi");
	if(extensionPosition!=string::npos){
		srtFile.replace(extensionPosition, 4, ".srt");
	}else{
		return result;
	}
	FILE * srt;
	srt = fopen (srtFile.c_str() , "r");
	if(srt==NULL)
		return result;
	int gn, sh, sm, ss, sms, eh, em, es, ems, startFrame, endFrame;
	char label[128];
	char tmp[128];
	startFrame=endFrame=gn=sh=sm=ss=sms=eh=em=es=ems=0;
	for(int i=0; i<totalFrames;i++){
		
		if(endFrame==i){
			fscanf(srt, "%d", &gn);
			fscanf(srt, "%d:%d:%d,%d --> %d:%d:%d,%d", &sh, &sm, &ss, &sms, &eh, &em, &es, &ems);
			fgets(tmp, 128, srt);
			fgets(label, 128, srt);
			startFrame = (int)((sms + 1000*ss + 60*1000*sm + 60*60*1000*sh)/frameDuration);
			endFrame = (int)((ems + 1000*es + 60*1000*em + 60*60*1000*eh)/frameDuration);
			fgets(tmp, 128, srt);
			label[strlen(label)-1]='\0';
		}


		if(i>=startFrame && i<endFrame){
			result.push_back(string(label));
		}else{
			result.push_back("");
		}
	}
	fclose (srt);
	return result;
}