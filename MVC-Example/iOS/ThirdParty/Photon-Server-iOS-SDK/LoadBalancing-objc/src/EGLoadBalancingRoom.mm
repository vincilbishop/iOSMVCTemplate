/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#import <Foundation/NSException.h>
#include "LoadBalancing-cpp/inc/Internal/Enums/Properties/Room.h"
#import "LoadBalancing-objc/inc/Internal/EGLoadBalancingPeer+Internal.h"
#import "LoadBalancing-objc/inc/Internal/EGLoadBalancingRoom+Internal.h"
#import "LoadBalancing-objc/inc/Protected/EGLoadBalancingPlayer+Protected.h"
#import "LoadBalancing-objc/inc/Protected/EGLoadBalancingRoom+Protected.h"

/** @file EGLoadBalancingRoom.h */

using namespace ExitGames::LoadBalancing;
using namespace ExitGames::LoadBalancing::Internal;

@interface EGLoadBalancingRoom ()

- (NSString*) payloadToString:(bool)withTypes :(bool)withCustomProperties;

@end



@implementation EGLoadBalancingRoom

@synthesize Name = mName;
@synthesize PlayerCount = mPlayerCount;
@synthesize MaxPlayers = mMaxPlayers;
@synthesize IsOpen = mIsOpen;

- (NSDictionary*) CustomProperties
{
	return [[mCustomProperties copyDeep] autorelease];
}



- (instancetype) init
{
	EG_ASSERT(false, @"not implemented");
	return nil;
}

- (instancetype) initLoadBalancingRoom:(NSString*)name :(NSDictionary*)properties
{
	self = [super init];
	mName = [name retain];
	mIsOpen = true;
	mCustomProperties = [[NSMutableDictionary alloc] init];
	return [self cacheProperties:properties];
}

+ (instancetype) loadBalancingRoom:(NSString*)name :(NSDictionary*)properties
{
	return [[[self alloc] initLoadBalancingRoom:name :properties] autorelease];
}

- (instancetype) cacheProperties:(NSDictionary*)properties
{
	if([properties objectForKey:[NSValue value:&Properties::Room::MAX_PLAYERS withObjCType:@encode(nByte)]]) [[properties objectForKey:[NSValue value:&Properties::Room::MAX_PLAYERS withObjCType:@encode(nByte)]] getValue:&mMaxPlayers];
	if([properties objectForKey:[NSValue value:&Properties::Room::IS_OPEN withObjCType:@encode(nByte)]]) [[properties objectForKey:[NSValue value:&Properties::Room::IS_OPEN withObjCType:@encode(nByte)]] getValue:&mIsOpen];
	if([properties objectForKey:[NSValue value:&Properties::Room::PLAYER_COUNT withObjCType:@encode(nByte)]]) [[properties objectForKey:[NSValue value:&Properties::Room::PLAYER_COUNT withObjCType:@encode(nByte)]] getValue:&mPlayerCount];
	[mCustomProperties addEntriesFromDictionary:[EGLoadBalancingPeer stripToCustomProperties:properties]];
	mCustomProperties = [[EGLoadBalancingPeer stripKeysWithNullValues:[mCustomProperties autorelease]] retain];
	return self;
}

- (NSString*) toString:(bool)withTypes
{
	return [self toString:withTypes :false];
}

- (NSString*) toString:(bool)withTypes :(bool)withCustomProperties
{
	return [NSString stringWithFormat:@"%@={%@}", mName, [self payloadToString:withTypes :withCustomProperties]];
}

- (NSString*) payloadToString:(bool)withTypes :(bool)withCustomProperties
{
	NSString* res = [NSString stringWithFormat:@"pl: %d, max: %d, open: %@", mPlayerCount, mMaxPlayers, mIsOpen?@"true":@"false"];
	if(withCustomProperties && mCustomProperties.count)
		res = [res stringByAppendingFormat:@", props: %@", [mCustomProperties toString:withTypes]];
	return res;
}

- (BOOL) isEqual:(id)object
{
	return [object isKindOfClass:[EGLoadBalancingRoom class]] && [self.Name compare:((EGLoadBalancingRoom*)object).Name] == NSOrderedSame;
}

- (NSUInteger)hash
{
	return [self.Name hash];
}

- (void) dealloc
{
	[mCustomProperties release];
	[mName release];
	[super dealloc];
}

@end



@interface EGLoadBalancingMutableRoom ()

- (NSString*) payloadToString:(bool)withTypes :(bool)withCustomProperties :(bool)withPlayers;

@end



@implementation EGLoadBalancingMutableRoom

- (nByte) PlayerCount
{
	return self.Players.count;
}

- (nByte) MaxPlayers
{
	return super.MaxPlayers;
}

- (void) setMaxPlayers:(nByte)maxPlayers
{
	[mLoadBalancingPeer opSetPropertiesOfRoom:[NSDictionary dictionaryWithObject:[NSValue value:&(mMaxPlayers=maxPlayers) withObjCType:@encode(nByte)] forKey:[NSValue value:&Properties::Room::MAX_PLAYERS withObjCType:@encode(nByte)]]];
}

- (bool) IsOpen
{
	return super.IsOpen;
}

- (void) setIsOpen:(bool)isOpen
{
	[mLoadBalancingPeer opSetPropertiesOfRoom:[NSDictionary dictionaryWithObject:[NSValue value:&(mIsOpen=isOpen) withObjCType:@encode(bool)] forKey:[NSValue value:&Properties::Room::IS_OPEN withObjCType:@encode(nByte)]]];
}

- (bool) IsVisible
{
	return mIsVisible;
}

- (void) setIsVisible:(bool)isVisible
{
	[mLoadBalancingPeer opSetPropertiesOfRoom:[NSDictionary dictionaryWithObject:[NSValue value:&(mIsVisible=isVisible) withObjCType:@encode(bool)] forKey:[NSValue value:&Properties::Room::IS_VISIBLE withObjCType:@encode(nByte)]]];
}

@synthesize Players = mPlayers;

- (void) setPlayers:(EGMutableArray*)players
{
	[mPlayers release];
	mPlayers = [players mutableCopy];
}

@synthesize MasterClientID = mMasterClientID;

- (EGArray*) PropsListedInLobby
{
	return [[mPropsListedInLobby copyDeep] autorelease];
}

- (void) setPropsListedInLobby:(EGArray*)propsListedInLobby
{
	if(mPropsListedInLobby != propsListedInLobby && ![mPropsListedInLobby isEqual:propsListedInLobby])
	{
		[mPropsListedInLobby release];
		[mLoadBalancingPeer opSetPropertiesOfRoom:[NSDictionary dictionaryWithObject:mPropsListedInLobby=[propsListedInLobby.Type isEqual:NSStringFromClass([NSString class])]?[[propsListedInLobby copyDeep] autorelease]:[EGArray arrayWithType:NSStringFromClass([NSString class])] forKey:[NSValue value:&Properties::Room::PROPS_LISTED_IN_LOBBY withObjCType:@encode(nByte)]]];
	}
}




- (instancetype) initLoadBalancingRoom:(NSString*)name :(NSDictionary*)properties
{
	EG_ASSERT(false, @"not implemented");
	return nil;
}

- (instancetype) initLoadBalancingRoom:(NSString*)name :(NSDictionary*)properties :(EGLoadBalancingPeer*)peer :(EGArray*)propsListedInLobby
{
	self = [super initLoadBalancingRoom:name :properties];
	mLoadBalancingPeer = peer;
	mIsVisible = true;
	mPlayers = [[EGMutableArray alloc] initWithType:NSStringFromClass([EGLoadBalancingPlayer class])];
	mMasterClientID = 0;
	mPropsListedInLobby = [propsListedInLobby mutableCopyDeep];
	return self;
}

+ (instancetype) loadBalancingRoom:(NSString*)name :(NSDictionary*)properties
{
	return [[[self alloc] initLoadBalancingRoom:name :properties] autorelease];
}

+ (instancetype) loadBalancingRoom:(NSString*)name :(NSDictionary*)properties :(EGLoadBalancingPeer*)peer :(EGArray*)propsListedInLobby
{
	return [[[self alloc] initLoadBalancingRoom:name :properties :peer :propsListedInLobby] autorelease];
}

- (instancetype) cacheProperties:(NSDictionary*)properties
{
	if([properties objectForKey:[NSValue value:&Properties::Room::IS_VISIBLE withObjCType:@encode(nByte)]]) [[properties objectForKey:[NSValue value:&Properties::Room::IS_VISIBLE withObjCType:@encode(nByte)]] getValue:&mIsVisible];
	return [super cacheProperties:properties];
}

- (EGLoadBalancingPlayer*) getPlayerForNumber:(int)playerNumber
{
	for(unsigned i=0; i<mPlayers.count; i++)
		if(((EGLoadBalancingPlayer*)[mPlayers objectAtIndex:i]).Number == playerNumber)
			return [mPlayers objectAtIndex:i];
	return nil;
}

- (void) mergeCustomProperties:(NSDictionary*)customProperties
{
	if(mCustomProperties == customProperties)
		return;
	NSDictionary* stripDict = [EGLoadBalancingPeer stripToCustomProperties:customProperties];
	if(!stripDict.count)
		return;
	NSDictionary* oldDict = [[mCustomProperties copyDeep] autorelease];
	[mCustomProperties addEntriesFromDictionary:stripDict];
	mCustomProperties = [[EGLoadBalancingPeer stripKeysWithNullValues:[mCustomProperties autorelease]] retain];
	if(![mCustomProperties isEqual:oldDict])
		[mLoadBalancingPeer opSetPropertiesOfRoom:stripDict];	
}

- (void) addCustomProperty:(NSString*)key :(id)value
{
	[self addCustomProperties:[NSDictionary dictionaryWithObject:value forKey:key]];
}

- (void) addCustomProperties:(NSDictionary*)customProperties
{
	[self mergeCustomProperties:[EGLoadBalancingPeer stripKeysWithNullValues:customProperties]];
}

- (void) removeCustomProperty:(NSString*)key
{
	[self removeCustomProperties:&key :1];
}

- (void) removeCustomProperties:(NSString**)keys :(NSUInteger)count
{
	NSNull** vals = new NSNull*[count];
	for(NSUInteger i=0; i<count; i++)
		vals[i] = [NSNull null];
	[self mergeCustomProperties:[NSDictionary dictionaryWithObjects:vals forKeys:keys count:count]];
	delete[] vals;
}

- (void) removeAllPlayers
{
	[mPlayers removeAllObjects];
}

- (void) addPlayer:(EGLoadBalancingPlayer*)player
{
	[mPlayers addObject:player];
	if(!mMasterClientID || player.Number < mMasterClientID)
		mMasterClientID = player.Number;
}

- (void) addPlayer:(int)number :(NSDictionary*)properties
{
	[self addPlayer:[self createPlayer:number :properties]];
}

- (bool) removePlayer:(int)number
{
	bool retVal = false;
	for(unsigned int i=0; i<mPlayers.count; ++i)
	{
		if([[mPlayers objectAtIndex:i] Number] == number)
		{
			[mPlayers removeObjectAtIndex:i];
			retVal = true;
		}
	}

	if(number == mMasterClientID)
	{
		mMasterClientID = mPlayers.count?[[mPlayers objectAtIndex:0] Number]:0;
		for(unsigned int i=1; i<mPlayers.count; ++i)
			if([[mPlayers objectAtIndex:i] Number] < mMasterClientID)
				mMasterClientID = [[mPlayers objectAtIndex:i] Number];
	}
	return retVal;
}

- (EGLoadBalancingPlayer*) createPlayer:(int)number :(NSDictionary*)properties
{
	return [EGLoadBalancingPlayer loadBalancingPlayer:number :properties :self];
}

- (NSString*) toString:(bool)withTypes :(bool)withCustomProperties :(bool)withPlayers
{
	return [NSString stringWithFormat:@"%@={%@}", mName, [self payloadToString:withTypes :withCustomProperties :withPlayers]];
}

- (NSString*) payloadToString:(bool)withTypes :(bool)withCustomProperties :(bool)withPlayers
{
	NSMutableString* res = [NSMutableString stringWithFormat:@"%@, visible: %@, masterClient: %d", [super payloadToString:false :false], mIsVisible?@"true":@"false", mMasterClientID];
	if(mPropsListedInLobby.count)
		[res appendFormat:@", propsListedInLobby: %@", [mPropsListedInLobby toString:withTypes]];
	if(withCustomProperties && mCustomProperties.count)
		[res appendFormat:@", props: %@", [mCustomProperties toString:withTypes]];
	if(withPlayers && mPlayers.count)
	{
		[res appendFormat:@", players:"];
		for(NSUInteger i=0; i<mPlayers.count; ++i)
			[res appendFormat:@" %@%@", [[mPlayers objectAtIndex:i] toString:withTypes :withCustomProperties], i<mPlayers.count-1?@",":@""];
	}
	return res;
}

- (void) dealloc
{
	[mPropsListedInLobby release];
	[mPlayers release];
	[super dealloc];
}

@end