//
//  Invitation.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "User.h"

/**
 Represents an Invitation object
 For more details see the SFSInvitation implementation
 
 See <SFSInvitation>
 */
@protocol Invitation <NSObject>

/** 
 The id of the invitation 
 */
-(NSInteger)id;
-(void)setId:(NSInteger)value;

/** 
 The inviter User 
 */
-(id <User>)inviter;
/** 
 The invited User 
 */
-(id <User>)invitee;

/** 
 The amount of time (seconds) allowed to reply to the invitation 
 */
-(NSInteger)secondsForAnswer;
/** 
 A custom set of parameters. 
 Can be used to provide a message for the invitation and any other relevant data 
 */
-(id <ISFSObject>)params;

@end