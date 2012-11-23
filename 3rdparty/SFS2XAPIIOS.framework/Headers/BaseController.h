//
//  BaseController.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "IController.h"

@class Logger;

@interface BaseController : NSObject <IController>{
@protected
	NSInteger _id; 
	Logger *_log;
}

@property (nonatomic, readwrite) NSInteger id; 
@property (nonatomic, retain) Logger *log;

@end
