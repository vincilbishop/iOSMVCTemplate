
@interface Console : UITextView<UIAlertViewDelegate>
{
	bool initialized;
	NSMutableArray* ary;
	unsigned short lineWidth;
}

+ (instancetype) consoleWithFrame:(CGRect)frame;
- (instancetype) initWithFrame:(CGRect)frame;
- (void) update;
- (void) writeWithoutUpdate:(NSString*)str, ...;
- (void) writeWithoutUpdate:(NSString*)str :(va_list)args;
- (void) writeLineWithoutUpdate:(NSString*)str, ...;
- (void) writeLineWithoutUpdate:(NSString*)str :(va_list)args;
- (void) write:(NSString*)str, ...;
- (void) write:(NSString*)str :(va_list)args;
- (void) writeLine:(NSString*)str, ...;
- (void) writeLine:(NSString*)str :(va_list)args;

@end