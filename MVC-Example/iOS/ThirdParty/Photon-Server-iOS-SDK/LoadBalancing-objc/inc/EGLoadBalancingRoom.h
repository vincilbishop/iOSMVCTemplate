/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#import "LoadBalancing-objc/inc/EGLoadBalancingPlayer.h"

@interface EGLoadBalancingRoom : EGBase
{
@protected
	NSString* mName;
	nByte mPlayerCount;
	nByte mMaxPlayers;
	bool mIsOpen;
	NSMutableDictionary* mCustomProperties;
}

@property (readonly, retain) NSString* Name;
@property (readonly) nByte PlayerCount;
@property (readonly) nByte MaxPlayers;
@property (readonly) bool IsOpen;
@property (readonly, copy) NSDictionary* CustomProperties;

- (NSString*) toString:(bool)withTypes :(bool)withCustomProperties;

@end



@interface EGLoadBalancingMutableRoom : EGLoadBalancingRoom
{
@private
	EGLoadBalancingPeer* mLoadBalancingPeer;
	bool mIsVisible;
	EGMutableArray* mPlayers;
	int mMasterClientID;
	EGMutableArray* mPropsListedInLobby;
}

@property (readwrite) nByte MaxPlayers;
@property (readwrite) bool IsOpen;
@property (readwrite) bool IsVisible;
@property (readonly, copy) EGArray* Players;
@property (readonly) int MasterClientID;
@property (readwrite, copy) EGArray* PropsListedInLobby;

- (EGLoadBalancingPlayer*) getPlayerForNumber:(int)playerNumber;
- (void) mergeCustomProperties:(NSDictionary*)customProperties;
- (void) addCustomProperty:(NSString*)key :(id)value;
- (void) addCustomProperties:(NSDictionary*)customProperties;
- (void) removeCustomProperty:(NSString*)key;
- (void) removeCustomProperties:(NSString**)keys :(NSUInteger)count;
- (NSString*) toString:(bool)withTypes :(bool)withCustomProperties :(bool)withPlayers;

@end