/* Exit Games Photon - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "Photon-cpp/inc/Internal/PhotonConnect.h"

namespace ExitGames
{
	namespace Photon
	{
		namespace Internal
		{
			class EnetConnect : public PhotonConnect
			{
			public:
				EnetConnect(PhotonConnectListener& listener);
				~EnetConnect(void);
			private:
				typedef PhotonConnect super;

				SOCKET socket(void);
				unsigned short getDefaultPort(void);
				bool checkConnection(void);
				int recvBuffer(nByte** ppBuff, int* pBytesRead);
			};
		}
	}
}