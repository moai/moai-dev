// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <UIKit/UIKit.h>

#define LOG_SILENT 0
#define LOG_FATAL 1
#define LOG_EXCEPTION 2
#define LOG_NONFATAL_ERROR 3
#define LOG_DEBUG 4
#define LOG_VERBOSE 5
#define LOG_ALL 6

/*!	\interface TJCLog
 *	\brief The Tapjoy Connect Log class.
 *
 * This class is used to print log messages to the console. A log threshold should be set before attempting to log console messages to ensure that the desired messages are being printed out. Higher threshold values translates to more verbosity.
 * The print method takes a threshold value and compares it to the set threshold value to determine whether or not to print the log message to the console.
 */
@interface TJCLog : NSObject 
{

}

/*! \fn setLogThreshold: (int) myThreshhold;
 *  \brief Sets the log threshold.
 *  
 *	\param myThreshhold The log threshold determines which kind of log messages to print.
 */
+ (void) setLogThreshold: (int) myThreshhold;

/*! Displays a log message on the console.
 *  
 *	\param myLevel The level of the log message that is passed in.
 *	\param myFormat The log message type, which may include NSString format specifiers.
 *	\param ... NSString format objects.
 */
+ (void) logWithLevel: (int) myLevel format: (NSString *) myFormat, ...;
@end
