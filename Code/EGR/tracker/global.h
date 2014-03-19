/* 
 * File:   global.h
 * Author: tmf
 *
 * Created on 23. Oktober 2009, 15:17
 */
#pragma once
#ifndef _GLOBAL_H
#define	_GLOBAL_H
#include <vector>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include "filter.h"
#include <time.h>
#include "cleyecameracapture.h"
#include "Step.h"

extern CRITICAL_SECTION cs_v;
extern CRITICAL_SECTION cs_img;

extern bool visible;
extern bool useAVIasSource;

extern CvFont titleFont;
extern CvFont menuFont;
extern CvFont menuFontBold;

extern std::vector<std::string> strFilterList;
extern std::vector<Filter*> filterList;
extern std::vector<std::string> strSourceList;
extern CvScalar green;
extern CvScalar black;

enum SO_PERSISTANCE { SOP_LOOP, SOP_INFINITE};
struct StorageObject {
    void* data;
	std::string name;
	SO_PERSISTANCE persistance;
};
struct Point
{
  int x;
  int y;
};

struct FCBlob
{
  int x[100];
  int y[100];
  int center_x;
  int center_y;
  int n;
};
extern StorageObject* getStorageObject(std::string name);
//extern void save(const std::vector<Step *> vec_steps, std::string file);
//extern std::vector<Step *>  restore(std::string file);

extern void logString(std::string s);

extern int currentScreen;
extern int MAX_STEPS;
extern std::vector<Step*> screenSteps;


extern CLEyeCameraCapture *ps3_eye_left;
extern CLEyeCameraCapture *ps3_eye_right;

extern double totalExecutionTime;

extern void saveScreenshot(IplImage* img);
extern void writeAVI(IplImage* img);
extern void prepareAVI(int fps);
extern bool recordingAVI;
extern bool doRecordAVI;
extern bool stopRecordingAVI;
extern std::string logFile;
extern std::string aviFile;
//extern std::string logFile;
//extern std::string logLine;
extern int frame_counter;

#ifdef _MSC_VER       // Identifies MS compilers (crude test for Windows!)
# include <Windows.h>
#endif
#include <ctime>

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif
extern int _gettimeofday(struct timeval *tv, struct timezone *tz);
extern float milliseconds();


#define WIDTH 640//320	
#define HEIGHT 480//240



#define KEY_LEFT 2424832 //29
#define KEY_RIGHT 2555904 //28
#define KEY_UP 2490368 //30
#define KEY_DOWN 2621440 //31
#define KEY_SHIFT_LEFT 2359296
#define KEY_SHIFT_RIGHT 2293760
#define KEY_SHIFT_UP 2162688
#define KEY_SHIFT_DOWN 2228224
/*
2424832
2555904
2490368
2621440
2359296
2293760
2162688
2228224*/


#endif	/* _GLOBAL_H */

