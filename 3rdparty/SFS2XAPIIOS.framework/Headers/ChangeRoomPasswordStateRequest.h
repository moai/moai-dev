//
//  ChangeRoomPasswordStateRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseRequest.h"
#import "Room.h"

EXTERN NSString * const ChangeRoomPasswordStateRequest_KEY_ROOM;
EXTERN NSString * const ChangeRoomPasswordStateRequest_KEY_PASS;

/** Changes the password of a Room at runtime.
 
 The request not only changes the password of a Room but also its "password state", which indicates if a Room is password protected or not.
 This request might fail if the Room was configured to deny password runtime changes or if the User sending the request
 is not the owner of the Room. 
 
 Moderators and Administrator can override this last constraint.
*/ 
@interface ChangeRoomPasswordStateRequest : BaseRequest {
	
	id<Room> _room;
	NSString *_newPass;

}

-(id)initWithRoom:(id<Room>)room newPass:(NSString *)newPass;

/**
 @param room the Room 
 @param newPass the new password (null or empty string means no password)
 
 @see [ISFSEvents onRoomPasswordStateChange:]
 @see [ISFSEvents onRoomPasswordStateChangeError
 @see ChangeRoomNameRequest
 @see ChangeRoomPasswordStateRequest
 
 */
+(id)requestWithRoom:(id<Room>)room newPass:(NSString *)newPass;

@end
