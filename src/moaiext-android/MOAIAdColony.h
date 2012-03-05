// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIADCOLONY_H
#define MOAIADCOLONY_H

#ifndef DISABLE_ADCOLONY

#include <moaicore/moaicore.h>

//================================================================//
// MOAIAdColony
//================================================================//
class MOAIAdColony :
	public MOAIGlobalClass < MOAIAdColony, MOAILuaObject > {
private:
    
	//----------------------------------------------------------------//
	static int	_init				( lua_State* L );
	static int	_playVideo			( lua_State* L );
	static int	_videoReadyForZone	( lua_State* L );
	
public:
    
	DECL_LUA_SINGLETON ( MOAIAdColony );
	
                    MOAIAdColony		();
					~MOAIAdColony       ();
	void			RegisterLuaClass	( MOAILuaState& state );
};

#endif  //DISABLE_ADCOLONY

#endif  //MOAIADCOLONY_H
