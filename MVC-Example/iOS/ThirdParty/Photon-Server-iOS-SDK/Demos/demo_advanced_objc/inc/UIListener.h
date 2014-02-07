@protocol UIListener <NSObject>

@required
-(void) writeToTextView:(NSString*) text;
-(BOOL) shouldStopDemo;

@end