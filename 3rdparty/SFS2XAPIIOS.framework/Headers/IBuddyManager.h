//
//  IBuddyManager.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "Buddy.h"

/**
 The BuddyList Manager interface
 */
@protocol IBuddyManager <NSObject>

/** 
 Checks if the current User's Buddy List is inited.
 If not you should send an InitBuddyListRequest to the server in order to retrieve your persistent Buddy List data.
 
 @see InitBuddyListRequest
 */
-(BOOL)isInited;


/**
 Checks if a Buddy is present in the current User's Buddy List
 @param name the Buddy name
 @return the true if the Buddy exists
 
 @see SFSBuddy
 */
-(BOOL)containsBuddy:(NSString *)name;

/**
 Find a Buddy from its User Id
 @param id_ the user id
 @return the Buddy, or null if not found
 
 @see SFSBuddy
 */
-(id <Buddy>)getBuddyById:(NSInteger)id_;

/**
 Find a Buddy from its name
 @param name the Buddy name
 @return the Buddy, or null if not found
 
 @see SFSBuddy
 */
-(id <Buddy>)getBuddyByName:(NSString *)name;

/**
 Find a Buddy from its optional nick name
 @param nickName the nickname
 @return the Buddy, or null if not found
 
 @see SFSBuddy
 */
-(id <Buddy>)getBuddyByNickName:(NSString *)nickName;

/** Get a list of all offline Buddies in the User's Buddy list */
-(NSArray *)offlineBuddies;

/** Get a list of all online Buddies in the User's Buddy list */
-(NSArray *)onlineBuddies;

/**
 Get the User's buddy list. It could be null if the Buddy List was not initialized
 @see InitBuddyListRequest
 */
-(NSArray *)buddyList;

/**
 Get a list of Strings representing the custom Buddy states that the application can use. The custom states
 are sent upon initialization of the Buddy List and are configured on the server side.
 */
-(NSArray *)buddyStates;

/**
 Get current Users' BuddyVariable by name
 @param varName the variable name
 */
-(id <BuddyVariable>)getMyVariable:(NSString *)varName;

/**
 Get all current User's Buddy Variables
 This operation is valid only if the User's BuddyList has already been initialized
 
 @see SFSBuddyVariable
 */
-(NSArray *)myVariables;

/**
 Get the current User's Online State
 This operation is valid only if the User's BuddyList has already been initialized
 The value is handled by a reserved Buddy Variable
 
 @return true if the User is online, false otherwise
 
 @see ReservedBuddyVariables
 */
-(BOOL)myOnlineState;

/**
 Get the current User's optional nickname
 This operation is valid only if the User's BuddyList has already been initialized
 The value is handled by a reserved Buddy Variable
 
 @return return the Users' nickname or null if the nickname was never set
 
 @see ReservedBuddyVariables
 */
-(NSString *)myNickName;

/**
 Get the current User's optional custom state (e.g. "Available"; "Busy", "Be right back"...)
 This operation is valid only if the User's BuddyList has already been initialized
 The value is handled by a reserved Buddy Variable
 
 @return return the Users' state or null if the state was not set
 
 @see ReservedBuddyVariables
 */
-(NSString *)myState;


-(void)setMyVariable:(id <BuddyVariable>)bVar;


-(void)setMyVariables:(NSArray *)variables;


-(void)setMyOnlineState:(BOOL)isOnline;


-(void)setMyNickName:(NSString *)nickName;


-(void)setMyState:(NSString *)state;


-(void)setBuddyStates:(NSArray *)states;

-(void)clearAll;


@end