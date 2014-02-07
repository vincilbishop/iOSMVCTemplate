/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#include "LoadBalancing-cpp/inc/Internal/PlayerMovementInformant.h"

namespace ExitGames
{
	namespace LoadBalancing
	{
		namespace Internal
		{
			template<>
			void PlayerMovementInformant::onEnter<false>(MutableRoom& room, Player& player)
			{
				room.addPlayer(player);
			}

			template<>
			void PlayerMovementInformant::onEnter<true>(MutableRoom& room, Player& player)
			{
				room.addLocalPlayer(player);
			}

			void PlayerMovementInformant::onEnter(MutableRoom& room, int number, const Common::Hashtable& properties)
			{
				room.addPlayer(number, properties);
			}

			template<>
			bool PlayerMovementInformant::onLeave<false>(MutableRoom& room, int number)
			{
				return room.removePlayer(number);
			}

			template<>
			bool PlayerMovementInformant::onLeave<true>(MutableRoom& room, int number)
			{
				room.removeAllPlayers();
				return true;
			}
		}
	}
}