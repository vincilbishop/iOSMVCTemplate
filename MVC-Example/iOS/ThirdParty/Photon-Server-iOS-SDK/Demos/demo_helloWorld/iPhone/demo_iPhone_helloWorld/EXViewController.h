
@interface EXViewController : UIViewController
{
	IBOutlet UITextView* textView;
	IBOutlet UIButton* stopButton;
}

@property (assign) BOOL buttonCloseClicked;

-(IBAction) onStopButtonClicked:(id)sender;

@end
