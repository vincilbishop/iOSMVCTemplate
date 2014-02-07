/* Exit Games Photon - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "Photon-cpp/inc/OperationRequest.h"
#import "Photon-objc/inc/EGOperationRequest.h"

@interface EGOperationRequest (Internal)

- (ExitGames::Photon::OperationRequest*) toCPP;

@end