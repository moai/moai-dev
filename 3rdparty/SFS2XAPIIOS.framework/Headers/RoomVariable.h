//
//  RoomVariable.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "UserVariable.h"

/**
 See <SFSRoomVariable>
 
 */
@protocol RoomVariable <UserVariable>

/** Checks if the variable is private. 
 
 A private RoomVariable is only modifiable by its owner (the user that created it)
 */
-(BOOL)isPrivate;
-(void)setIsPrivate:(BOOL)value;

/** Checks if the variable is persistent. 
 
 A persistent RoomVariable continues to exist in a Room after the User has left it and until he disconnects
 */
-(BOOL)isPersistent;
-(void)setIsPersistent:(BOOL)value;



@end