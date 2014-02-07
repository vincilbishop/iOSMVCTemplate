#import "Common-objc/inc/EGCustomType.h"

#define TYPECODE 1

@interface SampleCustomType : NSObject <EGCustomType>
{
@protected
	nByte mFoo;
	nByte mBar;
}

- (instancetype) init;
- (instancetype) init:(nByte) foo :(nByte)bar;
- (instancetype) initWithCustomType:(SampleCustomType*)toCopy;
- (void) setData:(nByte)foo :(nByte)bar;

@end