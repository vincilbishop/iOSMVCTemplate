#import "NetworkLogic.h"

#if defined _EG_IPHONE_PLATFORM
#	define PLAYER_NAME @"iOS"
#elif defined _EG_IMAC_PLATFORM
#	define PLAYER_NAME @"OS X"
#else
#	define PLAYER_NAME @"unknown platform"
#endif

namespace EGP = ExitGames::Photon;
namespace EGDbgLvl = ExitGames::Common::DebugLevel;

@interface NetworkLogic ()

@property (readonly) EGLogger* Logger;

@end



@implementation StateAccessor

- (State) State
{
	return mState;
}

- (void) setState:(State)State
{
	mState = State;
	for(unsigned int i=0; i<mStateUpdateListeners.count; i++)
		[[mStateUpdateListeners objectAtIndex:i] stateUpdate:mState];
}

- (id) init
{
	self = [super init];
	mStateUpdateListeners = [[EGMutableArray alloc] initWithType:NSStringFromClass([NSObject class])];
	return self;
}

- (void) registerForStateUpdates:(id<NetworkLogicListener>)listener
{
	[mStateUpdateListeners addObject:listener];
}

- (void) dealloc
{
	[mStateUpdateListeners release];
	[super dealloc];
}

@end



@implementation NetworkLogic

// properties

@synthesize LastInput = mLastInput;

- (State) State
{
	return mStateAccessor.State;
}

@synthesize Logger = mLogger;



// methods

+ (NetworkLogic*) networkLogicWithOutputListener:(id<OutputListener>)listener :(NSString*)appVersion
{
	return [[[NetworkLogic alloc] initWithOutputListener:listener :appVersion] autorelease];
}

- (NetworkLogic*) initWithOutputListener:(id<OutputListener>)listener :(NSString*)appVersion
{
	self = [super init];
	(mLogger=[[EGLogger alloc] initWithDebugOutputLevel:EGDbgLvl::INFO]).Listener = EGBase.Listener = self;
	mOutputListener = listener;
	(mLoadBalancingClient=[[EGLoadBalancingClient alloc] initClient:self :@"if connecting to the exitgamescloud, replace this string with the application id, that can be found after logging in at http://exitgamescloud.com/Dashboard" :appVersion :PLAYER_NAME]).DebugOutputLevel = EGDbgLvl::INFO;

	mStateAccessor = [[StateAccessor alloc] init];
	mStateAccessor.State = STATE_INITIALIZED;
	mLastInput = INPUT_NON;
	return self;
}

- (void) registerForStateUpdates:(id<NetworkLogicListener>)listener
{
	[mStateAccessor registerForStateUpdates:listener];
}

- (void) connect
{
	/* Specify the address of your Photon Server master server or the url of a Photon Cloud region here.
	If you don't specify a port, then Photons default master server ports (UDP: 5055, TCP: 4530) will be used.
	Valid cloud regions are:
	Western Europe: app-eu.exitgamescloud.com
	US: app-us.exitgamescloud.com
	Asia: app-asia.exitgamescloud.com
	Japan: app-jp.exitgamescloud.com
	Eastern Europe: app-eu-east.exitgamescloud.com
	The parameterless overload defaults to app-eu.exitgamescloud.com. */
	[mLoadBalancingClient connect:@"192.168.1.139"];
	mStateAccessor.State = STATE_CONNECTING;
}

- (void) disconnect
{
	[mLoadBalancingClient disconnect];
}

- (void) opCreateRoom
{
	// if last digits are always nearly the same, this is because of the timer calling this function being triggered every x ms with x being a factor of 10
	NSString* tmp = [NSString stringWithFormat:@"%d", GETTIMEMS()];
	[mLoadBalancingClient opCreateRoom:tmp];
	mStateAccessor.State = STATE_JOINING;
	[mOutputListener writeLine:@"creating room \"%@\"", tmp];
}

- (void) opJoinRandomRoom
{
	[mLoadBalancingClient opJoinRandomRoom];
}

- (void) run
{
	if(mLastInput == INPUT_EXIT && mStateAccessor.State != STATE_DISCONNECTING && mStateAccessor.State != STATE_DISCONNECTED)
	{
		[self disconnect];
		mStateAccessor.State = STATE_DISCONNECTING;
		[mOutputListener writeLine:@"terminating application"];
	}
	else
	{
		State state = mStateAccessor.State;
		switch(state)
		{
			case STATE_INITIALIZED:
				[self connect];
				mStateAccessor.State = STATE_CONNECTING;
				[mOutputListener writeLine:@"connecting"];
				break;
			case STATE_CONNECTING:
				break; // wait for callback
			case STATE_CONNECTED:
				switch(mLastInput)
				{
					case INPUT_CREATE_GAME: // create Game
						[self opCreateRoom];
						break;
					case INPUT_JOIN_RANDOM_GAME: // join Game
						[self opJoinRandomRoom];
						mStateAccessor.State = STATE_JOINING;
						[mOutputListener writeLine:@"joining random game"];
						break;
					case INPUT_EXIT: // exit
						[self disconnect];
						mStateAccessor.State = STATE_DISCONNECTING;
						[mOutputListener writeLine:@"terminating application"];
						break;
					default: // no or illegal input -> stay waiting for legal input
						break;
				}
				break;
			case STATE_JOINING:
				break; // wait for callback
			case STATE_JOINED:
				[self sendEvent];
				switch(mLastInput)
				{
					case INPUT_LEAVE_GAME: // leave Game
						[mLoadBalancingClient opLeaveRoom];
						mStateAccessor.State = STATE_LEAVING;
						[mOutputListener writeLine:@""];
						[mOutputListener writeLine:@"leaving game room"];
						break;
					default: // no or illegal input -> stay waiting for legal input
						break;
				}
				break;
			case STATE_LEAVING:
				break; // wait for callback
			case STATE_LEFT:
				mStateAccessor.State = STATE_CONNECTED;
				break;
			case STATE_DISCONNECTING:
				break; // wait for callback
			default:
				break;
		}
	}
	mLastInput = INPUT_NON;
	[mLoadBalancingClient service];
}

- (void) sendEvent
{
	static int64 count;
	[mLoadBalancingClient opRaiseEvent:false :[NSValue valueWithBytes:&++count objCType:@encode(int64)] :0];
	[mOutputListener write:@"s%lld ", count];
}

- (void) dealloc
{
	[mStateAccessor release];
	[mLoadBalancingClient release];
	[super dealloc];
}



// protocol implementations

- (void) debugReturn:(ExitGames::Common::DebugLevel::DebugLevel)debugLevel :(NSString*)string
{
	fwprintf(stderr, L"%ls\n", string.UTF32String);
}

- (void) connectionErrorReturn:(int)errorCode
{
	EGLOG(EGDbgLvl::ERRORS, L" code: %d", errorCode);
	[mOutputListener writeLine:@"connection failed with error %d", errorCode];
	mStateAccessor.State = STATE_DISCONNECTED;
}

- (void) clientErrorReturn:(int)errorCode
{
	EGLOG(EGDbgLvl::ERRORS, L" code: %d", errorCode);
	[mOutputListener writeLine:@"received error %d from client", errorCode];
}

- (void) warningReturn:(int)warningCode
{
	EGLOG(EGDbgLvl::WARNINGS, L" code: %d", warningCode);
	[mOutputListener writeLine:@"received warning %d from client", warningCode];
}

- (void) serverErrorReturn:(int)errorCode
{
	EGLOG(EGDbgLvl::ERRORS, L" code: %d", errorCode);
	[mOutputListener writeLine:@"received error %d from server", errorCode];
}

- (void) joinRoomEventAction:(int)playerNr :(EGArray*)playernrs :(EGLoadBalancingPlayer*)player
{
	EGLOG(EGDbgLvl::INFO, L"%ls joined the game", player.Name.UTF32String);
	[mOutputListener writeLine:@""];
	[mOutputListener writeLine:@"player %d %@ has joined the game", playerNr, player.Name];
}

- (void) leaveRoomEventAction:(int)playerNr
{
	EGLOG(EGDbgLvl::INFO, L"");
	[mOutputListener writeLine:@""];
	[mOutputListener writeLine:@"player %d has left the game", playerNr];
}

- (void) customEventAction:(int)playerNr :(nByte)eventCode :(NSObject*)eventContent
{
	// you do not receive your own events, unless you specify yourself as one of the receivers explicitly, so you must start 2 clients, to receive the events, which you have sent, as sendEvent() uses the default receivers of opRaiseEvent() (all players in same room like the sender, except the sender itself)
	EGLOG(EGDbgLvl::ALL, L"");
	[mOutputListener write:@"r"];
}

- (void) connectReturn:(int)errorCode :(NSString*)errorString
{
	EGLOG(EGDbgLvl::INFO, L"");
	if(errorCode)
	{
		EGLOG(EGDbgLvl::ERRORS, L"%ls", errorString.UTF32String);
		mStateAccessor.State = STATE_DISCONNECTING;
		return;
	}
	[mOutputListener writeLine:@"connected"];
	mStateAccessor.State = STATE_CONNECTED;
}

- (void) disconnectReturn
{
	EGLOG(EGDbgLvl::INFO, L"");
	[mOutputListener writeLine:@"disconnected"];
	mStateAccessor.State = STATE_DISCONNECTED;
}

- (void) createRoomReturn:(int)localPlayerNr :(NSDictionary*)gameProperties :(NSDictionary*)playerProperties :(int)errorCode :(NSString*)errorString
{
	EGLOG(EGDbgLvl::INFO, L"");
	if(errorCode)
	{
		EGLOG(EGDbgLvl::ERRORS, L"%ls", errorString.UTF32String);
		[mOutputListener writeLine:@"opCreateRoom() failed: %@", errorString];
		mStateAccessor.State = STATE_CONNECTED;
		return;
	}
	EGLOG(EGDbgLvl::INFO, L"localPlayerNr: %d", localPlayerNr);
	[mOutputListener writeLine:@"game room \"%@\" has been created", mLoadBalancingClient.CurrentlyJoinedRoom.Name];
	[mOutputListener write:@"regularly sending dummy events now"];
	mStateAccessor.State = STATE_JOINED;
}

- (void) joinRoomReturn:(int)localPlayerNr :(NSDictionary*)gameProperties :(NSDictionary*)playerProperties :(int)errorCode :(NSString*)errorString
{
	EGLOG(EGDbgLvl::INFO, L"");
	if(errorCode)
	{
		EGLOG(EGDbgLvl::ERRORS, L"%ls", errorString.UTF32String);
		[mOutputListener writeLine:@"opJoinRoom() failed: %@", errorString];
		mStateAccessor.State = STATE_CONNECTED;
		return;
	}
	EGLOG(EGDbgLvl::INFO, L"localPlayerNr: %d", localPlayerNr);
	[mOutputListener writeLine:@"game room \"%@\" has been successfully joined", mLoadBalancingClient.CurrentlyJoinedRoom.Name];
	[mOutputListener writeLine:@"regularly sending dummy events now"];
	mStateAccessor.State = STATE_JOINED;
}

- (void) joinRandomRoomReturn:(int)localPlayerNr :(NSDictionary*)gameProperties :(NSDictionary*)playerProperties :(int)errorCode :(NSString*)errorString
{
	EGLOG(EGDbgLvl::INFO, L"");
	if(errorCode)
	{
		EGLOG(EGDbgLvl::ERRORS, L"%ls", errorString.UTF32String);
		[mOutputListener writeLine:@"opJoinRandomRoom() failed: %@", errorString];
		mStateAccessor.State = STATE_CONNECTED;
		return;
	}
	EGLOG(EGDbgLvl::INFO, L"localPlayerNr: %d", localPlayerNr);
	[mOutputListener writeLine:@"game room \"%@\" has been successfully joined", mLoadBalancingClient.CurrentlyJoinedRoom.Name];
	[mOutputListener writeLine:@"regularly sending dummy events now"];
	mStateAccessor.State = STATE_JOINED;
}

- (void) leaveRoomReturn:(int)errorCode :(NSString*)errorString
{
	EGLOG(EGDbgLvl::INFO, L"");
	if(errorCode)
	{
		EGLOG(EGDbgLvl::ERRORS, L"%ls", errorString.UTF32String);
		[mOutputListener writeLine:@"opLeaveRoom() failed: %@", errorString];
		mStateAccessor.State = STATE_DISCONNECTING;
		return;
	}
	[mOutputListener writeLine:@"game room has been successfully left"];
	mStateAccessor.State = STATE_LEFT;
}

- (void) gotQueuedReturn
{
	EGLOG(EGDbgLvl::INFO, L"");
}

- (void) joinLobbyReturn
{
	EGLOG(EGDbgLvl::INFO, L"");
	[mOutputListener writeLine:@"joined lobby"];
}

- (void) leaveLobbyReturn
{
	EGLOG(EGDbgLvl::INFO, L"");
	[mOutputListener writeLine:@"left lobby"];
}

@end