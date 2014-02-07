/* Exit Games Photon - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

namespace ExitGames
{
	namespace Photon
	{
		namespace Internal
		{
			class ProtocolHeader
			{
			public:
				short peerID;
				nByte flags;
				nByte commandCount;
				int   sentTime;
				int   challenge;
			};
		}
	}
}