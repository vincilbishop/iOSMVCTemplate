/* Exit Games Photon - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "Common-cpp/inc/defines.h"

namespace ExitGames
{
	namespace Photon
	{
		namespace Internal
		{
			class PhotonConnectListener
			{
			public:
				virtual void onConnect(int nError)= 0;
				virtual void onReceiveData(nByte* pBuf, int iCount, int nError)= 0;
				virtual void onSendCommands(int nError)= 0;
			};
		}
	}
}