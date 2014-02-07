/**
 * player.h - Exit Games Photon "realtime iPhone demo"
 * Copyright (C) 2009 Exit Games GmbH
 * http://www.exitgames.com
 *
 * @author developer@exitgames.com
 */

#import <Foundation/Foundation.h>

#define MAXPLAYERS 1024

@interface Player : NSObject
{
@private
	int number;
	char posX;
	char posY;
	NSString* name;
	int color;
}

@property (readwrite) int Number;
@property (readonly) char PosX;
@property (readonly) char PosY;
@property (readonly) NSString* Name;
@property (readonly) int Color;

- (void) move :(char)x :(char)y;
- (void) setInfo :(NSString*)n :(int)c;

@end
