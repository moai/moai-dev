/*
  Buddy.h
  SFS2X
 *
  Created by Övünç Dalkıran | ovunc.dalkiran@infosfer.com on 27.08.2010.
  Copyright 2010 Infosfer Game Technologies Ltd. | http://www.infosfer.com. All rights reserved.
 *
 */

#import "BuddyVariable.h"

/**
 
 See <SFSBuddy>
 */
@protocol Buddy <NSObject>

/** The id of the Buddy 
 
 The id corresponds to the current id of the User in the system
 */
-(NSInteger)id;

/** The name of the Buddy
 */
-(NSString *)name;

/** Returns true if the Buddy is blocked in the current Users' BuddyList
  
 @see BlockBuddyRequest
 */
-(BOOL)isBlocked;

/** Return true if the User is online in the BuddyList system
 The value is stored in a reserved Buddy Variable
 
 @see GoOnlineRequest
 @see ReservedBuddyVariables
 */
-(BOOL)isOnline;

/** Return true if the Buddy is temporary (not persistent)
 */
-(BOOL)isTemp;

/** The current Buddy custom state (e.g. "Available", "Busy", etc...)
 
 The value is stored in a reserved Buddy Variable
 
 @see ReservedBuddyVariables
 */
-(NSString *)state;

/** An optional Buddy nickname
 
 The value is stored in a reserved Buddy Variable
 
 @see ReservedBuddyVariables
 */
-(NSString *)nickName;

/** Obtain a list of BuddyVariables.
 
 Please note that by convention any variable whose name starts with the dollar sign ($)
 will be regarded as persistent and stored locally by the server.
 
 These variables are also referred to as "offline variables" because they are available
 to other Users even when the Buddy is offline.
 
 @see SFSBuddyVariable
 @see SetBuddyVariablesRequest
 */
-(NSArray *)variables;

/** Get the BuddyVariable with the provided name.
 
 nil is returned if not BuddyVariable exists with that name
 */
-(id <BuddyVariable>)getVariable:(NSString *)varName;

/** Return true if a BuddyVariable with the provided name exists
 */
-(BOOL)containsVariable:(NSString *)varName;

/** Return a list of offline Buddy Variables
 */
-(NSArray *)getOfflineVariables;

/** Return a list of non-persistent Buddy Variables
*/
-(NSArray *)getOnlineVariables;
-(void)setVariable:(id <BuddyVariable>)bVar;
-(void)setVariables:(NSArray *)variables;
-(void)setId:(NSInteger)id_;
-(void)setBlocked:(BOOL)value;
-(void)removeVariable:(NSString *)varName;
-(void)clearVolatileVariables;

@end