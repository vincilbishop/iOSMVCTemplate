/* Exit Games Common - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */
 
#pragma once

#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import "Common-objc/inc/EGArray.h"

@interface EGDictionary : NSDictionary
{
@protected
	NSDictionary* mData;
	EGArray* mKeyTypes;
	EGArray* mValTypes;
	EGArray* mDimensions;
}

@property (readonly) NSDictionary* Data;
@property (readonly) EGArray* KeyTypes;
@property (readonly) EGArray* ValTypes;
@property (readonly) EGArray* Dimensions;
@property (readonly) NSString* KeyType;
@property (readonly) NSString* ValType;
@property (readonly) const char* CKeyType;
@property (readonly) const char* CValType;
@property (readonly) NSString* Type;
@property (readonly) const char* CType;

- (instancetype) initWithKeyType:(NSString*)keyType :(NSString*)valType;
- (instancetype) initWithCKeyType:(const char*)keyType :(const char*)valType;
- (instancetype) initWithKeyTypes:(NSString* const[])keys valueTypes:(NSString* const[])vals count:(NSUInteger)cnt;
+ (instancetype) dictionaryWithKeyType:(NSString*)keyType valueType:(NSString*)valType;
+ (instancetype) dictionaryWithCKeyType:(const char*)keyType valueType:(const char*)valType;
+ (NSString*) getTypeFromObj:(id)obj;
+ (const char* const) getCTypeFromObj:(id)obj;
+ (instancetype) dictionaryWithKeyTypes:(NSString* const[])keys valueTypes:(NSString* const[])vals count:(NSUInteger)cnt;

+ (instancetype) dictionaryWithKeyType:(NSString*)keyType valueType:(NSString*)valType valueDimensions:(unsigned int)dimension;
- (instancetype) initWithKeyType:(NSString*)keyType valueType:(NSString*)valType valueDimensions:(unsigned int)dimension;
+ (instancetype) dictionaryWithKeyTypes:(NSString* const[])keys valueTypes:(NSString* const[])vals valueDimensions:(const unsigned int*)dimension count:(NSUInteger)cnt;
- (instancetype) initWithKeyTypes:(NSString* const[])keys valueTypes:(NSString* const[])vals valueDimensions:(const unsigned int*)dimension count:(NSUInteger)cnt;

@end


@interface EGMutableDictionary : EGDictionary

@property (readwrite, copy) NSMutableDictionary* MutableData;

- (void)setObject:(id)anObject forKey:(id)aKey;
- (void)setValue:(id)value forKey:(NSString*)key;
- (void)addEntriesFromDictionary:(NSDictionary *)otherDictionary;
- (void)setDictionary:(NSDictionary*)otherDictionary;
- (void)removeObjectForKey:(id)aKey;
- (void)removeAllObjects;
- (void)removeObjectsForKeys:(NSArray*)keyArray;

+ (id)dictionaryWithCapacity:(NSUInteger)numItems;
- (instancetype) initWithCapacity:(NSUInteger)numItems;

@end