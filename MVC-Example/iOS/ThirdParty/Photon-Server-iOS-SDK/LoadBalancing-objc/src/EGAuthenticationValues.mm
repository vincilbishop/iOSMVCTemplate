/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#include "LoadBalancing-cpp/inc/Internal/Enums/CustomAuthenticationType.h"
#import "LoadBalancing-objc/inc/Internal/EGAuthenticationValues+Internal.h"

/** @file EGAuthenticationValues.h */

using namespace ExitGames::LoadBalancing::Internal;

@implementation EGAuthenticationValues

@synthesize Type = mType;
@synthesize Parameters = mParameters;
@synthesize Data = mData;
@synthesize Secret = mSecret;



- (instancetype) init
{
	EG_ASSERT(false, @"not implemented");
	return nil;
}

- (instancetype) initAuthenticationValuesWithUsername:(const NSString*)username :(const NSString*)token
{
	self = [super init];
	mType = CustomAuthenticationType::CUSTOM;
	mParameters = [[NSString alloc] initWithFormat:@"username=%@&token=%@", username, token];
	return self;
}

- (instancetype) initAuthenticationValuesWithData:(const NSData*)data
{
	self = [super init];
	mType = CustomAuthenticationType::CUSTOM;
	mData = [data copy];
	return self;
}

- (instancetype) initAuthenticationValuesWithUsernameAndData:(const NSString*)username :(const NSString*)token :(const NSData*)data
{
	self = [super init];
	mType = CustomAuthenticationType::CUSTOM;
	mParameters = [[NSString alloc] initWithFormat:@"username=%@&token=%@", username, token];
	mData = [data copy];
	return self;
}

+ (instancetype) authenticationValuesWithUsername:(const NSString*)username :(const NSString*)token
{
	return [[[self alloc] initAuthenticationValuesWithUsername:username :token] autorelease];
}

+ (instancetype) authenticationValuesWithData:(const NSData*)data
{
	return [[[self alloc] initAuthenticationValuesWithData:data] autorelease];
}

+ (instancetype) authenticationValuesWithUsernameAndData:(const NSString*)username :(const NSString*)token :(const NSData*)data
{
	return [[[self alloc] initAuthenticationValuesWithUsernameAndData:username :token :data] autorelease];
}

- (NSString*) toString:(bool)withTypes
{
	return [NSString stringWithFormat:@"{%d %@ %@ %@}", (int)self.Type, [self.Parameters toString:withTypes], [self.Data toString:withTypes], [self.Secret toString:withTypes]];
}

- (void) dealloc
{
	[mData release];
	[mParameters release];
	[super dealloc];
}

@end