#ifdef SRT2LABELLIBRARY_EXPORT // inside DLL
#define DLL	__declspec(dllexport)
#define API	WINAPI
#else // outside DLL
#define DLL __declspec(dllimport)
#define API __stdcall
#endif  // SRT2LABELLIBRARY_EXPORT

#include <string>
#include <vector>

 DLL std::vector<std::string> API srt2label(std::string avi);