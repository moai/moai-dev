//
//  BaseRequest.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//


#import "Common.h"
#import "ISFSObject.h"
#import "IRequest.h"
#import "SFSValidationException.h"

EXTERN NSInteger const BaseRequest_Handshake;
EXTERN NSInteger const BaseRequest_Login;
EXTERN NSInteger const BaseRequest_Logout;
EXTERN NSInteger const BaseRequest_GetRoomList;
EXTERN NSInteger const BaseRequest_JoinRoom;
EXTERN NSInteger const BaseRequest_AutoJoin;
EXTERN NSInteger const BaseRequest_CreateRoom;
EXTERN NSInteger const BaseRequest_GenericMessage;
EXTERN NSInteger const BaseRequest_ChangeRoomName;
EXTERN NSInteger const BaseRequest_ChangeRoomPassword;
EXTERN NSInteger const BaseRequest_ObjectMessage;
EXTERN NSInteger const BaseRequest_SetRoomVariables;
EXTERN NSInteger const BaseRequest_SetUserVariables;
EXTERN NSInteger const BaseRequest_CallExtension;
EXTERN NSInteger const BaseRequest_LeaveRoom;
EXTERN NSInteger const BaseRequest_SubscribeRoomGroup;
EXTERN NSInteger const BaseRequest_UnsubscribeRoomGroup;
EXTERN NSInteger const BaseRequest_SpectatorToPlayer;
EXTERN NSInteger const BaseRequest_PlayerToSpectator;
EXTERN NSInteger const BaseRequest_ChangeRoomCapacity;
EXTERN NSInteger const BaseRequest_PublicMessage;
EXTERN NSInteger const BaseRequest_PrivateMessage;
EXTERN NSInteger const BaseRequest_ModeratorMessage;
EXTERN NSInteger const BaseRequest_AdminMessage;
EXTERN NSInteger const BaseRequest_KickUser;
EXTERN NSInteger const BaseRequest_BanUser;
EXTERN NSInteger const BaseRequest_ManualDisconnection;
EXTERN NSInteger const BaseRequest_FindRooms;
EXTERN NSInteger const BaseRequest_FindUsers;


EXTERN NSInteger const BaseRequest_InitBuddyList;
EXTERN NSInteger const BaseRequest_AddBuddy;
EXTERN NSInteger const BaseRequest_BlockBuddy;
EXTERN NSInteger const BaseRequest_RemoveBuddy;
EXTERN NSInteger const BaseRequest_SetBuddyVariables;
EXTERN NSInteger const BaseRequest_GoOnline;


EXTERN NSInteger const BaseRequest_InviteUser;
EXTERN NSInteger const BaseRequest_InvitationReply;
EXTERN NSInteger const BaseRequest_CreateSFSGame;
EXTERN NSInteger const BaseRequest_QuickJoinGame;


EXTERN NSString * const BaseRequest_KEY_ERROR_CODE;
EXTERN NSString * const BaseRequest_KEY_ERROR_PARAMS;

@interface BaseRequest : NSObject <IRequest>{

@private
	
	BOOL _isEncrypted;
	NSInteger _id;

@protected	
	NSInteger _targetController;
	id<ISFSObject> _sfso;
    BOOL _isUDP;
	
}

-(id)initWithId:(NSInteger)id_;

@property (nonatomic, retain) id<ISFSObject> sfso;
@property (nonatomic, readwrite) BOOL isUDP;

@end
