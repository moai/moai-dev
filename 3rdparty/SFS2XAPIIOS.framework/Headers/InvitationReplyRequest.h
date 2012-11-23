//
//  InvitationReplyRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "BaseRequest.h"
#import "Invitation.h"
#import "InvitationReply.h"

EXTERN NSString * const InvitationReplyRequest_KEY_INVITATION_ID;
EXTERN NSString * const InvitationReplyRequest_KEY_INVITATION_REPLY;
EXTERN NSString * const InvitationReplyRequest_KEY_INVITATION_PARAMS;

/** Sends a reply to an Invitation.
 
 The reply can be either an ACCEPT or REFUSE. If the reply doesn't get to the inviter within the expected amount of time the system
 will handle it as a REFUSE.
 */
@interface InvitationReplyRequest : BaseRequest {
@private		
	id <Invitation> _invitation;
	NSInteger _reply;
	id <ISFSObject> _params;
}

@property (nonatomic, retain) id <Invitation> invitation;
@property (nonatomic, assign) NSInteger reply;
@property (nonatomic, retain) id <ISFSObject> params;

-(id)initWithInvitation:(id <Invitation>)invitation invitationReply:(NSInteger)invitationReply params:(id <ISFSObject>)params;

/**
 @param invitation a reference to the received invitation
 @param invitationReply the reply code
 @param params optional custom reply parameters 
 
 
 @see [ISFSEvents onInvitationReplyError:]
 @see SFSInvitation
 @see InvitationReply
 
 */
+(id)requestWithInvitation:(id <Invitation>)invitation invitationReply:(NSInteger)invitationReply params:(id <ISFSObject>)params;

@end
