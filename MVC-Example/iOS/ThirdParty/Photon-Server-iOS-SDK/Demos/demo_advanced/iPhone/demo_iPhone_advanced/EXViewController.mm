#import "EXViewController.h"

#include "AdvancedApplication.h"
#include "UIListener.h"



class iPhoneUILisener: public UIListener
{
public:
	iPhoneUILisener(id logger):
		_logger(logger)
	{
	}

	void writeString(const wchar_t* str)
	{
		NSString* pStr = [[NSString alloc] initWithBytes:str
												  length:(wcslen(str)*4)
												 encoding:NSUTF8StringEncoding];

		[_logger performSelectorOnMainThread:@selector(addString:)
								  withObject:pStr
							   waitUntilDone:NO];
		[pStr release];
	}

	bool anyKeyPressed() const
	{
		return _logger.buttonCloseClicked;
	}

	void onLibClosed()
	{
		[_logger performSelectorOnMainThread:@selector(addString:)
								  withObject:@"---CLOSED---"
							   waitUntilDone:NO];
	}


private:
	EXViewController* _logger;
};

@implementation EXViewController

@synthesize buttonCloseClicked;

- (void) startDemo
{
	AdvancedApplication::run(new iPhoneUILisener(self));
}

- (void) addString:(NSString*)string
{
	textView.text = [textView.text stringByAppendingFormat: @"\n%@", string];
	NSInteger len = [textView.text length];
	if(len > 10)
		[textView scrollRangeToVisible:NSMakeRange(len-10, 10)];
}

- (IBAction) onStopButtonClicked:(id)sender
{
	self.buttonCloseClicked = YES;
	stopButton.hidden = YES;
}

- (void) didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - View lifecycle

- (void) viewDidLoad
{
    [super viewDidLoad];

	textView.text = @"";
	[self performSelectorInBackground:@selector(startDemo)
						   withObject:nil];
}

- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return YES;
}

@end