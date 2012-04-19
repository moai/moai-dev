// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import <Foundation/Foundation.h>


/*!	\interface TJCUtil
 *	\brief The Tapjoy Connect Utilities class.
 *
 * This class contains various utility methods for which to get and set different properties of the Tapjoy Connect system.
 */
@interface TJCUtil : NSObject 
{
	
}

/*!	\fn caseInsenstiveCompare:AndString2:(NSString *str1, NSString *str2);
 *	\brief Compares two strings, ignoring case.
 *  
 *	\param str1 The first string to compare.
 *	\param str2 The second string to compare.
 *  \return TRUE if the strings are equal, FALSE otherwise.
 */
+(BOOL) caseInsenstiveCompare:(NSString*)str1 AndString2:(NSString*)str2 ;


+ (NSData *) dataWithBase64EncodedString:(NSString *) string; 


+ (id) initWithBase64EncodedString:(NSString *) string; 


+ (BOOL)isPad;

@end
