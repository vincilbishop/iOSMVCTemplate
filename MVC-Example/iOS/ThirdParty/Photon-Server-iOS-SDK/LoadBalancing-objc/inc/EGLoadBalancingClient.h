/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "LoadBalancing-cpp/inc/Enums/PeerStates.h"
#import "LoadBalancing-objc/inc/EGFriendInfo.h"
#import "LoadBalancing-objc/inc/EGLoadBalancingListener.h"
#import "LoadBalancing-objc/inc/EGLoadBalancingRoom.h"

@interface EGLoadBalancingClient : EGLoadBalancingPeer <EGPhotonListener>
{
@private
	id<EGLoadBalancingListener> mListener;
	NSString* mGameserver;
	NSString* mAppVersion;
	NSString* mRoomName;
	NSString* mAppID;
	int mPeerCount;
	int mRoomCount;
	int mMasterPeerCount;
	nByte mLastJoinType;
	EGLoadBalancingMutableRoom* mCurrentlyJoinedRoom;
	NSMutableDictionary* mRoomList;
	int mCachedErrorCodeFromGameServer;
	const NSString* mCachedErrorStringFromGameServer;
	bool mUseTcp;
	NSString* mMasterserver;
	bool mAutoJoinLobby;
	EGLoadBalancingMutablePlayer* mLocalPlayer;
	EGMutableArray* mFriendList;
	int mFriendListTimestamp;
	bool mIsFetchingFriendList;
	enum ExitGames::LoadBalancing::PeerStates::PeerStates mState;
	EGAuthenticationValues* mAuthenticationValues;
}

@property (readonly) ExitGames::LoadBalancing::PeerStates::PeerStates State;
@property (readonly) NSString* MasterserverAddress;
@property (readonly) int CountPlayersIngame;
@property (readonly) int CountGamesRunning;
@property (readonly) int CountPlayersOnline;
@property (readonly) EGLoadBalancingMutableRoom* CurrentlyJoinedRoom;
@property (readonly) NSDictionary* RoomList;
@property (readonly) bool IsInRoom;
@property (readonly) bool IsInGameRoom;
@property (readonly) bool IsInLobby;
@property (readwrite) bool AutoJoinLobby;
@property (readonly) EGLoadBalancingMutablePlayer* LocalPlayer;
@property (readonly) EGArray* FriendList;
@property (readonly) int FriendListAge;

- (instancetype) initClient:(id<EGLoadBalancingListener>)listener :(NSString*)applicationID :(NSString*)appVersion :(NSString*)username;
- (instancetype) initClient:(id<EGLoadBalancingListener>)listener :(NSString*)applicationID :(NSString*)appVersion :(NSString*)username :(bool)useTcp;
- (instancetype) initClient:(id<EGLoadBalancingListener>)listener :(NSString*)applicationID :(NSString*)appVersion :(NSString*)username :(bool)useTcp :(EGAuthenticationValues*)authenticationValues;
- (bool) connect;
- (bool) opCreateRoom:(NSString*)gameID :(bool)isVisible :(bool)isOpen :(nByte)maxPlayers :(NSDictionary*)customRoomProperties :(EGArray*)propsListedInLobby;

@end