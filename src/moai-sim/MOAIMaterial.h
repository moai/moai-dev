// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMATERIAL_H
#define	MOAIMATERIAL_H

#include <moai-sim/MOAIMaterialBase.h>

class MOAILight;

//================================================================//
// MOAIMaterialLight
//================================================================//
class MOAIMaterialLight {
private:

	friend class MOAIMaterial;
	friend class MOAIMaterialStackMgr;
	
	static const u32 UNKNOWN_ID = ( u32 )-1;
	
	u32					mGlobalID;
	MOAILight*			mLight;

public:

	//----------------------------------------------------------------//
	MOAIMaterialLight () :
		mGlobalID ( UNKNOWN_ID ),
		mLight ( 0 ) {
	}
};

//================================================================//
// MOAILightSet
//================================================================//
// TODO: doxygen
class MOAIMaterial :
	public MOAIMaterialBase {
private:

	friend class MOAIMaterialStackMgr;


	ZLLeanArray < MOAIMaterialLight >	mEntries;
	
	//----------------------------------------------------------------//
	//static int			_getLight					( lua_State* L );
	//static int			_setLight					( lua_State* L );

public:

	//DECL_LUA_FACTORY ( MOAILightSet )

	//----------------------------------------------------------------//
	void				ClearLights					( MOAILuaObject* owner = 0 );
	MOAILight*			GetLight					( u32 globalID );
						MOAIMaterial				();
	virtual				~MOAIMaterial				();
	void				ReserveLights				( u32 n );
	void				SetLight					( u32 globalID, MOAILight* light, MOAILuaObject* owner = 0 );
};

#endif
