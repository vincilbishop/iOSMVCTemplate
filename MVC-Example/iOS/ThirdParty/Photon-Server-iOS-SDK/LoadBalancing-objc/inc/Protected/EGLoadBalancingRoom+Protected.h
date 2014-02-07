/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#import "LoadBalancing-objc/inc/EGLoadBalancingRoom.h"

@interface EGLoadBalancingRoom ()

- (instancetype) initLoadBalancingRoom:(NSString*)name :(NSDictionary*)properties;
+ (instancetype) loadBalancingRoom:(NSString*)name :(NSDictionary*)properties;

@end



@interface EGLoadBalancingMutableRoom ()

- (instancetype) initLoadBalancingRoom:(NSString*)name :(NSDictionary*)properties :(EGLoadBalancingPeer*)peer :(EGArray*)propsListedInLobby;
+ (instancetype) loadBalancingRoom:(NSString*)name :(NSDictionary*)properties :(EGLoadBalancingPeer*)peer :(EGArray*)propsListedInLobby;
- (EGLoadBalancingPlayer*) createPlayer:(int)number :(NSDictionary*)properties;

@end