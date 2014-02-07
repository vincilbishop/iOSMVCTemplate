/* Exit Games Photon - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "Photon-cpp/inc/EventData.h"
#import "Photon-objc/inc/EGEventData.h"

@interface EGEventData (Internal)

- (EGEventData*) initWithCppEventData:(const ExitGames::Photon::EventData&)eventData;
+ (EGEventData*) eventDataWithCppEventData:(const ExitGames::Photon::EventData&)eventData;

@end