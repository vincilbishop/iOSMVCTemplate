/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#import "LoadBalancing-objc/inc/EGLoadBalancingRoom.h"
#import "LoadBalancing-objc/inc/Internal/EGLoadBalancingPlayer+Internal.h"

@interface EGLoadBalancingRoom ()

- (instancetype) cacheProperties:(NSDictionary*)properties;

@end



@interface EGLoadBalancingMutableRoom ()


- (void) setPlayers:(EGMutableArray*)players;

- (void) removeAllPlayers;
- (void) addPlayer:(EGLoadBalancingPlayer*)player;
- (void) addPlayer:(int)number :(NSDictionary*)properties;
- (bool) removePlayer:(int)number;

@end