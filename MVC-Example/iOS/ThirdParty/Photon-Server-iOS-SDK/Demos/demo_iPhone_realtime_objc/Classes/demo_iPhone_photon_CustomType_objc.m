#import "Foundation/NSString.h"
#import "demo_iPhone_photon_CustomType_objc.h"

@implementation SampleCustomType

- (id) init
{
	self = [super init];
    if (self != nil)
	{
		mFoo = 0;
		mBar = 0;
	}
	return self;
}

- (id) init:(nByte) foo :(nByte) bar
{
	self = [super init];
    if (self != nil)
	{
		mFoo = foo;
		mBar = bar;
	}
	return self;
}

- (id) initWithCustomType:(SampleCustomType*) toCopy
{
	self = [super init];
    if (self != nil)
	{
		mFoo = toCopy->mFoo;
		mBar = toCopy->mBar;
	}
	return self;
}

- (void) dealloc
{
	[self cleanup];
	[super dealloc];
}


- (void) setData:(nByte) foo :(nByte) bar
{
	mFoo = foo;
	mBar = bar;
}


// CustomType callbacks

- (void) cleanup
{
}

- (const BOOL) compare:(const SampleCustomType*) other
{
	return mFoo == other->mFoo && mBar == other->mBar;
}

- (const void) duplicate:(SampleCustomType* const) retVal
{
	if(retVal)
		*(SampleCustomType*)retVal = *(SampleCustomType*)self;
}

-(void) deserialize:(const nByte* const) pData :(short) length
{
	if(length!=2)
		return; // something went wrong
	mFoo = pData[0];
	mBar = pData[1];
}

- (const short) serialize:(nByte* const) retVal
{
	if(retVal)
	{
		retVal[0] = mFoo;
		retVal[1] = mBar;
	}
	return 2;
}

- (const NSString*) toString
{
	return [NSString stringWithFormat:@"<%d, %d>", mFoo, mBar];
}

- (void*) calloc:(short) count
{
	return [SampleCustomType alloc];
}

- (void) cfree:(void*) pData
{
	[(SampleCustomType*)pData release];
}

- (unsigned int) csizeof
{
	return sizeof(SampleCustomType);
}

@end

/*




(const BOOL) compare:(const id<CustomType>) other;
-(const void) duplicate:(id<CustomType> const) retVal;
-(void) deserialize:(const nByte* const) pData :(short) length;
-(const short) serialize:(nByte* const) retVal;
-(const NSString*) toString;*/
