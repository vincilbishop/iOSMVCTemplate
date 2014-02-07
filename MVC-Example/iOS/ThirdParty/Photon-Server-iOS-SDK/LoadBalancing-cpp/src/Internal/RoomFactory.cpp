/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#include "LoadBalancing-cpp/inc/Internal/RoomFactory.h"
#include "LoadBalancing-cpp/inc/Room.h"

namespace ExitGames
{
	namespace LoadBalancing
	{
		namespace Internal
		{
			using namespace Common::MemoryManagement;

			Room* RoomFactory::create(const Common::JString& name, const Common::Hashtable& properties)
			{
				Room* p;
				return ALLOCATE(Room, p, name, properties);
			}

			void RoomFactory::destroy(const Room* pRoom)
			{
				deallocate(pRoom);
			}
		}
	}
}