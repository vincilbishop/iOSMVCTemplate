/* Exit Games Photon - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#import "Common-objc/inc/EGBaseListener.h"
#import "Photon-objc/inc/EGEventData.h"
#import "Photon-objc/inc/EGOperationResponse.h"

@protocol EGPhotonListener <EGBaseListener>

- (void) onOperationResponse:(EGOperationResponse*)operationResponse;
- (void) onStatusChanged:(int)statusCode;
- (void) onEvent:(EGEventData*)eventData;

@end

/** @file */