//
//  Logger.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "ISFSEvents.h"

@interface Logger : NSObject {
	BOOL _enableConsoleTrace;
	BOOL _enableEventDispatching;
	NSInteger _loggingLevel;
	id <ISFSEvents> _delegate;
}

@property (nonatomic, readwrite) BOOL enableConsoleTrace;
@property (nonatomic, readwrite) BOOL enableEventDispatching;
@property (nonatomic, readwrite) NSInteger loggingLevel;
@property (nonatomic, assign) id <ISFSEvents> delegate;

+(Logger *)getInstance;

-(void)warn:(NSString *)params;
-(void)info:(NSString *)params;
-(void)debug:(NSString *)params;
-(void)error:(NSString *)params;

@end
