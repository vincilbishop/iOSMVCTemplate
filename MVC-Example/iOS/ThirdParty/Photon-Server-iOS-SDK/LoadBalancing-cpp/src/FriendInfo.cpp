/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#include "LoadBalancing-cpp/inc/FriendInfo.h"

/** @file inc/FriendInfo.h */

namespace ExitGames
{
	namespace LoadBalancing
	{
		using namespace Common;

		FriendInfo::FriendInfo(const JString& name, bool isOnline, const JString& room)
			: mName(name)
			, mIsOnline(isOnline)
			, mRoom(room)
		{
		}

		JString FriendInfo::getName(void) const
		{
			return mName;
		}

		bool FriendInfo::getIsOnline(void) const
		{
			return mIsOnline;
		}

		JString FriendInfo::getRoom(void) const
		{
			return mRoom;
		}

		bool FriendInfo::getIsInRoom(void) const
		{
			return !!getRoom().length();
		}

		JString& FriendInfo::toString(JString& retStr, bool withTypes) const
		{
			return retStr += L"{" + getName() + L" " + (getIsOnline()?(L"on, " + (getIsInRoom()?L"in room " + getRoom():L"not in a room")):L"off") + L"}";
		}
	}
}