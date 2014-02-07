/* Exit Games Photon LoadBalancing - C++ Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#include "LoadBalancing-cpp/inc/Client.h"
#include "LoadBalancing-cpp/inc/Internal/AuthenticationValuesSecretSetter.h"
#include "LoadBalancing-cpp/inc/Internal/PlayerMovementInformant.h"
#include "LoadBalancing-cpp/inc/Internal/Utils.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/CustomAuthenticationType.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/EventCode.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/JoinType.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/OperationCode.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/ParameterCode.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/Properties/Player.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/Properties/Room.h"

/** @file Client.h */

namespace ExitGames
{
	namespace LoadBalancing
	{
		using namespace Common;
		using namespace Common::MemoryManagement;
		using namespace Photon;
		using namespace Photon::StatusCode;
		using namespace Internal;

		// default master server address and port
		const EG_CHAR* const MASTERSERVER = L"app-eu.exitgamescloud.com";
		
		const bool SEND_AUTHENTICATE_ENCRYPTED = true;

		PeerStates::PeerStates Client::getState(void) const
		{
			return mState;
		}

		const JString& Client::getMasterserverAddress(void) const
		{
			return mMasterserver;
		}

		int Client::getCountPlayersIngame(void) const
		{
			return mPeerCount;
		}

		int Client::getCountGamesRunning(void) const
		{
			return mRoomCount;
		}

		int Client::getCountPlayersOnline(void) const
		{
			return mPeerCount + mMasterPeerCount;
		}

		MutableRoom& Client::getCurrentlyJoinedRoom(void)
		{
			if(!mpCurrentlyJoinedRoom)
				mpCurrentlyJoinedRoom = createMutableRoom(L"", Hashtable()); 
			return *mpCurrentlyJoinedRoom;
		}

		const JVector<Room*>& Client::getRoomList(void) const
		{
			return mRoomList;
		}
		
		const JVector<JString>& Client::getRoomNameList(void) const
		{
			return mRoomNameList;
		}

		bool Client::getIsInRoom(void) const
		{
			return getIsInGameRoom() || getIsInLobby();
		}
		
		bool Client::getIsInGameRoom(void) const
		{
			return mState == PeerStates::Joined;
		}

		bool Client::getIsInLobby(void) const
		{
			return mState == PeerStates::JoinedLobby;
		}

		bool Client::getAutoJoinLobby(void) const
		{
			return mAutoJoinLobby;
		}

		void Client::setAutoJoinLobby(bool onConnect)
		{
			mAutoJoinLobby = onConnect;
		}

		MutablePlayer& Client::getLocalPlayer(void)
		{
			if(!mpLocalPlayer)
				mpLocalPlayer = createMutablePlayer(-1, Hashtable()); 
			return *mpLocalPlayer;
		}

		const JVector<FriendInfo>& Client::getFriendList(void)
		{
			return mFriendList;
		}

		int Client::getFriendListAge(void)
		{
			return mIsFetchingFriendList||!mFriendListTimestamp ? 0 : GETTIMEMS()-mFriendListTimestamp;
		}

		Client::Client(Listener& listener, const JString& applicationID, const JString& appVersion, const JString& username, bool useTcp, AuthenticationValues authenticationValues)
#if defined _EG_MS_COMPILER
#	pragma warning(push)
#	pragma warning(disable:4355)
#endif
			: Peer(*this, useTcp)
			, mListener(listener)
			, mAppVersion(appVersion)
			, mAppID(applicationID)
			, mPeerCount(0)
			, mRoomCount(0)
			, mMasterPeerCount(0)
			, mLastJoinType(0)
			, mpCurrentlyJoinedRoom(NULL)
			, mSenttime(0)
			, mCachedErrorCodeFromGameServer(ErrorCode::OK)
			, mUseTcp(useTcp)
			, mAutoJoinLobby(true)
			, mpLocalPlayer(NULL)
			, mLocalPlayerName(username)
			, mIsFetchingFriendList(false)
			, mState(PeerStates::Uninitialized)
			, mAuthenticationValues(authenticationValues)
#ifdef _EG_MS_COMPILER
#	pragma warning(pop)
#endif
		{
		}

		Client::~Client(void)
		{
			destroyMutableRoom(mpCurrentlyJoinedRoom);
			destroyMutablePlayer(mpLocalPlayer);
			for(unsigned int i=0; i<mRoomList.getSize(); ++i)
				destroyRoom(mRoomList[i]);
		}

		bool Client::connect(void)
		{
			return connect(MASTERSERVER);
		}

		bool Client::connect(const JString& masterServerAddress)
		{
			getLocalPlayer().setName(mLocalPlayerName);
			mState = PeerStates::Connecting;
			return super::connect(mMasterserver=masterServerAddress);
		}

		void Client::disconnect(void)
		{
			mState = PeerStates::Disconnecting;
			super::disconnect();
		}

		bool Client::opJoinLobby(void)
		{
			if(getIsInRoom())
			{
				EGLOG(DebugLevel::ERRORS, L"already in a room");
				return false;
			}

			mLastJoinType = JoinType::EXPLICIT_JOIN_LOBBY;
			return opCustom(OperationRequest(OperationCode::JOIN_LOBBY), true);
		}

		bool Client::opLeaveLobby(void)
		{
			if(!getIsInLobby())
			{
				EGLOG(DebugLevel::ERRORS, L"lobby isn't currently joined");
				return false;
			}

			return opCustom(OperationRequest(OperationCode::LEAVE_LOBBY), true);
		}

		bool Client::opCreateRoom(const JString& gameID, bool isVisible, bool isOpen, nByte maxPlayers, const Hashtable& customRoomProperties, const JVector<JString>& propsListedInLobby)
		{
			if(getIsInGameRoom())
			{
				EGLOG(DebugLevel::ERRORS, L"already in a gameroom");
				return false;
			}

			mRoomName = gameID;

			OperationRequestParameters op(opCreateRoomImplementation(gameID, isVisible, isOpen, maxPlayers, getIsOnGameServer()?customRoomProperties:Hashtable(), getIsOnGameServer()?getLocalPlayer().getCustomProperties():Hashtable(), getIsOnGameServer()?propsListedInLobby:JVector<JString>()));
			if(getLocalPlayer().getName().length())
			{
				if((ValueObject<Hashtable>*)op.getValue(static_cast<nByte>(ParameterCode::PLAYER_PROPERTIES)))
					((ValueObject<Hashtable>*)op.getValue(static_cast<nByte>(ParameterCode::PLAYER_PROPERTIES)))->getDataAddress()->put(static_cast<nByte>(Properties::Player::PLAYERNAME), getLocalPlayer().getName());
				else
				{
					Hashtable playerProp;
					playerProp.put(static_cast<nByte>(Properties::Player::PLAYERNAME), getLocalPlayer().getName());
					op.put(static_cast<nByte>(ParameterCode::PLAYER_PROPERTIES), ValueObject<Hashtable>(playerProp));
				}
			}

			if(!opCustom(OperationRequest(OperationCode::CREATE_ROOM, op), true))
				return false;

			Hashtable roomProps(Utils::stripToCustomProperties(customRoomProperties));
			roomProps.put(static_cast<nByte>(Properties::Room::IS_OPEN), isOpen);
			roomProps.put(static_cast<nByte>(Properties::Room::IS_VISIBLE), isVisible);
			roomProps.put(static_cast<nByte>(Properties::Room::MAX_PLAYERS), maxPlayers);
			JString* propsListedInLobbyArr = allocateArray<JString>(propsListedInLobby.getSize());
			for(unsigned int i=0; i<propsListedInLobby.getSize(); ++i)
				propsListedInLobbyArr[i] = propsListedInLobby[i];
			roomProps.put(static_cast<nByte>(Properties::Room::PROPS_LISTED_IN_LOBBY), propsListedInLobbyArr, propsListedInLobby.getSize());
			deallocateArray(propsListedInLobbyArr);

			MutableRoom* pOldRoom = mpCurrentlyJoinedRoom;
			mpCurrentlyJoinedRoom = createMutableRoom(gameID, roomProps,  propsListedInLobby);
			destroyMutableRoom(pOldRoom);
			return true;
		}

		bool Client::opJoinRoom(const JString& gameID)
		{
			if(getIsInGameRoom())
			{
				EGLOG(DebugLevel::ERRORS, L"already in a gameroom");
				return false;
			}

			mRoomName = gameID;
			OperationRequestParameters op = opJoinRoomImplementation(gameID, getIsOnGameServer()?getLocalPlayer().getCustomProperties():Hashtable());
			if(getLocalPlayer().getName().length())
			{
				if((ValueObject<Hashtable>*)op.getValue(static_cast<nByte>(ParameterCode::PLAYER_PROPERTIES)))
					((ValueObject<Hashtable>*)op.getValue(static_cast<nByte>(ParameterCode::PLAYER_PROPERTIES)))->getDataAddress()->put(static_cast<nByte>(Properties::Player::PLAYERNAME), getLocalPlayer().getName());
				else
				{
					Hashtable playerProp;
					playerProp.put(static_cast<nByte>(Properties::Player::PLAYERNAME), getLocalPlayer().getName());
					op.put(static_cast<nByte>(ParameterCode::PLAYER_PROPERTIES), ValueObject<Hashtable>(playerProp));
				}
			}
			if(!gameID.length() || !opCustom(OperationRequest(OperationCode::JOIN_ROOM, op), true))
				return false;

			destroyMutableRoom(mpCurrentlyJoinedRoom);
			mpCurrentlyJoinedRoom = createMutableRoom(gameID, Hashtable());
			return true;
		}

		bool Client::opJoinRandomRoom(const Hashtable& customRoomProperties, nByte maxPlayers)
		{
			if(getIsInGameRoom())
			{
				EGLOG(DebugLevel::ERRORS, L"already in a gameroom");
				return false;
			}
			
			if(!super::opJoinRandomRoom(customRoomProperties, maxPlayers))
				return false;

			destroyMutableRoom(mpCurrentlyJoinedRoom);
			mpCurrentlyJoinedRoom = createMutableRoom(L"", Utils::stripToCustomProperties(customRoomProperties));
			return true;
		}

		bool Client::opLeaveRoom(void)
		{
			if(!getIsInGameRoom())
			{
				EGLOG(DebugLevel::ERRORS, L"no gameroom is currently joined");
				return false;
			}

			if(!super::opLeaveRoom())
				return false;

			mState = PeerStates::Leaving;
			return true;
		}
		
		bool Client::opFindFriends(const JString* friendsToFind, short numFriendsToFind)
		{
			if(getIsOnGameServer() || mIsFetchingFriendList)
				return false;
			mFriendList.removeAllElements();
			for(short i=0; i<numFriendsToFind; ++i)
				mFriendList.addElement(friendsToFind[i]);
			return mIsFetchingFriendList = super::opFindFriends(friendsToFind, numFriendsToFind);
		}



		// protocol implementations

		void Client::onOperationResponse(const OperationResponse& operationResponse)
		{
			EGLOG(operationResponse.getReturnCode()?DebugLevel::ERRORS:DebugLevel::INFO, operationResponse.toString(true, true));
			switch(operationResponse.getOperationCode())
			{
			case OperationCode::AUTHENTICATE:
				{
					PeerStates::PeerStates oldState = mState;
					if(operationResponse.getReturnCode())
					{
						EGLOG(DebugLevel::ERRORS, L"authentication failed with errorcode %d: %ls", operationResponse.getReturnCode(), operationResponse.getDebugMessage().cstr());
						handleConnectionFlowError(oldState, operationResponse.getReturnCode(), operationResponse.getDebugMessage());
						break;
					}
					else
					{
						if(mState == PeerStates::Connected || mState == PeerStates::ConnectedComingFromGameserver)
						{
							mState = mState==PeerStates::Connected?PeerStates::Authenticated:PeerStates::AuthenticatedComingFromGameserver;
							if(ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::SECRET)).getDataCopy() && mAuthenticationValues.getType() != CustomAuthenticationType::NONE)
							{
								AuthenticationValuesSecretSetter::setSecret(mAuthenticationValues, ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::SECRET)).getDataCopy());
								EGLOG(DebugLevel::INFO, L"Server returned secret: %ls.", mAuthenticationValues.getSecret().cstr());
							}
							if(mAutoJoinLobby)
							{
								opJoinLobby();
								mLastJoinType = JoinType::AUTO_JOIN_LOBBY;
							}
							else
								onConnectToMasterFinished(oldState==PeerStates::ConnectedComingFromGameserver);
						}
						else if(mState == PeerStates::ConnectedToGameserver)
						{
							AuthenticationValuesSecretSetter::setSecret(mAuthenticationValues, L""); //reset secret
							mState = PeerStates::Joining;
							if(mLastJoinType == JoinType::CREATE_ROOM)
								opCreateRoom(mRoomName, mpCurrentlyJoinedRoom->getIsVisible(), mpCurrentlyJoinedRoom->getIsOpen(), mpCurrentlyJoinedRoom->getMaxPlayers(), mpCurrentlyJoinedRoom->getCustomProperties(), mpCurrentlyJoinedRoom->getPropsListedInLobby());
							else
								opJoinRoom(mRoomName);
						}
					}
				}

				break;
			case OperationCode::CREATE_ROOM:
			case OperationCode::JOIN_ROOM:
				{
					if(getIsOnGameServer())
					{
						if(operationResponse.getReturnCode())
						{
							EGLOG(DebugLevel::ERRORS, L"%ls failed with errorcode %d: %ls. Client is therefore returning to masterserver!", operationResponse.getOperationCode()==OperationCode::CREATE_ROOM?L"opCreateRoom":L"opJoinRoom", operationResponse.getReturnCode(), operationResponse.getDebugMessage().cstr());
							handleConnectionFlowError(mState, operationResponse.getReturnCode(), operationResponse.getDebugMessage());
							break;
						}

						mState = PeerStates::Joined;
						int nr = ValueObject<int>(operationResponse.getParameterForCode(ParameterCode::PLAYERNR)).getDataCopy();
						Hashtable properties = getLocalPlayer().getCustomProperties();
						properties.put(static_cast<nByte>(Properties::Player::PLAYERNAME), getLocalPlayer().getName());
						destroyMutablePlayer(mpLocalPlayer);
						PlayerMovementInformant::onEnter<true>(*mpCurrentlyJoinedRoom, *(mpLocalPlayer=createMutablePlayer(nr, properties)));

						Hashtable roomProperties(ValueObject<Hashtable>(operationResponse.getParameterForCode(ParameterCode::ROOM_PROPERTIES)).getDataCopy());
						Hashtable playerProperties(ValueObject<Hashtable>(operationResponse.getParameterForCode(ParameterCode::PLAYER_PROPERTIES)).getDataCopy());
						const JVector<Object>& numbers = playerProperties.getKeys();
						for(unsigned int i=0 ; i<numbers.getSize() ; ++i)
							PlayerMovementInformant::onEnter(*mpCurrentlyJoinedRoom, KeyObject<int>(numbers[i]).getDataCopy(), ValueObject<Hashtable>(playerProperties.getValue(numbers[i])).getDataCopy());
						readoutProperties(roomProperties, playerProperties, true, 0);

						switch(mLastJoinType)
						{
						case JoinType::CREATE_ROOM:
							mListener.createRoomReturn(nr, roomProperties, playerProperties, operationResponse.getReturnCode(), operationResponse.getDebugMessage());
							break;
						case JoinType::JOIN_ROOM:
							mListener.joinRoomReturn(nr, roomProperties, playerProperties, operationResponse.getReturnCode(), operationResponse.getDebugMessage());
							break;
						case JoinType::JOIN_RANDOM_ROOM:
							mListener.joinRandomRoomReturn(nr, roomProperties, playerProperties, operationResponse.getReturnCode(), operationResponse.getDebugMessage());
							break;
						default:
							break;
						}
						break;
					}
					else
					{
						switch(operationResponse.getOperationCode())
						{
						case OperationCode::CREATE_ROOM:
							{
								if(operationResponse.getReturnCode())
								{
									EGLOG(DebugLevel::ERRORS, L"opCreateRoom failed with errorcode %d: %ls. Client is therefore staying on masterserver!", operationResponse.getReturnCode(), operationResponse.getDebugMessage().cstr());
									mListener.createRoomReturn(0, Hashtable(), Hashtable(), operationResponse.getReturnCode(), operationResponse.getDebugMessage());
									break;
								}
								JString gameID = ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::ROOM_NAME)).getDataCopy();
								if(gameID.length()) // is only sent by the server's response, if it has not been sent with the client's request before!
									mRoomName = gameID;

								mGameserver = ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::ADDRESS)).getDataCopy();
								mState = PeerStates::DisconnectingFromMasterserver;
								super::disconnect();
								mLastJoinType = JoinType::CREATE_ROOM;
							}
							break;
						case OperationCode::JOIN_ROOM:
							if(operationResponse.getReturnCode())
							{
								EGLOG(DebugLevel::ERRORS, L"opJoinRoom failed with errorcode %d: %ls. Client is therefore staying on masterserver!", operationResponse.getReturnCode(), operationResponse.getDebugMessage().cstr());
								mListener.joinRoomReturn(0, Hashtable(), Hashtable(), operationResponse.getReturnCode(), operationResponse.getDebugMessage());
								break;
							}

							mGameserver = ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::ADDRESS)).getDataCopy();
							mState = PeerStates::DisconnectingFromMasterserver;
							super::disconnect();
							mLastJoinType = JoinType::JOIN_ROOM;
							break;
						default:
							break;
						}
					}
				}
				break;
			case OperationCode::JOIN_RANDOM_ROOM:
				if(operationResponse.getReturnCode())
				{
					EGLOG(DebugLevel::ERRORS, L"opJoinRandomRoom failed with errorcode %d: %ls. Client is therefore staying on masterserver!", operationResponse.getReturnCode(), operationResponse.getDebugMessage().cstr());
					mListener.joinRandomRoomReturn(0, Hashtable(), Hashtable(), operationResponse.getReturnCode(), operationResponse.getDebugMessage());
					break;
				}

				// store the ID of the random game, joined on the masterserver, so that we know, which game to join on the gameserver
				mRoomName = ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::ROOM_NAME)).getDataCopy();
				mGameserver = ValueObject<JString>(operationResponse.getParameterForCode(ParameterCode::ADDRESS)).getDataCopy();
				mState = PeerStates::DisconnectingFromMasterserver;
				super::disconnect();
				mLastJoinType = JoinType::JOIN_RANDOM_ROOM;
				break;
			case OperationCode::JOIN_LOBBY:
				if(mLastJoinType == JoinType::AUTO_JOIN_LOBBY)
					onConnectToMasterFinished(mState==PeerStates::AuthenticatedComingFromGameserver);
				else
					mListener.joinLobbyReturn();
				mState = PeerStates::JoinedLobby;
				break;
			case OperationCode::LEAVE_LOBBY:
				{
					mState = PeerStates::Authenticated;
					mListener.leaveLobbyReturn();
				}
				break;
			case OperationCode::LEAVE:
				{
					PlayerMovementInformant::onLeave<true>(*mpCurrentlyJoinedRoom, getLocalPlayer().getNumber());
					Hashtable op = getLocalPlayer().getCustomProperties();
					op.put(static_cast<nByte>(Properties::Player::PLAYERNAME), getLocalPlayer().getName());
					destroyMutablePlayer(mpLocalPlayer);
					mpLocalPlayer = createMutablePlayer(-1, op);
					mIsFetchingFriendList = false;
					mState = PeerStates::DisconnectingFromGameserver;
					super::disconnect();
				}
				break;
			case OperationCode::FIND_FRIENDS:
				{
					mIsFetchingFriendList = false;
					if(operationResponse.getReturnCode())
					{
						EGLOG(DebugLevel::ERRORS, L"opFindFriends failed with errorcode %d: %ls.", operationResponse.getReturnCode(), operationResponse.getDebugMessage().cstr());
						break;
					}
					ValueObject<bool*> onlineList = operationResponse.getParameterForCode(ParameterCode::FIND_FRIENDS_RESPONSE_ONLINE_LIST);
					ValueObject<JString*> roomList = operationResponse.getParameterForCode(ParameterCode::FIND_FRIENDS_RESPONSE_ROOM_ID_LIST);
					bool* pOnlineList = *onlineList.getDataAddress();
					JString* pRoomList = *roomList.getDataAddress();
					for(unsigned int i=0; i<mFriendList.getSize(); ++i)
						mFriendList[i] = FriendInfo(mFriendList[i].getName(), pOnlineList[i], pRoomList[i]);
					if(!(mFriendListTimestamp=GETTIMEMS()))
						mFriendListTimestamp = 1;
					mListener.onFindFriendsResponse();
				}
				break;
			default:
				break;
			}
		}

		void Client::onStatusChanged(int statusCode)
		{
			switch(statusCode)
			{
			case 0:
				break;
			case StatusCode::CONNECT:
				{
					if(mState == PeerStates::ConnectingToGameserver)
					{
						EGLOG(DebugLevel::INFO, L"connected to gameserver");
						mState = PeerStates::ConnectedToGameserver;
					}
					else
					{
						EGLOG(DebugLevel::INFO, L"connected to masterserver");
						mState = mState == PeerStates::Connecting ? PeerStates::Connected : PeerStates::ConnectedComingFromGameserver;
					}
					if(SEND_AUTHENTICATE_ENCRYPTED)
						super::establishEncryption();
					else
						super::opAuthenticate(mAppID, mAppVersion, false, getLocalPlayer().getName(), mAuthenticationValues);
				}
				break;
			case StatusCode::DISCONNECT:
				{
					mIsFetchingFriendList = false;
					if(mState == PeerStates::DisconnectingFromMasterserver)
					{
						super::connect(mGameserver);
						mState = PeerStates::ConnectingToGameserver;
					}
					else if(mState == PeerStates::DisconnectingFromGameserver)
					{
						super::connect(mMasterserver);
						mState = PeerStates::ConnectingToMasterserver;
					}
					else
					{
						mState = PeerStates::PeerCreated;
						mListener.disconnectReturn();
					}
				}
				break;
			case StatusCode::ENCRYPTION_ESTABLISHED:
				super::opAuthenticate(mAppID, mAppVersion, true, getLocalPlayer().getName(), mAuthenticationValues);
				break;
			case StatusCode::ENCRYPTION_FAILED_TO_ESTABLISH:
				handleConnectionFlowError(mState, statusCode, "Encryption failed to establish");
				break;
			case StatusCode::EXCEPTION:
			case StatusCode::EXCEPTION_ON_CONNECT:
			case StatusCode::INTERNAL_RECEIVE_EXCEPTION:
			case StatusCode::TIMEOUT_DISCONNECT:
			case StatusCode::DISCONNECT_BY_SERVER:
			case StatusCode::DISCONNECT_BY_SERVER_USER_LIMIT:
			case StatusCode::DISCONNECT_BY_SERVER_LOGIC:
				mListener.connectionErrorReturn(statusCode);
				if(this->getPeerState() != PeerState::DISCONNECTED && this->getPeerState() != PeerState::DISCONNECTING)
					disconnect();
				break;
			case StatusCode::SEND_ERROR:
				mListener.clientErrorReturn(statusCode);
				break;
			case QUEUE_OUTGOING_RELIABLE_WARNING:
			case QUEUE_OUTGOING_UNRELIABLE_WARNING:
			case QUEUE_OUTGOING_ACKS_WARNING:
			case QUEUE_INCOMING_RELIABLE_WARNING:
			case QUEUE_INCOMING_UNRELIABLE_WARNING:
			case QUEUE_SENT_WARNING:
				mListener.warningReturn(statusCode);
				break;
			case ErrorCode::OPERATION_INVALID:
			case ErrorCode::INTERNAL_SERVER_ERROR:
				mListener.serverErrorReturn(statusCode);
				break;
			default:
				EGLOG(DebugLevel::ERRORS, L"received unknown status-code from server");
				break;
			}
		}

		void Client::onEvent(const EventData& eventData)
		{
			EGLOG(DebugLevel::INFO, eventData.toString(true, true)); // don't print out the payload here, as that can get too expensive for big events
			switch(eventData.getCode())
			{
			case EventCode::ROOM_LIST:
				{
					for(unsigned int i=0; i<mRoomList.getSize(); ++i)
						destroyRoom(mRoomList[i]);
					mRoomList.removeAllElements();
					mRoomNameList.removeAllElements();

					Hashtable roomList = ValueObject<Hashtable>(eventData.getParameterForCode(ParameterCode::ROOM_LIST)).getDataCopy();
					const JVector<Object>& keys = roomList.getKeys();
					JString roomNameValue;
					for(unsigned int i=0 ; i<keys.getSize(); ++i)
					{
						roomNameValue = KeyObject<JString>(keys[i]).getDataCopy();
						mRoomList.addElement(createRoom(roomNameValue, ValueObject<Hashtable>(roomList.getValue(keys[i])).getDataCopy()));
						mRoomNameList.addElement(roomNameValue);
					}
					mListener.onRoomListUpdate();
				}
				break;
			case EventCode::ROOM_LIST_UPDATE:
				{
					Hashtable roomListUpdate(ValueObject<Hashtable>(eventData.getParameterForCode(ParameterCode::ROOM_LIST)).getDataCopy());
					const JVector<Object>& keys = roomListUpdate.getKeys();
					for(unsigned int i=0; i<keys.getSize(); i++)
					{
						Hashtable val(ValueObject<Hashtable>(roomListUpdate.getValue(keys[i])).getDataCopy());
						bool removed = ValueObject<bool>(val.getValue(static_cast<nByte>(Properties::Room::REMOVED))).getDataCopy();
						int index = mRoomNameList.getIndexOf(KeyObject<JString>(keys[i]).getDataCopy());

						if(!removed)
						{
							if(index == -1) //add room
							{
								JString roomNameValue = KeyObject<JString>(keys[i]).getDataCopy();
								mRoomList.addElement(createRoom(roomNameValue, val));
								mRoomNameList.addElement(roomNameValue);
							}
							else // update room (only entries, which have been changed, have been sent)
								RoomPropertiesCacher::cache(*mRoomList[index], val);
						}
						else if(index > -1) // remove room
						{
							destroyRoom(mRoomList[i]);
							mRoomList.removeElementAt(index);
							mRoomNameList.removeElementAt(index);
						}
					}
					mListener.onRoomListUpdate();
				}
				break;
			case EventCode::APP_STATS:
				{
					mPeerCount = ValueObject<int>(eventData.getParameterForCode(ParameterCode::PEER_COUNT)).getDataCopy();
					mRoomCount = ValueObject<int>(eventData.getParameterForCode(ParameterCode::ROOM_COUNT)).getDataCopy();
					mMasterPeerCount = ValueObject<int>(eventData.getParameterForCode(ParameterCode::MASTER_PEER_COUNT)).getDataCopy();
					mListener.onAppStatsUpdate();
				}
				break;
			case EventCode::JOIN:
				{
					int nr = ValueObject<int>(eventData.getParameterForCode(ParameterCode::PLAYERNR)).getDataCopy();
					if(nr != getLocalPlayer().getNumber()) // the local player already got added in onOperationResponse cases OperationCode::CREATE_ROOM / OperationCode::JOIN_ROOM
						PlayerMovementInformant::onEnter(getCurrentlyJoinedRoom(), nr, ValueObject<Hashtable>(eventData.getParameterForCode(ParameterCode::PLAYER_PROPERTIES)).getDataCopy());
					int* players = ValueObject<int*>(eventData.getParameterForCode(ParameterCode::PLAYER_LIST)).getDataCopy();
					JVector<int> playerArr;
					int* playersPtr = players;
					for(int i=0 ; i<nr ; ++i, playersPtr++)
						playerArr.addElement(*playersPtr);
					mListener.joinRoomEventAction(nr, playerArr, *getCurrentlyJoinedRoom().getPlayerForNumber(nr));
					deallocateArray(players);
				}
				break;
			case EventCode::LEAVE:
				{
					ValueObject<int> nr = eventData.getParameterForCode(ParameterCode::PLAYERNR);
					if(!PlayerMovementInformant::onLeave<false>(getCurrentlyJoinedRoom(), nr.getDataCopy()))
						EGLOG(DebugLevel::WARNINGS, L"EventCode::LEAVE - player %d who is leaving the room, has not been found in list of players, who are currently in the room", nr.getDataCopy());
					mListener.leaveRoomEventAction(nr.getDataCopy());
				}
				break;
			case EventCode::PROPERTIES_CHANGED:
				{
					ValueObject<int> target = eventData.getParameterForCode(ParameterCode::TARGET_PLAYERNR);
					Hashtable playerProperties;
					Hashtable roomProperties;
					if(target.getDataCopy())
						playerProperties = ValueObject<Hashtable>(eventData.getParameterForCode(ParameterCode::PROPERTIES)).getDataCopy();
					else
						roomProperties = ValueObject<Hashtable>(eventData.getParameterForCode(ParameterCode::PROPERTIES)).getDataCopy();
					readoutProperties(roomProperties, playerProperties, false, target.getDataCopy());
					if(playerProperties.getSize())
						mListener.onPlayerPropertiesChange(target.getDataCopy(), playerProperties);
					else
						mListener.onRoomPropertiesChange(roomProperties);
				}
				break;
			default: // custom events are forwarded to the app
				{
					ValueObject<int> nr = eventData.getParameterForCode(ParameterCode::PLAYERNR);
					// all custom event data is stored at ParameterCode::DATA
					mListener.customEventAction(nr.getDataCopy(), eventData.getCode(), eventData.getParameterForCode(ParameterCode::DATA));
				}
				break;
			}
		}

		void Client::debugReturn(DebugLevel::DebugLevel debugLevel, const JString& string)
		{
			mListener.debugReturn(debugLevel, string);
		}

		bool Client::getIsOnGameServer(void) const
		{
			return mState >= PeerStates::ConnectingToGameserver && mState < PeerStates::ConnectingToMasterserver;
		}

		void Client::readoutProperties(Hashtable& roomProperties, Hashtable& playerProperties, bool multiplePlayers, int targetPlayerNr)
		{
			if(roomProperties.getSize())
			{
				RoomPropertiesCacher::cache(*mpCurrentlyJoinedRoom, roomProperties);
				roomProperties = Utils::stripKeysWithNullValues(Utils::stripToCustomProperties(roomProperties));
			}

			if(playerProperties.getSize())
			{
				for(unsigned int i=0; i<(multiplePlayers?playerProperties.getSize():1); ++i)
				{
					Hashtable props = multiplePlayers?ValueObject<Hashtable>(playerProperties[i]).getDataCopy():playerProperties;
					if(props.contains(static_cast<nByte>(Properties::Player::PLAYERNAME)))
					{
						PlayerPropertiesUpdateInformant::onUpdate(*mpCurrentlyJoinedRoom, multiplePlayers?ValueObject<int>(playerProperties.getKeys()[i]).getDataCopy():targetPlayerNr, props);
						for(unsigned int j=0; j<mpCurrentlyJoinedRoom->getPlayers().getSize(); ++j)
						{
							if(mpCurrentlyJoinedRoom->getPlayers()[j]->getNumber() == (multiplePlayers?ValueObject<int>(playerProperties.getKeys()[i]).getDataCopy():targetPlayerNr))
							{
								if(getLocalPlayer().getNumber() == mpCurrentlyJoinedRoom->getPlayers()[j]->getNumber())
								{
									destroyMutablePlayer(mpLocalPlayer);
									mpLocalPlayer = createMutablePlayer(mpCurrentlyJoinedRoom->getPlayers()[j]->getNumber(), props);
								}
							}
						}
					}
				}

				if(multiplePlayers)
					for(unsigned int i=0; i<playerProperties.getSize(); ++i)
						playerProperties[i] = ValueObject<Hashtable>(Utils::stripKeysWithNullValues(Utils::stripToCustomProperties(ValueObject<Hashtable>(playerProperties[i]).getDataCopy())));
				else
					playerProperties = Utils::stripKeysWithNullValues(Utils::stripToCustomProperties(playerProperties));
			}
		}

		void Client::handleConnectionFlowError(PeerStates::PeerStates oldState, int errorCode, const JString& errorString)
		{
			if(oldState == PeerStates::ConnectedToGameserver || oldState == PeerStates::AuthenticatedOnGameServer || oldState == PeerStates::Joining)
			{
				mCachedErrorCodeFromGameServer = errorCode;
				mCachedErrorStringFromGameServer = errorString;
				mState = PeerStates::DisconnectingFromGameserver;
				super::disconnect();
				// response to app has to wait until back on master
			}
			else
			{
				mState = PeerStates::Disconnecting;
				super::disconnect();
				mListener.connectReturn(errorCode, errorString);
			}
		}

		bool Client::opAuthenticate(const JString& appID, const JString& appVersion, bool encrypted, const JString& userID)
		{
			return super::opAuthenticate(appID, appVersion, encrypted, userID);
		}

		bool Client::opSetPropertiesOfPlayer(int playerNr, const Hashtable& properties)
		{
			if(!getIsInGameRoom())
				return false;
			return super::opSetPropertiesOfPlayer(playerNr, properties);
		}

		bool Client::opSetPropertiesOfRoom(const Hashtable& properties)
		{
			if(!getIsInGameRoom())
				return false;
			return super::opSetPropertiesOfRoom(properties);
		}

		void Client::onConnectToMasterFinished(bool comingFromGameserver)
		{
			if(comingFromGameserver)
			{
				if(mCachedErrorCodeFromGameServer)
				{
					switch(mLastJoinType)
					{
						case JoinType::CREATE_ROOM:
							mListener.createRoomReturn(0, Hashtable(), Hashtable(), mCachedErrorCodeFromGameServer, mCachedErrorStringFromGameServer);
							break;
						case JoinType::JOIN_ROOM:
							mListener.joinRoomReturn(0, Hashtable(), Hashtable(), mCachedErrorCodeFromGameServer, mCachedErrorStringFromGameServer);
							break;
						case JoinType::JOIN_RANDOM_ROOM:
							mListener.joinRandomRoomReturn(0, Hashtable(), Hashtable(), mCachedErrorCodeFromGameServer, mCachedErrorStringFromGameServer);
							break;
						default:
							break;
					}
					mCachedErrorCodeFromGameServer = LoadBalancing::ErrorCode::OK;
					mCachedErrorStringFromGameServer = L"";
				}
				else
					mListener.leaveRoomReturn(0, L"");
			}
			else
				mListener.connectReturn(0, L"");
		}

		MutablePlayer* Client::createMutablePlayer(int number, const Hashtable& properties)
		{
			mpMutablePlayerFactory = getMutablePlayerFactory();
			return mpMutablePlayerFactory->create(number, properties, mpCurrentlyJoinedRoom, this);
		}

		void Client::destroyMutablePlayer(const MutablePlayer* pPlayer) const
		{
			if(pPlayer)
				mpMutablePlayerFactory->destroy(pPlayer);
		}

		Room* Client::createRoom(const JString& name, const Hashtable& properties)
		{
			return RoomFactory::create(name, properties);
		}

		void Client::destroyRoom(const Room* pRoom) const
		{
			RoomFactory::destroy(pRoom);
		}

		MutableRoom* Client::createMutableRoom(const JString& name, const Hashtable& properties, const JVector<JString>& propsListedInLobby)
		{
			mpMutableRoomFactory = getMutableRoomFactory();
			return mpMutableRoomFactory->create(name, properties, this, propsListedInLobby);
		}

		void Client::destroyMutableRoom(const MutableRoom* pRoom) const
		{
			if(pRoom)
				mpMutableRoomFactory->destroy(pRoom);
		}

		MutablePlayerFactory* Client::getMutablePlayerFactory(void) const
		{
			static MutablePlayerFactory fac;
			return &fac;
		}

		MutableRoomFactory* Client::getMutableRoomFactory(void) const
		{
			static MutableRoomFactory fac;
			return &fac;
		}
	}
}