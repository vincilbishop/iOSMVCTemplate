#include "HelloWorldApplication.h"
#include "Photon_lib.h"
#include "UIListener.h"

#if defined _EG_WINDOWS_PLATFORM
#	define SLEEP(ms) Sleep(ms)
#else
#	if defined _EG_LINUX_PLATFORM || defined _EG_MARMALADE_PLATFORM || defined _EG_ANDROID_PLATFORM || defined _EG_BLACKBERRY_PLATFORM
#		include "unistd.h"
#	endif
#	define SLEEP(ms) usleep(ms*1000)
#endif

void HelloWorldApplication::run(UIListener* listener)
{
	PhotonLib lib;
	
	while(!listener->anyKeyPressed())
	{
		lib.update();
		listener->writeString(lib.getStateString());
		SLEEP(100);
	}

	listener->onLibClosed();
	delete listener;
}