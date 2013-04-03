//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef MOAISmartFoxIOS_H
#define MOAISmartFoxIOS_H

#ifndef DISABLE_SMARTFOX

#import <Foundation/Foundation.h> 
#import <moaicore/moaicore.h>
#import <SFS2XAPIIOS/SmartFox2XClient.h>


//================================================================//
// MOAISmartFoxIOS
//================================================================//
/**	@name	MOAISmartFoxIOS
	@text	Wrapper for SmartFox integration on iOS devices.


*/

@class MoaiSmartFoxDelegate;

class MOAISmartFoxIOS :
	public MOAIGlobalClass < MOAISmartFoxIOS, MOAILuaObject >,
	public MOAIGlobalEventSource {
private:
		
	//----------------------------------------------------------------//
	static int	_init					( lua_State* L );
	static int	_initWithSocket		   ( lua_State* L );
	static int	_connect				( lua_State* L );
	static int	_isConnected			( lua_State* L );
	static int	_login					( lua_State* L );
	static int	_sendPublicMessageRequest		( lua_State* L );
	static int	_sendJoinRoomRequest	( lua_State* L );
	static int	_sendRoomVariablesRequest		( lua_State* L );
	static int	_sendUserVariablesRequest		( lua_State* L );	
	static int	_sendObjectMessageRequest		( lua_State* L );
	static int	_sendQuickJoinGameRequest		( lua_State* L );
	static int	_sendLeaveRoomRequest	( lua_State* L );
	static int	_sendExtensionRequest	( lua_State* L );

	static int	_sendInitBuddyListRequest	( lua_State* L );
	static int	_sendAddBuddyRequest	( lua_State* L );
	static int	_sendRemoveBuddyRequest	( lua_State* L );
	static int	_sendBlockBuddyRequest	( lua_State* L );
	static int	_sendGoOnlineRequest	( lua_State* L );
	static int	_sendSetBuddyVariablesRequest	( lua_State* L );

	static int	_sendPlayerToSpectatorRequest	( lua_State* L );
	static int	_sendSpectatorToPlayerRequest	( lua_State* L );

	static int	_sendFindRoomsRequest	( lua_State* L );		

	static int	_sendSubscribeRoomGroupRequest      ( lua_State* L );
	static int	_sendUnsubscribeRoomGroupRequest	( lua_State* L );
        
public:
    
	DECL_LUA_SINGLETON ( MOAISmartFoxIOS );
		        
    SmartFox2XClient*               mSmartFox;
	MoaiSmartFoxDelegate*			mSmartFoxDelgate;
	SFSRoom*						mCurrentRoom;
	
		
	enum {
		ON_CONNECTION,
		ON_CONNECTION_LOST,
		ON_CONNECTION_RETRY,
		ON_CONNECTION_RESUME,
		ON_LOGIN,
		ON_LOGIN_ERROR,
		ON_ROOM_JOIN,
		ON_ROOM_JOIN_ERROR,
		ON_USER_ENTER_ROOM,
        ON_USER_EXIT_ROOM,
        ON_USER_COUNT_CHANGE,
        ON_ROOM_ADD,
        ON_ROOM_REMOVE,
        ON_PUBLIC_MESSAGE,
        ON_PRIVATE_MESSAGE,
        ON_ROOM_CREATION_ERROR,
        ON_ROOM_VARIABLES_UDATE,
        ON_OBJECT_MESSAGE,
		ON_EXTENSION_RESPONSE,
		ON_BUDDY_MESSAGE,
		ON_BUDDY_LIST_INIT,
		ON_BUDDY_ADD,
		ON_BUDDY_REMOVE,
		ON_BUDDY_BLOCK,
		ON_BUDDY_ONLINE_STATUS_UPDATE,
		ON_BUDDY_VARIABLE_UPDATE,
		ON_BUDDY_ERROR,
		ON_INVITATION,
		ON_INVITATION_REPLY,
		ON_INVITATION_REPLY_ERROR,
		ON_SPECTATOR_TO_PLAYER,
		ON_SPECTATOR_TO_PLAYER_ERROR,
		ON_PLAYER_TO_SPECTATOR,
		ON_PLAYER_TO_SPECTATOR_ERROR,
		ON_SUBSCRIBE_ROOM_GROUP,
		ON_SUBSCRIBE_ROOM_GROUP_ERROR,
		ON_UNSUBSCRIBE_ROOM_GROUP,
		ON_UNSUBSCRIBE_ROOM_GROUP_ERROR,
		ON_ROOM_FIND_RESULT,
		TOTAL
	};

	enum {
		BOOL_MATCH_EQUALS,
		BOOL_MATCH_NOT_EQUALS,
		
		NUMBER_MATCH_EQUALS,
		NUMBER_MATCH_NOT_EQUALS,
		NUMBER_MATCH_GREATER_THAN,
		NUMBER_MATCH_GREATER_THAN_OR_EQUAL_TO,
		NUMBER_MATCH_LESS_THAN,
		NUMBER_MATCH_LESS_THAN_OR_EQUAL_TO,
		
		STRING_MATCH_EQUALS,
		STRING_MATCH_NOT_EQUALS,
		STRING_MATCH_CONTAINS,
		STRING_MATCH_STARTS_WITH,
		STRING_MATCH_ENDS_WITH
	};
	
		

    MOAISmartFoxIOS			();
    ~MOAISmartFoxIOS		();

    //-------------------------------------------------------------
   	void	RegisterLuaClass		( MOAILuaState& state );
	void	Connection				(SFSEvent *evt);
    void	ConnectionLost			(SFSEvent *evt);
	void	ConnectionResume		(SFSEvent *evt);
	void	ConnectionRetry			(SFSEvent *evt);
	void	Login					(SFSEvent *evt);
    void	LoginError				(SFSEvent *evt);
	void	RoomJoin				(SFSEvent *evt);
    void	RoomJoinError			(SFSEvent *evt);
    void	UserEnterRoom			(SFSEvent *evt);
    void	UserExitRoom			(SFSEvent *evt);
    void	UserCountChange			(SFSEvent *evt);
    void	RoomAdd					(SFSEvent *evt);
    void	RoomRemove				(SFSEvent *evt);
    void	PublicMessage			(SFSEvent *evt);
    void	PrivateMessage			(SFSEvent *evt);
    void	RoomCreationError		(SFSEvent *evt);
    void	RoomVariablesUpdate		(SFSEvent *evt);
    void	ObjectMessage			(SFSEvent *evt);
	void	ExtensionResponse		(SFSEvent *evt);

	void	Invitation				(SFSEvent *evt);
	void	InvitationReply			(SFSEvent *evt);
	void	InvitationReplyError	(SFSEvent *evt);
	void	BuddyMessage			(SFSEvent *evt);
	void	BuddyListInit			(SFSEvent *evt);
	void	BuddyAdd				(SFSEvent *evt);
	void	BuddyRemove				(SFSEvent *evt);
	void	BuddyBlock				(SFSEvent *evt);
	void	BuddyOnlineStatusUpdate	(SFSEvent *evt);
	void	BuddyVariablesUpdate	(SFSEvent *evt);
	void	BuddyError				(SFSEvent *evt);

	void	SpectatorToPlayer		(SFSEvent *evt);
	void	SpectatorToPlayerError	(SFSEvent *evt);
	void	PlayerToSpectator		(SFSEvent *evt);
	void	PlayerToSpectatorError	(SFSEvent *evt);
		
	void	SubscribeRoomGroup		(SFSEvent *evt);
	void	SubscribeRoomGroupError	(SFSEvent *evt);
	void	UnsubscribeRoomGroup		(SFSEvent *evt);
	void	UnsubscribeRoomGroupError	(SFSEvent *evt);
	
	void	RoomFindResult	    	(SFSEvent *evt);
		
	// helper functions
    void	processUserVariables	(MOAILuaStateHandle stateOld,  SFSUser* player);
		
};


//================================================================//
// MoaiChartBoostDelegate
//================================================================//
@interface MoaiSmartFoxDelegate : NSObject < ISFSEvents > {
@private
}
@end


#endif  //DISABLE_SMARTFOX

#endif // MOAISMARTFOX_H
