//
//  SFSInvitation.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "Invitation.h"

/** An <b>Invitation</b> object provides the ability to send a generic invitation for a specific event going on
 in your application, be it a game, visiting a personal Room or another location of the virtual world etc...
 
 The SFS2X <b>Invitation Manager</b> is very simple to use. All you need to provide is:

 - <b>inviter:</b> the User starting the invitation (null == Server)
 - <b>invitee:</b> the User to invite
 - <b>secondsForAnswer:</b> the amount of time (in seconds) that the invitee has to accept/refuse the invitation. (default is 15 sec)
 - <b>params:</b> any custom params to send with the invitation (e.g. a message, game name etc...)
 
 See <InviteUsersRequest>
 */
@interface SFSInvitation : NSObject <Invitation>{
@protected
	NSInteger _id;
	id <User> _inviter;
	id <User> _invitee;
	NSInteger _secondsForAnswer;
	id <ISFSObject> _params;
}
/**
 @param inviter the User that sends the Invitation
 @param invitee the User that receives the Invitation
 @param secondsForAnswer the number of seconds allowed for the Invitee to reply (recommended range: 15-40 seconds)
 @param params custom params to send with the invitation
 */
-(id)initWithInviter:(id <User>)inviter invitee:(id <User>)invitee secondsForAnswer:(NSInteger)secondsForAnswer params:(id <ISFSObject>)params;
/**
 @param inviter the User that sends the Invitation
 @param invitee the User that receives the Invitation
 @param secondsForAnswer the number of seconds allowed for the Invitee to reply (recommended range: 15-40 seconds)
 @param params custom params to send with the invitation
 */
+(id)invitationWithInviter:(id <User>)inviter invitee:(id <User>)invitee secondsForAnswer:(NSInteger)secondsForAnswer params:(id <ISFSObject>)params;
/**
 @param inviter the User that sends the Invitation
 @param invitee the User that receives the Invitation
 */
+(id)invitationWithInviter:(id <User>)inviter invitee:(id <User>)invitee;
/** 
 The id of the invitation 
 */
@property (nonatomic, assign) NSInteger id;
/** 
 The inviter User 
 */
@property (nonatomic, readonly) id <User> inviter;
/** 
 The invited User 
 */
@property (nonatomic, readonly) id <User> invitee;
/** 
 The amount of time (seconds) allowed to reply to the invitation 
 */
@property (nonatomic, readonly) NSInteger secondsForAnswer;
/** 
 A custom set of parameters. 
 Can be used to provide a message for the invitation and any other relevant data 
 */
@property (nonatomic, readonly) id <ISFSObject> params;

@end
