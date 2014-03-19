#pragma once
#include "cleyecameracapture.h"
#include <cv.h>

 CLEyeCameraCapture *ps3_eye_left;
 CLEyeCameraCapture *ps3_eye_right;
void camera_setup();
void camera_stop();
bool ready(CLEyeCameraCapture *cam);
void saveScreenshot(IplImage* img);
void writeAVI(IplImage* img);
void prepareAVI(int fps);