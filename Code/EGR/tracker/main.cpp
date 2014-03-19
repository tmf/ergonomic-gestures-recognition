// tracker.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <string>
#include <vector>
#include <list>
#include <process.h> 
#include <stdlib.h>
#include <iostream>
#include "global.h"
//#include "BoostDefines.hpp"
//#include <boost/serialization/vector.hpp>
#include <fstream>
#include <libconfig.h++>

#include <windows.h>
// include headers that implement a archive in simple text format 
//#include <boost/archive/text_oarchive.hpp> 
//#include <boost/archive/text_iarchive.hpp>

//#include <boost/filesystem.hpp>


/*
boost_serialization-vc90-mt-gd-1_44.lib
boost_filesystem-vc90-mt-gd-1_44.lib
*/
#include "tracker.h"
#include "camera.h"

#include <cv.h>
#include <highgui.h>
#include <stdio.h>

using namespace std;
using namespace libconfig;

void init(std::string configfile);
void initFilters();
vector<Step *> loadTrackerConfiguration(string file);
void saveTrackerConfiguration(string file, vector<Step *> steps);
void loop(void *nothing);
void show_windows();

bool visible;
bool executeLoop = false;
bool livePause = false;
bool EGRTrackerIsRunning = true;

long frameNumber=0;

bool newAVISource = false;
int state=0;//0=stop, 1=initialized, 2=process loop, 3=show window&waitkey

CRITICAL_SECTION cs_so;
CRITICAL_SECTION cs_v;
CRITICAL_SECTION cs_img;

void (*finishedCallback)(void)=NULL;


__declspec(dllexport) void* EGRTrackerGetSomething(std::string s)
{
	StorageObject *so = getStorageObject(s); 
	return so->data;
}

list<Point> *so_list_buffer=0;
__declspec(dllexport) list<Point>* EGRTrackerGetList(std::string s){
	
	if(so_list_buffer!=0){
		so_list_buffer->clear();
	}else{
		so_list_buffer = new list<Point>();
	}
	EnterCriticalSection(&cs_v);
	list<Point> *so = (list<Point>*)(getStorageObject(s)->data);
	if(so!=0){
	for(list<Point>::iterator i=so->begin(); i!=so->end(); i++){
		Point p;
		p.x = (*i).x;
		p.y = (*i).y;
		so_list_buffer->push_back(p);
	}
	}
	LeaveCriticalSection(&cs_v);
	return so_list_buffer;
}

IplImage *so_image_buffer=0;
__declspec(dllexport) IplImage* EGRTrackerGetImage(std::string s){
	IplImage *so = (IplImage *)(getStorageObject(s)->data);
	if(so_image_buffer!=0){
		cvReleaseImage(&so_image_buffer);
	}
	if(so!=0){
		so_image_buffer = cvCreateImage(cvSize(so->width, so->height), so->depth, so->nChannels);
		EnterCriticalSection(&cs_img);
		cvCopy(so, so_image_buffer);
		LeaveCriticalSection(&cs_img);
	}else{
		so_image_buffer=0;
		//so_image_buffer = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 4);
	}
	return so_image_buffer;
	
}

//std::string boostConfigurationFile;
__declspec(dllexport) void EGRTrackerLoadConfiguration(std::string s)
{
	InitializeCriticalSection(&cs_so);
	InitializeCriticalSection(&cs_v);
	InitializeCriticalSection(&cs_img);
	//boostConfigurationFile = s;
	state = 1;
	visible = false;
	init(s);

}
__declspec(dllexport) long EGRTrackerGetFrameNumber()
{
	return frameNumber;
}
__declspec(dllexport) float EGRTrackerGetFPS(){
	return (float)1000.0/totalExecutionTime;
}
__declspec(dllexport) void EGRTrackerSaveConfiguration(std::string s)
{
	saveTrackerConfiguration(s, screenSteps);
}
char *tmpchar;
__declspec(dllexport) void EGRTrackerStart()
{
	EGRTrackerIsRunning = true;
	if(state>0){
		state=2;
		_beginthread( loop, 100000, NULL );
	}
}
__declspec(dllexport) void EGRTrackerStop()
{
	
	//state = 0;
	//save(screenSteps, "c:/test.config");
	EGRTrackerIsRunning = false;
}
__declspec(dllexport) void EGRTrackerPause(){
	executeLoop = !executeLoop;
}
__declspec(dllexport) void EGRTrackerLivePause(){
	livePause = true;
}
__declspec(dllexport) void EGRTrackerLiveUnPause(){
	livePause = false;
}
__declspec(dllexport) void EGRTrackerShow(){
	state = 3;
	visible = true;
}
__declspec(dllexport) void EGRTrackerHide(){
	state = 2; 
	visible = false;
}
__declspec(dllexport) void EGRTrackerStartRecording(string file){
	doRecordAVI = true;
	aviFile = file;
	int fps = (int)1000.0f/(totalExecutionTime+7);
			if(fps!=0)
				prepareAVI(fps);
}
__declspec(dllexport) void EGRTrackerStopRecording(){
	recordingAVI = false;
	doRecordAVI = false;
	prepareAVI(1);
}
//dst replace i.e "camera_A"
__declspec(dllexport) void EGRTrackerSetSource(std::string file, std::string dst, bool clear){
	useAVIasSource=true;
	StorageObject *so_avis_cap = getStorageObject(string("AVISourcesCap"));
	StorageObject *so_avis_dst = getStorageObject(string("AVISourcesDst"));
	if(so_avis_cap->data==0 || clear)
		so_avis_cap->data = new std::vector<CvCapture *>();
	if(so_avis_dst->data==0 || clear)
		so_avis_dst->data = new std::vector<string>();
	
	std::vector<CvCapture *> *AVISources = (std::vector<CvCapture *> *)so_avis_cap->data;
	std::vector<std::string> *AVISourcesDst = (std::vector<std::string> *)so_avis_dst->data;
	AVISources->push_back(cvCreateFileCapture(file.c_str()));
	AVISourcesDst->push_back(dst);
	newAVISource = true;
}
__declspec(dllexport) void EGRTrackerSetFinishedCallback(void (*callback)(void)){
	finishedCallback = callback;
}


void logString(std::string s)
{
	FILE * pFile=0;
	//pFile = fopen (logFile.c_str(),"a");
	if (pFile!=NULL)
	{
		fputs (s.c_str(),pFile);

		fclose (pFile);
	}
}

void show_windows(){
	cvStartWindowThread();
	cvNamedWindow("Calibration", 0);
	//cvNamedWindow("Screen");
	cvResizeWindow("Calibration", WIDTH, HEIGHT);
	//cvResizeWindow("Screen", 640, 480);
	cvMoveWindow("Calibration", 640, 0);
	visible = true;
}
void initFilters(){
	strFilterList.push_back("Empty Filter");
	strFilterList.push_back("Lab Threshold Filter");
	strFilterList.push_back("Erosion Filter");
	strFilterList.push_back("Dilation Filter");
	strFilterList.push_back("Blob Filter");
	strFilterList.push_back("Add Filter");
	strFilterList.push_back("Threshold Filter");
	strFilterList.push_back("RGB Threshold Filter");
	strFilterList.push_back("Split Filter");
	strFilterList.push_back("Fast Corner Filter");
	strFilterList.push_back("Smooth Filter");
	

	filterList.push_back(new EmptyFilter());
	filterList.push_back(new LabThresholdFilter());
	filterList.push_back(new ErosionFilter());
	filterList.push_back(new DilationFilter());
	filterList.push_back(new BlobFilter());
	filterList.push_back(new AddFilter());
	filterList.push_back(new ThresholdFilter());
	filterList.push_back(new RGBThresholdFilter());
	filterList.push_back(new SplitFilter());
	filterList.push_back(new FastCornerFilter());
	filterList.push_back(new SmoothFilter());
	
	
}
void init(std::string configfile){
	

	//cvMoveWindow("Screen", 640, 0);

	//capture = cvCreateCameraCapture(0);

	//frame = cvQueryFrame(capture);
	//frame = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 4);
	//lab = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 3);
	//mask = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 3);
	//while(!ready(ps3_eye_left)) cvWaitKey(50);
	//cvCopy(ps3_eye_left->pCapImage, frame);
	//img = cvCreateImage(cvSize(frame->width, frame->height ),frame->depth, frame->nChannels);

	cvInitFont(&titleFont, CV_FONT_HERSHEY_SIMPLEX, 0.7, 0.7, 0.0, 2,8);
	cvInitFont(&menuFont, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4, 0.0, 1,8);
	cvInitFont(&menuFontBold, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4, 0.0, 2,8);
	camera_setup();

	//boost::filesystem::path boostconfigfile(configfile);

	//if( boost::filesystem::exists(boostconfigfile) ){

	initFilters();

	screenSteps = loadTrackerConfiguration(configfile);
	if(screenSteps.size() > 0){ //config file valid and existing
		/*for(int i=0; i<screenSteps.size();i++){
			stringstream strs;
			strs <<"Screen " <<(i+1);
			string name(strs.str());
			strSourceList.push_back(name);
		}*/
		MAX_STEPS = screenSteps.size();
	}else{
		for(int i=0; i<MAX_STEPS;i++){
			Step *s = new Step(i);
			screenSteps.push_back(s);
		}
	}




	

	green = CV_RGB(100,200,40);
	black = CV_RGB(0,0,0);
}
void saveTrackerConfiguration(string file, vector<Step *> vsteps){
	Config cfg;
	Setting &root = cfg.getRoot();
	Setting &steps = root.add("steps", Setting::TypeList);
	for(int i=0; i<vsteps.size(); i++){

		Setting &step = steps.add(Setting::TypeGroup);
		if(vsteps[i]->flt!=0){
			step.add("filter", Setting::TypeString) = vsteps[i]->flt->name;
			Setting &parameters = step.add("parameters", Setting::TypeList);
			for(int j=0; j<vsteps[i]->flt->params.size(); j++){
				Setting &p = parameters.add(Setting::TypeGroup);
				p.add("type", Setting::TypeInt) = vsteps[i]->flt->params[j]->getType();
				p.add("x", Setting::TypeInt) = vsteps[i]->flt->params[j]->getPos().x;
				p.add("y", Setting::TypeInt) = vsteps[i]->flt->params[j]->getPos().y;
				p.add("screenx", Setting::TypeInt) = vsteps[i]->flt->params[j]->getScreenPos().x;
				p.add("screeny", Setting::TypeInt) = vsteps[i]->flt->params[j]->getScreenPos().y;
				p.add("toggle", Setting::TypeBoolean) = vsteps[i]->flt->params[j]->getToggle();
				p.add("min", Setting::TypeFloat) = vsteps[i]->flt->params[j]->getMin();
				p.add("max", Setting::TypeFloat) = vsteps[i]->flt->params[j]->getMax();
				p.add("val", Setting::TypeFloat) = vsteps[i]->flt->params[j]->getVal();
				p.add("low", Setting::TypeFloat) = vsteps[i]->flt->params[j]->getLow();
				p.add("high", Setting::TypeFloat) = vsteps[i]->flt->params[j]->getHigh();
			}
			Setting &sources = step.add("sources", Setting::TypeArray);
			for(int j=0; j<vsteps[i]->sources.size(); j++){
				sources.add(Setting::TypeString) = vsteps[i]->sources[j];
			}
		}
	}
	cfg.writeFile(file.c_str());
}
vector<Step *> loadTrackerConfiguration(string file){
	Config cfg;
	vector<Step*> result;
	try{
		cfg.readFile(file.c_str());
	}catch(const ParseException &pex){
		return result;
	}catch(const FileIOException &fex){
		return result;
	}

	Setting &root = cfg.getRoot();
	const Setting &steps = root["steps"];
	for(int i=0; i<steps.getLength(); i++){
		const Setting &step = steps[i];
		//new step
		Step *s = new Step(i);

		//step has one filter
		int filterIndex = 0;
		string filterName;
		//step.lookupValue("filter", filterName);
		try
		{
			 step.lookupValue("filter", filterName);

		}
		catch(const SettingException &ex)
		{

		}
		for(filterIndex=0;filterIndex<strFilterList.size(); filterIndex++){
			if(strFilterList[filterIndex].compare(filterName)==0){
				break;
			}
		}
		if(filterIndex<strFilterList.size()){
			s->setFilter(filterList[filterIndex]->clone());
			//s->flt->params.clear();

			//read parameter
			Setting &parameters = step["parameters"];
			for(int j=0; j<parameters.getLength(); j++){
				bool pToggle;
				float pMin, pMax, pVal, pLow, pHigh;
				int pX, pY, pScreenX, pScreenY, pType;

				parameters[j].lookupValue("type", pType);
				parameters[j].lookupValue("x", pX);
				parameters[j].lookupValue("y", pY);
				parameters[j].lookupValue("screenx", pScreenX);
				parameters[j].lookupValue("screeny", pScreenY);
				parameters[j].lookupValue("toggle", pToggle);
				parameters[j].lookupValue("min", pMin);
				parameters[j].lookupValue("max", pMax);
				parameters[j].lookupValue("val", pVal);
				parameters[j].lookupValue("low", pLow);
				parameters[j].lookupValue("high", pHigh);

				//Parameter *p = new Parameter(pType, pMin, pMax, pVal, pLow, pHigh, pToggle, pX, pY, pScreenX, pScreenY);
				//s->flt->params.push_back(p);
				s->flt->params[j]->setValues(pType, pMin, pMax, pVal, pLow, pHigh, pToggle, pX, pY, pScreenX, pScreenY);
			}

			//a step can have one or more sources
			Setting &sources = step["sources"];
			for(int j=0; j<sources.getLength(); j++){
				s->sources.push_back(sources[j]);
			}
			s->onscreen->mode = mBar;
		}
		
		result.push_back(s);
	}
	return result;
}
StorageObject* getStorageObject(std::string name){
	StorageObject* result = 0;
	EnterCriticalSection(&cs_so);
	for(int i=0;i<(int)storage.size();i++){
		const char *left, *right;
		left = storage[i]->name.c_str();
		right = name.c_str();
		//try{
		//if(storage[i]->name.compare(name)==0){
			if(strcmp(left,right)==0){
			result = storage[i];
			break;
		}//}catch(...){}
	}
	
	if(result==0){
		result = new StorageObject();
		//result->name = new char[32];
		std::string str = string(name);
		result->name = str;
		//strcpy(result->name, name);
		result->data =0;
		result->persistance = SOP_INFINITE;
		
		storage.push_back(result);
		
	}
	LeaveCriticalSection(&cs_so);
	return result;
}

void loop(void *nothing) {
	/*int tmpDbgFlag;
	tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(0);*/



	//init();
	int key=1, i;
	currentScreen=1;
	StorageObject *so_cameras = getStorageObject(std::string("cameras"));
	if(so_cameras->data == 0)
		so_cameras->data = new std::vector<CLEyeCameraCapture *>();
	std::vector<CLEyeCameraCapture *> cameras = *(std::vector<CLEyeCameraCapture *> *)so_cameras->data;

	StorageObject *so_avis_cap = getStorageObject(string("AVISourcesCap"));
	StorageObject *so_avis_dst = getStorageObject(string("AVISourcesDst"));
	if(so_avis_cap->data==0)
		so_avis_cap->data = new std::vector<CvCapture *>();
	if(so_avis_dst->data==0)
		so_avis_dst->data = new std::vector<string>();
	std::vector<CvCapture *> *AVISources = (vector<CvCapture *> *)so_avis_cap->data;
	std::vector<string> *AVISourcesDst = (vector<string> *)so_avis_dst->data;

	LONGLONG g_Frequency, g_CurentCount, g_LastCount;
	QueryPerformanceFrequency((LARGE_INTEGER*)&g_Frequency);
	frameNumber = 0;
	//for(;key!='q';key=cvWaitKey(1))
	bool continueLoop = true;
	bool notifyStuff=false;
	executeLoop = true;
	livePause = false;
	while(EGRTrackerIsRunning)
	{
		if(executeLoop){
			
			if(state==3 && !visible){
				show_windows();
				key=cvWaitKey(1);
			}else if(state==3 && visible){
				key=cvWaitKey(1);
			}else if(state==2){
				cvDestroyAllWindows();
				visible = false;
			}
			frameNumber++;
			QueryPerformanceCounter((LARGE_INTEGER*)&g_CurentCount);
			/*if(key>=0)
			printf("pressed %d\n", key);*/
			if(key > '0' && key<=('0'+MAX_STEPS)){
				currentScreen=key-'0';
				//printf("Screen %d\n", currentScreen);
			}
			if(recordingAVI){
				std::stringstream ss;
				ss << frame_counter << " ";
				std::string s = ss.str();
				logString(s);
			}
			if(!useAVIasSource){
				for(i=0; i<(int)cameras.size();i++){
					if(ready((CLEyeCameraCapture*)cameras[i])){
						std::stringstream str_camera;
						str_camera<<"camera_"<<('A'+i);
						std::string name(str_camera.str());
						//char *name = new char[32];
						//sprintf(name, "camera_%c", 'A' + i);
						StorageObject *so_frame = getStorageObject(name);
						IplImage *frame = (IplImage *)so_frame->data;
						cvCopy(cameras[i]->pCapImage, frame);
						
						//delete name;
					}
				}
			}
			if(!livePause){
			if(useAVIasSource){
				if(newAVISource){
					newAVISource = false;
					so_avis_cap = getStorageObject(string("AVISourcesCap"));
					so_avis_dst = getStorageObject(string("AVISourcesDst"));
					AVISources = (vector<CvCapture *> *)so_avis_cap->data;
					AVISourcesDst = (vector<string> *)so_avis_dst->data;
					frameNumber = 0;
				}
				for(i=0; i<AVISources->size(); i++){
					
					IplImage *aviframe = cvQueryFrame(AVISources->at(i));
					if(!aviframe) {
						
						executeLoop = false;
						notifyStuff = true;
						
						//continueLoop = false;
					}else{
					
						StorageObject *so_frame = getStorageObject(AVISourcesDst->at(i));
						IplImage *frame = (IplImage *)so_frame->data;
						if(frame==0)
							frame = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 4);
						cvCvtColor(aviframe, frame, CV_BGR2BGRA);
						so_frame->data = frame;
					}
					
				}
			}
			}
			if(!executeLoop && notifyStuff){
				//break;
				continue;
			}
			int lastActiveScreenStep = 0;
			for(int i=0; i<MAX_STEPS;i++){
				if(screenSteps[i]->process(currentScreen==(i+1), key))
					lastActiveScreenStep = i;
			}
			//std::stringstream str_screen;
			//str_screen<<"Screen 1"<<lastActiveScreenStep+1;
			//std::string name(str_screen.str());
			//char *name = new char[32];
			//sprintf(name, "Screen %d", lastActiveScreenStep+1);
			StorageObject *so_img = getStorageObject(string("Screen 9"));	
			IplImage *img = (IplImage *)so_img->data;

			
			
			if(doRecordAVI){
				writeAVI(img);
			}
			//cvShowImage("Screen", tmp);
			//delete name;


			QueryPerformanceCounter((LARGE_INTEGER*)&g_LastCount);

			totalExecutionTime = 1000 * (((double)(g_LastCount-g_CurentCount))/((double)g_Frequency));
			//logFile << std::endl;
			if(recordingAVI){
				std::string tmpstr = "\n";
				logString(tmpstr);
			}
			//logString();
			/*
			if(ready(ps3_eye_left)){
			cvCopy(ps3_eye_left->pCapImage, frame);
			//frame=cvQueryFrame(capture);
			//cvResize(frame, img, CV_INTER_AREA);
			cvCopy(frame, img);
			tmp=img;
			for(int i=0; i<MAX_STEPS;i++){
			tmp = screenSteps[i]->process(tmp, currentScreen==(i+1), key);
			}

			cvShowImage("Screen", tmp);
			}*/
		}else{
			if(notifyStuff){
				notifyStuff=false;
			if(finishedCallback!=NULL){
		finishedCallback();
	}
			}
			}

	}
	//save(screenSteps);
	//logFile.close();
	
	//camera_stop();
	//cvDestroyAllWindows();
	
	
	//_endthread();
	//cvReleaseCapture(&capture);

	//return 0;
}
