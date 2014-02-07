/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "Common-cpp/inc/Common.h"

namespace ExitGames
{
	namespace LoadBalancing
	{
		class Client;
		class MutableRoom;

		namespace Internal
		{
			class PlayerPropertiesUpdateInformant
			{
				static void onUpdate(MutableRoom& room, int number, const Common::Hashtable& properties);

				friend class LoadBalancing::Client;
			};
		}
	}
}