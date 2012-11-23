//
//  SubscribeRoomGroupRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseRequest.h"


EXTERN NSString * const SubscribeRoomGroupRequest_KEY_GROUP_ID;
EXTERN NSString * const SubscribeRoomGroupRequest_KEY_ROOM_LIST;


/** Subscribe to the events of a Room Group
*/
@interface SubscribeRoomGroupRequest : BaseRequest {
	
	NSString *_groupId;
}

@property (retain) NSString *groupId;

-(id)initWithGroupId:(NSString *)groupId;
/**
 @param groupId the name of the group
 
 
 @see [ISFSEvents onSubscribeRoomGroup:]
 @see [ISFSEvents onSubscribeRoomGroupError:]
 @see UnsubscribeRoomGroupRequest
 
 */
+(id)requestWithGroupId:(NSString *)groupId;

@end
