/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "Common-cpp/inc/Common.h"
#include "LoadBalancing-cpp/inc/MutableRoom.h"

namespace ExitGames
{
	namespace LoadBalancing
	{
		class Client;
		class Player;

		namespace Internal
		{
			class PlayerMovementInformant
			{
				template<bool isLocalPlayer> static void onEnter(MutableRoom& room, Player& player);
				static void onEnter(MutableRoom& room, int number, const Common::Hashtable& properties);
				template<bool isLocalPlayer> static bool onLeave(MutableRoom& room, int number);

				friend class LoadBalancing::Client;
			};
		}
	}
}