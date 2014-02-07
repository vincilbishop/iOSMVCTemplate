/**
 * game.m - Exit Games Photon "realtime iPhone demo"
 * Copyright (C) 2009 Exit Games GmbH
 * http://www.exitgames.com
 *
 * @author developer@exitgames.com
 */

#import "game.h"
#import "PhotonDemo.h"

@implementation Game

// properties

@synthesize CurrentPlayers = currentPlayers;

-(int) RoundTripTime
{
	return [mPhoton RoundTripTime];
}

-(int) RoundTripTimeVariance
{
	return [mPhoton RoundTripTimeVariance];
}

-(int) ServerTimeOffset
{
	return [mPhoton ServerTimeOffset];
}

-(int) SPS
{
	return mPhoton.SPS;
}

-(int) RPS
{
	return mPhoton.RPS;
}



// methods

- (Game*) initWithView:(EAGLView*)GLView
{
	self = [super init];
	srandom(GETUPTIMEMS());
	mGLView = GLView;
	for(int i=0; i<MAXPLAYERS; i++)
		mPlayer[i] = [[Player alloc] init];
	[mPlayer[0] setInfo:@"iPhone objC" :random()];
	[self increaseCurrentPlayers];
	mPhoton = [[CPhotonLib alloc] initWithPlayerArray:self :mPlayer];
	[mGLView setPlayerArray:self :mPlayer];
	return self;
}

- (void) run
{
	[mPhoton sendEvMove:false :mPlayer[0].PosX :mPlayer[0].PosY];
	[mPhoton Run];
}

- (void) automove
{
	char posX = mPlayer[0].PosX, posY = mPlayer[0].PosY;
	switch(rand()%4)
	{
		case 0:
			posX--;
			break;
		case 1:
			posY++;
			break;
		case 2:
			posX++;
			break;
		case 3:
		default:
			posY--;
			break;
	}
	[mPlayer[0] move:posX :posY];
}

-(void) dealloc
{
	if(mPhoton)
		[mPhoton release];
	for(int i=0; i<MAXPLAYERS; i++)
		if(mPlayer[i])
			[mPlayer[i] release];
	[super dealloc];
}

- (void) increaseCurrentPlayers
{
	currentPlayers++;
}

- (void) decreaseCurrentPlayers
{
	currentPlayers--;
}

- (void) opLeaveRoom
{
	[mPhoton opLeaveRoom];
}

- (void) everySecond
{
	[mPhoton everySecond];
}


@end
