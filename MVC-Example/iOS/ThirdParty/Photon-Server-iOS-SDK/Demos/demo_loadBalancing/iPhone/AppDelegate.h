@class Console;
@class ViewController;

@interface AppDelegate : UIResponder <UIApplicationDelegate>
{
@private
	UIView* viewMasterServer;
	UIView* viewGameRoom;
	Console* console;
}

@property (strong, nonatomic) UIWindow* window;
@property (strong, nonatomic) ViewController* viewController;

- (void) stateUpdate:(int)newState;

@end
