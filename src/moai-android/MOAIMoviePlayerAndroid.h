// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMOVIEPLAYERANDROID_H
#define	MOAIMOVIEPLAYERANDROID_H

#include <moaicore/moaicore.h>

//================================================================//
// MOAIMoviePlayerAndroid
//================================================================//
/**	@name	MOAIMoviePlayerAndroid
	@text	Wrapper for simple video playback. Exposed to lua via 
			MOAIMoviePlayer on all mobile platforms.
*/
class MOAIMoviePlayerAndroid :
	public MOAIGlobalClass < MOAIMoviePlayerAndroid, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_init			( lua_State* L );
	static int	_play			( lua_State* L );
	static int	_pause			( lua_State* L );
	static int	_setListener	( lua_State* L );
	static int	_stop			( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAIMoviePlayerAndroid );
	
	enum {
		MOVIE_PLAYER_READY,
		MOVIE_PLAYER_COMPLETED,
		TOTAL,
	};
	
	MOAILuaRef		mListeners [ TOTAL ];
	
			MOAIMoviePlayerAndroid		();
			~MOAIMoviePlayerAndroid		();
	void	NotifyMoviePlayerCompleted	();
	void	NotifyMoviePlayerReady		();
	void	RegisterLuaClass			( MOAILuaState& state );
};

#endif  //MOAIMOVIEPLAYERANDROID_H