#include "stdafx.h"
#include "cleyecameracapture.h"
#include "cv.h"


CLEyeCameraCapture::CLEyeCameraCapture(GUID cameraGUID, CLEyeCameraColorMode mode, CLEyeCameraResolution resolution, int fps) :
_cameraGUID(cameraGUID), _cam(NULL), _mode(mode), _resolution(resolution), _fps(fps), _running(false), pCapImage(NULL)
{


}
bool CLEyeCameraCapture::StartCapture()
{
	_running = true;

	// Start CLEye image capture thread
	_hThread = CreateThread(NULL, 0, &CLEyeCameraCapture::CaptureThread, this, 0, 0);
	if(_hThread == NULL)
	{
		//MessageBox(NULL,"Could not create capture thread","CLEyeMulticamTest", MB_ICONEXCLAMATION);
		return false;
	}
	return true;
}
void CLEyeCameraCapture::StopCapture()
{
	if(!_running)	return;
	_running = false;
	WaitForSingleObject(_hThread, 1000);

}
void CLEyeCameraCapture::IncrementCameraParameter(int param)
{
	if(!_cam)	return;
	CLEyeSetCameraParameter(_cam, (CLEyeCameraParameter)param, CLEyeGetCameraParameter(_cam, (CLEyeCameraParameter)param)+10);
}
void CLEyeCameraCapture::DecrementCameraParameter(int param)
{
	if(!_cam)	return;
	CLEyeSetCameraParameter(_cam, (CLEyeCameraParameter)param, CLEyeGetCameraParameter(_cam, (CLEyeCameraParameter)param)-10);
}
void CLEyeCameraCapture::Capture()
{
	int w, h;
	IplImage *pBufferImage;
	PBYTE pCapBuffer = NULL;

	// Create camera instance
	_cam = CLEyeCreateCamera(_cameraGUID, _mode, _resolution, _fps);
	if(_cam == NULL)		return;
	// Get camera frame dimensions
	CLEyeCameraGetFrameDimensions(_cam, w, h);
	// Depending on color mode chosen, create the appropriate OpenCV image
	if(_mode == CLEYE_COLOR_PROCESSED || _mode == CLEYE_COLOR_RAW){
		//pCapImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 4);
		pBufferImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 4);
	}else
		pCapImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);

	// Set some camera parameters
	//CLEyeSetCameraParameter(_cam, CLEYE_GAIN, 20);
	CLEyeSetCameraParameter(_cam, CLEYE_AUTO_GAIN, TRUE);
	CLEyeSetCameraParameter(_cam, CLEYE_AUTO_EXPOSURE, TRUE);

	// Start capturing
	CLEyeCameraStart(_cam);
	// image capturing loop
	while(_running)
	{

		//cvGetImageRawData(pBufferImage, &pCapBuffer);
		if(pCapImage!=NULL)
			cvGetImageRawData(pCapImage, &pCapBuffer);
		else
			pCapImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 4);
		CLEyeCameraGetFrame(_cam, pCapBuffer);
		//cvCvtColor(pBufferImage, pCapImage, CV_BGRA2BGR);

	}
	// Stop camera capture
	CLEyeCameraStop(_cam);
	// Destroy camera object
	CLEyeDestroyCamera(_cam);
	// Destroy the allocated OpenCV image
	cvReleaseImage(&pCapImage);
	_cam = NULL;
}
DWORD WINAPI CLEyeCameraCapture::CaptureThread(LPVOID instance)
{
	// seed the rng with current tick count and thread id
	srand(GetTickCount() + GetCurrentThreadId());
	// forward thread to Capture function
	CLEyeCameraCapture *pThis = (CLEyeCameraCapture *)instance;
	pThis->Capture();
	return 0;
}