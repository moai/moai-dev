//
//  InitBuddyListRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "BaseRequest.h"

EXTERN NSString * const InitBuddyListRequest_KEY_BLIST;
EXTERN NSString * const InitBuddyListRequest_KEY_BUDDY_STATES;
EXTERN NSString * const InitBuddyListRequest_KEY_MY_VARS;

/** Initializes the BuddyList system.
 
 This involves loading any previously stored Buddy data from the server, such as the User's Buddy List, his previous state and his persistent Buddy Variables. 
 The init request is <b>the first operation required</b> to initialize the BuddyList system. 
 Once the Init is executed the User can have access to all his previous data and start to interact with his Buddies.
 */
@interface InitBuddyListRequest : BaseRequest {
	
}

-(id)init;
/** 
 
 @see [ISFSEvents onBuddyListInit:]
 @see [ISFSEvents onBuddyError:]
 @see SFSBuddy
 @see SFSBuddyVariable
 
 */
+(id)request;

@end
