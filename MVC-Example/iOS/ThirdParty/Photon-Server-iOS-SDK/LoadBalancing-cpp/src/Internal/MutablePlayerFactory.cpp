/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#include "LoadBalancing-cpp/inc/Internal/MutablePlayerFactory.h"
#include "LoadBalancing-cpp/inc/MutablePlayer.h"

namespace ExitGames
{
	namespace LoadBalancing
	{
		using namespace Common::MemoryManagement;

		MutablePlayerFactory::~MutablePlayerFactory(void)
		{
		}

		MutablePlayer* MutablePlayerFactory::create(int number, const Common::Hashtable& properties, const MutableRoom* const pRoom, Peer* pPeer)
		{
			MutablePlayer* p;
			return ALLOCATE(MutablePlayer, p, number, properties, pRoom, pPeer);
		}

		void MutablePlayerFactory::destroy(const MutablePlayer* pPlayer)
		{
			deallocate(pPlayer);
		}
	}
}