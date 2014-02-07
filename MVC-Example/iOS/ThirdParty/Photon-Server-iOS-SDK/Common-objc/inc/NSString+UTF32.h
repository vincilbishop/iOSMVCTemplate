/* Exit Games Common - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#import <Foundation/NSString.h>
#include "Common-cpp/inc/defines.h"

@interface NSString (UTF32)

+ (NSString*) stringWithUTF32String:(const EG_CHAR* const)str;
- (NSString*) initWithUTF32String:(const EG_CHAR* const)str;
- (const EG_CHAR* const) UTF32String;
- (EG_CHAR*) mallocedUTF32String; // the caller is responsible for freeing the returnvalue of this method!

@end