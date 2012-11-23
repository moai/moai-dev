//
//  ISFSEvents.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

@class SFSEvent;
@class SFSBuddyEvent;

#import "IConfigLoader.h"
#import "LoggerEvent.h"
/**
 
 */
@protocol ISFSEvents <IConfigLoader>

@optional

/** This event notifies the result of a connection attempt.
 
 @param success (contains an NSNumber with BOOL in it) true if a connection was established, false otherwise.
 
 The following example shows how to start a connection

    smartFox = [[SmartFox2XClient smartFoxWithDebugMode:NO delegate:self] retain];
    [smartFox loadConfig:@"config.xml" connectOnSuccess:YES];

    -(void)onConnection:(SFSEvent *)evt
    {
        if ([[evt.params objectForKey:@"success"] boolValue])
        {
            NSLog(@"Connection was established");
        }
        else
        {
            NSLog(@"Connection failed");
        }
    }

 @see onConnectionRetry:
 @see onConnectionResume:
 @see onConnectionLost:
 
 */
-(void)onConnection:(SFSEvent *)evt;

/** This event notifies of a disconnection from the Server.
 
 When possible the API will also provide a reason for the disconnection:

 - <b>Idle</b>: the connection was dropped because the User was inactive for too long
 - <b>Manual</b>: the User requested the disconnection
 - <b>Kick</b>: the connection was dropped because the User was kicked
 - <b>Ban</b>: the connection was dropped because the User was banned
 - <b>Unknown</b>: the connection was lost for reasons outside of the API (slow or bad connection, network problems etc...)

 @param	reason (contains NSString) the reason of the disconnection.

 The following example shows how to handle a disconnection

	-(void)onConnectionLost:(SFSEvent *)evt
	{
		NSLog(@"Connection was lost, Reason: %@", [evt.params objectForKey:@"reason"]);
	}

 @see onConnectionRetry:
 @see onConnectionResume:
 @see onConnection:
 
 */
-(void)onConnectionLost:(SFSEvent *)evt;

/** This event notifies a temporary disconnection from the Server followed by an immediate attempt to reconnect.
 
 SmartFoxServer 2X provides a powerful new feature that allows a broken connection to be re-established transparently
 without loosing any of the current application state. This allows any player to get back in the game without fear of loosing 
 the match because of a sloppy internet connection. 
 
 When this event is dispatched the API enter a "freeze" mode where no new requests can be sent until the reconnection is 
 successfully performed. It is highly recommended to handle this event and freeze your application interface accordingly until the
 onConnectionResume: event is fired.
 
 The following example shows how to handle a reconnection
 
     -(void)onConnectionRetry:(SFSEvent *)evt
     {
        // Freeze your GUI and provide some feedback to the Player
     }
     
     -(void)onConnectionResume:(SFSEvent *)evt
     {
        // Unfreeze the GUI and let the player continue with the game...
     }

 @see onConnectionLost
 @see onConnectionResume
 @see onConnection
  
 */
-(void)onConnectionRetry:(SFSEvent *)evt;

/** This event is dispatched after a temporary disconnection, when the connection to the Server has been re-establisehd.

 SmartFoxServer 2X provides a powerful new feature that allows a broken connection to be re-established transparently
 without loosing any of the current application state. This allows any player to get back in the game without fear of loosing 
 the match because of a sloppy internet connection or a sudden disconnection. 
 
 In case the re-connection attempts fail onConnectionLost: event will be fired.
 
 The following example shows how to handle a reconnection
 
     -(void)onConnectionRetry:(SFSEvent *)evt
     {
        // Freeze your GUI and provide some feedback to the Player
     }
     
     -(void)onConnectionResume:(SFSEvent *)evt
     {
        // Unfreeze the GUI and let the player continue with the game...
     }
     
     -(void)onConnectionLost:(SFSEvent *)evt
     {
        NSLog(@"Ouch, connection was lost. Reason: %@", [evt.params objectForKey:@"reason"]);
     }

 @see onConnectionLost:
 @see onConnectionResume:
 @see onConnection:
  
 */
-(void)onConnectionResume:(SFSEvent *)evt;

/** The onUDPSocketConnect event is dispatched after a UDP connection has been attempted and either failed (0) or succedded (1) to establish a connection.
 
 The client will attempt to connect three (3) time before dispatching a failed event.
 
 The following example shows how to handle the event

    -(void)onUDPSocketConnect:(SFSEvent *)evt
    {
        if ([[evt.params objectForKey:@"success"] boolValue])
        {
            NSLog(@"UDP connected");
        }
        else
        {
            NSLog(@"UDP not connected");
        }
    }

 @see onConnectionLost
 @see onConnectionResume
 @see onConnection
 @see UDPManager
 
 */
-(void)onUDPSocketConnect:(SFSEvent *)evt;

/** Dispatched when the external SmartFox configuration file has been loaded successfully.
 
 This event is dispatched only if the <i>connectOnSuccess</i> parameter of the SmartFox2XClient::loadConfig:connectOnSuccess: method is set to NO; otherwise the connection is made and the related onConnection: event is fired.
 
 No parameters are provided.
 
 The following example shows how to handle a successful configuration loading.

     [smartFox loadConfig:@"testEnvironmentConfig.xml" connectOnSuccess:NO];
            
     function onConfigLoadSuccess(evt:SFSEvent):void
     {
         NSLog(@"Config file loaded, now connecting...");
         [smartFox connect:smartFox.currentIp port:smartFox.currentPort];
     }

 @see onConfigLoadFailure
 @see loadConfig:connectOnSuccess
 
 */
-(void)onConfigLoadSuccess:(SFSEvent *)evt;

/** Dispatched when an error occurs while loading the external SmartFox configuration file.
 
.
 @param message (NSString *) the error message.
 
 The following example shows how to handle a potential error in configuration loading.

 		[smartFox loadConfig:@"testEnvironmentConfig.xml" connectOnSuccess:YES];
 		
 		function onConfigLoadFailure:(SFSEvent *)evt
 		{
 			NSLog(@"Failed loading config file: %@", [evt.params objectForKey:@"message"]);
 		}

 @see onConfigLoadSuccess:
 @see SmartFox2XClient#loadConfig:connectOnSuccess:
 
 */

-(void)onConfigLoadFailure:(SFSEvent *)evt;
/** This event is dispatched in response to a login request.
 
 The params object may contain the following keys.
 @param user (SFSUser *) the User object 
 @param zone (NSString *) the zone 
 @param data (SFSObject *) a custom object with extra params (if you have added custom server side logic)
 
 The following example shows how to handle a login event

     -(void)onLogin:(SFSEvent *)evt
     {
         NSLog(@"Hi, I have just logged in as: %@" + [[evt.params objectForKey:@"user"] name]);
     }

 @see onLoginError
 @see LoginRequest
 
 */
-(void)onLogin:(SFSEvent *)evt;

/** This event is dispatched in response to a failed login request.
 
 @param errorMessage (NSString *) the error message 
  
     -(void)onLoginError:(SFSEvent *)evt
     {
         NSLog(@"Login Failed. Reason: %@" ,[evt.params objectForKey:@"errorMessage"]);
     }

 @see onLogin
 @see LoginRequest

 */
-(void)onLoginError:(SFSEvent *)evt;

/** This event is dispatched in response to a logout request.
 
 Usually this event needs to be handled when you want to logout from the current Zone and join another one.
 In order to perform the operation correctly you need to do as follows:

 - Send a LogoutRequest to the Server
 - Handle the onLogout event on the client
 - Perform a new login in the new Zone
 
 No parameters are passed for this event
 		
     -(void)onLogout:(SFSEvent *)evt
     {
        NSLog(@"Logout was successful");
        // Proceed with more logic here...
     }

 @see LogoutRequest
 @see LoginRequest
 
 */
-(void)onLogout:(SFSEvent *)evt;

/** This event is dispatched when a new Room was created inside the Zone under any of the Room Groups that the User is listening for.
 
 The  params object contains the following keys.
 @param room (SFSRoom *) the room object 

 -(void)onRoomAdd:(SFSEvent *)evt
 {
 	NSLog(@"A new Room was added: %@", [evt.params objectForKey:@"room"]);
 }

 @see onRoomRemove
 @see CreateRoomRequest
 
 */
-(void)onRoomAdd:(SFSEvent *)evt;

/** This event is dispatched whenever a Room is removed from the Zone, under any of the Room Groups that the User is listening for.
 
 @param room (SFSRoom *) the room object 
 		
     -(void)onRoomRemove:(SFSEvent *)evt
     {
        NSLog(@"A new Room was removed: %@", [evt.params objectForKey:@"room"]);
     }
 
 @see onRoomAdd
 
 */
-(void)onRoomRemove:(SFSEvent *)evt;

/** This event is dispatched in response to a failed create room request.
 
 @param errorMessage (NSString *) the error message 
 		
 -(void)onRoomCreationError:(SFSEvent *)evt
 {
 	NSLog(@"Room Creation Error: %@", [evt.params objectForKey:@"errorMessage"]); 	
 }
 
 @see onRoomAdd
 @see CreateRoomRequest
 
*/
-(void)onRoomCreationError:(SFSEvent *)evt;

/** This event is dispatched in response to a JoinRoomRequest.
 
 @param room (SFSRoom *) the joined Room 
 
     -(void)onRoomJoin:(SFSEvent *)evt
     {
        SFSRoom *room = [evt.params objectForKey:@"room"];
        NSLog(@"The Room %@ was successfully joined!", room.name);
     }
  
 @see onRoomJoinError
 @see JoinRoomRequest
 
 */
-(void)onRoomJoin:(SFSEvent *)evt;

/** This event is dispatched in response to a failed JoinRoomRequest.
  
 The params object contains the following parameters.
 @param errorMessage (NSString *) the error message
 
 -(void)onRoomJoinError:(SFSEvent *)evt
 {
 	NSLog(@"Join Room Failure:  %@", [evt.params objectForKey:@"errorMessage"]);
 }
  
 @see onRoomJoin
 @see JoinRoomRequest
 
 */ 
-(void)onRoomJoinError:(SFSEvent *)evt;

/** This event is fired whenever a User enters one of the Rooms where the current User is joined.
 
 @param user (SFSUser *) the User object
 @param room (SFSRoom *) the Room object
 
     -(void)onUserEnterRoom:(SFSEvent *)evt
     {
        SFSRoom *room = [evt.params objectForKey:@"room"];
        SFSUser *user = [evt.params objectForKey:@"user"];
        NSLog(@"User: %@ has just joined Room: %@", user.name, room.name);
     }

 @see onUserExitRoom
 @see onRoomJoin
 @see JoinRoomRequest
 
 */
-(void)onUserEnterRoom:(SFSEvent *)evt;

/** This event is fired whenever a User leaves one of the Rooms where the current User is joined.
  
 @param user (SFSUser *) the User object
 @param room (SFSRoom *) the Room object
 
     -(void)onUserExitRoom:(SFSEvent *)evt
     {
        SFSRoom *room = [evt.params objectForKey:@"room"];
        SFSUser *user = [evt.params objectForKey:@"user"];
        NSLog(@"User: %@ has just left Room: %@", user.name, room.name);
     }
  
 @see onUserEnterRoom
 @see onRoomJoin
 @see LeaveRoomRequest
 
 */ 
-(void)onUserExitRoom:(SFSEvent *)evt;

/** This event is fired whenever the number of users/spectators in a Room changes.
 
 The client receives updates for all Rooms inside the Room Groups that he is listening to.
 
     -(void)onUserCountChange:(SFSEvent *)evt
     {
         SFSRoom *room = [evt.params objectForKey:@"room"];
         int uCount = [[evt.params objectForKey:@"uCount"] intValue];
         int sCount = [[evt.params objectForKey:@"sCount"] intValue];
         
         NSLog(@"Room: %@ contains %d users and %d spectators", room.name, uCount, sCount); 	
     }

 @param room (Room</b>) the Room 
 @param uCount (NSNumber *) the new user count
 @param sCount (NSNumber *) the new spectator count

 */
-(void)onUserCountChange:(SFSEvent *)evt;

/** This event is fired when a User sends a public message.
 
 Also the User receives this event every time he sends a public message. 

     -(void)onPublicMessage:(SFSEvent *)evt
     {
        SFSUser *sender = [evt.params objectForKey:@"sender"];
        NSLog(@"%@: %@", sender.name, [evt.params objectForKey:@"message"]);
     }
 
 @param room (SFSRoom *) the target Room  
 @param sender (SFSUser *) the sender
 @param message (NSString *) the message
 @param data (SFSObject *) an object with custom data

 @see PublicMessageRequest
 
 */
-(void)onPublicMessage:(SFSEvent *)evt;

/** This event is fired when a User receives a private message.
 
     -(void)onPrivateMessage:(SFSEvent *)evt
     {
        SFSUser *sender = [evt.params objectForKey:@"sender"];
        NSLog(@"PM: %@, from: %@", [evt.params objectForKey:@"message"], sender.name);
     }
 
 @param sender (SFSUser *) the sender of the message 
 @param message (NSString *) the message 
 @param data (SFSObject *) an object with custom data 
 
 @see PrivateMessageRequest
 
 */ 
-(void)onPrivateMessage:(SFSEvent *)evt;

/** This event is fired when a User receives a Moderator message.
 
     -(void)onModeratorMessage:(SFSEvent *)evt
     {
        SFSUser *sender = [evt.params objectForKey:@"sender"];
        NSLog(@"Moderator says: %@, from: %@", [evt.params objectForKey:@"message"], sender.name);
     }
 
 @param sender (SFSUser *) the sender of the message 
 @param message (NSString *) the message 
 @param data (SFSObject *) an object with custom data 
 
 @see ModeratorMessageRequest
 
 */
-(void)onModeratorMessage:(SFSEvent *)evt;

/** This event is fired when a User receives an Admin message.

     -(void)onAdminMessage:(SFSEvent *)evt
     {
        NSLog(@"Admin says: %@", [evt.params objectForKey:@"message"]);
     }
 
 @param sender (SFSUser *) the sender of the message 
 @param message (NSString *) the message 
 @param data (SFSObject *) an object with custom data 
 
 @see AdminMessageRequest
 
 */
-(void)onAdminMessage:(SFSEvent *)evt;

/** This event is received when a User has sent an ObjectMessage request

 The object sent by the sender can contain custom data, such as a game move etc...
 
     -(void)onObjectMessage:(SFSEvent *)evt
     {
        SFSObject *obj = [evt.params objectForKey:@"message"];
        NSLog(@"Received data: %@", [obj getDump]);
     }
 
 @param sender (SFSUser *) the sender of the message 
 @param message (SFSObject *) the object data 
 
 @see ObjectMessageRequest
 
 */
-(void)onObjectMessage:(SFSEvent *)evt;

/** This event is fired every time a server-side Extension sends a message, usually in response to an ExtensionRequest. 

 (A server Extension can send messages at any moment, not just in response to a specific client request)

     -(void)onExtensionResponse:(SFSEvent *)evt
     {
         NSArray *keys = [evt.params allKeys];
         for (NSString *key in keys)
         {
             NSLog(@"%@: %@", key, [evt.params objectForKey:key]);
         }
     }
 
 @param cmd (NSString *) the name of the "command" (or "action id") associated with the message
 @param params (INSSFSObject *) custom parameters
 
 @see ExtensionRequest
 @see SFSObject
 
 */
-(void)onExtensionResponse:(SFSEvent *)evt;

/** This event is fired in response to a SetRoomVariablesRequest

 @param room (SFSRoom *) the Room in which the event occurred
 @param changedVars (NSArray *) the list of variable names that where modified or created
 
 @see SetRoomVariablesRequest
 @see SFSRoomVariable
 
 */
-(void)onRoomVariablesUpdate:(SFSEvent *)evt;

/** This event is fired in response to a SetUserVariablesRequest

 @param user (SFSUser *) the User that created or modified the Variables
 @param changedVars (NSArray *) the list of variable names that where modified or created

 @see SetUserVariablesRequest
 @see SFSUserVariable
 
 */
-(void)onUserVariablesUpdate:(SFSEvent *)evt;

/** This event is fired in response to a SubscribeRoomGroupRequest

 @param groupId (NSString *) the name of thr group that was subscribed
 @param newRooms (NSArray *) the list of rooms contained in the new group 

 @see SubscribeRoomGroupRequest
 
 */
-(void)onSubscribeRoomGroup:(SFSEvent *)evt;

/** This event is fired in response to a failed SubscribeRoomGroupRequest
 
 The params object contains the following parameters.
 @param errorMessage (NSString *) the error message
 
 @see SubscribeRoomGroupRequest
 
 */ 
-(void)onSubscribeRoomGroupError:(SFSEvent *)evt;

/** This event is fired in response to an UnsubscribeRoomGroupRequest
 
 @param groupId (NSString *) the name of the group that was unsubscribed.
 
 @see UnsubscribeRoomGroupRequest
 
 */
-(void)onUnsubscribeRoomGroup:(SFSEvent *)evt;

/** This event is fired in response to a failed UnsubscribeRoomGroupRequest
 
 The params object contains the following parameters.
 @param errorMessage (NSString *) the error message
 
 @see UnsubscribeRoomGroupRequest
 
 */ 
-(void)onUnsubscribeRoomGroupError:(SFSEvent *)evt;

/** This event is fired in response to a successful SpectatorToPlayerRequest

 @param room (SFSRoom *) the Room where the spectator became a player
 @param user (SFSUser *) the User who became a player
 @param playerId (NSNumber *) the new playerId of the User
 
	 //This is how to dispatch the event
	 -(void)switchSpectatorToPlayer
	 {
		 [client send:[SpectatorToPlayerRequest requestWithTargetRoom:nil]];
	 } 
	 
	 //This is how to handle the event
	 -(void)onSpectatorToPlayer:(SFSEvent *)evt 
	 {
		 NSLog(@"Hi, I'm handling an onSpectatorToPlayer event. My params are %@",evt.params); 
	 }
 
 @see SpectatorToPlayerRequest
 
 */
-(void)onSpectatorToPlayer:(SFSEvent *)evt;

/** This event is fired in response to a failed SpectatorToPlayerRequest

 @param errorMessage (NSString *) the error message
 
	 //This is how to handle the event
	 -(void)onSpectatorToPlayerError:(SFSEvent *)evt 
	 {
		 NSLog(@"Hi, I'm handling an onSpectatorToPlayerError event. My params are %@",evt.params); 
	 }

 @see SpectatorToPlayerRequest
 
 */
-(void)onSpectatorToPlayerError:(SFSEvent *)evt;

/** This event is fired in response to a successful PlayerToSpectatorRequest
 
 @param room (SFSRoom *) the Room where the player became a spectator
 @param user (SFSUser *) the User who became a spectator
 
	 //This is how to dispatch the event
	 -(void)switchPlayerToSpectator
	 {
		 [client send:[PlayerToSpectatorRequest requestWithTargetRoom:nil]];
	 } 
 
	 //This is how to handle the event
	 -(void)onPlayerToSpectator:(SFSEvent *)evt 
	 {
		 NSLog(@"Hi, I'm handling an onPlayerToSpectator event. My params are %@",evt.params);
	 }
 
 @see PlayerToSpectatorRequest
 
 */
-(void)onPlayerToSpectator:(SFSEvent *)evt;

/** This event is fired in response to a failed PlayerToSpectatorRequest

 @param errorMessage (NSString *) the error message
	 
	 //This is how to handle the event
	 -(void)onSpectatorToPlayerError:(SFSEvent *)evt 
	 {
		 NSLog(@"Hi, I'm handling an onPlayerToSpectatorError event. My params are %@",evt.params); 
	 }
 
 @see PlayerToSpectatorRequest
 
 */ 
-(void)onPlayerToSpectatorError:(SFSEvent *)evt;

/** This event is fired in response to a ChangeRoomNameRequest

 @param room (SFSRoom *) the Room where the change occurred
 @param oldName (NSString *) the old Room name
 
	 //This is how to handle the event
	 -(void)onRoomNameChange:(SFSEvent *)evt 
	 {
		 NSLog(@"Hi, I'm handling an onRoomNameChange event. My params are %@",evt.params); 
	 }
  
 @see ChangeRoomNameRequest
 
 */
-(void)onRoomNameChange:(SFSEvent *)evt;

/** This event is fired in response to a failed ChangeRoomNameRequest

 @param errorMessage (NSString *) the error message

 @see ChangeRoomNameRequest
 
 */ 
-(void)onRoomNameChangeError:(SFSEvent *)evt;

/** This event is fired in response to a ChangeRoomPasswordStateRequest.
 
 In particular this event when the <b>password state</b> is changed. For example when a non-password protected Room is assigned a password, or vice-versa 

 @param room (SFSRoom *) the Room where the change occurred

 @see ChangeRoomPasswordStateRequest
 
 */
-(void)onRoomPasswordStateChange:(SFSEvent *)evt;

/** This event is fired in response to a failed ChangeRoomPasswordStateRequest

 @param errorMessage (NSString *) the error message

 @see ChangeRoomPasswordStateRequest
 
 */
-(void)onRoomPasswordStateChangeError:(SFSEvent *)evt;

/** This event is fired in response to a ChangeRoomCapacityRequest

 @param room (SFSRoom *) the Room where the change occurred

 @see ChangeRoomCapacityRequest
 
 */
-(void)onRoomCapacityChange:(SFSEvent *)evt;

/** This event is fired in response to a failed ChangeRoomCapacityRequest

 @param errorMessage (NSString *) the error message
 
 @see ChangeRoomCapacityRequest
 
 */
-(void)onRoomCapacityChangeError:(SFSEvent *)evt;

/** This event is fired in response to a FindRoomsRequest

 @param rooms (NSArray *) a list of Rooms matching the request criteria. If no Room was found the list will be empty.

 @see FindRoomsRequest
 
 */
-(void)onRoomFindResult:(SFSEvent *)evt;

/** This event is fired in response to a FindUsersRequest
 
 @param users (NSArray *) a list of Users matching the request criteria. If no User was found the list will be empty.

 @see FindUsersRequest

 */
-(void)onUserFindResult:(SFSEvent *)evt;

/** This event is fired when another User has sent you an Invitation

 @param invitation (SFSInvitation *) the invitation object

 @see InviteUsersRequest
 @see SFSInvitation
 
 */
-(void)onInvitation:(SFSEvent *)evt;

/** This event is fired when a User sends a reply to your Invitation

 @param invitee (SFSUser) the User that was invited
 @param reply (NSNumber *) the <i>reply code</i>. It can be either InvitationReply.ACCEPT or InvitationReply.REFUSE
 @param data (SFSObject *) additional custom paramters (e.g. a reason for the refusal, a message etc...)
 
 @see InvitationReply
 @see InviteUsersRequest
 
 */
-(void)onInvitationReply:(SFSEvent *)evt;

/** This event is fired in response to a failed InvitationReplyRequest
 The failure is usually caused by a late reply which doesn't make it to the server within the expected time

 @param errorMessage (NSString *) the error message
 
 @see InvitationReplyRequest
 @see InviteUsersRequest
 
 */
-(void)onInvitationReplyError:(SFSEvent *)evt;

/** This event is fired in response to a BuddyMessageRequest.

 @param buddy (SFSBuddy *) the Buddy that updated the BuddyVariables
 @param isItMe (NSNumber *) true if the Buddy is actually the current User
 @param message (NSString *) a list with the names of the Variables that where changed/set
 @param data (SFSObject *) optional custom parameters
 
 @see SFSBuddy
 @see BuddyMessageRequest
 
 */
-(void)onBuddyMessage:(SFSBuddyEvent *)evt;

/** This event is fired in response to a InitBuddyListRequest.
 
 After the Buddy List initialization the User will return to his previous Buddy state. His online/offline status, the Buddy state
 and his persistent Buddy Variables will all be loaded and broadcast in the system. In particular the online status determines if the
 User will appear online or not to other Users that have added him/her to their lists.

 At this stage the custom <b>Buddy states</b> are also loaded and populated locally by the API.
 
     NSLog(@"Buddy States: " + [smartFox.buddyManager buddyStates]);

 @param buddyList (NSArray *) a list of Buddy objects
 @param myVariables (NSArray *) a list of the User's BuddyVariable(s)
 
 @see SFSBuddy
 @see SFSBuddyVariable
 @see InitBuddyListRequest
 
 */
-(void)onBuddyListInit:(SFSBuddyEvent *)evt;

/** This event is fired in response to a AddBuddyRequest.

 @param buddy (SFSBuddy *) the Buddy that was added

 @see SFSBuddy
 @see AddBuddyRequest
 
 */
-(void)onBuddyAdd:(SFSBuddyEvent *)evt;

/** This event is fired in response to a RemoveBuddyRequest.

 @param buddy (SFSBuddy *) the Buddy that was removed
 
 @see SFSBuddy
 @see RemoveBuddyRequest
 
 */
-(void)onBuddyRemove:(SFSBuddyEvent *)evt;

/** This event is fired in response to a BlockBuddyRequest.

 @param buddy (SFSBuddy *) the Buddy that was blocked/unblocked

 @see SFSBuddy
 @see BlockBuddyRequest
 
 */
-(void)onBuddyBlock:(SFSBuddyEvent *)evt;

/** This event is fired in response to a GoOnlineRequest.

 @param buddy (SFSBuddy *) the Buddy that was has changed his online status
 @param isItMe (NSNumber *) (contains a BOOL value) true if the Buddy is actually the current User
 
 @see SFSBuddy
 @see GoOnlineRequest
 
 */
-(void)onBuddyOnlineStateUpdate:(SFSBuddyEvent *)evt;

/** This event is fired in response to a SetBuddyVariablesRequest.

 @param buddy (SFSBuddy *) the Buddy that updated the BuddyVariables
 @param changedVars (NSArray *) a list with the names of the Variables that where changed/set
 @param isItMe (NSNumber *) true if the Buddy is actually the current User
 
 @see SFSBuddy
 @see SetBuddyVariablesRequest
 
 */
-(void)onBuddyVariablesUpdate:(SFSBuddyEvent *)evt;

/** This event is fired in response to a failed Buddy request, such as AddBuddy, RemoveBuddy etc...

 @param errorMessage (NSString *) the error message
 
 */
-(void)onBuddyError:(SFSBuddyEvent *)evt;

/** This event is fired in response to a console message from logger...
 
 @param message (String) the debug message.
 
 */
-(void)onDebugMessage:(LoggerEvent *)evt;

@end