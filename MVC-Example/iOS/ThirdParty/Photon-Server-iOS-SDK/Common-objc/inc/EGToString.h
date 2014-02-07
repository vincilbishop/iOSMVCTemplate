/* Exit Games Common - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#import <Foundation/NSString.h>

static NSString* const EG_STR_CHAR         = @"char";
static NSString* const EG_STR_SCHAR        = @"schar";
static NSString* const EG_STR_UCHAR        = @"uchar";
static NSString* const EG_STR_SHORT        = @"short";
static NSString* const EG_STR_USHORT       = @"ushort";
static NSString* const EG_STR_INT          = @"int";
static NSString* const EG_STR_UINT         = @"uint";
static NSString* const EG_STR_LONG         = @"long";
static NSString* const EG_STR_ULONG        = @"ulong";
static NSString* const EG_STR_LONGLONG     = @"longlong";
static NSString* const EG_STR_ULONGLONG    = @"ulonglong";
static NSString* const EG_STR_FLOAT        = @"float";
static NSString* const EG_STR_DOUBLE       = @"double";
static NSString* const EG_STR_LONGDOUBLE   = @"longdouble";
static NSString* const EG_STR_BOOL         = @"bool";
static NSString* const EG_STR_NSSTRING     = @"NSString";
static NSString* const EG_STR_EGARRAY      = @"EGArray";
static NSString* const EG_STR_NSARRAY      = @"NSArray";
static NSString* const EG_STR_EGDICTIONARY = @"EGDictionary";
static NSString* const EG_STR_NSDICTIONARY = @"NSDictionary";

@protocol EGToString

- (NSString*) typeToString;
- (NSString*) toString; // = false
- (NSString*) toString:(bool)withTypes;

@end



@interface NSObject (EGToString) <EGToString>

@end



@interface NSValue (EGToString)

+ (NSString*) typeToString:(const char*)typeEncoding;

@end