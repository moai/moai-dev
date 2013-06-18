//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef MOAIGROWMOBILEIOS_H
#define MOAIGROWMOBILEIOS_H

#ifndef DISABLE_GROW_MOBILE

#import <Foundation/Foundation.h>
#import <moaicore/moaicore.h>

#import "GrowMobileSDK.h"

//================================================================//
// MOAIGrowMobileIOS
//================================================================//
/**	@name	MOAIGrowMobileIOS
 @text	Wrapper for Flurry integration on iOS devices.
 Flurry provides analytics and behaviour statistics.
 Exposed to lua via MOAIFlurry on
 all mobile platforms.
 
 */
class MOAIGrowMobileIOS :
public MOAIGlobalClass < MOAIGrowMobileIOS, MOAILuaObject > {
private:
    
	//----------------------------------------------------------------//
	static int _init					( lua_State* L );
	static int _reportOpen              ( lua_State* L );
	static int _reportAction			( lua_State* L );
	static int _reportInAppPurchase		( lua_State* L );
	static int _reportAdView            ( lua_State* L );
	static int _reportOfferWall         ( lua_State* L );
    
public:
    
	DECL_LUA_SINGLETON ( MOAIGrowMobileIOS );
    
	MOAIGrowMobileIOS		();
	~MOAIGrowMobileIOS	();
	void	RegisterLuaClass	( MOAILuaState& state );
};

#endif  //DISABLE_GROW_MOBILE

#endif // MOAIFLURRY_H