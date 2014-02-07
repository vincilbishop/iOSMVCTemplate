#import "AppDelegate.h"
#import "ViewController.h"

@implementation AppDelegate

@synthesize window = _window;
@synthesize viewController = _viewController;

- (void) createGame
{
	viewMasterServer.hidden = true;
	app.LastInput = INPUT_CREATE_GAME;
}

- (void) joinRandomGame
{
	viewMasterServer.hidden = true;
	app.LastInput = INPUT_JOIN_RANDOM_GAME;
}

- (void) leaveGame
{
	viewGameRoom.hidden = true;
	app.LastInput = INPUT_LEAVE_GAME;
}

- (void) run:(NSTimer*)timer
{
	[app run];
}

- (BOOL) application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions
{
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];

	if([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone)
	    self.viewController = [[ViewController alloc] initWithNibName:@"ViewController_iPhone" bundle:nil];
	else
	    self.viewController = [[ViewController alloc] initWithNibName:@"ViewController_iPad" bundle:nil];

	[self.viewController.view addSubview:viewGameRoom=[[UIView alloc] initWithFrame:CGRectMake(0, 0, self.window.frame.size.width, self.window.frame.size.height/2)]];
	[self.viewController.view addSubview:viewMasterServer=[[UIView alloc] initWithFrame:CGRectMake(0, 0, self.window.frame.size.width, self.window.frame.size.height/2)]];
	viewGameRoom.backgroundColor = viewMasterServer.backgroundColor = [UIColor colorWithRed:0.5f green:0.5f blue:0.5f alpha:1.0f];
	viewGameRoom.hidden = viewMasterServer.hidden = true;

	unsigned short buttonWidth = 120, buttonHeight = 40;

	UIButton* button0 = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	button0.frame = CGRectMake(viewMasterServer.frame.size.width/2-buttonWidth/2, viewMasterServer.frame.size.height/3-buttonHeight/2, buttonWidth, buttonHeight);
	[button0 setTitle:@"Create Game" forState:UIControlStateNormal];
	[button0 addTarget:self action:@selector(createGame) forControlEvents:UIControlEventTouchUpInside];
	[viewMasterServer addSubview:button0];

	UIButton* button1 = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	button1.frame = CGRectMake(viewMasterServer.frame.size.width/2-buttonWidth/2, viewMasterServer.frame.size.height*2/3-buttonHeight/2, buttonWidth, buttonHeight);
	[button1 setTitle:@"Join Game" forState:UIControlStateNormal];
	[button1 addTarget:self action:@selector(joinRandomGame) forControlEvents:UIControlEventTouchUpInside];
	[viewMasterServer addSubview:button1];

	UIButton* button2 = [UIButton buttonWithType:UIButtonTypeRoundedRect];
	button2.frame = CGRectMake(viewGameRoom.frame.size.width/2-buttonWidth/2, viewGameRoom.frame.size.height/2-buttonHeight/2, buttonWidth, buttonHeight);
	[button2 setTitle:@"Leave Game" forState:UIControlStateNormal];
	[button2 addTarget:self action:@selector(leaveGame) forControlEvents:UIControlEventTouchUpInside];
	[viewGameRoom addSubview:button2];

	[self.viewController.view addSubview:console=[Console consoleWithFrame:CGRectMake(0, self.window.frame.size.height/2, self.window.frame.size.width, self.window.frame.size.height/2)]];

	self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];

	[app=[NetworkLogic networkLogicWithConsole:console] registerForStateUpdates:self];

	[NSTimer scheduledTimerWithTimeInterval:0.1f target:self selector:@selector(run:) userInfo:nil repeats:true];

    return YES;
}

- (void) applicationWillResignActive:(UIApplication*)application
{
	/*
	 Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
	 Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
	 */
}

- (void) applicationDidEnterBackground:(UIApplication*)application
{
	/*
	 Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
	 If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
	 */
}

- (void) applicationWillEnterForeground:(UIApplication*)application
{
	/*
	 Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
	 */
}

- (void) applicationDidBecomeActive:(UIApplication*)application
{
	/*
	 Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
	 */
}

- (void) applicationWillTerminate:(UIApplication*)application
{
	app.LastInput = INPUT_EXIT;
}

// protocol implementations

- (void) stateUpdate:(State)newState
{
	switch(newState)
	{
		case STATE_JOINED:
			viewGameRoom.hidden = false;
			break;
		case STATE_CONNECTED:
			viewMasterServer.hidden = false;
			break;
		default:
			break;
	}
}

@end