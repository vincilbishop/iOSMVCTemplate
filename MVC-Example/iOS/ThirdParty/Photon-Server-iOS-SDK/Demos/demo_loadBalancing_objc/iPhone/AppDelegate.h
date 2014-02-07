#import "NetworkLogic.h"
#import "Console.h"

@class ViewController;

@interface AppDelegate : UIResponder <UIApplicationDelegate, NetworkLogicListener>
{
@private
	NetworkLogic* app;
	UIView* mViewMasterServer;
	UIView* mViewGameRoom;
	Console* console;
}

@property (strong, nonatomic) UIWindow* window;
@property (strong, nonatomic) ViewController* viewController;

@end