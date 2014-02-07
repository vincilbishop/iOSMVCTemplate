
#import "EXViewController.h"
#import "DemoApplication.h"

@implementation EXViewController

@synthesize stopDemo;

#pragma mark demo

-(void) addLine:(NSString*) text
{
	textView.text = [textView.text stringByAppendingFormat: @"\n%@", text];
	NSInteger len = [textView.text length];
	if(len > 10)
		[textView scrollRangeToVisible:NSMakeRange(len-10, 10)];
}

-(void) writeToTextView:(NSString*) text
{
	[self performSelectorOnMainThread:@selector(addLine:) withObject:text waitUntilDone:NO];
}

-(BOOL) shouldStopDemo
{
	return self.stopDemo;
}

-(IBAction) onButtonCloseClicked:(id) sender
{
	self.stopDemo = YES;
}

-(void) startDemo
{
	[DemoApplication startDemoWithListener:self];
}

#pragma mark memory management

-(void) didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - View lifecycle

-(void) viewDidLoad
{
    [super viewDidLoad];
	[self performSelectorInBackground:@selector(startDemo) withObject:nil];
}

-(void) viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

-(BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	return YES;
}

@end
