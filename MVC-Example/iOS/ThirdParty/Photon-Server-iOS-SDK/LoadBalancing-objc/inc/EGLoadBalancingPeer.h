/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#define DBGPRINTF_MEMORY_ACTIVE 1

#include "Photon-cpp/inc/Enums/EventCache.h"
#include "Photon-cpp/inc/Enums/EventCode.h"
#include "Photon-cpp/inc/Enums/EventKey.h"
#include "Photon-cpp/inc/Enums/OperationCode.h"
#include "Photon-cpp/inc/Enums/ParameterCode.h"
#include "Photon-cpp/inc/Enums/ReceiverGroup.h"
#include "LoadBalancing-cpp/inc/Enums/ErrorCode.h"
#include "LoadBalancing-cpp/inc/Enums/PeerStates.h"
#import "Photon-objc/inc/EGPhotonPeer.h"
#import "LoadBalancing-objc/inc/EGAuthenticationValues.h"

@interface EGLoadBalancingPeer : EGPhotonPeer
{
}

- (bool) opJoinLobby;
- (bool) opLeaveLobby;
- (bool) opCreateRoom:(NSString*)gameID;
- (bool) opCreateRoom:(NSString*)gameID :(bool)isVisible :(bool)isOpen;
- (bool) opCreateRoom:(NSString*)gameID :(bool)isVisible :(bool)isOpen :(nByte)maxPlayers;
- (bool) opCreateRoom:(NSString*)gameID :(bool)isVisible :(bool)isOpen :(nByte)maxPlayers :(NSDictionary*)customRoomProperties;
- (bool) opCreateRoom:(NSString*)gameID :(bool)isVisible :(bool)isOpen :(nByte)maxPlayers :(NSDictionary*)customRoomProperties :(NSDictionary*)customLocalPlayerProperties;
- (bool) opCreateRoom:(NSString*)gameID :(bool)isVisible :(bool)isOpen :(nByte)maxPlayers :(NSDictionary*)customRoomProperties :(NSDictionary*)customLocalPlayerProperties :(EGArray*)propsListedInLobby;
- (bool) opJoinRoom:(NSString*)gameID;
- (bool) opJoinRoom:(NSString*)gameID :(NSDictionary*)customLocalPlayerProperties;
- (bool) opJoinRandomRoom;
- (bool) opJoinRandomRoom:(NSDictionary*)customRoomProperties;
- (bool) opJoinRandomRoom:(NSDictionary*)customRoomProperties :(nByte)maxPlayers;
- (bool) opLeaveRoom;
- (bool) opRaiseEvent:(bool)reliable :(id<NSObject>)parameters :(nByte)eventCode; // = 0
- (bool) opRaiseEvent:(bool)reliable :(id<NSObject>)parameters :(nByte)eventCode :(nByte)channelID; // = EventCache::DO_NOT_CACHE
- (bool) opRaiseEvent:(bool)reliable :(id<NSObject>)parameters :(nByte)eventCode :(nByte)channelID :(nByte)eventCaching; // = NULL, = 0
- (bool) opRaiseEvent:(bool)reliable :(id<NSObject>)parameters :(nByte)eventCode :(nByte)channelID :(nByte)eventCaching :(int*)targetPlayers :(short)numTargetPlayers; // = ReceiverGroup::OTHERS
- (bool) opRaiseEvent:(bool)reliable :(id<NSObject>)parameters :(nByte)eventCode :(nByte)channelID :(nByte)eventCaching :(int*)targetPlayers :(short)numTargetPlayers :(nByte)receiverGroup;
- (bool) opAuthenticate:(NSString*)appID :(NSString*)appVersion :(bool)encrypted;
- (bool) opAuthenticate:(NSString*)appID :(NSString*)appVersion :(bool)encrypted :(NSString*)userID;
- (bool) opAuthenticate:(NSString*)appID :(NSString*)appVersion :(bool)encrypted :(NSString*)userID :(EGAuthenticationValues*)authenticationValues;
- (bool) opFindFriends:(NSString**)friendsToFind :(short)numFriendsToFind;

@end