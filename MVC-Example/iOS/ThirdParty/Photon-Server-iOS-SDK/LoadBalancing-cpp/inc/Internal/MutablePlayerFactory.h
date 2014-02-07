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
		class MutablePlayer;
		class MutableRoom;
		class Peer;

		class MutablePlayerFactory
		{
		protected:
			virtual ~MutablePlayerFactory(void);

			virtual MutablePlayer* create(int number, const Common::Hashtable& properties, const MutableRoom* const pRoom, Peer* pPeer);
			virtual void destroy(const MutablePlayer* pPlayer);
		private:
			friend class LoadBalancing::Client;
		};
	}
}