#include "CustomType.h"
#include "CustomTypeUtils.h"

@interface SampleCustomType : NSObject <CustomType>
{
@protected
	nByte mFoo;
	nByte mBar;
}

- (id) init;
- (id) init:(nByte) foo :(nByte) bar;
- (id) initWithCustomType:(SampleCustomType*) toCopy;
- (void) setData:(nByte) foo :(nByte) bar;

@end
