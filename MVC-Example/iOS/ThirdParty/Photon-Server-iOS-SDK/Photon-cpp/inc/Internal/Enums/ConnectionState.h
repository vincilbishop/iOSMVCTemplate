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
			namespace ConnectionState
			{
				enum ConnectionState
				{
					DISCONNECTED             = 0,
					CONNECTING               = 1,
					ACKNOWLEDGING_CONNECT    = 2,
					CONNECTED                = 3,
					DISCONNECTING            = 4,
					ACKNOWLEDGING_DISCONNECT = 5,
					ZOMBIE                   = 6
				};

				/** @file */
			}
		}
	}
}