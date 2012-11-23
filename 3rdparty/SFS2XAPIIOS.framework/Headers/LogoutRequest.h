//
//  LogoutRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseRequest.h"

EXTERN NSString * const LogoutRequest_KEY_ZONE_NAME;

/** Log out of the current Zone
 
 This will not shut down the connection and User will be able to login again in the same Zone or another one
 */
@interface LogoutRequest : BaseRequest {

}

/**
 
 @see [ISFSEvents onLogout:]
 @see LoginRequest
 
 */
+(id)request;

@end
