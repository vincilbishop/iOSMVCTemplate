#ifndef __OUTPUT_LISTENER_H
#define __OUTPUT_LISTENER_H

#ifndef _EG_ANDROID_PLATFORM
#	include <iostream>
#endif
#include "LoadBalancing-cpp/inc/Client.h"

class OutputListener
{
public:
	virtual ~OutputListener(void){};
	virtual void write(const ExitGames::Common::JString& str) = 0;
	virtual void writeLine(const ExitGames::Common::JString& str) = 0;
	virtual void debugReturn(const ExitGames::Common::JString& str)
	{
		#ifndef _EG_ANDROID_PLATFORM
			std::wcerr << str << std::endl;
		#endif
	}
};

#endif