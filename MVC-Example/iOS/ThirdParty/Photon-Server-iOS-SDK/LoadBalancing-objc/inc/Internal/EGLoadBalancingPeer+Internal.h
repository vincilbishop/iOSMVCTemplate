/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#import "LoadBalancing-objc/inc/EGLoadBalancingClient.h"

@interface EGLoadBalancingPeer ()

- (bool) opSetPropertiesOfPlayer:(int)playerNr :(NSDictionary*)properties;
- (bool) opSetPropertiesOfRoom:(NSDictionary*)properties;
+ (NSMutableDictionary*) stripToCustomProperties:(NSDictionary*)properties;
+ (NSMutableDictionary*) stripKeysWithNullValues:(NSDictionary*)orig;
- (NSMutableDictionary*) opCreateRoomImplementation:(NSString*)gameID :(bool)isVisible :(bool)isOpen :(nByte)maxPlayers :(NSDictionary*)customRoomProperties :(NSDictionary*)customLocalPlayerProperties :(EGArray*)propsListedInLobby;
- (NSMutableDictionary*) opJoinRoomImplementation:(NSString*)gameID :(NSDictionary*)customLocalPlayerProperties;

@end