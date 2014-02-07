/**
 * player.m - Exit Games Photon "realtime iPhone demo"
 * Copyright (C) 2009 Exit Games GmbH
 * http://www.exitgames.com
 *
 * @author developer@exitgames.com
 */

#import "player.h"

const char POS_MAX = 15;

@implementation Player

@synthesize Number = number;
@synthesize PosX = posX;
@synthesize PosY = posY;
@synthesize Name = name;
@synthesize Color = color;

- (Player*) init
{
	self = [super init];
	number = -1;
	posX = 0;
	posY = 0;
	name = [[NSString alloc] initWithString:@""];
	color = 0;
	return self;
}

- (void) move :(char)x :(char)y
{
	posX = x;
	posY = y;
	if (posX < 0) posX = 0;
	if (posX >= POS_MAX) posX = POS_MAX;
	if (posY < 0) posY = 0;
	if (posY > POS_MAX) posY = POS_MAX;
}

- (void) setInfo :(NSString*)n :(int)c
{
	if(name)
		[name release];
	name = [[NSString alloc] initWithString:n];
	color = c;
}

-(void) dealloc
{
	if(name)
		[name release];
	[super dealloc];
}

@end
