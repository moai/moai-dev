//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef	MOAITENJINIOS_H
#define	MOAITENJINIOS_H

#include <moai-core/headers.h>

//================================================================//
// MOAITenjinIOS
//================================================================//
class MOAITenjinIOS :
	public MOAIGlobalClass < MOAITenjinIOS, MOAIGlobalEventSource > {
private:

	//----------------------------------------------------------------//
	static int		_cacheInterstitial				( lua_State* L );
	static int		_hasCachedInterstitial			( lua_State* L );
	static int		_init							( lua_State* L );
	static int		_showInterstitial				( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAITenjinIOS );

	//----------------------------------------------------------------//
					MOAITenjinIOS				();
					~MOAITenjinIOS				();
	void 			NotifyInterstitialDismissed		();
	void 			NotifyInterstitialLoadFailed	();
	void			RegisterLuaClass				( MOAILuaState& state );
};

#endif  //MOAITENJINIOS_H
