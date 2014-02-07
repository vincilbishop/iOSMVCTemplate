/* Exit Games Common - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "Common-cpp/inc/defines.h"
#import "Common-objc/inc/EGBase.h"
#import "Common-objc/inc/EGCustomType.h"

@interface EGCustomTypeUtils : EGBase
{
}

+ (void) initClass:(nByte)typeCode :(Class<EGCustomType>)delegate;
+ (Class<EGCustomType>) getClassForCode:(nByte)typeCode;

@end