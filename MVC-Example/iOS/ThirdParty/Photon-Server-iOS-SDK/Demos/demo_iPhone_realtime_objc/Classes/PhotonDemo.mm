/**
 * PhotonDemo.m - Exit Games Photon "realtime iPhone demo"
 * Copyright (C) 2009 Exit Games GmbH
 * http://www.exitgames.com
 *
 * @author developer@exitgames.com
 */

#import "PhotonDemo.h"
#import "game.h"

const char EV_PLAYER_INFO = 0;
const char EV_MOVE        = 1;

const char STATUS_PLAYER_POS_X = 0;
const char STATUS_PLAYER_POS_Y = 1;
const char STATUS_PLAYER_NAME  = 2;
const char STATUS_PLAYER_COLOR = 3;

const char* DEFAULT_GAME_ID = "realtimeDemoGame000";

@implementation CPhotonLib

// properties

@synthesize SPS = SPS;
@synthesize RPS = RPS;

- (int) RoundTripTime
{
	return [m_pLitePeer RoundTripTime];
}

- (int) RoundTripTimeVariance
{
	return [m_pLitePeer RoundTripTimeVariance];
}

- (int) ServerTimeOffset
{
	return [m_pLitePeer ServerTimeOffset];
}



// methods

- (void) onOperationResponse:(EGOperationResponse*)operationResponse
{ 
	NSLog(@"%@", [operationResponse toString:true :true :true]);
	switch(operationResponse.OperationCode)
	{
		case ExitGames::Lite::OperationCode::JOIN:
			m_currentState = stateJoined;
			break;
		case ExitGames::Lite::OperationCode::LEAVE:
			m_currentState = stateLeft;
			break;
		default:
			break;
	}
}

- (void) onStatusChanged:(int)statusCode
{ 
	switch(statusCode)
	{
		case ExitGames::Photon::StatusCode::CONNECT:
			m_currentState = stateConnected;
			break;
		case ExitGames::Photon::StatusCode::DISCONNECT:
			m_currentState = stateDisconnected;
			break;
		default:
			break;
	}
}

- (void) onEvent:(EGEventData*)eventData
{
	// you do not receive your own events, so you must start 2 clients, to receive the events, you sent

	NSMutableDictionary* eventDataContent = nil;

	if(eventData.Code == EV_MOVE || eventData.Code == EV_PLAYER_INFO)
	{
		// custom event dat is inside an inner hash
		if(!(eventDataContent = [eventData objectForKey:[NSValue value:&ExitGames::Lite::ParameterCode::DATA withObjCType:@encode(nByte)]]))
			return;
	}

	if(eventData.Code == EV_MOVE)
	{
		receivedCount++;
		int n = 0;
		[(NSValue*)[eventData objectForKey:[NSValue value:&ExitGames::Lite::ParameterCode::ACTORNR withObjCType:@encode(nByte)]] getValue:&n];
		char x = 0;
		[(NSValue*)[eventDataContent objectForKey:[NSValue value:&STATUS_PLAYER_POS_X withObjCType:@encode(nByte)]] getValue:&x];
		char y = 0;
		[(NSValue*)[eventDataContent objectForKey:[NSValue value:&STATUS_PLAYER_POS_Y withObjCType:@encode(nByte)]] getValue:&y];

		for(int i=0; i<MAXPLAYERS; i++)
			if(mPlayer[i].Number == n)
				[mPlayer[i] move :x :y];
	}
	else if(eventData.Code == EV_PLAYER_INFO)
	{
		int n = 0;
		[(NSValue*)[eventData objectForKey:[NSValue value:&ExitGames::Lite::ParameterCode::ACTORNR withObjCType:@encode(nByte)]] getValue:&n];
		NSString* name = (NSString*)[eventDataContent objectForKey:[NSValue value:&STATUS_PLAYER_NAME withObjCType:@encode(nByte)]];
		int color = 0;
		[(NSValue*)[eventDataContent objectForKey:[NSValue value:&STATUS_PLAYER_COLOR withObjCType:@encode(nByte)]] getValue:&color];
		
		for(int i=0; i<MAXPLAYERS; i++)
			if(mPlayer[i].Number == n)
				[mPlayer[i] setInfo :name :color];
	}
	else if(eventData.Code == ExitGames::Lite::EventCode::JOIN)
	{
		int n = 0;
		[(NSValue*)[eventData objectForKey:[NSValue value:&ExitGames::Lite::ParameterCode::ACTORNR withObjCType:@encode(nByte)]] getValue:&n];
		NSArray* actors = (NSArray*)[eventData objectForKey:[NSValue value:&ExitGames::Lite::ParameterCode::ACTOR_LIST withObjCType:@encode(nByte)]];
		int amountOfActors = actors.count;

		if(mPlayer[0].Number == -1) // if local player is the joining player
		{
			mPlayer[0].Number = n;
			for(int i=0; i<amountOfActors; i++)
			{
				int temp;
				[[actors objectAtIndex:i] getValue:&temp];
				if (temp != mPlayer[0].Number) // for every existing player except local player
				{
					mPlayer[mGame.CurrentPlayers].Number = temp;
					[mGame increaseCurrentPlayers];
				}
			}
		}
		else
		{
			mPlayer[mGame.CurrentPlayers].Number = n;
			[mGame increaseCurrentPlayers];
		}
		[self sendPlayerInfo:true :mPlayer[0].Name :mPlayer[0].Color];
	}
	else if(eventData.Code == ExitGames::Lite::EventCode::LEAVE)
	{
		int n = 0;
		[(NSValue*)[eventData objectForKey:[NSValue value:&ExitGames::Lite::ParameterCode::ACTORNR withObjCType:@encode(nByte)]] getValue:&n];

		int current = mGame.CurrentPlayers;
		for(int i=0; i<current; i++)
		{
			if (mPlayer[i].Number == n)
			{
				if(i != current-1)
					mPlayer[i] = mPlayer[current-1];
				[mGame decreaseCurrentPlayers];
				current = 0;
			}
		}
	}
}

-(void) debugReturn:(int)debugLevel :(NSString*)string
{
}



-(CPhotonLib*) initWithPlayerArray:(Game*)game :(Player*[MAXPLAYERS])player
{
	self = [super init];
	mGame = game;
	mPlayer = player;
	m_pLitePeer = [[EGLitePeer alloc] initWithPhotonListener:self :false];
	m_currentState = statePhotonPeerCreated;

	// used to specify "room" on realtime-server
	mGameID = [[NSString alloc] initWithUTF8String:DEFAULT_GAME_ID];

	isInGame = false;

	sentCount = 0, receivedCount = 0, SPS = 0, RPS = 0;

	return self;
}

- (void) dealloc
{
	[mGameID release];
	[m_pLitePeer release];
	[super dealloc];
}

- (void) CreateConnection
{
	[m_pLitePeer connect:[NSString stringWithUTF32String:L"192.168.42.43:5055"]];
	m_currentState = stateConnecting;
}

- (void) CloseConnection
{
	[m_pLitePeer disconnect];
}

- (void) Run
{
	[m_pLitePeer service:true];

	switch (m_currentState)
	{
		case statePhotonPeerCreated:
			[self CreateConnection];
			break;
		case stateConnecting:
			// Waiting callback function
			break;
		case stateConnected:
			// joining game
			[m_pLitePeer opJoin:mGameID];
			m_currentState = stateJoining;
			break;
		case stateLeaving:
			[m_pLitePeer opLeave];
			break;
		case stateDisconnecting:
			[self CloseConnection];
			break;
		case stateDisconnected:
			isInGame = false;
			break;
		case stateErrorConnecting:
			isInGame = false;
			break;
		default:
			break;
	}
}

- (void) DebugReturn:(ExitGames::Common::DebugLevel::DebugLevel)debugLevel :(NSString*)string
{
}

- (void) SetState:(States) new_state
{
	m_currentState = new_state;
}

- (void) sendPlayerInfo:(bool)mode :(NSString*)name :(int)color
{
	NSMutableDictionary* ev = [[NSMutableDictionary alloc] init];

	[ev setObject:[NSString stringWithString:name] forKey:[NSValue value:&STATUS_PLAYER_NAME withObjCType:@encode(nByte)]];
	[ev setObject:[NSValue valueWithBytes:&color objCType:@encode(int)] forKey:[NSValue value:&STATUS_PLAYER_COLOR withObjCType:@encode(nByte)]];

	[m_pLitePeer opRaiseEvent :mode :ev :EV_PLAYER_INFO :0];
	[ev release];
}

- (void) sendEvMove:(bool)mode :(nByte)x :(nByte)y
{
	NSMutableDictionary* ev = [[NSMutableDictionary alloc] init];

	[ev setObject:[NSValue valueWithBytes:&x objCType:@encode(nByte)] forKey:[NSValue value:&STATUS_PLAYER_POS_X withObjCType:@encode(nByte)]];
	[ev setObject:[NSValue valueWithBytes:&y objCType:@encode(nByte)] forKey:[NSValue value:&STATUS_PLAYER_POS_Y withObjCType:@encode(nByte)]];

	[m_pLitePeer opRaiseEvent :mode :ev :EV_MOVE :0];
	[ev release];
	sentCount++;
}

- (void) opLeaveRoom
{
	// if user closes application, leave current game and close connection to server
	if(m_currentState == stateJoined)
	{
		m_currentState = stateLeaving;
		[self Run];
		while(m_currentState == stateLeaving)
			[m_pLitePeer service];
	}
	m_currentState = stateDisconnecting;
	[self Run];
	while(m_currentState == stateDisconnecting)
		[m_pLitePeer service];
}

- (void) everySecond
{
	SPS = sentCount;
	sentCount = 0;
	RPS = receivedCount;
	receivedCount = 0;
}

@end