//
//  User.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "UserVariable.h"

@protocol Room;
@protocol IUserManager;
/** 
 See <SFSUser>
 */
@protocol User <NSObject>

/** Get the unique User Id */
-(NSInteger)id;

-(void)setId:(NSInteger)value;

/** The user name */
-(NSString *)name;

/** Get the <b>playerId</b> of the User.
 
 The <b>playerId</b> is different from the User ID and it used to indicate which player number is the user inside a Game Room.
 Example: in a Game Room for 5 players the first client joining will have <b>playerId = 1</b>, the 2nd will have <b>playerId = 2</b> and so forth. 
 When a User leaves the Room its player slot is freed up and the next User joining the Room will take it.
 
 The <b>playerId</b> is only applicable for Game Rooms, in the other Rooms it is always == 0
 A playerId &lt; 0 indicates that the User is a spectator.
 
 If the User is joined in multiple game Rooms at the same time he will be assigned one <b>playerId</b> per Room.
 In order to obtain a specific <b>playerId</b> you can use the [id <User> getPlayerId:room] method.
 */ 
-(NSInteger)playerId;

/** 
 Return true if the User is a Player (playerId &gt; 0) in the last joined Room 
 (non game Rooms will always return false)
 
 @see lastJoinedRoom 
 */
-(BOOL)isPlayer;

/** 
 Return true if the User is a Spectator (playerId &lt; 0) in the last joined Room
 (non game Rooms will always return false)
 
 @see lastJoinedRoom 
 */
-(BOOL)isSpectator;

/** 
 Return the <b>playerId</b> for the specific Room.
 If you don't use multi-room you can use playerId
 
 @see playerId 
 */
-(NSInteger)getPlayerId:(id<Room>)room;


-(void)setPlayerId:(NSInteger)id_ room:(id<Room>)room;
-(void)removePlayerId:(id<Room>)room;


/** 
 Get the privilegeId of the User
 @see UserPrivileges
 */
-(NSInteger)privilegeId;

-(void)setPrivilegeId:(NSInteger)value;

/** 
 Get the UserManager of this User
 @see SFSUserManager 
 */
-(id<IUserManager>)userManager;

-(void)setUserManager:(id<IUserManager>)value;

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
 Return true if the User object is the client's User object, also known as SmartFox2XClient.mySelf
 @see mySelf 
 */
-(BOOL)isItMe;
/** 
 Get all the User Variables
 
 @see UserVariable 
 */
-(NSArray *)getVariables;

/**
 Get a UserVariable
 
 @param varName : the name of the variable
 @return the UserVariable or null if the variable doesn't exist
 @see UserVariable
 */
-(id<UserVariable>)getVariable:(NSString *)varName;

-(void)setVariable:(id<UserVariable>)userVariable;

-(void)setVariables:(NSArray *)userVariables;

/**
 Check if a UserVariable exists 
 
 @param name : the name of the variable
 @return true if the UserVariable exists
 @see UserVariable
 */
-(BOOL)containsVariable:(NSString *)name;
/** 
 A generic object that can be used to store any User custom data needed at runtime.
 The values added/removed in this object are for client side use only an are never transmitted to the Server.
 */
-(NSMutableDictionary *)properties;

-(void)setProperties:(NSMutableDictionary *)value;


@end