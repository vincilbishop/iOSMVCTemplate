#import "Console.h"

@implementation Console

+ (instancetype) consoleWithFrame:(CGRect)frame
{
	return [[[self alloc] initWithFrame:frame] autorelease];
}

- (instancetype) initWithFrame:(CGRect)frame
{
	if(self=[super initWithFrame:frame])
	{
		self.backgroundColor = [UIColor blackColor];
		self.font = [UIFont fontWithName:@"Courier New" size:10];
		self.textColor = [UIColor whiteColor];
		self.editable = false;
		self.frame = frame;
		self.indicatorStyle = UIScrollViewIndicatorStyleWhite;
		// breaking lines after about 95% of the screen as parent class would break them, if we would use 100%, to find room for it's scrollbar, and we want full control over line breaks
		lineWidth = (self.frame.size.width*0.95f)/(self.font.pointSize*0.6f); // width to height ration for Courier New is 0.6
		ary = [[NSMutableArray alloc] initWithObjects:@"", nil];
		initialized = true;
	}
	return self;
}

- (void) update
{
	NSString* newTxt = [NSString string];
	for(int i=0; i<ary.count; i++)
		newTxt = [newTxt stringByAppendingString:[[ary objectAtIndex:i] stringByAppendingString:@"\n"]];
	self.text = newTxt;
	[self scrollRectToVisible:CGRectMake(0, self.contentSize.height-self.frame.size.height, self.contentSize.width, self.contentSize.height) animated:false];
}

- (void) writeWithoutUpdate:(NSString*)str, ...
{
    va_list args;
    va_start(args, str);
	[self writeWithoutUpdate:str :args];
    va_end(args);
}

- (void) writeWithoutUpdate:(NSString*)str :(va_list)args
{
	if(!initialized || ![str length])
		return;
	
	NSString* tmpStr = [[ary lastObject] stringByAppendingString:[[[NSString alloc] initWithFormat:str arguments:args] autorelease]];
	[ary removeLastObject];
	for(int i=0; i<[tmpStr length]; i+=lineWidth)
		[ary addObject:[tmpStr substringWithRange:NSMakeRange(i, [tmpStr length]-i<lineWidth?[tmpStr length]-i:lineWidth)]];
}

- (void) writeLineWithoutUpdate:(NSString*)str, ...
{
    va_list args;
    va_start(args, str);
	[self writeLineWithoutUpdate:str :args];
    va_end(args);
}

- (void) writeLineWithoutUpdate:(NSString*)str :(va_list)args
{
	[self writeWithoutUpdate:str :args];
	[ary addObject:@""];
}

- (void) write:(NSString*)str, ...
{
    va_list args;
    va_start(args, str);
	[self write:str :args];
    va_end(args);
}

- (void) write:(NSString*)str :(va_list)args
{
	[self writeWithoutUpdate:str :args];
	[self update];
}

- (void) writeLine:(NSString*)str, ...
{
    va_list args;
    va_start(args, str);
	[self writeLine:str :args];
    va_end(args);
}

- (void) writeLine:(NSString*)str :(va_list)args
{
	[self writeLineWithoutUpdate:str :args];
	[self update];
}

-(void) dealloc
{
	[ary release];
	[super dealloc];
}

@end