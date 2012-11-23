//
//  SmartFox2XClient.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "Room.h"
#import "SFSRoom.h"
#import "User.h"
#import "SFSUser.h"
#import "UserPrivileges.h"
#import "SFSBuddy.h"
#import "IBuddyManager.h"
#import "IUserManager.h"
#import "IRoomManager.h"
#import "IBitSwarmEvents.h"
#import "ISFSEvents.h"
#import "IConfigLoader.h"
#import "SFSEvent.h"
#import "SFSObject.h"
#import "SFSArray.h"
#import "RoomSettings.h"
#import "RoomProperties.h"
#import "UserProperties.h"
#import "IRequest.h"
#import "LoginRequest.h"
#import "LogoutRequest.h"
#import "JoinRoomRequest.h"
#import "LeaveRoomRequest.h"
#import "ChangeRoomNameRequest.h"
#import "ChangeRoomCapacityRequest.h"
#import "CreateRoomRequest.h"
#import "PlayerToSpectatorRequest.h"
#import "RoomPermissions.h"
#import "PublicMessageRequest.h"
#import "ChangeRoomPasswordStateRequest.h"
#import "AdminMessageRequest.h"
#import "BanMode.h"
#import "BanUserRequest.h"
#import "KickUserRequest.h"
#import "MessageRecipientMode.h"
#import "SpectatorToPlayerRequest.h"
#import "SubscribeRoomGroupRequest.h"
#import "UnsubscribeRoomGroupRequest.h"
#import "PrivateMessageRequest.h"
#import "SFSInvitation.h"
#import "ModeratorMessageRequest.h"
#import "InvitationReplyRequest.h"
#import "InviteUsersRequest.h"
#import "FindRoomsRequest.h"
#import "FindUsersRequest.h"
#import "SetRoomVariablesRequest.h"
#import "SFSRoomVariable.h"
#import "SetUserVariablesRequest.h"
#import "ObjectMessageRequest.h"
#import "SFSBuddyEvent.h"
#import "AddBuddyRequest.h"
#import "InitBuddyListRequest.h"
#import "RemoveBuddyRequest.h"
#import "BlockBuddyRequest.h"
#import "BuddyMessageRequest.h"
#import "GoOnlineRequest.h"
#import "SetBuddyVariablesRequest.h"
#import "SFSBuddyVariable.h"
#import "CreateSFSGameRequest.h"
#import "SFSGameSettings.h"
#import "QuickJoinGameRequest.h"
#import "MatchExpression.h"
#import "BoolMatch.h"
#import "StringMatch.h"
#import "LogicOperator.h"
#import "NumberMatch.h"
#import "ExtensionRequest.h"
#import "Logger.h"
#import "LogLevel.h"

@class ConfigData;
@class BitSwarmClient;
@class Logger;

/** SmartFox2XClient is the main class in the SFS2X API.
 
 This class is responsible for connecting to the Server and dispatching all asynchronous events.
 @warning *<b>Important:</b>* in the provided examples, <b>smartFox</b> always indicates a SmartFox2XClient instance.
 */

@interface SmartFox2XClient : NSObject <IBitSwarmEvents, IConfigLoader>{
	
@private
	NSInteger _majVersion;
	NSInteger _minVersion;
	NSInteger _subVersion;
	
	BitSwarmClient *_bitSwarm;
	
	// If true the client is using Http Tunnelling
	BOOL _isHttpMode;
	
	// If true the client is in the middle of a join transaction
	BOOL _isJoining;
	
	// References the client's User object
	id<User> _mySelf;
	
	// A unique session token, sent by the server during the handshake
	NSString *_sessionToken;
	
	// Last joined room
	id <Room> _lastJoinedRoom;
	
	// The logger
	Logger *_log;
	
	// API initialization flag
	BOOL _inited;
	
	// Protocol debug flag
	BOOL _debug;
	
	// Connection attempt flag
	BOOL _isConnecting;
	
	// The global user manager
	id <IUserManager> _userManager;
	
	// The global room manager
	id <IRoomManager> _roomManager;
	
	// The global buddy manager
	id <IBuddyManager> _buddyManager;
	
	ConfigData *_config;
	
	// The name of the currently joined Zone
	NSString *_currentZone;
	
	// When true -> starts the connection right after successful config loading
	BOOL _autoConnectOnConfig;
	
	id <ISFSEvents> _delegate;
}

/** Returns the Buddy Manager. 

 @see SFSBuddyManager
 */
@property (retain) id <IBuddyManager> buddyManager;

/** Returns the current compression threshold. 

 The value represent the maximum message size (in bytes) before the protocol compression is activated.
 The value is determined by the server configuration.
 */
@property (readonly) NSInteger compressionThreshold;

/** Get the current client configuration. 
 
 It can return null if the configuration was not loaded.
 
 @see loadConfig:connectOnSuccess:
 */
@property (retain) ConfigData *config;

/** The address where the client is connected (if the client is connected)
 
 @see connect:port:
 @see loadConfig:connectOnSuccess:
 */
@property (readonly) NSString *currentIp;

/** The TCP port of the server where the client is connected (if the client is connected) 

 @see loadConfig:connectOnSuccess:
 @see LoginRequest
 */
@property (readonly) NSInteger currentPort;

/** The current Zone in use, if user is logged in 

 @see loadConfig:connectOnSuccess:
 @see LoginRequest
 */
@property (retain) NSString *currentZone;

/** When set to true, provides detailed debugging data for all incoming/outgoing messages  
 */
@property (assign) BOOL debug;

/** Returns true if the client connection is http tunnelled
 */
@property (readonly) BOOL isHttpMode;

/** Returns true if the client connection is active   
 */
@property (readonly) BOOL isConnected;

/** Returns a list of Rooms currently joined by the client   
 
 @see JoinRoomRequest
 */
@property (readonly) NSMutableArray *joinedRooms;

/** Return the last joined Room, if any.    

 Null is returned if there's no currently joined Room.

 @see JoinRoomRequest
 */
@property (retain) id <Room> lastJoinedRoom;

/** Get a reference to the logger
 
 @return the logger
 */
@property (readonly) Logger *logger;

/** This User object represents the User connected to SmartFoxServer 2X
 
 @see LoginRequest
 */
@property (retain) id <User> mySelf;

/** Returns the list of Rooms currently "watched" by the client.    

 The list will contain all the Rooms that are currently joined and all the Rooms from the Room Groups that are subscribed.
 At login time the User is automatically subscribed to all Room Groups specified in the Zone's *Default Groups* setting.
 
 @see SubscribeRoomGroupRequest
 @see UnsubscribeRoomGroupRequest
 */
@property (readonly) NSArray *roomList;

/** Returns the Room Manager.    
 @see SFSRoomManager
 */
@property (retain) id <IRoomManager> roomManager;

/** Returns the unique session token.   

 The Session token is a String sent by the server to the client after the initial handshake.
 */
@property (retain) NSString *sessionToken;

/** Returns the User Manager.    

 @see SFSUserManager
 */
@property (retain) id <IUserManager> userManager;

/** Return the API current version
 */
@property (readonly) NSString *version;

@property (retain) BitSwarmClient *bitSwarm;
@property (readwrite) BOOL isJoining;
@property (readonly) id <ISFSEvents> delegate;

-(void)handleHandshake:(SFSEvent *)evt;
-(void)setReconnectionSeconds:(NSInteger)seconds;
-(NSInteger)getReconnectionSeconds;
-(void)handleClientDisconnection:(NSString *)reason;
-(void)handleLogout;
-(void)handleLogin:(SFSEvent *)evt;
-(BitSwarmClient *)getSocketEngine;

/** Initializes SmartFox Client

 @param debug (BOOL) determines debugging mode.
 @param delegate (id <ISFSEvents>) delegate object that listens api events.

 @see smartFoxWithDebugMode:delegate:
 */
-(id)initSmartFoxWithDebugMode:(BOOL)debug delegate:(id <ISFSEvents>)delegate;

/** Initializes and returns SmartFox2XClient instance.

 @param debug (BOOL) determines debugging mode.
 @param delegate (id <ISFSEvents>) delegate object that listens api events.

 @see initSmartFoxWithDebugMode:delegate:
 */
+(id)smartFoxWithDebugMode:(BOOL)debug delegate:(id <ISFSEvents>)delegate;

/** Establish connection with SmartFoxServer 2X.
 
 Client will use the settings loaded via loadConfig

 @exception NSException if host/address or port in the config file is invalid.

 @see loadConfig:connectOnSuccess:
 */
-(void)connect;

/** Establish connection with SmartFoxServer 2X.

 Client will use the settings loaded via loadConfig if you pass nil value for host or -1 for port.

 @param host (NSString *) the address of the server to connect to
 @param port (NSInteger) the TCP port (default port used by SFS2X is 9933)

 @exception NSException if an invalid host/address or port is passed 

 @see connect
 @see loadConfig:connectOnSuccess:
 */
-(void)connect:(NSString *)host port:(NSInteger)port;

/** Disconnect from SmartFoxServer.
 */
-(void)disconnect;

/** Simulate an abrupt disconnection

 To be used for testing/simulations only.
 */
-(void)killConnection;

/** Load a client configuration file.

 The SmartFox2XClient instance can be configured through an external xml configuration file loaded at run-time.
 By default, the loadConfig method loads a file named sfs—config.xml, placed in the main bundle of the application.
 If the connectOnSuccess parameter is set to <b>YES</b>, on loading completion the connect:port: method is automatically called by the API, otherwise the onConfigLoadSuccess event is dispatched.
 In case of a loading error, the onConfigLoadFailure (SmartFoxISFSEvents) event is fired.

 @param filePath (NSString *) external xml configuration file name (optional).
 @param connectOnSuccess (BOOL) a boolean flag indicating if the connection to SmartFoxServer must be attempted upon configuration loading completion.

 <b>Usage Note:</b><br/>
 The external xml configuration file has the following structure; ip, port and zone parameters are mandatory, all other parameters are optional.

    <SmartFoxConfig>
        <ip>127.0.0.1</ip>
        <port>9933</port>
        <udpHost>127.0.0.1</udpHost>
        <udpPort>9934</udpPort>
        <zone>simpleChat</zone>
        <debug>true</debug>
        <blueBoxIpAddress>127.0.0.1</blueBoxIpAddress>
        <blueBoxPort>9339</blueBoxPort>
        <smartConnect>true</smartConnect>
        <httpPort>8080</httpPort>
        <httpPollSpeed>750</httpPollSpeed>
        <rawProtocolSeparator>%</rawProtocolSeparator>
    </SmartFoxConfig>

 <b>Example:</b><br/>
 The following example shows how to load an external configuration file.

    [smartfox loadConfig:@"sfs-config.xml" connectOnSuccess:NO];
     
    -(void)onConfigLoadSuccess:(SFSEvent *)evt
    {
        NSLog(@"Config file loaded, now connecting...");
        [smartFox connect:smartFox.ipAddress, smartFox.port];
    }
    
     -(void)onConfigLoadFailure:(SFSEvent *)evt
    {
        NSLog(@"Failed loading config file: %@", [evt.params objectForKey:@"message"]);
    }

 @see [ISFSEvents onConfigLoadSuccess:]
 @see [ISFSEvents onConfigLoadFailure:]
 
 @warning *<b>Important:</b>* The SmartFox2XClient configuration file (client-side) should not be confused with the SmartFoxServer configuration file (server-side).
 */
-(void)loadConfig:(NSString *)filePath connectOnSuccess:(BOOL)connectOnSuccess;

-(void)loadConfig;

/** Sends a request to the Server.

 All the available request objects are found in the documentation. 

 @param request (id <IRequest>) a request object.
 */
-(void)send:(id <IRequest>)request;

/** Finds a Room from its Id or returns nil if no Room with that Id exist in the room list
 
 @param roomId (NSInteger) the id of the room.

 @see roomList
 @see SFSRoom
 */
-(id <Room>)getRoomById:(NSInteger)roomId;

/** Finds a Room from its name or returns nil if no Room with that name exist in the room list

 @param roomName (NSString *) the name of the room.

 @see roomList
 @see SFSRoom
 */
-(id <Room>)getRoomByName:(NSString *)roomName;

/** Return the list of Rooms from a specific Room Group
 
 @param groupId: (NSString *)the name of the Group
 @return the list of rooms
 @see SFSRoom
 */
-(NSArray *)getRoomListFromGroup:(NSString *)groupId;

/** Initialize the UDP Manager to create a UDP connection. If the host and port parameters are left nil and -1
 
 The UDP Manager will use the settings found in the config XML. UDP is only available through an ExtensionRequest.
 
 @param udpHost: (NSString *) the host (IP) of the SFS Server
 @param udpPort: (NSInteger) the port of the SFS Server to handle UDP

    -(void)onLogin:(SFSEvent *)evt
    {
        NSArray *keys = [evt.params allKeys];
        for (NSString *key in keys)
        {
            // do somthing...
        }
        //initialize the UDP socket
        [smartFox initUDP :nil :-1];
    }
 
 @warning *<b>Important:</b>* 
 It is recommended to initialize the UDP connection after successful login to a zone.
 */
-(void)initUDP:(NSString *)udpHost :(NSInteger)udpPort;

@end
