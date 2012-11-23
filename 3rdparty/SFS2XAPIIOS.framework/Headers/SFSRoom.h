//
//  SFSRoom.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "Room.h"
#import "IUserManager.h"
#import "IRoomManager.h"


/** The Room object that represents a server Room.
 
 The client API don't know about all Rooms on the server side but only about those that are joined and those in the Room Groups that were subscribed.
 
 Subscribing to one or more Group allows the client to listen for Room events in specific "areas" of the Zone without having to know and download details for all Rooms available.
 
 The RoomList is created after a succesful login in the <SmartFox2XClient> roomList object and it is kept updated at all times by the Server.
 
 See <[SmartFox2XClient roomList]>, <CreateRoomRequest>, <JoinRoomRequest>, <SubscribeRoomGroupRequest>, <UnsubscribeRoomGroupRequest>, <ChangeRoomNameRequest>, <ChangeRoomPasswordStateRequest>, <ChangeRoomCapacityRequest> 
 */
@interface SFSRoom : NSObject <Room>
{

@protected	
	NSInteger _id;
	NSString *_name;
	NSString *_groupId;
	BOOL _isGame;
	BOOL _isHidden;
	BOOL _isJoined;
	BOOL _isPasswordProtected;
	BOOL _isManaged;
	NSMutableDictionary *_variables;
	NSDictionary *_properties;
	id <IUserManager> _userManager;
	NSInteger _maxUsers;
	NSInteger _maxSpectators;
	NSInteger _userCount;
	NSInteger _spectatorCount;
	id <IRoomManager> _roomManager;
}

/**
 The unique id of the Room
 */
@property (nonatomic, readonly) NSInteger id;

/**
 The Room name
 */
@property (nonatomic, retain) NSString *name;

/**
 The Room Group. Each Room is assigned to its Group.
 By default SmartFoxServer uses one single group called <i>default</i>
 */
@property (nonatomic, readonly) NSString *groupId;

/**
 Determines if a Room is a Game Room
 */
@property (nonatomic, assign) BOOL isGame;


/**
 Determines if the Room is hidden
 */
@property (nonatomic, assign) BOOL isHidden;

/**
 Returns true if the Room is joined by the current User
 */
@property (nonatomic, assign) BOOL isJoined;

/**
 Returns true if the Room requires a password for joining it
 */
@property (nonatomic, assign) BOOL isPasswordProtected;

@property (nonatomic, assign) BOOL isManaged;

/**
 Get the maximum number of users allowed for this Room
 */
@property (nonatomic, assign) NSInteger maxUsers;

/**
 Get the maximum number of spectators allowed in the Room
 */
@property (nonatomic, assign) NSInteger maxSpectators;

/**
 Get the current number of users
 */
@property (nonatomic, assign) NSInteger userCount;

/**
* Returns the max amount of users (both Users and Spectators) that can be contained in this room
*/
@property (nonatomic, readonly) NSInteger capacity;

/**
 Get the number of spectators (only for Game Rooms)
 */
@property (nonatomic, assign) NSInteger spectatorCount;

/**
 The properties object can be used to store any custom value/object to the Room at runtime
 */
@property (nonatomic, retain) NSDictionary *properties;

@property (nonatomic, assign) id <IRoomManager> roomManager;

/**
 Get the full list of users in the Room
 */
@property (nonatomic, readonly) NSArray *userList;

/**
 Returns all the Users that are players in this Room (must be Game Room) 
 */
@property (nonatomic, readonly) NSArray *playerList;

/**
 Returns all the Users that are spectators in this Room (must be Game Room) 
 */
@property (nonatomic, readonly) NSArray *spectatorList;


-(id)initWithId:(NSInteger)id name:(NSString *)name groupId:(NSString *)groupId;
+(id <Room>)fromSFSArray:(id <ISFSArray>)sfsa;


/** Returns all the Room Variables
 
 @return an array of Room Variables
 */
-(NSArray *)getVariables;

/** Get a Room Variable
 
 @param name the name of the variable
 @return the Room Variable, or null if no variable exists with that name
 */
-(id <RoomVariable>)getVariable:(NSString *)name;


/** Get a User from its name
 
 @return the User, or null if no User with that name exists in the Room
 */
-(id <User>)getUserByName:(NSString *)name;


/** Get a User from its ID
 @return the User, or null if no User with that ID exists in the Room
 */
-(id <User>)getUserById:(NSInteger)id;

/** Check for the presence of a Room Variable 
 
 @return true if the the Room Variable exists
 */
-(BOOL)containsVariable:(NSString *)name;

/** Checks if a User is joined in this Room
 
 @return true if the User exists in the Room
 */
-(BOOL)containsUser:(id <User>)user;

@end
