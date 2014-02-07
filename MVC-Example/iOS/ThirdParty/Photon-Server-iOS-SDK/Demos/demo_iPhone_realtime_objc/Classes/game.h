/**
 * game.h - Exit Games Photon "realtime iPhone demo"
 * Copyright (C) 2009 Exit Games GmbH
 * http://www.exitgames.com
 *
 * @author developer@exitgames.com
 */

#import "EAGLView.h"
#import "player.h"

@class CPhotonLib;

@interface Game : NSObject
{
@private
	CPhotonLib* mPhoton;
	EAGLView* mGLView;

	Player* mPlayer[MAXPLAYERS];
	int currentPlayers;
}

@property (readonly) int CurrentPlayers;
@property (readonly) int RoundTripTime;
@property (readonly) int RoundTripTimeVariance;
@property (readonly) int ServerTimeOffset;
@property (readonly) int SPS;
@property (readonly) int RPS;

- (Game*) initWithView:(EAGLView*)GLView;
- (void) run;
- (void) automove;
- (void) increaseCurrentPlayers;
- (void) decreaseCurrentPlayers;
- (void) opLeaveRoom;
- (void) everySecond;

@end