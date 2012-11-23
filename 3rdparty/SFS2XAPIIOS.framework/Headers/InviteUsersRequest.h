//
//  InviteUsersRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "BaseRequest.h"


EXTERN NSString * const InviteUsersRequest_KEY_USER;
EXTERN NSString * const InviteUsersRequest_KEY_USER_ID;
EXTERN NSString * const InviteUsersRequest_KEY_INVITATION_ID;
EXTERN NSString * const InviteUsersRequest_KEY_TIME;
EXTERN NSString * const InviteUsersRequest_KEY_PARAMS;
EXTERN NSString * const InviteUsersRequest_KEY_INVITEE_ID;
EXTERN NSString * const InviteUsersRequest_KEY_INVITED_USERS;
EXTERN NSString * const InviteUsersRequest_KEY_REPLY_ID;
EXTERN NSInteger const InviteUsersRequest_MAX_INVITATIONS_FROM_CLIENT_SIDE;
EXTERN NSInteger const InviteUsersRequest_MIN_EXPIRY_TIME;
EXTERN NSInteger const InviteUsersRequest_MAX_EXPIRY_TIME;


/**
 Send one or more generic invitations to a list of Users.
 Invitations can be used for different purposes such as playing games, visiting a specific Room, Buddy requests etc...
 
 See <SFSInvitation>
 */
@interface InviteUsersRequest : BaseRequest {
@private
	NSArray *_invitedUsers;
	NSInteger _secondsForAnswer;
	id <ISFSObject> _params;
}

-(id)initWithInvitedUsers:(NSArray *)invitedUsers secondsForAnswer:(NSInteger)secondsForAnswer params:(id <ISFSObject>)params;
/**
 @param invitedUsers a list of invited Users
 @param secondsForAnswer the amount of seconds allowed to the invited User to reply (recommended range 15-40 seconds)
 @param params optional custom invitation parameters 
 
 @see [ISFSEvents onInvitationReply:]
 @see SFSInvitation
 @see InvitationReply
 
 */
+(id)requestWithInvitedUsers:(NSArray *)invitedUsers secondsForAnswer:(NSInteger)secondsForAnswer params:(id <ISFSObject>)params;

@property (nonatomic, retain) NSArray *invitedUsers;
@property (nonatomic, assign) NSInteger secondsForAnswer;
@property (nonatomic, retain) id <ISFSObject> params;

@end
