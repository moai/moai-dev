// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIKONTAGENTIOS_H
#define	MOAIKONTAGENTIOS_H

#import <moai-core/headers.h>

//================================================================//
// MOAIKontagentIOS
//================================================================//
/**	@lua	MOAIKontagentIOS
	@text	Wrapper for Mixpanel interface.
*/
class MOAIKontagentIOS :
	public MOAIGlobalClass < MOAIKontagentIOS, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_startSession				( lua_State* L );
	static int	_stopSession				( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIKontagentIOS )
	
	//----------------------------------------------------------------//
			MOAIKontagentIOS				();
			~MOAIKontagentIOS				();
	void	RegisterLuaClass				( MOAILuaState& state );
};

#endif
