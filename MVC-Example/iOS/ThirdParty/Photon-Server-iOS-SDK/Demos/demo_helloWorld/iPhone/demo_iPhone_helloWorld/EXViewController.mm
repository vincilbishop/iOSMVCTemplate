
#import "EXViewController.h"

#include "HelloWorldApplication.h"
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
		
		[_logger performSelectorOnMainThread:@selector(setString:)
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
		[_logger performSelectorOnMainThread:@selector(setString:)
								  withObject:@"---CLOSED---"
							   waitUntilDone:NO];
	}

	void sleep(int milliseconds)
	{
		usleep(milliseconds*1000);
	}

private:
	EXViewController* _logger;
};

@implementation EXViewController

@synthesize buttonCloseClicked;

- (void) startDemo
{
	HelloWorldApplication::run(new iPhoneUILisener(self));
}

- (void) setString:(NSString*)string
{
	textView.text = string;
}

- (IBAction) onStopButtonClicked:(id)sender
{
	self.buttonCloseClicked = YES;
	stopButton.hidden = YES;
}

#pragma mark - View lifecycle

- (void) didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

- (void) viewDidLoad
{
    [super viewDidLoad];

	textView.text = @"";
	[self performSelectorInBackground:@selector(startDemo) withObject:nil];
}

@end