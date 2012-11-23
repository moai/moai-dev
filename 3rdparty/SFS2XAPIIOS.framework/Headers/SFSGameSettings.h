//
//  SFSGameSettings.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "RoomSettings.h"
#import "ISFSObject.h"

@class MatchExpression;

/** This class provides all the settings required to create an SFSGame(*).
 
 The SFSGame(*) object extends the Room object providing new advanced features such as Player matching, Game invitations, public and private Games, quick game joining etc.
 
 (*) = the <SFSGame> class exists only on the server side as extension of the <SFSRoom> class. On the client side it's seen as a regular Room.
 
 This is a quick overview of the settings that you can use to setup an <SFSGame>

 - <b>isGamePublic</b>: a public game can be joined by any Player whose variables match the SFSGame Player Match Expression. Private games are based on invitations sent by the Game creator.
 - <b>minPlayersToStartGame</b>: the minimum number of players to start the game.
 - <b>invitedPlayers</b>: (private games only) a list of players invited in the Game
 - <b>searchableRooms</b>: (private games only) a list of Rooms where the Game API can search for more players to invite. 
 		The API will look for more players if the number of people invited is smaller than the minPlayersToStartGame. This way you can add your friends to the game and let the system find more players to start it.
 - <b>leaveLastJoinedRoom</b>: auto-remove players from their previous Room after successful join
 - <b>playerMatchExpression</b>: an expression to match players willing to play the game, by default no expression is used
 - <b>spectatorMatchExpression</b>: an expression to match spectators willing to play the game, by default no expression is used
 - <b>invitationExpiryTime</b>: the amount of time allowed for invited players to accept / refuse
 - <b>invitationParameters</b>: optional custom invitation parameters.These could provide details about the inviter, the game, an invitation message etc...
 - <b>notifyGameStartedViaRoomVariable</b>: automatically update a reserved Room Variable to signal that the game is started/stopped. The Room variable uses the <b>global</b> setting to be broadcast outside of the Room. This can be used on the client side to show the game state in your game list.

 See <CreateSFSGameRequest>, <MatchExpression>
 
 */
@interface SFSGameSettings : RoomSettings {
@private
	BOOL _isPublic;
	NSInteger _minPlayersToStartGame;
	NSArray *_invitedPlayers;
	NSArray *_searchableRooms;
	MatchExpression *_playerMatchExpression;
	MatchExpression *_spectatorMatchExpression;
	NSInteger _invitationExpiryTime; 
	BOOL _leaveJoinedLastRoom;
	BOOL _notifyGameStarted;
	id <ISFSObject> _invitationParams;
}

/**
 Check if the Game is public or private
 */
@property (nonatomic, assign) BOOL isPublic;
/**
 The minimum number of players to start the Game
 */
@property (nonatomic, assign) NSInteger minPlayersToStartGame;
/**
 Private games only: a list of invited Players (an Array of Users)
 */
@property (nonatomic, retain) NSArray *invitedPlayers;
/**
 Private games only: a list of room groups (Array of String) where to search for more players
 */
@property (nonatomic, retain) NSArray *searchableRooms;
/**
 (Recommended for public Games only)
 A MatchExpression for filtering Users joining the Game.
 
 @see MatchExpression
 */ 
@property (nonatomic, retain) MatchExpression *playerMatchExpression;
/**
 A MatchExpression for filtering Spectators joining the Game.
 
 @see MatchExpression
 */ 
@property (nonatomic, retain) MatchExpression *spectatorMatchExpression;
/**
 The amount of time (in seconds) available for the User to reply to the invitation.
 Suggested range 10-40 seconds
 */
@property (nonatomic, assign) NSInteger invitationExpiryTime;
/**
 Determines if the Players will leave their previous Room when joining the Game
 */
@property (nonatomic, assign) BOOL leaveJoinedLastRoom;
/**
 Uses a "reserved" global Room Variable to notify the started/stopped status of the game.
 This depends on the numer of Users inside the Room.
 
 @see SFSRoomVariable
 @see ReservedRoomVariables
 */
@property (nonatomic, assign) BOOL notifyGameStarted;
/**
 An optional custom object with parameters (e.g. a message, game details etc...)
 */ 
@property (nonatomic, retain) id <ISFSObject> invitationParams;

-(id)initWithName:(NSString *)name;
+(id)settingsWithName:(NSString *)name;

@end
