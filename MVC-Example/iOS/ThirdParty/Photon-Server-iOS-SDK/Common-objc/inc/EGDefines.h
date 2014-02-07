/* Exit Games Common - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#import "Foundation/NSString.h"
#include "Common-cpp/inc/defines.h"

#ifdef _EG_IPHONE_PLATFORM
#define IOS_OSX(ios, osx) ios
#else
#define IOS_OSX(ios, osx) osx
#endif

// don't ever use NSAssert(), as it is evil! Details: NSAssert() isn't actually triggering an assert, but an InternalInconsistencyException, which could be caught by the app and that can lead into hard to find memory leaks (example: classic objC [[[Foo alloc] init] autorelease] and an NSAssert in init that gets catched at a higher level location in the calling stack -> autorelease won't ever be called and Foo leaks, although the intention of the NSAssert()-call has been to either terminate the app or continue with normal code execution, but under no circumstances to recover without calling the full code)
#define EG_ASSERT(condition, msg) DEBUG_RELEASE((!(condition)?DBGPRINTF_FATAL((msg).UTF8String):(void)0, assert(condition)), (void)0)