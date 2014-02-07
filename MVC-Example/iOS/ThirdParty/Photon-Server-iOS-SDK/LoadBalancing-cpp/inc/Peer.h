/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "Photon-cpp/inc/PhotonPeer.h"
#include "Photon-cpp/inc/Enums/EventCode.h"
#include "Photon-cpp/inc/Enums/EventCache.h"
#include "Photon-cpp/inc/Enums/EventKey.h"
#include "Photon-cpp/inc/Enums/OperationCode.h"
#include "Photon-cpp/inc/Enums/ParameterCode.h"
#include "Photon-cpp/inc/Enums/ReceiverGroup.h"
#include "LoadBalancing-cpp/inc/Enums/ErrorCode.h"
#include "LoadBalancing-cpp/inc/Enums/PeerStates.h"
#include "LoadBalancing-cpp/inc/AuthenticationValues.h"

namespace ExitGames
{
	namespace LoadBalancing
	{
		class Peer : public Photon::PhotonPeer
		{
		public:
			Peer(Photon::PhotonListener& listener, bool useTcp=false);
			virtual ~Peer(void);

			virtual bool opJoinLobby(void);
			virtual bool opLeaveLobby(void);
			virtual bool opCreateRoom(const Common::JString& gameID, bool isVisible=true, bool isOpen=true, nByte maxPlayers=0, const Common::Hashtable& customRoomProperties=Common::Hashtable(), const Common::Hashtable& customLocalPlayerProperties=Common::Hashtable(), const Common::JVector<Common::JString>& propsListedInLobby=Common::JVector<Common::JString>());
			virtual bool opJoinRoom(const Common::JString& gameID, const Common::Hashtable& customLocalPlayerProperties=Common::Hashtable());
			virtual bool opJoinRandomRoom(const Common::Hashtable& customRoomProperties=Common::Hashtable(), nByte maxPlayers=0);
			virtual bool opLeaveRoom(void);
			template<typename Ftype> typename Common::Helpers::EnableIf<!Common::Helpers::ConfirmAllowed<Ftype>::dimensions, bool>::type opRaiseEvent(bool reliable, Ftype parameters, nByte eventCode, nByte channelID=0, nByte eventCaching=Lite::EventCache::DO_NOT_CACHE, const int* targetPlayers=NULL, short numTargetPlayers=0, nByte receiverGroup=Lite::ReceiverGroup::OTHERS)
			{
				return opRaiseEvent(reliable, Common::Helpers::ValueToObject::get(parameters), eventCode, channelID, eventCaching, targetPlayers, numTargetPlayers, receiverGroup);
			}
			template<typename Ftype> typename Common::Helpers::EnableIf<Common::Helpers::ConfirmAllowed<Ftype>::dimensions==1, bool>::type opRaiseEvent(bool reliable, Ftype pParameterArray, short arrSize, nByte eventCode, nByte channelID=0, nByte eventCaching=Lite::EventCache::DO_NOT_CACHE, const int* targetPlayers=NULL, short numTargetPlayers=0, nByte receiverGroup=Lite::ReceiverGroup::OTHERS)
			{
				return opRaiseEvent(reliable, Common::Helpers::ValueToObject::get(pParameterArray, arrSize), eventCode, channelID, eventCaching, targetPlayers, numTargetPlayers, receiverGroup);
			}
			template<typename Ftype> typename Common::Helpers::EnableIf<(Common::Helpers::ConfirmAllowed<Ftype>::dimensions>1), bool>::type opRaiseEvent(bool reliable, Ftype pParameterArray, short* pArrSizes, nByte eventCode, nByte channelID=0, nByte eventCaching=Lite::EventCache::DO_NOT_CACHE, const int* targetPlayers=NULL, short numTargetPlayers=0, nByte receiverGroup=Lite::ReceiverGroup::OTHERS)
			{
				return opRaiseEvent(reliable, Common::Helpers::ValueToObject::get(pParameterArray, pArrSizes), eventCode, channelID, eventCaching, targetPlayers, numTargetPlayers, receiverGroup);
			}
			virtual bool opAuthenticate(const Common::JString& appID, const Common::JString& appVersion, bool encrypted, const Common::JString& userID=Common::JString(), const AuthenticationValues& authenticationValues=AuthenticationValues());
			virtual bool opFindFriends(const Common::JString* friendsToFind, short numFriendsToFind);
		protected:
			virtual bool opSetPropertiesOfPlayer(int playerNr, const Common::Hashtable& properties);
			virtual bool opSetPropertiesOfRoom(const Common::Hashtable& properties);
			Photon::OperationRequestParameters opCreateRoomImplementation(const Common::JString& gameID, bool isVisible, bool isOpen, nByte maxPlayers, const Common::Hashtable& customRoomProperties, const Common::Hashtable& customLocalPlayerProperties, const Common::JVector<Common::JString>& propsListedInLobby=Common::JVector<Common::JString>());
			Photon::OperationRequestParameters opJoinRoomImplementation(const Common::JString& gameID, const Common::Hashtable& customLocalPlayerProperties);
			virtual bool opRaiseEvent(bool reliable, const Common::Object& parameters, nByte eventCode, nByte channelID=0, nByte eventCaching=Lite::EventCache::DO_NOT_CACHE, const int* targetPlayers=NULL, short numTargetPlayers=0, nByte receiverGroup=Lite::ReceiverGroup::OTHERS);
		private:
			typedef PhotonPeer super;

			friend class MutablePlayer;
			friend class MutableRoom;
		};
	}
}