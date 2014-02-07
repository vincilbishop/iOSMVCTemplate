
#import "UIListener.h"

@class CPhotonLib;
@class Listener;

@interface EXViewController : UIViewController<UIListener>
{
	IBOutlet UITextView* textView;
}

@property (assign) BOOL stopDemo;

-(IBAction) onButtonCloseClicked:(id) sender;

@end
