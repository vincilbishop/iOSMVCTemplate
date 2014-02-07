#import "Foundation/NSString.h"
#import "demo_photon_CustomType_objc.h"

@implementation SampleCustomType

- (instancetype) init
{
    if(self=[super init])
	{
		mFoo = 0;
		mBar = 0;
	}
	return self;
}

- (instancetype) init:(nByte)foo :(nByte)bar
{
    if(self=[super init])
	{
		mFoo = foo;
		mBar = bar;
	}
	return self;
}

- (instancetype) initWithCustomType:(SampleCustomType*)toCopy
{
    if(self=[super init])
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

- (nByte) TypeCode
{
    return TYPECODE;
}

- (void) setData:(nByte)foo :(nByte)bar
{
	mFoo = foo;
	mBar = bar;
}


// CustomType callbacks

- (void) cleanup
{
}

- (bool) compare:(const SampleCustomType*)other
{
	return mFoo == other->mFoo && mBar == other->mBar;
}

- (void) duplicate:(SampleCustomType* const)retVal
{
	if(retVal)
	{
		retVal->mFoo = mFoo;
		retVal->mBar = mBar;
	}
}

- (void) deserialize:(const nByte* const)pData :(short)length
{
	if(length!=2)
		return; // something went wrong
	mFoo = pData[0];
	mBar = pData[1];
}

- (short) serialize:(nByte* const)retVal
{
	if(retVal)
	{
		retVal[0] = mFoo;
		retVal[1] = mBar;
	}
	return 2;
}

- (NSString*) typeToString
{
	return [NSString stringWithFormat:@"(%@)", NSStringFromClass(self.class)];
}

- (NSString*) toString
{
	return [self toString:false];
}

- (const NSString*) toString:(bool)withTypes
{
	return [NSString stringWithFormat:@"<%@%d, %@%d>", withTypes?[NSString stringWithFormat:@"(%@)", EG_STR_UCHAR]:@"", mFoo, withTypes?[NSString stringWithFormat:@"(%@)", EG_STR_UCHAR]:@"", mBar];
}

@end