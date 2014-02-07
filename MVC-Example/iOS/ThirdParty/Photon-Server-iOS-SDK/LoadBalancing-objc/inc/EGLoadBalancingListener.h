/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#import <Foundation/Foundation.h>
#include "Common-cpp/inc/Enums/DebugLevel.h"
#import "Common-objc/inc/EGBaseListener.h"

@class EGLoadBalancingPlayer;
@class EGArray;

@protocol EGLoadBalancingListener <NSObject, EGBaseListener>

// implement your error-handling here
- (void) connectionErrorReturn:(int)errorCode;
- (void) clientErrorReturn:(int)errorCode;
- (void) warningReturn:(int)warningCode;
- (void) serverErrorReturn:(int)errorCode;

// events, triggered by certain operations of all players in the same room
- (void) joinRoomEventAction:(int)playerNr :(const EGArray* const)playernrs :(const EGLoadBalancingPlayer* const)player;
- (void) leaveRoomEventAction:(int)playerNr;
- (void) customEventAction:(int)playerNr :(nByte)eventCode :(const NSObject* const)eventContent;

// callbacks for operations on server
- (void) connectReturn:(int)errorCode :(const NSString* const)errorString;
- (void) disconnectReturn;
- (void) createRoomReturn:(int)localPlayerNr :(const NSDictionary* const)roomProperties :(const NSDictionary* const)playerProperties :(int)errorCode :(const NSString* const)errorString;
- (void) joinRoomReturn:(int)localPlayerNr :(const NSDictionary* const)roomProperties :(const NSDictionary* const)playerProperties :(int)errorCode :(const NSString* const)errorString;
- (void) joinRandomRoomReturn:(int)localPlayerNr :(const NSDictionary* const)roomProperties :(const NSDictionary* const)playerProperties :(int)errorCode :(const NSString* const)errorString;
- (void) leaveRoomReturn:(int)errorCode :(const NSString* const)errorString;
- (void) joinLobbyReturn;
- (void) leaveLobbyReturn;



@optional

// callbacks for operations on server
- (void) onFindFriendsResponse;

// info, that the values of certain properties have been updated
- (void) onRoomListUpdate;
- (void) onRoomPropertiesChange:(NSDictionary*)changes;
- (void) onPlayerPropertiesChange:(int)playerNr :(NSDictionary*)changes;
- (void) onAppStatsUpdate;

@end

/** @file */