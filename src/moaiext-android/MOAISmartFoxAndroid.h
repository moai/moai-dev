// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISMARTFOXANDROID_H
#define	MOAISMARTFOXANDROID_H

#ifndef DISABLE_SMARTFOX


#import <moaicore/moaicore.h>

#import "smartfox/Core/SFSEvent.h"

//using namespace Sfs2X;


//================================================================//
// MOAISmartFoxIOS
//================================================================//
/**	@name	MOAISmartFoxIOS
 @text	Wrapper for SmartFox integration on iOS devices.
 
 
 */

class MOAISmartFoxAndroid :
public MOAIGlobalClass < MOAISmartFoxAndroid, MOAILuaObject >,
public MOAIGlobalEventSource {
private:
    
	//----------------------------------------------------------------//
	static int	_init					( lua_State* L );
	static int	_initWithSocket			( lua_State* L );
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
    
	DECL_LUA_SINGLETON ( MOAISmartFoxAndroid );
    
	enum {
		ON_CONNECTION,
		ON_CONNECTION_LOST,
		ON_CONNECTION_RETRY,
		ON_CONNECTION_RESUME,
		ON_LOGIN,
		ON_LOGOUT,
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
        ON_SMARTFOX_EVENT,
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
	
    
    
    MOAISmartFoxAndroid			();
    ~MOAISmartFoxAndroid		();
    
    //-------------------------------------------------------------
   	void	RegisterLuaClass		( MOAILuaState& state );
	void	Connection				(Sfs2X::Core::SFSEvent *evt);
    void	ConnectionLost			(Sfs2X::Core::SFSEvent *evt);
	void	ConnectionResume		(Sfs2X::Core::SFSEvent *evt);
	void	ConnectionRetry			(Sfs2X::Core::SFSEvent *evt);
	void	Login					(Sfs2X::Core::SFSEvent *evt);
	void	Logout					(Sfs2X::Core::SFSEvent *evt);
    void	LoginError				(Sfs2X::Core::SFSEvent *evt);
	void	RoomJoin				(Sfs2X::Core::SFSEvent *evt);
    void	RoomJoinError			(Sfs2X::Core::SFSEvent *evt);
    void	UserEnterRoom			(Sfs2X::Core::SFSEvent *evt);
    void	UserExitRoom			(Sfs2X::Core::SFSEvent *evt);
    void	UserCountChange			(Sfs2X::Core::SFSEvent *evt);
    void	RoomAdd					(Sfs2X::Core::SFSEvent *evt);
    void	RoomRemove				(Sfs2X::Core::SFSEvent *evt);
    void	PublicMessage			(Sfs2X::Core::SFSEvent *evt);
    void	PrivateMessage			(Sfs2X::Core::SFSEvent *evt);
    void	RoomCreationError		(Sfs2X::Core::SFSEvent *evt);
    void	RoomVariablesUpdate		(Sfs2X::Core::SFSEvent *evt);
    void	ObjectMessage			(Sfs2X::Core::SFSEvent *evt);
	void	ExtensionResponse		(Sfs2X::Core::SFSEvent *evt);
    
	void	Invitation				(Sfs2X::Core::SFSEvent *evt);
	void	InvitationReply			(Sfs2X::Core::SFSEvent *evt);
	void	InvitationReplyError	(Sfs2X::Core::SFSEvent *evt);
	void	BuddyMessage			(Sfs2X::Core::SFSEvent *evt);
	void	BuddyListInit			(Sfs2X::Core::SFSEvent *evt);
	void	BuddyAdd				(Sfs2X::Core::SFSEvent *evt);
	void	BuddyRemove				(Sfs2X::Core::SFSEvent *evt);
	void	BuddyBlock				(Sfs2X::Core::SFSEvent *evt);
	void	BuddyOnlineStatusUpdate	(Sfs2X::Core::SFSEvent *evt);
	void	BuddyVariablesUpdate	(Sfs2X::Core::SFSEvent *evt);
	void	BuddyError				(Sfs2X::Core::SFSEvent *evt);
    
	void	SpectatorToPlayer		(Sfs2X::Core::SFSEvent *evt);
	void	SpectatorToPlayerError	(Sfs2X::Core::SFSEvent *evt);
	void	PlayerToSpectator		(Sfs2X::Core::SFSEvent *evt);
	void	PlayerToSpectatorError	(Sfs2X::Core::SFSEvent *evt);
    
	void	SubscribeRoomGroup		(Sfs2X::Core::SFSEvent *evt);
	void	SubscribeRoomGroupError	(Sfs2X::Core::SFSEvent *evt);
	void	UnsubscribeRoomGroup		(Sfs2X::Core::SFSEvent *evt);
	void	UnsubscribeRoomGroupError	(Sfs2X::Core::SFSEvent *evt);
	
	void	RoomFindResult	    	(Sfs2X::Core::SFSEvent *evt);
    
    void    SmartFoxEvent           (const char* type, const char* arguments);
    
	// helper functions
    
};


#endif  //DISABLE_SMARTFOX
#endif  //MOAISMARTFOX_H
