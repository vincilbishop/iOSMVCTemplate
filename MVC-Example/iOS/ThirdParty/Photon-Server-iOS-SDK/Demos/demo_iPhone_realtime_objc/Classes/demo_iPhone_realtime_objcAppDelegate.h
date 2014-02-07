/**
 * demo_RT_iPhone_photon_objcAppDeleagate.h - Exit Games Photon "realtime iPhone demo"
 * Copyright (C) 2009 Exit Games GmbH
 * http://www.exitgames.com
 *
 * @author developer@exitgames.com
 */

#import <UIKit/UIKit.h>
#import "game.h"

@class EAGLView;

@interface demo_RT_iPhone_photon_objcAppDelegate : NSObject <UIApplicationDelegate>
{
    UIWindow* window;
    EAGLView* glView;
	Game* mGame;
	NSTimer* m_timer;
	NSTimer* m_automoveTimer;
	NSTimer* m_fullSecond;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet EAGLView *glView;

@end

