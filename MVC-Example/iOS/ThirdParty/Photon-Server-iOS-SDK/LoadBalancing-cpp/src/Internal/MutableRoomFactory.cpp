/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#include "LoadBalancing-cpp/inc/Internal/MutableRoomFactory.h"
#include "LoadBalancing-cpp/inc/MutableRoom.h"

namespace ExitGames
{
	namespace LoadBalancing
	{
		using namespace Common::MemoryManagement;

		MutableRoomFactory::~MutableRoomFactory(void)
		{
		}

		MutableRoom* MutableRoomFactory::create(const Common::JString& name, const Common::Hashtable& properties, Peer* pPeer, const Common::JVector<Common::JString>& propsListedInLobby)
		{
			MutableRoom* p;
			return ALLOCATE(MutableRoom, p, name, properties, pPeer, propsListedInLobby);
		}

		void MutableRoomFactory::destroy(const MutableRoom* pRoom)
		{
			deallocate(pRoom);
		}
	}
}