//
//  RoomSettings.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "RoomPermissions.h"
#import "RoomEvents.h"
#import "RoomExtension.h"


/** This class provides the Room settings that you can pass to a CreateRoomRequest
 
 Some of these fields are mandatory and some other are populated by default if you don't specify them.
 
 - <b>name:</b> mandatory, each Room must have a unique name
 - <b>password:</b> optional, by default a Room is created without a password
 - <b>groupId:</b> optional, by default Rooms are created in the 'default' group if none is specified
 - <b>isGame:</b> optional, by default isGame flage is set to false
 - <b>maxUsers:</b> optional, if no value is provided the default value of 10 is used
 - <b>maxSpectators:</b> optional, if no value is provided the default value of 0 is used
 - <b>maxVariables:</b> optional, if no value is provided the default value of 5 is used
 - <b>variables:</b> optional, an array of RoomVariables to be set at creation time
 - <b>permissions:</b> optional, if no value is provided the default RoomPermissions configuration is used
 - <b>events:</b> optional, if no value is provided the default RoomEvents configuration is used
 - <b>extension:</b> optional. No default values
 
 See <RoomEvents>, <RoomPermissions>, <RoomExtension>, <CreateRoomRequest>
 
 **/
@interface RoomSettings : NSObject {
@private
	NSString *_name;
	NSString *_password;
	NSString *_groupId;
	BOOL _isGame;
	NSInteger _maxUsers;
	NSInteger _maxSpectators;
	NSInteger _maxVariables;
	NSArray *_variables;
	RoomPermissions *_permissions;
	RoomEvents *_events;
	RoomExtension *_extension;
}

/** The name of the new Room */
@property (retain) NSString *name;
/** The password of the Room. If null is used the Room will not be password protected */
@property (retain) NSString *password;
/** the if of the Room Group in which the new Room should be created */ 
@property (retain) NSString *groupId;
/** Indicates if this is a Game Room */
@property (assign) BOOL isGame;
/** The maximum number of users/players allowed in the Room */
@property (assign) NSInteger maxUsers;
/** The maximum number of spectators allowed for this Room (only for Game Rooms) */
@property (assign) NSInteger maxSpectators;
/** The maximum number of Room Variables allowed for this Room */
@property (assign) NSInteger maxVariables;

/** A list of Room Variables that should be attached to the newly created Room
 
 @see SFSRoomVariable 
 */
@property (retain) NSArray *variables;

/** The Room Permission settings of the new Room
 
 @see RoomPermissions 
 */ 
@property (retain) RoomPermissions *permissions;

/** The Room Event settings of the new Room
 
 @see RoomEvents 
 */ 
@property (retain) RoomEvents *events;


/** The Extension settings for the new Room
 
 @see RoomExtension
 */
@property (retain) RoomExtension *extension;


-(id)initWithName:(NSString *)name;
+(id)settingsWithName:(NSString *)name;

@end
