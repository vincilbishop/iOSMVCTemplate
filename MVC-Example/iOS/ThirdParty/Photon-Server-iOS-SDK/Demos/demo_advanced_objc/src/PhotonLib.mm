#import "PhotonLib.h"

@implementation CPhotonLib

@synthesize  State = currentState;

- (void) InitCPhotonLib:(id<UIListener>)txtView
{
	textView = txtView;
	currentState = SampleStarted;
	mpLitePeer = NULL;
}

- (void) dealloc
{
	[mpCustomType release];
	[mpLitePeer release];
	[super dealloc];
}

- (int) InitLib:(id<EGPhotonListener>)listener
{
	[textView writeToTextView:@"Initialize EG library"];
	EGBase.Listener = listener;
	mpLitePeer = [[EGLitePeer alloc] initWithPhotonListener:listener];
	
	currentState = PhotonPeerCreated;
	mpLitePeer.DebugOutputLevel = ExitGames::Common::DebugLevel::INFO;

	mpCustomType = [[SampleCustomType alloc] init];
	[EGCustomTypeUtils initClass:mpCustomType.TypeCode :[SampleCustomType class]];

	return ExitGames::Photon::ErrorCode::SUCCESS;
}

- (int) CreateConnection
{
	[mpLitePeer connect:[NSString stringWithUTF32String:L"54.197.129.196"]];
	currentState = Connecting;
	return 0;
}

- (int) CloseConnection
{
	[mpLitePeer disconnect];
	currentState = Disconnecting;
	return 0;
}

- (short) Join:(NSString*)gameId
{
	return [mpLitePeer opJoin:gameId];
}

- (short) Leave
{
	return [mpLitePeer opLeave];
}

- (int) Run
{
	char gameId[] = "demo_photon_game";

	[mpLitePeer service];
	switch(currentState)
	{
		case PhotonPeerCreated:
			[textView writeToTextView:@"-------CONNECTING-------"];
			[self CreateConnection];
			break;
		case Connecting:
			// Waiting for callback function
			break;
		case ErrorConnecting:
			// Stop run cycle
			[textView writeToTextView:@"ErrorConnecting"];
			return -1;
			break;
		case Connected:
			[textView writeToTextView:@"-------ESTABLISHING ENCRYPTION-------"];
			currentState = EstablishingEncryption;
			[mpLitePeer establishEncryption];
			break;
		case EstablishingEncryption:
			// Waiting for callback function
			break;
		case ErrorEstablishingEncryption:
			// Stop run cycle
			return -1;
			[textView writeToTextView:@"ErrorEstablishingEncryption"];
			break;
		case EncryptionEstablished:
			[textView writeToTextView:@"-------JOINING-------"];
			currentState = Joining;
			if([self Join:[NSString stringWithUTF8String:gameId]] == -1)
				currentState = ErrorJoining;
			break;
		case Joining :
			// Waiting for callback function
			break;
		case ErrorJoining:
			// Stop run cycle
			return -1;
			[textView writeToTextView:@"ErrorJoining"];
			break;
		case Joined :
			currentState = Receiving;
			[textView writeToTextView:@"-------SENDING/RECEIVING DATA-------"];
			break;
		case Receiving:
			[self sendData];
			break;
		case Leaving:
			// Waiting for callback function
			break;
		case ErrorLeaving:
			// Stop run cycle
			return -1;
			[textView writeToTextView:@"ErrorLeaving"];
			break;
		case Left :
			currentState = Disconnecting;
			[textView writeToTextView:@"-------DISCONNECTING-------"];
			[self CloseConnection];
			break;
		case Disconnecting:
			// Waiting for callback function
			break;
		case Disconnected:
			// Stop run cycle
			return -1;
			break;
		default:
			break;
	}
	return 0;
}

- (void) sendData
{
	NSMutableDictionary* ev = [[NSMutableDictionary alloc] init];

	// nByte key and int value:
	nByte POS_X = 101;
	int x = 10;
	[ev setObject:[NSValue valueWithBytes:&x objCType:@encode(int)] forKey:[NSValue value:&POS_X withObjCType:@encode(nByte)]];

	// NSString key and EGArray of float value:
	NSValue* valArray[10];
	float j=0.0f;
	for(int i=0; i<10; i++, j+=1.1f)
		valArray[i] = [NSValue valueWithBytes:&j objCType:@encode(float)];
	[ev setObject:[EGArray arrayWithObjects:valArray count:10] forKey:@"testKey"];

	// nByte key and NSDictionary value:
	const nByte POS_Y = 102;
	const nByte key2 = 103;
	int y = 20;
	NSDictionary* testHash = [NSDictionary dictionaryWithObjectsAndKeys:[NSValue valueWithBytes:&x objCType:@encode(int)], [NSValue value:&POS_X withObjCType:@encode(nByte)],
																		[NSValue valueWithBytes:&y objCType:@encode(int)], [NSValue value:&POS_Y withObjCType:@encode(nByte)],
																		nil];
	[ev setObject:testHash forKey:[NSValue value:&key2 withObjCType:@encode(nByte)]];

	// NSString key and empty EGArray of int value:
	[ev setObject:[EGArray arrayWithCType:@encode(int)] forKey:@"emptyArray"];
	
	// NSString key and empty EGDictionary value:
	[ev setObject:[EGDictionary dictionaryWithKeyType:NSStringFromClass([NSString class]) valueType:[NSString stringWithUTF8String:@encode(int)]] forKey:@"dic"];

	// NSString key and multidimensional EGArray of EGDictionary value:
	NSString* dicType = [EGDictionary dictionaryWithCKeyType:@encode(int) valueType:NSStringFromClass([NSString class]).UTF8String].Type;
    EGMutableArray* array = [EGMutableArray arrayWithType:dicType];
    for(int i=0; i<3; i++)
    {
		[array addObject:[EGMutableArray arrayWithType:dicType]];
		for(int j=0; j<3; j++)
		{
			[[array objectAtIndex:i] addObject:[EGMutableArray arrayWithType:dicType]];
			for(int k=0; k<3; k++)
			{
				int key =100*i+10*j+k;
				[[[array objectAtIndex:i] objectAtIndex:j] addObject:[EGDictionary dictionaryWithObject:[NSString stringWithFormat:@"%d", 100*i+10*j+k] forKey:[NSValue value:&key withObjCType:@encode(int)]]];
			}
		}
    }
	[ev setObject:array forKey:@"array3d"];

	// short key and NSNull value
	short key = 1;
	[ev setObject:[NSNull null] forKey:[NSValue value:&key withObjCType:@encode(short)]];

	// NSString key and NSArray value, filled with a colorful bouquet of different datatypes
	NSMutableArray* objectArray = [NSMutableArray array];
	nByte tempByte = 10;
	[objectArray addObject:[NSValue valueWithBytes:&tempByte objCType:@encode(nByte)]];
	[objectArray addObject:@"string"];
	[objectArray addObject:[NSDictionary dictionary]];
	[objectArray addObject:[EGArray arrayWithObjects:valArray count:10]];
	[objectArray addObject:[EGArray arrayWithCType:@encode(int)]];
	[objectArray addObject:[NSArray arrayWithArray:objectArray]];
	[ev setObject:objectArray forKey:@"objectArray"];

	// CustomType array
	SampleCustomType* cusArray[10];
	for(int i=0; i<10; i++)
		cusArray[i] = [[SampleCustomType alloc] init:i :i*i];
	[ev setObject:[EGArray arrayWithObjects:cusArray count:10] forKey:@"customType"];

	[mpLitePeer opRaiseEvent :false :ev :0];
	[ev release];
}

- (void) Stop
{
	currentState = Leaving;
	[textView writeToTextView:@"-------LEAVING-------"];
	if([self Leave] == -1)
		currentState = ErrorLeaving;
}

@end