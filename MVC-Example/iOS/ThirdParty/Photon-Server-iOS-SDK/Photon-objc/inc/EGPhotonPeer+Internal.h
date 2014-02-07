/* Exit Games Photon - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#import "Photon-objc/inc/EGPhotonPeer.h"

@interface EGPhotonPeer ()

@property (readonly) id<EGPhotonListener> InternalListener;

- (instancetype) initWithPhotonListener:(id<EGPhotonListener>)listener :(bool)useTcp :(bool)useLitePeer;

@end