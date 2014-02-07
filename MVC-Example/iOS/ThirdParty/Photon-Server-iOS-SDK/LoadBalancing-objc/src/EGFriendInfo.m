/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#import "LoadBalancing-objc/inc/EGFriendInfo.h"

@implementation EGFriendInfo

@synthesize Name = mName;
@synthesize IsOnline = mIsOnline;
@synthesize Room = mRoom;

- (bool) IsInRoom
{
	return self.Room.length;
}



- (instancetype) init
{
	EG_ASSERT(false, @"not implemented");
	return nil;
}

- (instancetype) initFriendInfo:(const NSString*)name
{
	self = [super init];
	mName = [name copy];
	return self;
}

- (instancetype) initFriendInfo:(const NSString*)name :(bool)isOnline :(const NSString*)room
{
	self = [super init];
	mName = [name copy];
	mIsOnline = isOnline;
	mRoom = [room copy];
	return self;
}

+ (instancetype) friendInfo:(const NSString*)name
{
	return [[[self alloc] initFriendInfo:name] autorelease];
}

+ (instancetype) friendInfo:(const NSString*)name :(bool)isOnline :(const NSString*)room
{
	return [[[self alloc] initFriendInfo:name :isOnline : room] autorelease];
}

- (NSString*) toString:(bool)withTypes
{
	return [NSString stringWithFormat:@"{%@ %@}", self.Name, self.IsOnline?[NSString stringWithFormat:@"on, %@", self.IsInRoom?[NSString stringWithFormat:@"in room %@", self.Room]:@"not in a room"]:@"off"];
}

- (void) dealloc
{
	[mName release];
	[super dealloc];
}

@end