/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#include "LoadBalancing-cpp/inc/Internal/PlayerPropertiesUpdateInformant.h"
#include "LoadBalancing-cpp/inc/MutableRoom.h"

namespace ExitGames
{
	namespace LoadBalancing
	{
		namespace Internal
		{
			void PlayerPropertiesUpdateInformant::onUpdate(MutableRoom& room, int number, const Common::Hashtable& properties)
			{
				for(unsigned int i=0; i<room.getPlayers().getSize(); ++i)
					if(room.getPlayers()[i]->getNumber() == number)
						PlayerPropertiesCacher::cache(*room.getNonConstPlayers()[i], properties);
			}
		}
	}
}