/**
 * demo_RT_iPhone_photon_objcAppDeleagate.m - Exit Games Photon "realtime iPhone demo"
 * Copyright (C) 2009 Exit Games GmbH
 * http://www.exitgames.com
 *
 * @author developer@exitgames.com
 */

#import "demo_iPhone_realtime_objcAppDelegate.h"

@implementation demo_RT_iPhone_photon_objcAppDelegate

@synthesize window;
@synthesize glView;

-(void) onTime:(NSTimer*)Timer
{
	[mGame run];
}

-(void) automove:(NSTimer*)Timer
{
	[mGame automove];
}

-(void) everySecond:(NSTimer*)Timer
{
	[mGame everySecond];
}

- (void)applicationDidFinishLaunching:(UIApplication *)application
{
	// open gl graphic rendering
	glView.animationInterval = 1.0 / 60.0;
	[glView startAnimation];

	// demo game logic
	mGame = [[Game alloc] initWithView:glView];

	m_timer=[[NSTimer scheduledTimerWithTimeInterval:0.1 target:self selector:@selector(onTime:) userInfo:nil repeats:true]retain];
	m_automoveTimer=[[NSTimer scheduledTimerWithTimeInterval:1.0 target:self selector:@selector(automove:) userInfo:nil repeats:true]retain];
	m_fullSecond=[[NSTimer scheduledTimerWithTimeInterval:1.0 target:self selector:@selector(everySecond:) userInfo:nil repeats:true]retain];
}


- (void)applicationWillResignActive:(UIApplication *)application
{
	glView.animationInterval = 1.0 / 5.0;
}


- (void)applicationDidBecomeActive:(UIApplication *)application
{
	glView.animationInterval = 1.0 / 60.0;
}

- (void)applicationWillTerminate:(UIApplication *)application
{
	[mGame opLeaveRoom];
}

- (void)dealloc
{
	[window release];
	[glView release];
	[mGame release];
	[super dealloc];
}

@end
