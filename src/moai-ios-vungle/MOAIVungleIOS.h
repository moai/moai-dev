//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef	MOAIVUNGLEIOS_H
#define	MOAIVUNGLEIOS_H

#include <moai-core/headers.h>

@class MoaiVungleDelegate;

//================================================================//
// MOAIVungleIOS
//================================================================//
class MOAIVungleIOS :
	public MOAIGlobalClass < MOAIVungleIOS, MOAIGlobalEventSource > {
private:

	MoaiVungleDelegate*	mDelegate;
	
	//----------------------------------------------------------------//
	static int		_allowAutoRotate			( lua_State* L );
	static int		_cacheSizeGet				( lua_State* L );
	static int		_cacheSizeSet				( lua_State* L );
	static int		_displayAdvert				( lua_State* L );
	static int		_init						( lua_State* L );
	static int		_isVideoAvailable			( lua_State* L );
	
public:
		
	enum {
		MOVIE_PLAYED,
		TOTAL,
	};

	DECL_LUA_SINGLETON ( MOAIVungleIOS );

	//----------------------------------------------------------------//
					MOAIVungleIOS				();
					~MOAIVungleIOS				();
	void			NotifyMoviePlayed			( bool playedFull );
	void			RegisterLuaClass			( MOAILuaState& state );
};

#endif  //MOAIVUNGLEIOS_H
