// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITSTOREGAMECENTERANDROID_H
#define	MOAITSTOREGAMECENTERANDROID_H

#ifndef DISABLE_TSTOREGAMECENTER

#include <moaicore/moaicore.h>

//================================================================//
// MOAITstoreGamecenterAndroid
//================================================================//
/**	@name	MOAITstoreGamecenterAndroid
*/
class MOAITstoreGamecenterAndroid :
	public MOAIGlobalClass < MOAITstoreGamecenterAndroid, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_authTstore					( lua_State* L );
	static int	_checkTstoreInstalled		( lua_State* L );
	static int	_disableGamecenter			( lua_State* L );
	static int	_enableGamecenter			( lua_State* L );
	static int	_getRankList				( lua_State* L );
	static int	_installGamecenter			( lua_State* L );
	static int	_installTstore				( lua_State* L );
	static int	_setListener				( lua_State* L );
	static int	_setPoint  					( lua_State* L );
	static int	_startGamecenter			( lua_State* L );
	
	
public:

	DECL_LUA_SINGLETON ( MOAITstoreGamecenterAndroid );
	
	enum {
		AUTH_RESPONSE_SUCCESS,
		AUTH_RESPONSE_EXITED,
		DISABLE_GAMECENTER_RESPONSE,
		GET_RANKLIST_RESPONSE,
		SET_POINT_RESPONSE,
		TOTAL
	};
	
	enum {
		GAMECENTER_INSTALLED,
		GAMECENTER_UPGRADING,
		GAMECENTER_NOT_INSTALLED
	};

	MOAILuaRef		mListeners [ TOTAL ];
	
			MOAITstoreGamecenterAndroid		();
			~MOAITstoreGamecenterAndroid	();
	void	AKUNotifyAuthExitResponse		();		
	void	AKUNotifyAuthSuccessResponse	();
	void	AKUNotifyDisableSuccessResponse	();
	void	AKUNotifyScoreListResponse		( cc8* jsonData );
	void	RegisterLuaClass				( MOAILuaState& state );
};

#endif  //DISABLE_TSTOREGAMECENTER

#endif  //MOAITSTOREGAMECENTERANDROID_H