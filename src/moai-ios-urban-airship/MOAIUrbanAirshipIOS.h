// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIURBANAIRSHIPIOS_H
#define	MOAIURBANAIRSHIPIOS_H

#import <Foundation/Foundation.h> 
#import <moai-core/headers.h>

#import "UAConfig.h"

//================================================================//
// MOAIUrbanAirshipIOS
//================================================================//
// TODO: doxygen
class MOAIUrbanAirshipIOS :
	public MOAIGlobalClass < MOAIUrbanAirshipIOS, MOAILuaObject > {
private:

	UAConfig* mConfig;

	//----------------------------------------------------------------//
	static int	_resetBadge 			( lua_State* L );
	static int	_setAppKeyAndSecret 	( lua_State* L );
	static int	_setClearKeychain 		( lua_State* L );
	static int	_setProductionFlag 		( lua_State* L );
	static int	_setTags				( lua_State* L );
	static int	_takeOff 				( lua_State* L );
	static int	_validate 				( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAIUrbanAirshipIOS );
	
	//----------------------------------------------------------------//
			MOAIUrbanAirshipIOS		();
			~MOAIUrbanAirshipIOS	();
	void	RegisterLuaClass		( MOAILuaState& state );
};

#endif  //MOAICRITTERCISM_H
