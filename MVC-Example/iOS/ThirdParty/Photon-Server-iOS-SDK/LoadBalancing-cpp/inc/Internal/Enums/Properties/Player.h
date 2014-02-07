/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "Photon-cpp/inc/PhotonPeer.h"

namespace ExitGames
{
	namespace LoadBalancing
	{
		namespace Internal
		{
			// Photon properties, internally set by LoadBalancing (LoadBalancing builtin properties)
			namespace Properties
			{
				namespace Player
				{
					static const nByte PLAYERNAME =  255;
				};
			}
		}
	}
}