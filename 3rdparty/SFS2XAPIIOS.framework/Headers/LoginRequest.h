//
//  LoginRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseRequest.h"

EXTERN NSString * const LoginRequest_KEY_ZONE_NAME;
EXTERN NSString * const LoginRequest_KEY_USER_NAME;
EXTERN NSString * const LoginRequest_KEY_PASSWORD;
EXTERN NSString * const LoginRequest_KEY_PARAMS;
EXTERN NSString * const LoginRequest_KEY_PRIVILEGE_ID;
EXTERN NSString * const LoginRequest_KEY_ID;
EXTERN NSString * const LoginRequest_KEY_ROOMLIST;
EXTERN NSString * const LoginRequest_KEY_RECONNECTION_SECONDS;


/** Join one of the Zones in the Server.
 
 Each Zone represents a different application running independently
 In order to join a Zone a user name and password are usually required.
 If the Zone allows guest users, user name and password are not needed and a guest name will be created on the server side
 If the Zone uses a server side extension to handle the login, you can also send a custom <SFSObject> with any extra parameters.
 */ 
@interface LoginRequest : BaseRequest {

@private
	
	NSString *_zoneName;
	NSString *_userName;
	NSString *_password;
	id <ISFSObject> _params;
}


@property (retain) NSString *zoneName;
@property (retain) NSString *userName;
@property (retain) NSString *password;
@property (retain) id <ISFSObject> params;

-(id)initWithUserName:(NSString *)userName password:(NSString *)password zoneName:(NSString *)zoneName params:(id <ISFSObject>)params;
/**
 @param userName the user name.
 @param password the password.
 @param zoneName the name of an existing Zone in the server (case sensitive, 'SimpleChat' != 'simplechat'). If no Zone is specified the SmartFox.config.zone parameter will be used.
 @param params the custom parameters (optional, if you use an Extension on server side to handle the login)
 
 @see [ISFSEvents onLogin:]
 @see [ISFSEvents onLoginError:]
 @see [SmartFox2XClient loadConfig:connectOnSuccess:]
 
 */
+(id)requestWithUserName:(NSString *)userName password:(NSString *)password zoneName:(NSString *)zoneName params:(id <ISFSObject>)params;
+(id)request;

@end
