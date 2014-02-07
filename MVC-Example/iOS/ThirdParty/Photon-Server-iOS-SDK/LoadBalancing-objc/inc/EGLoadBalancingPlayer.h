/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#import "LoadBalancing-objc/inc/EGLoadBalancingPeer.h"

@class EGLoadBalancingMutableRoom;

@interface EGLoadBalancingPlayer : EGBase
{
@protected
	int mNumber;
	NSMutableString* mName;
	NSMutableDictionary* mCustomProperties;
	const EGLoadBalancingMutableRoom* mRoom;
}

@property (readonly) int Number;
@property (readonly, copy) NSString* Name;
@property (readonly, copy) NSDictionary* CustomProperties;

-(NSString*) toString:(bool)withTypes :(bool)withCustomProperties;

@end



@interface EGLoadBalancingMutablePlayer : EGLoadBalancingPlayer
{
@private
	EGLoadBalancingPeer* mLoadBalancingPeer;
}

@property (readwrite, copy) NSString* Name;
@property (readonly) bool IsMasterClient;

- (void) mergeCustomProperties:(NSDictionary*)customProperties;
- (void) addCustomProperty:(NSString*)key :(id)value;
- (void) addCustomProperties:(NSDictionary*)customProperties;
- (void) removeCustomProperty:(NSString*)key;
- (void) removeCustomProperties:(NSString**)keys :(NSUInteger)count;

@end