/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#include "LoadBalancing-cpp/inc/Internal/Utils.h"

namespace ExitGames
{
	using namespace Common;

	namespace LoadBalancing
	{
		namespace Internal
		{
			namespace Utils
			{
				Hashtable stripToCustomProperties(const Hashtable& properties)
				{
					Hashtable retVal;
					const JVector<Object>& keys = properties.getKeys();
					for(unsigned int i=0 ; i<keys.getSize() ; i++)
						if(keys[i].getType() == Common::TypeCode::STRING)
							retVal.put(keys[i], properties[i]);
					return retVal;
				}

				Hashtable stripKeysWithNullValues(const Hashtable& orig)
				{
					Hashtable stripped;
					const JVector<Object>& keys = orig.getKeys();
					for(unsigned int i=0; i<orig.getSize(); i++)
						if(orig[i] != Object())
							stripped.put(keys[i], orig[i]);
					return stripped;
				}
			}
		}
	}
}