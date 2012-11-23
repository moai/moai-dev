//
//  RoomPermissions.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


/** This object is part of the <RoomSettings> parameters used for creating a new Room.
 
 It describes the permission settings of a new Room
 
 See <RoomSettings>, <CreateRoomRequest>
 
 */
@interface RoomPermissions : NSObject {
	BOOL _allowNameChange;
	BOOL _allowPasswordStateChange;
	BOOL _allowPublicMessages;
	BOOL _allowResizing;
}

@property (readwrite) BOOL allowNameChange;
@property (readwrite) BOOL allowPasswordStateChange;
@property (readwrite) BOOL allowPublicMessages;
@property (readwrite) BOOL allowResizing;

+(id)permissions;

@end
