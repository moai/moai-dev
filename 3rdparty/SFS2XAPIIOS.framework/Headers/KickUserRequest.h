//
//  KickUserRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "BaseRequest.h"

EXTERN NSString * const KickUserRequest_KEY_USER_ID;
EXTERN NSString * const KickUserRequest_KEY_MESSAGE;
EXTERN NSString * const KickUserRequest_KEY_DELAY;

/** Kicks a User out of the server (for moderators and admins only)
 
 Allows to send a custom message and to set a delay in seconds.
 */
@interface KickUserRequest : BaseRequest {
	
@private
	NSInteger _userId;
	NSString *_message;
	NSInteger _delay;
}


-(id)initWithUserId:(NSInteger)userId message:(NSString *)message delaySeconds:(NSInteger)delaySeconds;

/**
 @param userId the id of the User to be kicked out of the server
 @param message a custom kick message, if nil, the message configured on the server side will be used
 @param delaySeconds a delay in seconds between the message (immediate) and the kick (delayed). Default is 5 seconds
 
 @see BanUserRequest
 
 */
+(id)requestWithUserId:(NSInteger)userId message:(NSString *)message delaySeconds:(NSInteger)delaySeconds;
+(id)requestWithUserId:(NSInteger)userId;

@end
