//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef	MOAIVUNGLEIOS_H
#define	MOAIVUNGLEIOS_H

#include <moai-core/headers.h>

@class MOAIVungleDelegate;

//================================================================//
// MOAIVungleIOS
//================================================================//
class MOAIVungleIOS :
	public ZLContextClass < MOAIVungleIOS, MOAIGlobalEventSource > {
private:

	MOAIVungleDelegate*		mDelegate;
	bool					mWatchedAd;
	
	//----------------------------------------------------------------//
	static int		_allowAutoRotate			( lua_State* L );
	static int		_cacheSizeGet				( lua_State* L );
	static int		_cacheSizeSet				( lua_State* L );
	static int		_displayAdvert				( lua_State* L );
	static int		_init						( lua_State* L );
	static int		_isVideoAvailable			( lua_State* L );
	
public:
		
	enum {
		AD_VIEWED,
	};

	DECL_LUA_SINGLETON ( MOAIVungleIOS );

	//----------------------------------------------------------------//
					MOAIVungleIOS				();
					~MOAIVungleIOS				();
	void			NotifyMoviePlayed			();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			WatchedAd					( bool playedFull );
};

#endif  //MOAIVUNGLEIOS_H
