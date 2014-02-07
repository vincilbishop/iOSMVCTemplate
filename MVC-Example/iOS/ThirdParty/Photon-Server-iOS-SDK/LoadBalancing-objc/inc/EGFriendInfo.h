/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#import "Common-objc/inc/EGBase.h"

@interface EGFriendInfo : EGBase
{
@private
	NSString* mName;
	bool mIsOnline;
	NSString* mRoom;
}

@property (readonly) NSString* Name;
@property (readonly) bool IsOnline;
@property (readonly) NSString* Room;
@property (readonly) bool IsInRoom;

- (instancetype) initFriendInfo:(const NSString*)name;
- (instancetype) initFriendInfo:(const NSString*)name :(bool)isOnline :(const NSString*)room;
+ (instancetype) friendInfo:(const NSString*)name;
+ (instancetype) friendInfo:(const NSString*)name :(bool)isOnline :(const NSString*)room;

@end