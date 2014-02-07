/* Exit Games Photon - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "Photon-cpp/inc/Internal/TPeer.h"

namespace ExitGames
{
	namespace Photon
	{
		namespace Internal
		{
			class TConnect : public PhotonConnect
			{
			public:
				TConnect(PhotonConnectListener& listener);
				~TConnect(void);
			private:
				typedef PhotonConnect super;

				SOCKET socket(void);
				unsigned short getDefaultPort(void);
				bool checkConnection(void);
				int recvBuffer(nByte** ppBuff, int* pBytesRead);

			private:
				unsigned int mPayloadSize;
				nByte* mReadData;
				unsigned int mReadSize;
			};
		}
	}
}