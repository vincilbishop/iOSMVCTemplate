/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "LoadBalancing-cpp/inc/MutablePlayer.h"
#include "LoadBalancing-cpp/inc/MutableRoom.h"
#include "LoadBalancing-cpp/inc/Listener.h"
#include "LoadBalancing-cpp/inc/FriendInfo.h"

namespace ExitGames
{
	namespace LoadBalancing
	{
		class Client : public Peer, protected Photon::PhotonListener
		{
		public:
			Client(LoadBalancing::Listener& listener, const Common::JString& applicationID, const Common::JString& appVersion, const Common::JString& username, bool useTcp=false, AuthenticationValues authenticationValues=AuthenticationValues());
			virtual ~Client(void);

			virtual bool connect(void);
			virtual bool connect(const Common::JString& masterServerAddress);
			virtual void disconnect(void);

			virtual bool opJoinLobby(void);
			virtual bool opLeaveLobby(void);
			virtual bool opCreateRoom(const Common::JString& gameID, bool isVisible=true, bool isOpen=true, nByte maxPlayers=0, const Common::Hashtable& customRoomProperties=Common::Hashtable(), const Common::JVector<Common::JString>& propsListedInLobby=Common::JVector<Common::JString>());
			virtual bool opJoinRoom(const Common::JString& gameID);
			virtual bool opJoinRandomRoom(const Common::Hashtable& customRoomProperties=Common::Hashtable(), nByte maxPlayers=0);
			virtual bool opLeaveRoom(void);
			virtual bool opFindFriends(const Common::JString* friendsToFind, short numFriendsToFind);

			PeerStates::PeerStates getState(void) const;
			const Common::JString& getMasterserverAddress(void) const;
			int getCountPlayersIngame(void) const;
			int getCountGamesRunning(void) const;
			int getCountPlayersOnline(void) const;
			MutableRoom& getCurrentlyJoinedRoom(void);
			const Common::JVector<Room*>& getRoomList(void) const;
			const Common::JVector<Common::JString>& getRoomNameList(void) const;
			bool getIsInRoom(void) const;
			bool getIsInGameRoom(void) const;
			bool getIsInLobby(void) const;
			bool getAutoJoinLobby(void) const;
			void setAutoJoinLobby(bool onConnect);
			MutablePlayer& getLocalPlayer(void);
			const Common::JVector<FriendInfo>& getFriendList(void);
			int getFriendListAge(void);
		protected:
			virtual bool opAuthenticate(const Common::JString& appID, const Common::JString& appVersion, bool encrypted, const Common::JString& userID=Common::JString());
			virtual bool opSetPropertiesOfPlayer(int playerNr, const Common::Hashtable& properties);
			virtual bool opSetPropertiesOfRoom(const Common::Hashtable& properties);

			virtual Room* createRoom(const Common::JString& name, const Common::Hashtable& properties);
			virtual void destroyRoom(const Room* pRoom) const;
			virtual MutablePlayerFactory* getMutablePlayerFactory(void) const;
			virtual MutableRoomFactory* getMutableRoomFactory(void) const;

			// From Photon::PhotonListener
			virtual void onOperationResponse(const Photon::OperationResponse& operationResponse);
			virtual void onStatusChanged(int statusCode);
			virtual void onEvent(const Photon::EventData& eventData);
			virtual void debugReturn(Common::DebugLevel::DebugLevel debugLevel, const Common::JString& string);
		private:
			void readoutProperties(Common::Hashtable& roomProperties, Common::Hashtable& playerProperties, bool multiplePlayers, int targetPlayerNr);
			void handleConnectionFlowError(PeerStates::PeerStates oldState, int errorCode, const Common::JString& errorString);
			void onConnectToMasterFinished(bool comingFromGameserver);
			MutablePlayer* createMutablePlayer(int number, const Common::Hashtable& properties);
			void destroyMutablePlayer(const MutablePlayer* pPlayer) const;
			MutableRoom* createMutableRoom(const Common::JString& name, const Common::Hashtable& properties, const Common::JVector<Common::JString>& propsListedInLobby=Common::JVector<Common::JString>());
			void destroyMutableRoom(const MutableRoom* pRoom) const;

			bool getIsOnGameServer(void) const;

			typedef Peer super;

			Listener& mListener;
			Common::JString mGameserver;
			Common::JString mAppVersion;
			Common::JString mRoomName;
			Common::JString mAppID;
			int mPeerCount;
			int mRoomCount;
			int mMasterPeerCount;
			nByte mLastJoinType;
			MutableRoom* mpCurrentlyJoinedRoom;
			Common::JVector<Room*> mRoomList;
			Common::JVector<Common::JString> mRoomNameList;
			int mSenttime;
			int mCachedErrorCodeFromGameServer;
			Common::JString mCachedErrorStringFromGameServer;
			bool mUseTcp;
			Common::JString mMasterserver;
			bool mAutoJoinLobby;
			MutablePlayer* mpLocalPlayer;
			Common::JString mLocalPlayerName;
			Common::JVector<FriendInfo> mFriendList;
			int mFriendListTimestamp;
			bool mIsFetchingFriendList;
			PeerStates::PeerStates mState;
			AuthenticationValues mAuthenticationValues;
			MutablePlayerFactory* mpMutablePlayerFactory;
			MutableRoomFactory* mpMutableRoomFactory;
		};
	}
}