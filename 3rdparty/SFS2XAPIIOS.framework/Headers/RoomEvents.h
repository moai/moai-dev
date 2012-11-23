//
//  RoomEvents.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import <Foundation/Foundation.h>

/** This class provides Room event settings that can be specified when creating a new Room via the <CreateRoomRequest>
 
 You can configure which events the Room will fire in the Zone:
 
 - <b>allowUserEnter<b>: fire an event each time a User enters/joins the Room
 - <b>allowUserExit<b>: fire an event each time a User exits/leaves the Room
 - <b>allowUserCountChange<b>: fire an event each time the user count/spectator count changes in the Room
 - <b>allowUserVariablesUpdate<b>: fire an event each time a user in the Room has updated his/her UserVariables
 
 Each of these events can be turned ON (true) or OFF (false)
 
 See <CreateRoomSettings>, <CreateRoomRequest>
 
 */
@interface RoomEvents : NSObject {
	BOOL _allowUserEnter;
	BOOL _allowUserExit;
	BOOL _allowUserCountChange;
	BOOL _allowUserVariablesUpdate;
}

/** fire an event each time a User enters/joins the Room  */
@property (readwrite) BOOL allowUserEnter;
/** fire an event each time a User exits/leaves the Room   */
@property (readwrite) BOOL allowUserExit;
/** fire an event each time the user count/spectator count changes in the Room   */
@property (readwrite) BOOL allowUserCountChange;
/** fire an event each time a user in the Room has updated his/her UserVariables   */
@property (readwrite) BOOL allowUserVariablesUpdate;

@end
