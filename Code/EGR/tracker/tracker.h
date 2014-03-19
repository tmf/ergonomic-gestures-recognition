/* 
 * File:   tracker.h
 * Author: tmf
 *
 * Created on 23. Oktober 2009, 15:01
 */

#ifndef _CALIBRATION_H
#define	_CALIBRATION_H

#include <vector>
#include <string>
#include <time.h>
#include <ctime>

#include "step.h"
#include "ThresholdFilter.h"
#include "EmptyFilter.h"
#include "RGBThresholdFilter.h"
#include "LabThresholdFilter.h"
#include "SplitFilter.h"
#include "AddFilter.h"
#include "BlobFilter.h"
#include "FastCornerFilter.h"
#include "SmoothFilter.h"
#include "ErosionFilter.h"
#include "DilationFilter.h"
#include "global.h"

int MAX_STEPS=9;
int currentScreen;
std::vector<Step*> screenSteps;
CvCapture *capture;
/*IplImage *frame, *img;
//IplImage *frame;// = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 4);
IplImage *lab;// = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 3);
IplImage *mask;// = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 3);
*/
CvFont titleFont;
CvFont menuFont;
CvFont menuFontBold;

std::vector<std::string> strFilterList;
std::vector<Filter*> filterList;
std::vector<std::string> strSourceList;

CvScalar green;
CvScalar black;


static std::vector<StorageObject*> storage;
StorageObject* getStorageObject(std::string name);

double totalExecutionTime;

int _gettimeofday(struct timeval *tv, struct timezone *tz)
{
  FILETIME ft;
  unsigned __int64 tmpres = 0;
  static int tzflag;
 
  if (NULL != tv)
  {
    GetSystemTimeAsFileTime(&ft);
 
    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;
 
    /*converting file time to unix epoch*/
    tmpres -= DELTA_EPOCH_IN_MICROSECS; 
    tmpres /= 10;  /*convert into microseconds*/
    tv->tv_sec = (long)(tmpres / 1000000UL);
    tv->tv_usec = (long)(tmpres % 1000000UL);
  }
 
  
 
  return 0;
}
float milliseconds(){
	struct timeval tmp;
	_gettimeofday(&tmp, NULL);
	float ms = (int)(tmp.tv_sec + tmp.tv_usec / 1000);
	return ms;
}
#endif	/* _CALIBRATION_H */

