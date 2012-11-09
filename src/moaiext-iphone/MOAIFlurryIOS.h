//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef MOAIFLURRYIOS_H
#define MOAIFLURRYIOS_H

#ifndef DISABLE_FLURRY

#import <Foundation/Foundation.h>
#import <moaicore/moaicore.h>

#import "Flurry.h"

//================================================================//
// MOAIFlurryIOS
//================================================================//
/**	@name	MOAIFlurryIOS
 @text	Wrapper for Flurry integration on iOS devices.
 Flurry provides analytics and behaviour statistics.
 Exposed to lua via MOAIFlurry on
 all mobile platforms.
 
 */
class MOAIFlurryIOS :
public MOAIGlobalClass < MOAIFlurryIOS, MOAILuaObject > {
private:
    
	//----------------------------------------------------------------//
	static int _startSession		( lua_State* L );
	static int _logEvent			( lua_State* L );
	static int _logTimedEvent		( lua_State* L );
	static int _endTimedEvent		( lua_State* L );
    
public:
    
	DECL_LUA_SINGLETON ( MOAIFlurryIOS );
    
	MOAIFlurryIOS		();
	~MOAIFlurryIOS		();
	void	RegisterLuaClass	( MOAILuaState& state );
};

#endif  //DISABLE_FLURRY

#endif // MOAIFLURRY_H