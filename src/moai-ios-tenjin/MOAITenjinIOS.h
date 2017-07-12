//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef	MOAITENJINIOS_H
#define	MOAITENJINIOS_H

#include <moai-core/headers.h>
#include <moai-ios-tenjin/MOAITenjinIOSStoreKitListener.h>

//================================================================//
// MOAITenjinIOS
//================================================================//
class MOAITenjinIOS :
	public MOAIGlobalClass < MOAITenjinIOS, MOAIGlobalEventSource > {
private:

	//----------------------------------------------------------------//
	static int		_init							( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAITenjinIOS );
		
	MOAITenjinIOSStoreKitListener*	mStoreKitListener;

	//----------------------------------------------------------------//
					MOAITenjinIOS				();
					~MOAITenjinIOS				();
	void			LogPurchaseEvent			( SKPaymentTransaction * transaction );
	void			RegisterLuaClass			( MOAILuaState& state );
};

#endif  //MOAITENJINIOS_H
