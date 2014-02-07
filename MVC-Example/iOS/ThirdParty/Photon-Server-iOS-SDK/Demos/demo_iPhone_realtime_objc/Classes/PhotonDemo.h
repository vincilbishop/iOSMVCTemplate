/**
 * PhotonDemo.h - Exit Games Photon "realtime iPhone demo"
 * Copyright (C) 2009 Exit Games GmbH
 * http://www.exitgames.com
 *
 * @author developer@exitgames.com
 */

#import "Photon-objc/inc/EGLitePeer.h"
#import "player.h"

@class Game;

// internal states
typedef enum States
{
	statePhotonPeerCreated,
	stateConnecting,
	stateConnected,
	stateJoining,
	stateErrorJoining,
	stateJoined,
	stateLeaving,
	stateErrorLeaving,
	stateLeft,
	stateErrorConnecting,
	stateReceiving,
	stateDisconnecting,
	stateDisconnected,
} States;

@interface CPhotonLib : NSObject <EGPhotonListener>
{
@private
	States m_currentState;

	EGLitePeer* m_pLitePeer;
	Game* mGame;
	Player** mPlayer;

	NSString* mGameID;
	bool isInGame;
	
	int sentCount, receivedCount, SPS, RPS;
}

@property (readonly) int RoundTripTime;
@property (readonly) int RoundTripTimeVariance;
@property (readonly) int ServerTimeOffset;
@property (readonly) int SPS;
@property (readonly) int RPS;

-(CPhotonLib*) initWithPlayerArray:(Game*)game :(Player*[MAXPLAYERS])player;
-(void) Run;
-(void) CreateConnection;
-(void) CloseConnection;
-(void) sendPlayerInfo:(bool)mode :(NSString*)name :(int)color;
-(void) sendEvMove:(bool)mode :(nByte)x :(nByte)y;
-(void) opLeaveRoom;
-(void) everySecond;

@end