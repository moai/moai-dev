//
//  UnsubscribeRoomGroupRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "BaseRequest.h"


EXTERN NSString * const UnsubscribeRoomGroupRequest_KEY_GROUP_ID;

/** Stop listening to events of a specific Room Group
 */
@interface UnsubscribeRoomGroupRequest : BaseRequest {
	
	NSString *_groupId;
}

@property (retain) NSString *groupId;

-(id)initWithGroupId:(NSString *)groupId;
/**
 
 @param groupId the name of the group
 
 @see [ISFSEvents onUnsubscribeRoomGroup:]
 @see [ISFSEvents onUnsubscribeRoomGroupError:]
 @see SubscribeRoomGroupRequest
 
 */
+(id)requestWithGroupId:(NSString *)groupId;

@end
