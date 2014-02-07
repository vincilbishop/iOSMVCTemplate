/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "Common-cpp/inc/defines.h"
#import "Common-objc/inc/EGBase.h"

@interface EGAuthenticationValues : EGBase
{
@private
	nByte mType;
	NSString* mParameters;
	NSData* mData;
	NSString* mSecret;
}

@property (readonly) nByte Type;
@property (readonly) NSString* Parameters;
@property (readonly) NSData* Data;
@property (readonly, copy) NSString* Secret;

- (instancetype) initAuthenticationValuesWithUsername:(const NSString*)username :(const NSString*)token;
- (instancetype) initAuthenticationValuesWithData:(const NSData*)data;
- (instancetype) initAuthenticationValuesWithUsernameAndData:(const NSString*)username :(const NSString*)token :(const NSData*)data;
+ (instancetype) authenticationValuesWithUsername:(const NSString*)username :(const NSString*)token;
+ (instancetype) authenticationValuesWithData:(const NSData*)data;
+ (instancetype) authenticationValuesWithUsernameAndData:(const NSString*)username :(const NSString*)token :(const NSData*)data;

@end