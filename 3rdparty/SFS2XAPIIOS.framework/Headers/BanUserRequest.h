//
//  BanUserRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "BaseRequest.h" 

EXTERN NSString * const BanUserRequest_KEY_USER_ID;
EXTERN NSString * const BanUserRequest_KEY_MESSAGE;
EXTERN NSString * const BanUserRequest_KEY_DELAY;
EXTERN NSString * const BanUserRequest_KEY_BAN_MODE;

/** Banish a User from the system (for moderators and admins only)
 
 Allows to send a custom message and to set a delay in seconds.
 It also allows to choose banning by user name or user IP address
 */
@interface BanUserRequest : BaseRequest {
	
	NSInteger _userId;
	NSString *_message;
	NSInteger _delay;
	NSInteger _banMode;
}


/**
 @param userId (NSInteger) the id of the User to be banned
 @param message (NSString *) a custom ban message, if null the message configured on the server side will be used
 @param banMode (NSInteger) choose between banning by Ip Address or User name, see @link BanMode BanMode	
 @param delaySeconds	(NSInteger) a delay in seconds between the message (immediate) and the kick (delayed). Default is 5 seconds
 
 @see BanMode
 @see KickUserRequest
 
 
 */
-(id)initWithUserId:(NSInteger)userId message:(NSString *)message banMode:(NSInteger)banMode delaySeconds:(NSInteger)delaySeconds;

/**
 @param userId	(NSInteger) the id of the User to be banned
 @param message (NSString *) a custom ban message, if null the message configured on the server side will be used
 @param banMode (NSInteger) choose between banning by Ip Address or User name, see @link BanMode BanMode	
 @param delaySeconds (NSInteger) a delay in seconds between the message (immediate) and the kick (delayed). Default is 5 seconds
 
 
 @see BanMode
 @see KickUserRequest
 
 
 */
+(id)requestWithUserId:(NSInteger)userId message:(NSString *)message banMode:(NSInteger)banMode delaySeconds:(NSInteger)delaySeconds;

/**
 @param userId (NSInteger) the id of the User to be banned
 
 @see BanMode
 @see KickUserRequest
 
 */
+(id)requestWithUserId:(NSInteger)userId;

@end
