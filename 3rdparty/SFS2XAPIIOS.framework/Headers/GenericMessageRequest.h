//
//  GenericMessageRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseRequest.h"
#import "Room.h"
#import "User.h"

EXTERN NSString * const GenericMessageRequest_KEY_ROOM_ID;
EXTERN NSString * const GenericMessageRequest_KEY_USER_ID;
EXTERN NSString * const GenericMessageRequest_KEY_MESSAGE;
EXTERN NSString * const GenericMessageRequest_KEY_MESSAGE_TYPE;
EXTERN NSString * const GenericMessageRequest_KEY_RECIPIENT;
EXTERN NSString * const GenericMessageRequest_KEY_RECIPIENT_MODE;
EXTERN NSString * const GenericMessageRequest_KEY_XTRA_PARAMS;
EXTERN NSString * const GenericMessageRequest_KEY_SENDER_DATA;

@class Logger;

@interface GenericMessageRequest : BaseRequest {
		
@protected
	
	NSInteger _type;
	id <Room> _room;
	id <User> _user;
	NSString *_message;
	id <ISFSObject> _params;
	id _recipient;
	NSInteger _sendMode;
	Logger *_log;
}

@property (retain) id <Room> room;
@property (retain) id <User> user;
@property (retain) NSString *message;
@property (retain) id <ISFSObject> params;
@property (retain) id recipient;
@property (retain) Logger *log;

@end
