#pragma once
#include <CLEyeMulticam.h>
#include <cv.h>

class CLEyeCameraCapture
{
private:
	GUID _cameraGUID;
	CLEyeCameraInstance _cam;
	CLEyeCameraColorMode _mode;
	CLEyeCameraResolution _resolution;
	int _fps;
	HANDLE _hThread;
	
	
public:
	bool _running;
	IplImage* pCapImage;
	CLEyeCameraCapture(GUID cameraGUID, CLEyeCameraColorMode mode, CLEyeCameraResolution resolution, int fps);
	bool StartCapture();
	void StopCapture();
	void IncrementCameraParameter(int param);
	void DecrementCameraParameter(int param);
	void Capture();
	static DWORD WINAPI CaptureThread(LPVOID instance);
};
