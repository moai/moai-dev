//
//  BaseEvent.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "Common.h"
/**
 This is the Base class of all Events in the API

 See <SFSEvent>, <ISFSEvent>, and <SFSBuddyEvent>.
 
 */
@interface BaseEvent : NSObject {
@protected
	NSString *_type;
	NSDictionary *_params;
}

/** An Object containing the event's parameters
 */
@property (nonatomic, retain) NSDictionary *params;

/** A string defining the event type
 */
@property (nonatomic, retain) NSString *type;

-(id)initWithType:(NSString *)type params:(NSDictionary *)params;


@end
