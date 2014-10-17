// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMIXPANELIOS_H
#define	MOAIMIXPANELIOS_H

#import <moai-core/headers.h>

//================================================================//
// MOAIMixpanelIOS
//================================================================//
/**	@lua	MOAIMixpanelIOS
	@text	Wrapper for Mixpanel interface.
*/
class MOAIMixpanelIOS :
	public MOAIGlobalClass < MOAIMixpanelIOS, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_startSession						( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIMixpanelIOS )
	
	//----------------------------------------------------------------//
			MOAIMixpanelIOS				();
			~MOAIMixpanelIOS				();
	void	RegisterLuaClass			( MOAILuaState& state );
};

#endif
