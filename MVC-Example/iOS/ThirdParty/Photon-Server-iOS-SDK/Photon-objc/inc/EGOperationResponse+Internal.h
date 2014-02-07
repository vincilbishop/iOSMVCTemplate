/* Exit Games Photon - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "Photon-cpp/inc/OperationResponse.h"
#import "Photon-objc/inc/EGOperationResponse.h"

@interface EGOperationResponse (Internal)

- (EGOperationResponse*) initWithCppOperationResponse:(const ExitGames::Photon::OperationResponse&)operationResponse;
+ (EGOperationResponse*) operationResponseWithCppOperationResponse:(const ExitGames::Photon::OperationResponse&)operationResponse;

@end