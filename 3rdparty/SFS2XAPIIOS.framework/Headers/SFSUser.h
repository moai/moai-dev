//
//  SFSUser.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "IUserManager.h"

/** The SFSUser object represents a client logged in to the Server.
 
 The client API doesn't know about all Users connected to the server side but only about those that are in the same Rooms where the client is joined.
 In order to interact with other Users the client can join different Rooms or use a <BuddyList> to keep track of and interact with his/her friends.
 */
@interface SFSUser : NSObject <User>{
@protected
	NSInteger _id;
	NSInteger _privilegeId;
	NSString *_name;
	BOOL _isItMe;
	NSMutableDictionary *_variables;
	NSMutableDictionary *_properties;
	BOOL _isModerator;
	NSMutableDictionary *_playerIdByRoomId;
	id <IUserManager> _userManager;
}

/** Get the unique User Id */
@property (nonatomic, assign) NSInteger id;
/** 
 Get the <b>playerId</b> of the User.
 The <b>playerId</b> is different from the User ID and it used to indicate which player number is the user inside a Game Room.
 Example: in a Game Room for 5 players the first client joining will have <b>playerId = 1</b>, the 2nd will have <b>playerId = 2</b> and so forth. 
 When a User leaves the Room its player slot is freed up and the next User joining the Room will take it.
 
 The <b>playerId</b> is only applicable for Game Rooms, in the other Rooms it is always == 0
 A playerId &lt; 0 indicates that the User is a spectator.
 
 If the User is joined in multiple game Rooms at the same time he will be assigned one <b>playerId</b> per Room.
 */
@property (nonatomic, readonly) NSInteger playerId;
/** 
 Get the privilegeId of the User
 @see UserPrivileges
 */
@property (nonatomic, assign) NSInteger privilegeId;
/** The user name */
@property (nonatomic, readonly) NSString *name;
/** 
 Return true if the User object is the client's User object, also known as SmartFox2XClient#mySelf
 @see SmartFox2XClient#mySelf 
 */
@property (nonatomic, readonly) BOOL isItMe;
/** 
 A generic object that can be used to store any User custom data needed at runtime.
 The values added/removed in this object are for client side use only an are never transmitted to the Server.
 */
@property (nonatomic, retain) NSMutableDictionary *properties;
/** 
 Return true if the User is a Player (playerId &gt; 0) in the last joined Room 
 (non game Rooms will always return false)
 
 @see SmartFox2XClient#lastJoinedRoom 
 */
@property (nonatomic, readonly) BOOL isPlayer;
/** 
 Return true if the User is a Spectator (playerId &lt; 0) in the last joined Room
 (non game Rooms will always return false)
 
 @see SmartFox2XClient#lastJoinedRoom
 */
@property (nonatomic, readonly) BOOL isSpectator;
/** 
 Get the UserManager of this User
 @see SFSUserManager 
 */
@property (nonatomic, assign) id <IUserManager> userManager;


-(id)initWithId:(NSInteger)id name:(NSString *)name isItMe:(BOOL)isItMe;
+(id <User>)fromSFSArray:(id <ISFSArray>)sfsa room:(id <Room>)room;

/** 
 Return the <b>playerId</b> for the specific Room.
 If you don't use multi-room you can use playerId
  
 */
-(NSInteger)getPlayerId:(id<Room>)room;

/** Return true if the User is logged in as guest user */
-(BOOL)isGuest;
/** Return true if the User is logged in as standard user */
-(BOOL)isStandardUser;
/** Return true if the User is logged in as moderator user */
-(BOOL)isModerator;
/** Return true if the User is logged in as administrator user */
-(BOOL)isAdmin;

/** Return true if the User is a Player in the specified Room */
-(BOOL)isPlayerInRoom:(id<Room>)room;

/** Return true if the User is a Spectator in the specified Room */
-(BOOL)isSpectatorInRoom:(id<Room>)room;

/** Return true if the User is joined in the specified Room */
-(BOOL)isJoinedInRoom:(id<Room>)room;

/** 
 Get all the User Variables
 
 @see UserVariable 
 */
-(NSArray *)getVariables;

/**
* Get a UserVariable
* 
* @param varName the name of the variable
* @return the UserVariable or nil if the variable doesn't exist
* @see UserVariable
*/
-(id<UserVariable>)getVariable:(NSString *)varName;

/**
 Check if a UserVariable exists 
 
 @param name the name of the variable
 @return true if the UserVariable exists
 @see UserVariable
 */
-(BOOL)containsVariable:(NSString *)name;
@end
