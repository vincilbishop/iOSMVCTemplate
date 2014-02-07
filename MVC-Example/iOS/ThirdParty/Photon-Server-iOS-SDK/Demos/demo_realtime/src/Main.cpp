// Application main file.

#include "AppDelegate.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include <windows.h>
#include <tchar.h>
#define  RUN_FUNCTION run

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

#else

#if(CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
#define  RUN_FUNCTION run
#else
#define  RUN_FUNCTION Run
#endif

int main()
{

#endif

	AppDelegate* app;
	int nRet = 0;
	
	app = new AppDelegate;
	nRet = cocos2d::CCApplication::sharedApplication().RUN_FUNCTION();
	delete app;
	return nRet;
}
