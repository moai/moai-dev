//
//  SFSBuddy.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "Buddy.h"

/** The SFSBuddy class represent a <Buddy> in the User's <b>Buddy List</b>.
 
 Each Buddy has several properties:
  
 - <b>name</b>: the name of the Buddy, corresponds to the User name
 - <b>nickname</b>: an optional nickname (default = null)
 - <b>isOnline</b>: whether the Buddy is online in the Buddy system or not
 - <b>state</b>: a string representing a "state message", such as: <i>Available</i>, <i>Busy</i>...
 - <b>isBlocked</b>: whether the Buddy is blocked in the User Buddy List
 - <b>isTemp</b>: a temporary Buddy is not stored in the Buddy List, it will be removed at the end of the session (see below)
 - <b>variables</b>: the Buddy variables which can be transient or persistent (see below)


 <b>Online State</b><br/>
 Any user can decide if they want to be online/off-line as Buddy in the Buddy system. By default a User is online 
 every time he joins the Zone, but the User can also turn off this flag at runtime and disappear from other User's buddy lists.
 The <b>Online State</b> is persistent and based on a reserved Buddy Variable.
 
 <b>Blocked Buddies</b><br/>
 Buddies that are blocked won't be able to send any messages to the User, also they won't be able to see if the Buddy is online/off-line.
 
 <b>Buddy State message</b><br/>
 The state message represents a typical IM Buddy state such as Available, Busy etc...
 By default the system already provides 3 default states: <b>Available</b>, <b>Away</b>, <b>Occupied</b> which can be changed or enriched at any time 
 The state message is based on a reserved Buddy Variable.
 
 <b>Temp Buddy</b><br/>
 A temporary Buddy is added to the User List when another User adds me to his Buddy List. 
 This way we can see each others and exchange messages. 
 If I don't add the User as Buddy in my list the User will remain temporary and it won't be persisted. 
 
 <b>Variables</b><br/>
 Buddy Variables enable each Buddy to show/send updates for specific informations to each User that has the Buddy in the list. 
 For example one could send real-time updates on his last activity (ala Twitter) or post the title of the song he's listening right now, scores, 
 rankings and whatnot.
 
 */
@interface SFSBuddy : NSObject <Buddy>{
@protected
	NSInteger _id;
	NSString *_name;
	BOOL _isBlocked;
	NSMutableDictionary *_variables;
	BOOL _isTemp;
}
/**
 The name of the Buddy
 */
@property (nonatomic, retain) NSString *name;
/**
 Obtain a list of BuddyVariables.
 Please note that by convention any variable whose name starts with the dollar sign ($)
 will be regarded as persistent and stored locally by the server.
 
 These variables are also referred to as "offline variables" because they are available
 to other Users even when the Buddy is offline.
 
 @see SFSBuddyVariable
 @see SetBuddyVariablesRequest
 */
@property (nonatomic, retain) NSArray *variables;
/**
 The id of the Buddy 
 The id corresponds to the current id of the User in the system
 */
@property (nonatomic, assign) NSInteger id;
/**
 Return true if the Buddy is blocked in the current Users' BuddyList
 
 @see BlockBuddyRequest
 */
@property (nonatomic, assign) BOOL isBlocked;
/**
 Return true if the Buddy is temporary (not persistent)
 */
@property (nonatomic, assign) BOOL isTemp;

/**
 Return true if the User is online in the BuddyList system
 The value is stored in a reserved Buddy Variable
 
 @see GoOnlineRequest
 @see ReservedBuddyVariables
 */
@property (readonly) BOOL isOnline;
/**
 An optional Buddy nickname
 The value is stored in a reserved Buddy Variable
 
 @see ReservedBuddyVariables
 */
@property (readonly) NSString *nickName;
/**
 The current Buddy custom state (e.g. "Available", "Busy", etc...)
 The value is stored in a reserved Buddy Variable
 
 @see ReservedBuddyVariables
 */
@property (readonly) NSString *state;

+(id <Buddy>)fromSFSArray:(id <ISFSArray>)arr;
-(id)initWithId:(NSInteger)id_ name:(NSString *)name isBlocked:(BOOL)isBlocked isTemp:(BOOL)isTemp;

/**
 Return true if a BuddyVariable with the provided name exists
 */
-(BOOL)containsVariable:(NSString *)varName;
/**
* Return a list of offline Buddy Variables
*/
-(NSArray *)getOfflineVariables;
/**
 Return a list of non-persistent Buddy Variables
 */
-(NSArray *)getOnlineVariables;
/**
 Get the BuddyVariable with the provided name.
 nil is returned if not BuddyVariable exists with that name
 */
-(id <BuddyVariable>)getVariable:(NSString *)varName;
@end
