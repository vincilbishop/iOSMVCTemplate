/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#include "LoadBalancing-cpp/inc/Peer.h"
#include "Common-cpp/inc/Enums/DebugLevel.h"
#include "Photon-cpp/inc/Enums/ReceiverGroup.h"
#include "Photon-cpp/inc/Enums/EventCache.h"
#include "Photon-cpp/inc/Enums/ParameterCode.h"
#include "LoadBalancing-cpp/inc/Internal/AuthenticationValuesSecretSetter.h"
#include "LoadBalancing-cpp/inc/Internal/Utils.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/CustomAuthenticationType.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/JoinType.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/OperationCode.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/ParameterCode.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/Properties/Player.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/Properties/Room.h"

/** @file inc/Peer.h */

namespace ExitGames
{
	namespace LoadBalancing
	{
		using namespace Common;
		using namespace Common::MemoryManagement;
		using namespace Photon;
		using namespace Lite;
		using namespace Lite::ParameterCode;
		using namespace Internal;



		Peer::Peer(Photon::PhotonListener& listener, bool useTcp)
			: PhotonPeer(listener, useTcp)
		{
		}

		Peer::~Peer(void)
		{
		}

		bool Peer::opJoinLobby(void)
		{
			return opCustom(OperationRequest(OperationCode::JOIN_LOBBY), true);
		}

		bool Peer::opLeaveLobby(void)
		{
			return opCustom(OperationRequest(OperationCode::LEAVE_LOBBY), true);
		}

		bool Peer::opCreateRoom(const JString& gameID, bool isVisible, bool isOpen, nByte maxPlayers, const Hashtable& customRoomProperties, const Hashtable& customLocalPlayerProperties, const JVector<JString>& propsListedInLobby)
		{
			return opCustom(OperationRequest(OperationCode::CREATE_ROOM, opCreateRoomImplementation(gameID, isVisible, isOpen, maxPlayers, customRoomProperties, customLocalPlayerProperties, propsListedInLobby)), true);
		}

		bool Peer::opJoinRoom(const JString& gameID, const Hashtable& customLocalPlayerProperties)
		{
			return gameID.length() ? opCustom(OperationRequest(OperationCode::JOIN_ROOM, opJoinRoomImplementation(gameID, customLocalPlayerProperties)), true) : false;
		}
		
		bool Peer::opJoinRandomRoom(const Hashtable& customRoomProperties, nByte maxPlayers)
		{
			Hashtable roomProps(Utils::stripToCustomProperties(customRoomProperties));
			if(maxPlayers)
				roomProps.put(Properties::Room::MAX_PLAYERS, maxPlayers);

			OperationRequest opRequest(OperationCode::JOIN_RANDOM_ROOM);
			
			if(roomProps.getSize())
			{
				OperationRequestParameters op;
				op.put(ParameterCode::ROOM_PROPERTIES, ValueObject<Hashtable>(roomProps));
				opRequest.setParameters(op);
			}
			
			return opCustom(opRequest, true);
		}

		bool Peer::opLeaveRoom(void)
		{
			return opCustom(OperationRequest(OperationCode::LEAVE), true);
		}

		bool Peer::opRaiseEvent(bool reliable, const Common::Object& parameters, nByte eventCode, nByte channelID, nByte eventCaching, const int* targetPlayers, short numTargetPlayers, nByte receiverGroup)
		{
			OperationRequestParameters op;
			op.put(ParameterCode::DATA, parameters);
			op.put(ParameterCode::CODE, ValueObject<nByte>(eventCode));

			if(eventCaching != EventCache::DO_NOT_CACHE)
				op.put(Lite::ParameterCode::CACHE, ValueObject<nByte>(eventCaching));
			if(targetPlayers)
				op.put(Lite::ParameterCode::ACTOR_LIST, ValueObject<const int*>(targetPlayers, numTargetPlayers));
			if(receiverGroup != ReceiverGroup::OTHERS)
				op.put(Lite::ParameterCode::RECEIVER_GROUP, ValueObject<nByte>(receiverGroup));

			return opCustom(OperationRequest(OperationCode::RAISE_EVENT, op), reliable, channelID);
		}

		bool Peer::opAuthenticate(const JString& appID, const JString& appVersion, bool encrypted, const JString& userID, const AuthenticationValues& authenticationValues)
		{
			OperationRequestParameters op;
			op.put(ParameterCode::APPLICATION_ID, ValueObject<JString>(appID));
			op.put(ParameterCode::APP_VERSION, ValueObject<JString>(appVersion));
			if(userID.length())
				op.put(ParameterCode::USER_ID, ValueObject<JString>(userID));
			if(authenticationValues.getType() != CustomAuthenticationType::NONE)
			{
				op.put(ParameterCode::CLIENT_AUTHENTICATION_TYPE, ValueObject<nByte>(authenticationValues.getType()));
				if(authenticationValues.getSecret().length())
					op.put(ParameterCode::SECRET, ValueObject<JString>(authenticationValues.getSecret()));
				else
				{
					if(authenticationValues.getParameters().length())
						op.put(ParameterCode::CLIENT_AUTHENTICATION_PARAMETERS, ValueObject<JString>(authenticationValues.getParameters()));
					if(authenticationValues.getData().getSize())
						op.put(ParameterCode::CLIENT_AUTHENTICATION_DATA, ValueObject<const nByte*>(authenticationValues.getData().getCArray(), static_cast<int>(authenticationValues.getData().getSize())));
				}
			}
			EGLOG(DebugLevel::INFO, OperationRequest(OperationCode::AUTHENTICATE, op).toString(true));
			return opCustom(OperationRequest(OperationCode::AUTHENTICATE, op), true, 0, encrypted);
		}

		bool Peer::opFindFriends(const JString* friendsToFind, short numFriendsToFind)
		{
			if(!friendsToFind || !numFriendsToFind)
				return false;
			OperationRequestParameters op;
			op.put(ParameterCode::FIND_FRIENDS_REQUEST_LIST, ValueObject<const JString*>(friendsToFind, numFriendsToFind));

			return opCustom(OperationRequest(OperationCode::FIND_FRIENDS, op), true);
		}

		bool Peer::opSetPropertiesOfPlayer(int playerNr, const Hashtable& properties)
		{
			OperationRequestParameters op;
			op.put(ParameterCode::PROPERTIES, ValueObject<Hashtable>(properties));
			op.put(Lite::ParameterCode::ACTORNR, ValueObject<int>(playerNr));
			
			bool broadcast = true;
			op.put(ParameterCode::BROADCAST, ValueObject<bool>(broadcast));

			return opCustom(OperationRequest(OperationCode::SET_PROPERTIES, op), true);
		}

		bool Peer::opSetPropertiesOfRoom(const Hashtable& properties)
		{
			OperationRequestParameters op;
			op.put(ParameterCode::PROPERTIES, ValueObject<Hashtable>(properties));

			bool broadcast = true;
			op.put(ParameterCode::BROADCAST, ValueObject<bool>(broadcast));

			return opCustom(OperationRequest(OperationCode::SET_PROPERTIES, op), true);
		}

		OperationRequestParameters Peer::opCreateRoomImplementation(const JString& gameID, bool isVisible, bool isOpen, nByte maxPlayers, const Hashtable& customRoomProperties, const Hashtable& customLocalPlayerProperties, const JVector<JString>& propsListedInLobby)
		{
			OperationRequestParameters op;
			if(gameID.length())
				op.put(ParameterCode::ROOM_NAME, ValueObject<JString>(gameID));

			Hashtable roomProps(Utils::stripToCustomProperties(customRoomProperties));
			if(!isOpen)
				roomProps.put(Properties::Room::IS_OPEN, isOpen);
			if(!isVisible)
				roomProps.put(Properties::Room::IS_VISIBLE, isVisible);
			if(maxPlayers)
				roomProps.put(Properties::Room::MAX_PLAYERS, maxPlayers);
			JString* propsListedInLobbyArr = allocateArray<JString>(propsListedInLobby.getSize());
			for(unsigned int i=0; i<propsListedInLobby.getSize(); ++i)
				propsListedInLobbyArr[i] = propsListedInLobby[i];
			roomProps.put(Properties::Room::PROPS_LISTED_IN_LOBBY, propsListedInLobbyArr, propsListedInLobby.getSize());
			deallocateArray(propsListedInLobbyArr);
			op.put(ParameterCode::ROOM_PROPERTIES, ValueObject<Hashtable>(roomProps));

			Hashtable playerProperties = Utils::stripToCustomProperties(customLocalPlayerProperties);
			if(playerProperties.getSize())
				op.put(ParameterCode::PLAYER_PROPERTIES, ValueObject<Hashtable>(playerProperties));
			op.put(ParameterCode::BROADCAST, ValueObject<bool>(true));
			op.put(ParameterCode::CLEANUP_CACHE_ON_LEAVE, ValueObject<bool>(true));
			return op;
		}

		OperationRequestParameters Peer::opJoinRoomImplementation(const JString& gameID, const Hashtable& customLocalPlayerProperties)
		{
			OperationRequestParameters op;
			op.put(ParameterCode::ROOM_NAME, ValueObject<JString>(gameID));
			
			Hashtable playerProps = Utils::stripToCustomProperties(customLocalPlayerProperties);
			if(playerProps.getSize())
				op.put(ParameterCode::PLAYER_PROPERTIES, ValueObject<Hashtable>(playerProps));
			op.put(ParameterCode::BROADCAST, ValueObject<bool>(true));
			return op;
		}
	}
}