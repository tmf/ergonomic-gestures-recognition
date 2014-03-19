#pragma once



__declspec(dllimport) void EGRTrackerStart();
__declspec(dllimport) void EGRTrackerStop();
__declspec(dllimport) void EGRTrackerPause();
__declspec(dllimport) void EGRTrackerLivePause();
__declspec(dllimport) void EGRTrackerLiveUnPause();

__declspec(dllimport) void EGRTrackerShow();
__declspec(dllimport) void EGRTrackerHide();
__declspec(dllimport) void* EGRTrackerGetSomething(std::string s);
__declspec(dllimport) void EGRTrackerLoadConfiguration(std::string s);
__declspec(dllimport) void EGRTrackerSaveConfiguration(std::string s);
__declspec(dllimport) void EGRTrackerSetSource(std::string file, std::string dst, bool replace);
__declspec(dllimport) long EGRTrackerGetFrameNumber();
__declspec(dllimport) void EGRTrackerStartRecording(std::string file);
__declspec(dllimport) void EGRTrackerStopRecording();
__declspec(dllimport) IplImage* EGRTrackerGetImage(std::string s);
__declspec(dllimport) std::list<Point>* EGRTrackerGetList(std::string s);
__declspec(dllimport) float EGRTrackerGetFPS();
__declspec(dllexport) void EGRTrackerSetFinishedCallback(void (*callback)(void));