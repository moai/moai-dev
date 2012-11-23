//
//  Room.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "User.h"

@protocol RoomVariable;
@protocol IRoomManager;

/**
 
 See <SFSRoom>
 */
@protocol Room <NSObject>

/** The unique Id of the Room */
-(NSInteger)id;

/** The unique name of the Room */
-(NSString *)name;
-(void)setName:(NSString *)value;

/** The Room <b>Group</b> name. Each <b>Group</b> is identified by a unique String and it represent a different 'container' for Rooms.
 
 The idea behind the Room Groups is to enable the developer to organize Rooms under different types or categories and let clients select only those Groups they are interested in.
 This is done via the SubscribeRoomGroupRequest and UnsubscribeRoomGroupRequest requests.
 
 @see SubscribeRoomGroupRequest
 @see UnsubscribeRoomGroupRequest
 */
-(NSString *)groupId;

/** Returns true if the Room is joined by the client User */
-(BOOL)isJoined;
-(void)setIsJoined:(BOOL)value;

/** Returns true is this is a game Room */
-(BOOL)isGame;
-(void)setIsGame:(BOOL)value;

/** 
 Returns true if the Room <b>hidden</b> flag is turned on.
 The flag can be used to hide the rooms marked with this flag from the User  
 */
-(BOOL)isHidden;
-(void)setIsHidden:(BOOL)value;

/** Returns true if the Room requires a password to be joined */
-(BOOL)isPasswordProtected;
-(void)setIsPasswordProtected:(BOOL)value;

-(BOOL)isManaged;
-(void)setIsManaged:(BOOL)value;

/** Get the number of Users in the Room */
-(NSInteger)userCount;
-(void)setUserCount:(NSInteger)value;

/** Get the max number of Users allowed in the Room */ 
-(NSInteger)maxUsers;
-(void)setMaxUsers:(NSInteger)value;

/** Get the number of Spectators in the Room (applies only for game Rooms) */
-(NSInteger)spectatorCount;
-(void)setSpectatorCount:(NSInteger)value;

/** Get the max number of Spectators allowed in the Room (applies only for game Rooms) */
-(NSInteger)maxSpectators;
-(void)setMaxSpectators:(NSInteger)value;

-(void)addUser:(id <User>)user;
-(void)removeUser:(id <User>)user;
/** Returns true if the the User is found in this Room */
-(BOOL)containsUser:(id <User>)user;

/**
 Finds a User from its name
 
 @param name the User name
 @return the User or nil if not found
 */
-(id <User>)getUserByName:(NSString *)name;
/**
 Finds a User from its Id
 
 @param id the User Id
 @return the User or null if not found
 */ 
-(id <User>)getUserById:(NSInteger)id;

/** The Room User-list */
-(NSArray *)userList;
/**
 Returns all the Users that are players in this Room (must be Game Room) 
 */
-(NSArray *)playerList;
/**
 Returns all the Users that are spectators in this Room (must be Game Room) 
 */
-(NSArray *)spectatorList;
/**
 Gets a RoomVariable in the Room
 
 @param name : the variable name
 @return the RoomVariable or nil if the variable doesn't exist
 @see RoomVariable
 */
-(id <RoomVariable>)getVariable:(NSString *)name;
/** 
 Return the full list of RoomVariables in the Room 
 @see RoomVariable
 */
-(NSArray *)getVariables;
-(void)setVariable:(id <RoomVariable>)roomVariable;
-(void)setVariables:(NSArray *)roomVariables;
/** 
 Returns true if a RoomVariable with the passed name exists in the Room
 @see RoomVariable
 */
-(BOOL)containsVariable:(NSString *)name;

/** 
 A generic object that can be used to store any Room custom data needed at runtime.
 The values added/removed in this object are for client side use only an are never transmitted to the Server.
 */
-(NSDictionary *)properties;
-(void)setProperties:(NSDictionary *)value;

/** A reference to the RoomManager, managing this Room */
-(id <IRoomManager>)roomManager;
-(void)setRoomManager:(id <IRoomManager>)manager;

-(NSInteger)capacity;

-(void)setPasswordProtected:(BOOL)isProtected;

@end