#import "demo_Photon_objC.h"

using namespace ExitGames::Photon;
using namespace ExitGames::Lite;

@implementation Listener

-(void) InitListener:(CPhotonLib*)lib :(id<UIListener>)txtView
{
	current = lib;
	textView = txtView;
}

- (void) onOperationResponse:(EGOperationResponse*)operationResponse
{
	[textView writeToTextView:[operationResponse toString:true :true :true]];
	switch(current.State)
	{
		case Joining:
			if(operationResponse.OperationCode == OperationCode::JOIN)
			{
				current.State = Joined;
				[textView writeToTextView:@"----------JOINED-------"];
			}
			else current.State = ErrorJoining;
			break;
		case Leaving:
			if(operationResponse.OperationCode == OperationCode::LEAVE)
			{
				current.State = Left;
				[textView writeToTextView:@"----------LEFT-------"];
			}
			else
				current.State = ErrorLeaving;
			break;
		default:
			break;
	}
}

- (void) onStatusChanged:(int)statusCode
{
	switch(current.State)
	{
		case Connecting:
			if(statusCode == StatusCode::CONNECT)
			{
				current.State = Connected;
				[textView writeToTextView:@"-------CONNECTED-------"];
			}
			else
				current.State = ErrorConnecting;
			break;
		case Disconnecting:
			if(statusCode == StatusCode::DISCONNECT)
			{
				current.State = Disconnected;
				[textView writeToTextView:@"-------DISCONNECTED-------"];
			}
			break;
		case EstablishingEncryption:
			if(statusCode == StatusCode::ENCRYPTION_ESTABLISHED)
			{
				current.State = EncryptionEstablished;
				[textView writeToTextView:@"-------ENCRYPTION ESTABLISHED-------"];
			}
			else
				current.State = ErrorEstablishingEncryption;
			break;
		case Receiving:
			[textView writeToTextView:[NSString stringWithFormat:@"-->! PeerStatus function was called, statusCode: %d", statusCode]];
			break;
		default:
			break;
	}
}

- (void) onEvent:(EGEventData*)eventData
{
	[textView writeToTextView:[eventData toString:true :true]];
	switch(eventData.Code)
	{
		// you do not receive your own events, so you must start 2 clients, to receive the events, you sent
		case 0:
		{
			NSDictionary* eventDataContent = nil;
			const nByte POS_X = 101, POS_Y = 102, key2 = 103;
			EGArray* ary = nil, *ary2 = nil, *ary3 = nil;
			NSArray* objAry = nil;
			int x = 0, y = 0;
			NSDictionary* testHash = nil;

			// first access the inner Hash with your userdata inside the outer hash with general event data
			if(!(eventDataContent=[eventData objectForKey:[NSValue value:&ParameterCode::DATA withObjCType:@encode(nByte)]]))
				break;

			// then take your user data from within the inner hash

			// nByte key and int value:
			[(NSValue*)[eventDataContent objectForKey:[NSValue value:&POS_X withObjCType:@encode(nByte)]] getValue:&x];

			// NSString key and EGArray of float value
			ary = [eventDataContent objectForKey:@"testKey"];

			// nByte key and NSDictionary value
			testHash = [eventDataContent objectForKey:[NSValue value:&key2 withObjCType:@encode(nByte)]];
			[((NSValue*)[testHash objectForKey:[NSValue value:&POS_X withObjCType:@encode(nByte)]])getValue:&x];
			[((NSValue*)[testHash objectForKey:[NSValue value:&POS_Y withObjCType:@encode(nByte)]])getValue:&y];

			// NSString key and empty EGArray of int value
			ary2 = [eventDataContent objectForKey:@"emptyArray"];

			// NSString key and 3-dimensional EGArray of NSString value
			ary3 = [eventDataContent objectForKey:@"array3d"];

			// NSString key and NSArray value
			objAry = [eventDataContent objectForKey:@"objectArray"];
		}
			break;
		default:
			break;
	}
}

- (void) debugReturn:(int)debugLevel :(NSString*)string
{
	printf("%s\n", [string UTF8String]);
}

@end