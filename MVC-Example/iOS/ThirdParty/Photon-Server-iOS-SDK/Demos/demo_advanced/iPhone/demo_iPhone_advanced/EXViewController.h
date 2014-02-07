@interface EXViewController : UIViewController
{
	IBOutlet UITextView* textView;
	IBOutlet UIButton* stopButton;
}

@property (assign) BOOL buttonCloseClicked;

- (void) addString:(NSString*)string;
- (IBAction) onStopButtonClicked:(id)sender;

@end