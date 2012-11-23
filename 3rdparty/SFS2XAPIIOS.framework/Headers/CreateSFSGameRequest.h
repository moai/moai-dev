//
//  CreateSFSGameRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

#import "BaseRequest.h"


EXTERN NSString * const CreateSFSGameRequest_KEY_IS_PUBLIC;
EXTERN NSString * const CreateSFSGameRequest_KEY_MIN_PLAYERS;
EXTERN NSString * const CreateSFSGameRequest_KEY_INVITED_PLAYERS;
EXTERN NSString * const CreateSFSGameRequest_KEY_SEARCHABLE_ROOMS;
EXTERN NSString * const CreateSFSGameRequest_KEY_PLAYER_MATCH_EXP;
EXTERN NSString * const CreateSFSGameRequest_KEY_SPECTATOR_MATCH_EXP;
EXTERN NSString * const CreateSFSGameRequest_KEY_INVITATION_EXPIRY;
EXTERN NSString * const CreateSFSGameRequest_KEY_LEAVE_ROOM;
EXTERN NSString * const CreateSFSGameRequest_KEY_NOTIFY_GAME_STARTED;
EXTERN NSString * const CreateSFSGameRequest_KEY_INVITATION_PARAMS;


@class CreateRoomRequest;
@class SFSGameSettings;

/** The request launches a new public or private game, including game invitations, player matching and a lot more.
 
 The game is launched via the creation of an <SFSGame>, a specialized Room type that provides advanced features during the creation phase of a game. 
 
 A more in-depth overview of the SFS2X Game API is provided in the general documentation
  
 See <SFSGameSettings>
 */
@interface CreateSFSGameRequest : BaseRequest {
@private
	CreateRoomRequest *_createRoomRequest;
	SFSGameSettings *_settings;
}

@property (nonatomic, retain) CreateRoomRequest *createRoomRequest;
@property (nonatomic, retain) SFSGameSettings *settings;

-(id)initWithSettings:(SFSGameSettings *)settings;
/**
 @param settings the game Room settings

 @see SFSGameSettings
 @see MatchExpression
 @see SFSInvitation
 @see [ISFSEvents onRoomAdd:] 
 @see [ISFSEvents onRoomCreationError:]
 
 */
+(id)requestWithSettings:(SFSGameSettings *)settings;

@end
