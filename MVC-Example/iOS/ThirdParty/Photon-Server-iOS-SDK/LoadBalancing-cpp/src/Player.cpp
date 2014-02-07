/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#include "LoadBalancing-cpp/inc/Player.h"
#include "LoadBalancing-cpp/inc/MutableRoom.h"
#include "LoadBalancing-cpp/inc/Internal/Utils.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/Properties/Player.h"

/** @file inc/Player.h */

namespace ExitGames
{
	namespace LoadBalancing
	{
		using namespace Common;
		using namespace Internal;

		Player::Player(void)
			: mNumber(-1)
		{
		}

		Player::Player(int number, const Hashtable& properties, const MutableRoom* const pRoom)
			: mNumber(number), mpRoom(pRoom)
		{
			cacheProperties(properties);
		}

		Player::~Player(void)
		{
		}

		Player::Player(const Player& toCopy)
		{
			*this = toCopy;
		}

		Player& Player::operator=(const Player& toCopy)
		{
			return assign(toCopy);
		}

		bool Player::getIsMutable(void) const
		{
			return false;
		}

		Player& Player::assign(const Player& toCopy)
		{
			mNumber = toCopy.mNumber;
			mName = toCopy.mName;
			mCustomProperties = toCopy.mCustomProperties;
			mpRoom = toCopy.mpRoom;
			return *this;
		}

		int Player::getNumber(void) const
		{
			return mNumber;
		}

		const JString& Player::getName(void) const
		{
			return mName;
		}

		const Hashtable& Player::getCustomProperties(void) const
		{
			return mCustomProperties;
		}

		void Player::cacheProperties(const Hashtable& properties)
		{
			if(properties.contains(static_cast<nByte>(Properties::Player::PLAYERNAME)))
				mName = ValueObject<JString>(properties.getValue(static_cast<nByte>(Properties::Player::PLAYERNAME))).getDataCopy();

			mCustomProperties.put(Utils::stripToCustomProperties(properties));
			mCustomProperties = Utils::stripKeysWithNullValues(mCustomProperties);
		}

		bool Player::operator==(const Player& player) const
		{
			return getNumber() == player.getNumber();
		}

		JString& Player::toString(JString& retStr, bool withTypes) const
		{
			return retStr += toString(withTypes, false);
		}

		JString Player::toString(bool withTypes, bool withCustomProperties) const
		{
			return JString() + mNumber + L"={" + payloadToString(withCustomProperties, withTypes) + L"}";
		}

		JString Player::payloadToString(bool withTypes, bool withCustomProperties) const
		{
			JString res = JString(L"num: ") + mNumber + L" name: " + mName;
			if(withCustomProperties && mCustomProperties.getSize())
				res += L" props: " + mCustomProperties.toString(withTypes);
			return res;
		}
	}
}