// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITSTOREWALLANDROID_H
#define	MOAITSTOREWALLANDROID_H

#include <moai-core/headers.h>

//================================================================//
// MOAITstoreWallAndroid
//================================================================//
/**	@lua	MOAITstoreWallAndroid
*/
class MOAITstoreWallAndroid :
	public MOAIGlobalClass < MOAITstoreWallAndroid, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_setListener		( lua_State* L );
	static int	_showOfferWall		( lua_State* L );

public:

	DECL_LUA_SINGLETON ( MOAITstoreWallAndroid );

	enum {
		TSTORE_CURRENCY_AWARDED,
		TOTAL
	};

	MOAILuaStrongRef		mListeners [ TOTAL ];

			MOAITstoreWallAndroid	();
			~MOAITstoreWallAndroid	();
	void	NotifyCurrencyAwarded	( int amount );
	void	RegisterLuaClass		( MOAILuaState& state );
};

#endif  //MOAITSTOREWALLANDROID_H