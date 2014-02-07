/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */
#import <Foundation/NSException.h>
#include "LoadBalancing-cpp/inc/Enums/ErrorCode.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/EventCode.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/JoinType.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/OperationCode.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/ParameterCode.h"
#include "LoadBalancing-cpp/inc/Internal/Enums/Properties/Room.h"
#import "LoadBalancing-objc/inc/Internal/EGAuthenticationValues+Internal.h"
#import "LoadBalancing-objc/inc/Internal/EGLoadBalancingPeer+Internal.h"
#import "LoadBalancing-objc/inc/Internal/EGLoadBalancingPlayer+Internal.h"
#import "LoadBalancing-objc/inc/Internal/EGLoadBalancingRoom+Internal.h"
#import "LoadBalancing-objc/inc/Protected/EGLoadBalancingClient+Protected.h"
#import "LoadBalancing-objc/inc/Protected/EGLoadBalancingPlayer+Protected.h"
#import "LoadBalancing-objc/inc/Protected/EGLoadBalancingRoom+Protected.h"

/** @file EGLoadBalancingClient.h */

namespace ExitGames
{
	namespace LoadBalancing
	{
		// default master server address and port
		const NSString* const MASTERSERVER = @"app-eu.exitgamescloud.com";
		
		const bool SEND_AUTHENTICATE_ENCRYPTED = true;
	}
}

using namespace ExitGames;
using namespace ExitGames::Photon;
using namespace ExitGames::Common::DebugLevel;
using namespace ExitGames::LoadBalancing;
using namespace ExitGames::LoadBalancing::Internal;

@interface EGLoadBalancingClient ()

@property (readonly) bool IsOnGameServer;
- (EGLoadBalancingMutablePlayer*) setLocalPlayer:(EGLoadBalancingMutablePlayer*)localPlayer;
- (EGLoadBalancingMutablePlayer*) setLocalPlayer:(int)number :(NSDictionary*)properties;

- (void) readoutProperties:(NSMutableDictionary**)pRoomProperties :(NSMutableDictionary**)pPlayerProperties :(bool)multiplePlayers :(int)targetPlayerNr;
- (void) handleConnectionFlowError:(PeerStates::PeerStates)oldState :(int)errorCode :(const NSString* const)errorString;
- (void) onConnectToMasterFinished:(bool)comingFromGameserver;

@end



@implementation EGLoadBalancingClient

// properties

@synthesize Listener = mListener;
@synthesize State = mState;

- (const NSString*) MasterserverAddress
{
	return mMasterserver;
}

@synthesize CountPlayersIngame = mPeerCount;
@synthesize CountGamesRunning = mRoomCount;

- (int) CountPlayersOnline
{
	return mPeerCount + mMasterPeerCount;
}

@synthesize RoomList = mRoomList;
@synthesize CurrentlyJoinedRoom = mCurrentlyJoinedRoom;

- (bool) IsInRoom
{
	return self.IsInGameRoom || self.IsInLobby;
}

- (bool) IsInGameRoom
{
	return mState == PeerStates::Joined;
}

- (bool) IsInLobby
{
	return mState == PeerStates::JoinedLobby;
}

@synthesize AutoJoinLobby = mAutoJoinLobby;
@synthesize LocalPlayer = mLocalPlayer;
@synthesize FriendList = mFriendList;

- (int) FriendListAge
{
	return mIsFetchingFriendList||!mFriendListTimestamp ? 0 : GETTIMEMS()-mFriendListTimestamp;
}



// methods

- (instancetype) initWithPhotonListener:(id<EGPhotonListener>)listener
{
	EG_ASSERT(false, @"not implemented!");
	return nil;
}

- (instancetype) initWithPhotonListener:(id<EGPhotonListener>)listener :(bool)useTcp
{
	EG_ASSERT(false, @"not implemented");
	return nil;
}

- (instancetype) initClient:(id<EGLoadBalancingListener>)listener :(NSString*)applicationID :(NSString*)appVersion :(NSString*)username
{
	return [self initClient:listener :applicationID :appVersion :username :false];
}

- (instancetype) initClient:(id<EGLoadBalancingListener>)listener :(NSString*)applicationID :(NSString*)appVersion :(NSString*)username :(bool)useTcp
{
	return [self initClient:listener :applicationID :appVersion :username :useTcp :nil];
}

- (instancetype) initClient:(id<EGLoadBalancingListener>)listener :(NSString*)applicationID :(NSString*)appVersion :(NSString*)username :(bool)useTcp :(EGAuthenticationValues*)authenticationValues
{
	self = [super initWithPhotonListener:self :mUseTcp=useTcp];
	mListener = listener;
	mState = PeerStates::PeerCreated;
	mAppVersion = [appVersion retain];
	mCurrentlyJoinedRoom = [[self createMutableRoom:@"" :nil :nil] retain];
	[self setLocalPlayer:-1 :[NSDictionary dictionaryWithObject:username forKey:[NSValue value:&Properties::Player::PLAYERNAME withObjCType:@encode(nByte)]]];
	mAppID = [applicationID retain];
	mAutoJoinLobby = true;
	mAuthenticationValues = [authenticationValues retain];
	return self;
}

- (bool) connect
{
	return [self connect:MASTERSERVER];
}

- (bool) connect:(const NSString* const)masterServerAddress
{
	mState = PeerStates::Connecting;
	return [super connect:mMasterserver=[masterServerAddress copy]];
}

- (void) disconnect
{
	mState = PeerStates::Disconnecting;
	[super disconnect];
}

- (bool) opJoinLobby
{
	if(self.IsInRoom)
	{
		EGLOG(ERRORS, L"already in a room");
		return false;
	}
	mLastJoinType = JoinType::EXPLICIT_JOIN_LOBBY;
	return [super opJoinLobby];
}

- (bool) opLeaveLobby
{
	if(!self.IsInLobby)
	{
		EGLOG(ERRORS, L"lobby isn't currently joined");
		return false;
	}
	return [super opLeaveLobby];
}

- (bool) opCreateRoom:(NSString*)gameID :(bool)isVisible :(bool)isOpen :(nByte)maxPlayers :(NSDictionary*)customRoomProperties
{
	return [self opCreateRoom:gameID :isVisible :isOpen :maxPlayers :customRoomProperties :nil];
}

- (bool) opCreateRoom:(NSString*)gameID :(bool)isVisible :(bool)isOpen :(nByte)maxPlayers :(NSDictionary*)customRoomProperties :(EGArray*)propsListedInLobby
{
	if(self.IsInGameRoom)
	{
		EGLOG(ERRORS, L"already in a gameroom");
		return false;
	}
	if(mRoomName != gameID)
	{
		[mRoomName release];
		mRoomName = [gameID retain];
	}

	NSMutableDictionary* op = [self opCreateRoomImplementation:gameID :isVisible :isOpen :maxPlayers :self.IsOnGameServer?customRoomProperties:nil :self.IsOnGameServer?self.LocalPlayer.CustomProperties:nil :self.IsOnGameServer?propsListedInLobby:nil];
	if(self.LocalPlayer.Name)
	{
		if([op objectForKey:[NSValue value:&ParameterCode::PLAYER_PROPERTIES withObjCType:@encode(nByte)]])
			[[op objectForKey:[NSValue value:&ParameterCode::PLAYER_PROPERTIES withObjCType:@encode(nByte)]] setObject:self.LocalPlayer.Name forKey:[NSValue value:&Properties::Player::PLAYERNAME withObjCType:@encode(nByte)]];
		else
			[op setObject:[NSDictionary dictionaryWithObject:self.LocalPlayer.Name forKey:[NSValue value:&Properties::Player::PLAYERNAME withObjCType:@encode(nByte)]] forKey:[NSValue value:&ParameterCode::PLAYER_PROPERTIES withObjCType:@encode(nByte)]];
	}
	if(![self opCustom:[EGOperationRequest operationRequestWithOperationCode:OperationCode::CREATE_ROOM :op] :true])
		return false;

	NSMutableDictionary* roomProps = [EGLoadBalancingPeer stripToCustomProperties:customRoomProperties];
	[roomProps setObject:[NSValue value:&isOpen withObjCType:@encode(bool)] forKey:[NSValue value:&Properties::Room::IS_OPEN withObjCType:@encode(nByte)]];
	[roomProps setObject:[NSValue value:&isVisible withObjCType:@encode(bool)] forKey:[NSValue value:&Properties::Room::IS_VISIBLE withObjCType:@encode(nByte)]];
	[roomProps setObject:[NSValue value:&maxPlayers withObjCType:@encode(nByte)] forKey:[NSValue value:&Properties::Room::MAX_PLAYERS withObjCType:@encode(nByte)]];
	[roomProps setObject:[propsListedInLobby.Type isEqual:NSStringFromClass([NSString class])]?[[propsListedInLobby copyDeep] autorelease]:[EGArray arrayWithType:NSStringFromClass([NSString class])] forKey:[NSValue value:&Properties::Room::PROPS_LISTED_IN_LOBBY withObjCType:@encode(nByte)]];

	[mCurrentlyJoinedRoom release];
	mCurrentlyJoinedRoom = [[self createMutableRoom:gameID :roomProps :propsListedInLobby] retain];
	return true;
}

- (bool) opJoinRoom:(NSString*)gameID
{
	if(self.IsInGameRoom)
	{
		EGLOG(ERRORS, L"already in a gameroom");
		return false;
	}
	if(mRoomName != gameID)
	{
		[mRoomName release];
		mRoomName = [gameID retain];
	}
	NSMutableDictionary* op = [self opJoinRoomImplementation:gameID :self.IsOnGameServer?self.LocalPlayer.CustomProperties:nil];
	if(self.LocalPlayer.Name)
	{
		if([op objectForKey:[NSValue value:&ParameterCode::PLAYER_PROPERTIES withObjCType:@encode(nByte)]])
			[[op objectForKey:[NSValue value:&ParameterCode::PLAYER_PROPERTIES withObjCType:@encode(nByte)]] setObject:self.LocalPlayer.Name forKey:[NSValue value:&Properties::Player::PLAYERNAME withObjCType:@encode(nByte)]];
		else
			[op setObject:[NSDictionary dictionaryWithObject:self.LocalPlayer.Name forKey:[NSValue value:&Properties::Player::PLAYERNAME withObjCType:@encode(nByte)]] forKey:[NSValue value:&ParameterCode::PLAYER_PROPERTIES withObjCType:@encode(nByte)]];
	}
	if(!gameID || ![super opCustom:[EGOperationRequest operationRequestWithOperationCode:OperationCode::JOIN_ROOM :op] :true])
		return false;
	[mCurrentlyJoinedRoom release];
	mCurrentlyJoinedRoom = [[self createMutableRoom:gameID :nil :nil] retain];
	return true;
}

- (bool) opJoinRandomRoom:(NSDictionary*)customRoomProperties :(nByte)maxPlayers
{
	if(self.IsInGameRoom)
	{
		EGLOG(ERRORS, L"already in a gameroom");
		return false;
	}
	if(![super opJoinRandomRoom:customRoomProperties :maxPlayers])
		return false;

	[mCurrentlyJoinedRoom release];
	mCurrentlyJoinedRoom = [[self createMutableRoom:@"" :[EGLoadBalancingPeer stripToCustomProperties:customRoomProperties] :nil] retain];
	return true;
}


- (bool) opLeaveRoom
{
	if(!self.IsInGameRoom)
	{
		EGLOG(ERRORS, L"no gameroom is currently joined");
		return false;
	}
	if(![super opLeaveRoom])
		return false;
	mState = PeerStates::Leaving;
	return true;
}

- (bool) opAuthenticate:(NSString*)appID :(NSString*)appVersion :(bool)encrypted
{
	EG_ASSERT(false, @"opAuthenticate() is not part of the public interface of class LoadBalancingClient!");
	return false;
}

- (bool) opAuthenticate:(NSString*)appID :(NSString*)appVersion :(bool)encrypted :(NSString*)userID
{
	EG_ASSERT(false, @"opAuthenticate() is not part of the public interface of class LoadBalancingClient!");
	return false;
}
			  
- (bool) opAuthenticate:(NSString*)appID :(NSString*)appVersion :(bool)encrypted :(NSString*)userID :(EGAuthenticationValues*)authenticationValues
{
	EG_ASSERT(false, @"opAuthenticate() is not part of the public interface of class LoadBalancingClient!");
	return false;
}

- (bool) opFindFriends:(NSString**)friendsToFind :(short)numFriendsToFind
{
	if(self.IsOnGameServer || mIsFetchingFriendList)
		return false;
	[mFriendList release];
	EGMutableArray* friendList = [EGMutableArray arrayWithType:NSStringFromClass([EGFriendInfo class])];
	for(unsigned int i=0; i<numFriendsToFind; ++i)
		[friendList addObject:[EGFriendInfo friendInfo:friendsToFind[i]]];
	mFriendList = [[EGMutableArray alloc] initWithArray:friendList];
	return mIsFetchingFriendList = [super opFindFriends:friendsToFind :numFriendsToFind];
}



// inherited

- (void) dealloc
{
	[mFriendList release];
	[mRoomList release];
	[mRoomName release];
	[mGameserver release];
	[mAuthenticationValues release];
	[mAppID release];
	[mLocalPlayer release];
	[mCurrentlyJoinedRoom release];
	[mAppVersion release];
	[super dealloc];
}



// protocol implementations

- (void) onOperationResponse:(EGOperationResponse*)operationResponse
{
	EGLOG(operationResponse.ReturnCode?ERRORS:INFO, [operationResponse toString:true :true].UTF32String);
	switch(operationResponse.OperationCode)
	{
		case OperationCode::AUTHENTICATE:
		{
			PeerStates::PeerStates oldState = mState;
			if(operationResponse.ReturnCode)
			{
				EGLOG(ERRORS, L"authentication failed with errorcode %d: %ls", operationResponse.ReturnCode, operationResponse.DebugMessage.UTF32String);
				[self handleConnectionFlowError:oldState :operationResponse.ReturnCode :operationResponse.DebugMessage];
				break;
			}
			else
			{
				if(mState == PeerStates::Connected || mState == PeerStates::ConnectedComingFromGameserver)
				{
					mState = mState==PeerStates::Connected?PeerStates::Authenticated:PeerStates::AuthenticatedComingFromGameserver;
					if([operationResponse objectForKey:[NSValue value:&ParameterCode::SECRET withObjCType:@encode(nByte)]] && mAuthenticationValues)
					{
						mAuthenticationValues.Secret = [operationResponse objectForKey:[NSValue value:&ParameterCode::SECRET withObjCType:@encode(nByte)]];
						EGLOG(INFO, L"Server returned secret: %ls.", mAuthenticationValues.Secret.UTF32String);
					}
					if(mAutoJoinLobby)
					{
						[self opJoinLobby];
						mLastJoinType = JoinType::AUTO_JOIN_LOBBY;
					}
					else
						[self onConnectToMasterFinished:oldState==PeerStates::ConnectedComingFromGameserver];
					
				}
				else if(mState == PeerStates::ConnectedToGameserver)
				{
					mAuthenticationValues.Secret = nil;
					mState = PeerStates::Joining;
					if(mLastJoinType == JoinType::CREATE_ROOM)
						[self opCreateRoom:mRoomName :mCurrentlyJoinedRoom.IsVisible :mCurrentlyJoinedRoom.IsOpen :mCurrentlyJoinedRoom.MaxPlayers :mCurrentlyJoinedRoom.CustomProperties :mCurrentlyJoinedRoom.PropsListedInLobby];
					else
						[self opJoinRoom:mRoomName];
				}
			}
		}
			break;
		case OperationCode::CREATE_ROOM:
		case OperationCode::JOIN_ROOM:
			if(self.IsOnGameServer)
			{
				if(operationResponse.ReturnCode)
				{
					EGLOG(ERRORS, L"%ls failed with errorcode %d: %ls. Client is therefore returning to masterserver!", operationResponse.OperationCode==OperationCode::CREATE_ROOM?L"opCreateRoom":L"opJoinRoom", operationResponse.ReturnCode, operationResponse.DebugMessage);
					[self handleConnectionFlowError:mState :operationResponse.ReturnCode :operationResponse.DebugMessage];
					break;
				}
				mState = PeerStates::Joined;
				int nr;
				[[operationResponse objectForKey:[NSValue value:&ParameterCode::PLAYERNR withObjCType:@encode(nByte)]] getValue:&nr];
				NSMutableDictionary* dic = [NSMutableDictionary dictionaryWithDictionary:self.LocalPlayer.CustomProperties];
				[dic setObject:self.LocalPlayer.Name forKey:[NSValue value:&Properties::Player::PLAYERNAME withObjCType:@encode(nByte)]];
				mCurrentlyJoinedRoom.Players = [EGMutableArray arrayWithType:NSStringFromClass([EGLoadBalancingPlayer class])];
				[mCurrentlyJoinedRoom addPlayer:[self setLocalPlayer:nr :dic]];
				NSMutableDictionary* roomProperties = [NSMutableDictionary dictionaryWithDictionary:[operationResponse objectForKey:[NSValue value:&ParameterCode::ROOM_PROPERTIES withObjCType:@encode(nByte)]]];
				NSMutableDictionary* playerProperties = [NSMutableDictionary dictionaryWithDictionary:[operationResponse objectForKey:[NSValue value:&ParameterCode::PLAYER_PROPERTIES withObjCType:@encode(nByte)]]];
				NSValue** numbers = (id*)EG_MALLOC(playerProperties.count*sizeof(NSValue*));
				NSDictionary** vals = (NSDictionary**)EG_MALLOC(playerProperties.count*sizeof(NSString*));
				[playerProperties getObjects:vals andKeys:numbers];
				for(int i=0; i<playerProperties.count; i++)
				{
					[numbers[i] getValue:&nr];
					[mCurrentlyJoinedRoom addPlayer:nr :vals[i]];
				}
				EG_FREE(numbers);
				EG_FREE(vals);
				[self readoutProperties:&roomProperties :&playerProperties :true :0];
				switch(mLastJoinType)
				{
					case JoinType::CREATE_ROOM:
						[mListener createRoomReturn:nr :roomProperties :playerProperties :operationResponse.ReturnCode :operationResponse.DebugMessage];
						break;
					case JoinType::JOIN_ROOM:
						[mListener joinRoomReturn:nr :roomProperties :playerProperties :operationResponse.ReturnCode :operationResponse.DebugMessage];
						break;
					case JoinType::JOIN_RANDOM_ROOM:
						[mListener joinRandomRoomReturn:nr :roomProperties :playerProperties :operationResponse.ReturnCode :operationResponse.DebugMessage];
						break;
					default:
						break;
				}
				break;
			}
			else
			{
				switch(operationResponse.OperationCode)
				{
					case OperationCode::CREATE_ROOM:
					{
						if(operationResponse.ReturnCode)
						{
							EGLOG(ERRORS, L"opCreateRoom failed with errorcode %d: %ls. Client is therefore staying on masterserver!", operationResponse.ReturnCode, operationResponse.DebugMessage.UTF32String);
							[mListener createRoomReturn:0 :nil :nil :operationResponse.ReturnCode :operationResponse.DebugMessage];
							break;
						}
						NSString* gameID = [operationResponse objectForKey:[NSValue value:&ParameterCode::ROOM_NAME withObjCType:@encode(nByte)]];
						if(gameID) // is only sent by the server's response, if it has not been sent with the client's request before!
						{
							[mRoomName release];
							mRoomName = [gameID retain];
						}
						[mGameserver release];
						mGameserver = [[operationResponse objectForKey:[NSValue value:&ParameterCode::ADDRESS withObjCType:@encode(nByte)]] retain];
						mState = PeerStates::DisconnectingFromMasterserver;
						[super disconnect];
						mLastJoinType = JoinType::CREATE_ROOM;
					}
						break;
					case OperationCode::JOIN_ROOM:
						if(operationResponse.ReturnCode)
						{
							EGLOG(ERRORS, L"opJoinRoom failed with errorcode %d: %ls. Client is therefore staying on masterserver!", operationResponse.ReturnCode, operationResponse.DebugMessage.UTF32String);
							[mListener joinRoomReturn:0 :nil :nil :operationResponse.ReturnCode :operationResponse.DebugMessage];
							break;
						}
						[mGameserver release];
						mGameserver = [[operationResponse objectForKey:[NSValue value:&ParameterCode::ADDRESS withObjCType:@encode(nByte)]] retain];
						mState = PeerStates::DisconnectingFromMasterserver;
						[super disconnect];
						mLastJoinType = JoinType::JOIN_ROOM;
						break;
					default:
						break;
				}
			}
			break;
		case OperationCode::JOIN_RANDOM_ROOM:
			if(operationResponse.ReturnCode)
			{
				EGLOG(ERRORS, L"opJoinRandomRoom failed with errorcode %d: %ls. Client is therefore staying on masterserver!", operationResponse.ReturnCode, operationResponse.DebugMessage.UTF32String);
				[mListener joinRandomRoomReturn:0 :nil :nil :operationResponse.ReturnCode :operationResponse.DebugMessage];
				break;
			}
			// store the ID of the random game, joined on the masterserver, so that we know, which game to join on the gameserver
			[mRoomName release];
			mRoomName = [[operationResponse objectForKey:[NSValue value:&ParameterCode::ROOM_NAME withObjCType:@encode(nByte)]] retain];
			[mGameserver release];
			mGameserver = [[operationResponse objectForKey:[NSValue value:&ParameterCode::ADDRESS withObjCType:@encode(nByte)]] retain];
			mState = PeerStates::DisconnectingFromMasterserver;
			[super disconnect];
			mLastJoinType = JoinType::JOIN_RANDOM_ROOM;
			break;
		case OperationCode::JOIN_LOBBY:
			if(mLastJoinType == JoinType::AUTO_JOIN_LOBBY)
				[self onConnectToMasterFinished:mState==PeerStates::AuthenticatedComingFromGameserver];
			else
				[mListener joinLobbyReturn];
			mState = PeerStates::JoinedLobby;
			break;
		case OperationCode::LEAVE_LOBBY:
			mState = PeerStates::Authenticated;
			[mListener leaveLobbyReturn];
			break;
		case OperationCode::LEAVE:
		{
			NSMutableDictionary* dic = [NSMutableDictionary dictionaryWithDictionary:self.LocalPlayer.CustomProperties];
			[dic setObject:self.LocalPlayer.Name forKey:[NSValue value:&Properties::Player::PLAYERNAME withObjCType:@encode(nByte)]];
			[self setLocalPlayer:-1 :dic];
			[mCurrentlyJoinedRoom removeAllPlayers];
			mState = PeerStates::DisconnectingFromGameserver;
			[super disconnect];
		}
			break;
		case OperationCode::FIND_FRIENDS:
		{
			mIsFetchingFriendList = false;
			if(operationResponse.ReturnCode)
			{
				EGLOG(ERRORS, L"opFindFriends failed with errorcode %d: %ls.", operationResponse.ReturnCode, operationResponse.DebugMessage.UTF32String);
				break;
			}
			bool* pOnlineList = static_cast<bool*>(const_cast<void*>([[operationResponse objectForKey:[NSValue value:&ParameterCode::FIND_FRIENDS_RESPONSE_ONLINE_LIST withObjCType:@encode(nByte)]] CArray]));
			NSString** pRoomList = static_cast<NSString**>(const_cast<void*>([[operationResponse objectForKey:[NSValue value:&ParameterCode::FIND_FRIENDS_RESPONSE_ROOM_ID_LIST withObjCType:@encode(nByte)]] CArray]));
			for(unsigned int i=0; i<mFriendList.count; ++i)
				[mFriendList replaceObjectAtIndex:i withObject:[EGFriendInfo friendInfo:[[mFriendList objectAtIndex:i] Name] :pOnlineList[i] :pRoomList[i]]];
			mFriendListTimestamp = GETTIMEMS()?:1;
			[mListener onFindFriendsResponse];
		}
			break;
		default:
			break;
	}
}

- (void) onStatusChanged:(int)statusCode
{
	switch(statusCode)
	{
		case 0:
			break;
		case StatusCode::CONNECT:
		{
			if(mState == PeerStates::ConnectingToGameserver)
			{
				EGLOG(INFO, L"connected to gameserver");
				mState = PeerStates::ConnectedToGameserver;
			}
			else
			{
				EGLOG(INFO, L"connected to masterserver");
				mState = mState==PeerStates::Connecting?PeerStates::Connected:PeerStates::ConnectedComingFromGameserver;
			}
			if(SEND_AUTHENTICATE_ENCRYPTED)
				[super establishEncryption];
			else
				[super opAuthenticate:mAppID :mAppVersion :false :self.LocalPlayer.Name :mAuthenticationValues];
		}
			break;
		case StatusCode::DISCONNECT:
		{
			if(mState == PeerStates::DisconnectingFromMasterserver)
			{
				[super connect:mGameserver];
				mState = PeerStates::ConnectingToGameserver;
			}
			else if(mState == PeerStates::DisconnectingFromGameserver)
			{
				[super connect:(NSString*)mMasterserver];
				mState = PeerStates::ConnectingToMasterserver;
			}
			else
			{
				mState = PeerStates::PeerCreated;
				[mListener disconnectReturn];
			}
		}
			break;
		case StatusCode::ENCRYPTION_ESTABLISHED:
			[super opAuthenticate:mAppID :mAppVersion :true :self.LocalPlayer.Name :mAuthenticationValues];
			break;
		case StatusCode::ENCRYPTION_FAILED_TO_ESTABLISH:
			[self handleConnectionFlowError:mState :statusCode :@"Encryption failed to establish"];
			break;
		case StatusCode::EXCEPTION:
		case StatusCode::EXCEPTION_ON_CONNECT:
		case StatusCode::INTERNAL_RECEIVE_EXCEPTION:
		case StatusCode::TIMEOUT_DISCONNECT:
		case StatusCode::DISCONNECT_BY_SERVER:
		case StatusCode::DISCONNECT_BY_SERVER_USER_LIMIT:
		case StatusCode::DISCONNECT_BY_SERVER_LOGIC:
			[mListener connectionErrorReturn:statusCode];
			if(self.Peerstate != PeerState::DISCONNECTED && self.Peerstate != PeerState::DISCONNECTING)
				[self disconnect];
			break;
		case StatusCode::SEND_ERROR:
			[mListener clientErrorReturn:statusCode];
			break;
		case StatusCode::QUEUE_OUTGOING_RELIABLE_WARNING:
		case StatusCode::QUEUE_OUTGOING_UNRELIABLE_WARNING:
		case StatusCode::QUEUE_OUTGOING_ACKS_WARNING:
		case StatusCode::QUEUE_SENT_WARNING:
			[mListener warningReturn:statusCode];
			break;
		case LoadBalancing::ErrorCode::OPERATION_INVALID:
		case LoadBalancing::ErrorCode::INTERNAL_SERVER_ERROR:
			[mListener serverErrorReturn:statusCode];
			break;
		default:
			EGLOG(ERRORS, L"received unknown status-code from server");
			break;
	}
}

- (void) onEvent:(EGEventData*)eventData
{
	EGLOG(INFO, [eventData toString].UTF32String); // don't print out the payload here, as that can get too expensive for big events
	switch(eventData.Code)
	{
		case EventCode::ROOM_LIST:
		{
			[mRoomList release];
			NSDictionary* roomList = [eventData objectForKey:[NSValue value:&ParameterCode::ROOM_LIST withObjCType:@encode(nByte)]];
			NSString** keys = new NSString*[roomList.count];
			NSDictionary** vals = new NSDictionary*[roomList.count];
			[roomList getObjects:vals andKeys:keys];
			mRoomList = [[NSMutableDictionary alloc] initWithCapacity:roomList.count];
			for(int i=0; i<roomList.count; i++)
				[mRoomList setObject:[self createRoom:keys[i] :vals[i]] forKey:keys[i]];
			delete[] keys;
			delete[] vals;
			if([mListener respondsToSelector:@selector(onRoomListUpdate)])
				[mListener onRoomListUpdate];
		}
			break;
		case EventCode::ROOM_LIST_UPDATE:
		{
			NSDictionary* roomListUpdate = [eventData objectForKey:[NSValue value:&ParameterCode::ROOM_LIST withObjCType:@encode(nByte)]];
			EGArray* keys = [EGArray arrayWithArray:roomListUpdate.allKeys];
			for(int i=0; i<keys.count; i++)
			{
				NSDictionary* val = [roomListUpdate objectForKey:[keys objectAtIndex:i]];
				bool removed = false;
				[(NSValue*)[val objectForKey:[NSValue value:&Properties::Room::REMOVED withObjCType:@encode(nByte)]] getValue:&removed];
				if(removed) // remove room
					[mRoomList removeObjectForKey:[keys objectAtIndex:i]];
				else
				{
					EGLoadBalancingRoom* room = [mRoomList objectForKey:[keys objectAtIndex:i]];
					if(room) // update room (only entries, which have been changed, have been sent)
						[room cacheProperties:val];
					else // add room
						[mRoomList setObject:[self createRoom:[keys objectAtIndex:i] :val] forKey:[keys objectAtIndex:i]];
				}
			}
			if([mListener respondsToSelector:@selector(onRoomListUpdate)])
				[mListener onRoomListUpdate];
		}
			break;
		case EventCode::APP_STATS:
			[[eventData objectForKey:[NSValue value:&ParameterCode::PEER_COUNT withObjCType:@encode(nByte)]] getValue:&mPeerCount];
			[[eventData objectForKey:[NSValue value:&ParameterCode::ROOM_COUNT withObjCType:@encode(nByte)]] getValue:&mRoomCount];
			[[eventData objectForKey:[NSValue value:&ParameterCode::MASTER_PEER_COUNT withObjCType:@encode(nByte)]] getValue:&mMasterPeerCount];
			if([mListener respondsToSelector:@selector(onAppStatsUpdate)])
				[mListener onAppStatsUpdate];
			break;
		case EventCode::JOIN:
		{
			EGLOG(INFO, L"%ls", [eventData toString:true :true].UTF32String);
			int nr = -1;
			[(NSValue*)[eventData objectForKey:[NSValue value:&ParameterCode::PLAYERNR withObjCType:@encode(nByte)]] getValue:&nr];
			EGLoadBalancingPlayer* player = [mCurrentlyJoinedRoom createPlayer:nr :[eventData objectForKey:[NSValue value:&ParameterCode::PLAYER_PROPERTIES withObjCType:@encode(nByte)]]];
			if(nr != self.LocalPlayer.Number)
				[mCurrentlyJoinedRoom addPlayer:player];
			[mListener joinRoomEventAction:nr :[eventData objectForKey:[NSValue value:&ParameterCode::PLAYER_LIST withObjCType:@encode(nByte)]] :player];
		}
			break;
		case EventCode::LEAVE:
		{
			int nr = -1;
			[(NSValue*)[eventData objectForKey:[NSValue value:&ParameterCode::PLAYERNR withObjCType:@encode(nByte)]] getValue:&nr];
			if(![mCurrentlyJoinedRoom removePlayer:nr])
				EGLOG(WARNINGS, L"EventCode::LEAVE - player %d, who is leaving the room, has not been found in list of players, who are currently in the room", nr);
			[mListener leaveRoomEventAction:nr];
		}
			break;
		case EventCode::PROPERTIES_CHANGED:
		{
			int target = 0;
			[(NSValue*)[eventData objectForKey:[NSValue value:&ParameterCode::TARGET_PLAYERNR withObjCType:@encode(nByte)]] getValue:&target];
			NSMutableDictionary* roomProperties = nil;
			NSMutableDictionary* playerProperties = nil;
			if(target)
				playerProperties = [eventData objectForKey:[NSValue value:&ParameterCode::PROPERTIES withObjCType:@encode(nByte)]];
			else
				roomProperties = [eventData objectForKey:[NSValue value:&ParameterCode::PROPERTIES withObjCType:@encode(nByte)]];
			[self readoutProperties:&roomProperties :&playerProperties :false :target];
			if(playerProperties && [mListener respondsToSelector:@selector(onPlayerPropertiesChange:)])
				[mListener onPlayerPropertiesChange:target :playerProperties];
			else if([mListener respondsToSelector:@selector(onRoomPropertiesChange:)])
				[mListener onRoomPropertiesChange:roomProperties];
		}
			break;
		default: // custom events are forwarded to the custom room code
		{
			int nr = 0;
			[(NSValue*)[eventData objectForKey:[NSValue value:&ParameterCode::PLAYERNR withObjCType:@encode(nByte)]] getValue:&nr];
			// custom event data is inside an inner hash
			[mListener customEventAction:nr :eventData.Code :[eventData objectForKey:[NSValue value:&ParameterCode::DATA withObjCType:@encode(nByte)]]];
		}
			break;
	}
}

- (void) debugReturn:(DebugLevel)debugLevel :(NSString*)string
{
	[mListener debugReturn:debugLevel :string];
}



// protecteds

- (EGLoadBalancingMutablePlayer*) createMutablePlayer:(int)number :(NSDictionary*)properties
{
	return [EGLoadBalancingMutablePlayer loadBalancingPlayer:number :properties :self.CurrentlyJoinedRoom :self];
}

- (id) createRoom:(NSString*)name :(NSDictionary*)properties
{
	return [EGLoadBalancingRoom loadBalancingRoom:name :properties];
}

- (id) createMutableRoom:(NSString*)name :(NSDictionary*)properties :(EGArray*)propsListedInLobby
{
	return [EGLoadBalancingMutableRoom loadBalancingRoom:name :properties :self :propsListedInLobby];
}



// internals

- (bool) opSetPropertiesOfPlayer:(int)playerNr :(NSDictionary*)properties
{
	if(!self.IsInGameRoom)
		return false;
	return [super opSetPropertiesOfPlayer:(int)playerNr :(NSDictionary*)properties];
}

- (bool) opSetPropertiesOfRoom:(NSDictionary*)properties
{
	if(!self.IsInGameRoom)
		return false;
	return [super opSetPropertiesOfRoom:(NSDictionary*)properties];
}



// privates

- (bool) IsOnGameServer
{
	return mState >= PeerStates::ConnectingToGameserver && mState < PeerStates::ConnectingToMasterserver;
}

- (EGLoadBalancingMutablePlayer*) setLocalPlayer:(EGLoadBalancingMutablePlayer*)localPlayer
{
	if(mLocalPlayer == localPlayer)
		return mLocalPlayer;
	[mLocalPlayer release];
	return mLocalPlayer = [localPlayer retain];
}

- (EGLoadBalancingMutablePlayer*) setLocalPlayer:(int)number :(NSDictionary*)properties
{
	return self.LocalPlayer = [self createMutablePlayer:number :properties];
}

- (void) readoutProperties:(NSMutableDictionary**)pRoomProperties :(NSMutableDictionary**)pPlayerProperties :(bool)multiplePlayers :(int)targetPlayerNr
{
	if(pRoomProperties && *pRoomProperties)
	{
		[mCurrentlyJoinedRoom cacheProperties:*pRoomProperties];
		*pRoomProperties = [EGLoadBalancingPeer stripKeysWithNullValues:[EGLoadBalancingPeer stripToCustomProperties:*pRoomProperties]];
	}

	if(pPlayerProperties && *pPlayerProperties && (*pPlayerProperties).count)
	{
		EGArray* players = multiplePlayers?[EGArray arrayWithArray:[*pPlayerProperties allValues]]:nil;
		EGArray* playerNrs = multiplePlayers?[EGArray arrayWithArray:[*pPlayerProperties allKeys]]:nil;
		for(int i=0; i<(multiplePlayers?(*pPlayerProperties).count:1); ++i)
		{
			NSDictionary* props = multiplePlayers?[players objectAtIndex:i]:*pPlayerProperties;
			if([props objectForKey:[NSValue value:&Properties::Player::PLAYERNAME withObjCType:@encode(nByte)]])
			{
				for(int j=0; j<mCurrentlyJoinedRoom.Players.count; ++j)
				{
					int tmp;
					if(multiplePlayers)
						[[playerNrs objectAtIndex:i] getValue:&tmp];
					else
						tmp = targetPlayerNr;
					if(((EGLoadBalancingPlayer*)[mCurrentlyJoinedRoom.Players objectAtIndex:j]).Number == tmp)
					{
						[(EGLoadBalancingPlayer*)[mCurrentlyJoinedRoom.Players objectAtIndex:j] cacheProperties:props];
						if(self.LocalPlayer.Number == ((EGLoadBalancingPlayer*)[mCurrentlyJoinedRoom.Players objectAtIndex:j]).Number)
							mLocalPlayer = [mCurrentlyJoinedRoom.Players objectAtIndex:j];
					}
				}
			}
		}

		if(multiplePlayers)
		{
			for(NSUInteger i=0; i<(*pPlayerProperties).count; ++i)
			{
				id key = [[*pPlayerProperties allKeys] objectAtIndex:i];
				[*pPlayerProperties setObject:[EGLoadBalancingPeer stripKeysWithNullValues:[EGLoadBalancingPeer stripToCustomProperties:[*pPlayerProperties objectForKey:key]]] forKey:key];
			}
		}
		else
			*pPlayerProperties = [EGLoadBalancingPeer stripKeysWithNullValues:[EGLoadBalancingPeer stripToCustomProperties:*pPlayerProperties]];
	}
}

- (void) handleConnectionFlowError:(PeerStates::PeerStates)oldState :(int)errorCode :(const NSString* const)errorString
{
	if(oldState == PeerStates::ConnectedToGameserver || oldState == PeerStates::AuthenticatedOnGameServer || oldState == PeerStates::Joining)
	{
		mCachedErrorCodeFromGameServer = errorCode;
		[mCachedErrorStringFromGameServer release];
		mCachedErrorStringFromGameServer = [errorString retain];
		mState = PeerStates::DisconnectingFromGameserver;
		[super disconnect];
		// response to app has to wait until back on master
	}
	else
	{
		mState = PeerStates::Disconnecting;
		[super disconnect];
		[mListener connectReturn:errorCode :errorString];
	}
}

- (void) onConnectToMasterFinished:(bool)comingFromGameserver
{
	if(comingFromGameserver)
	{
		if(mCachedErrorCodeFromGameServer)
		{
			switch(mLastJoinType)
			{
				case JoinType::CREATE_ROOM:
					[mListener createRoomReturn:0 :nil :nil :mCachedErrorCodeFromGameServer :mCachedErrorStringFromGameServer];
					break;
				case JoinType::JOIN_ROOM:
					[mListener joinRoomReturn:0 :nil :nil :mCachedErrorCodeFromGameServer :mCachedErrorStringFromGameServer];
					break;
				case JoinType::JOIN_RANDOM_ROOM:
					[mListener joinRandomRoomReturn:0 :nil :nil :mCachedErrorCodeFromGameServer :mCachedErrorStringFromGameServer];
					break;
				default:
					break;
			}
			mCachedErrorCodeFromGameServer = LoadBalancing::ErrorCode::OK;
			[mCachedErrorStringFromGameServer release];
			mCachedErrorStringFromGameServer = nil;
		}
		else
			[mListener leaveRoomReturn:0 :nil];
	}
	else
		[mListener connectReturn:0 :nil];
}

@end