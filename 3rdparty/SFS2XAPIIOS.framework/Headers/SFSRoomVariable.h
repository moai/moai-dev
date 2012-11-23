//
//  SFSRoomVariable.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RoomVariable.h"
#import "ISFSArray.h"
#import "SFSUserVariable.h"

/** The RoomVariable class is used to represent variables maintained on the Server side and automatically updated to the clients. 
 
 They are particularly useful to "attach" any custom data to each Room such as the current game status and other Room-specific properties etc...
 
 RoomVariables support basic data types and nested complex objects:
 
 - Null
 - Bool
 - Int
 - Double
 - String
 - SFSObject
 - SFSArray
 
 RoomVariables also support different flags:

 - <b>Private</b>: a private variable can only be modified by its creator
 - <b>Persistent</b>: a persistent variable will continue to exist even if its creator has left the room. 
 - <b>Global</b>: a global variable will fire updates not only to all Users in the Room but also to all Users in the Room Group
 
 See <SFSRoom>, <SetRoomVariablesRequest>
 
 */
@interface SFSRoomVariable : SFSUserVariable <RoomVariable>{
	BOOL _isPersistent;
	BOOL _isPrivate;
}
/** Checks if the variable is persistent. 
 
 A persistent RoomVariable continues to exist in a Room after the User has left it and until he disconnects
 */
@property (nonatomic, readwrite) BOOL isPersistent;
/** Checks if the variable is private. 
 
 A private RoomVariable is only modifiable by its owner (the user that created it)
 */
@property (nonatomic, readwrite) BOOL isPrivate;

+(id <RoomVariable>)fromSFSArray:(id <ISFSArray>)sfsa;
-(id <RoomVariable>)initWithName:(NSString *)name value:(id)value type:(NSInteger)type;
+(id <RoomVariable>)variableWithName:(NSString *)name value:(id)value;
/**
 @param name the name of the variable
 @param value the variable value ( can be Boolean, int, Number, String, SFSObject, SFSArray )
 @param type (optional: -1) it's usually not necessary to pass this parameter as the variable value is auto-detected
 */
+(id <RoomVariable>)variableWithName:(NSString *)name value:(id)value type:(NSInteger)type;
-(id <ISFSArray>)toSFSArray;

@end
