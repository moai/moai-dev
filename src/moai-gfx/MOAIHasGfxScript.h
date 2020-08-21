// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHASGFXSCRIPT_H
#define	MOAIHASGFXSCRIPT_H

typedef MOAIRetainedCmdStream MOAIGfxScript;

//================================================================//
// MOAIHasGfxScript
//================================================================//
// TODO: doxygen
class MOAIHasGfxScript :
	public virtual MOAILuaObject {
protected:

	ZLStrongPtr < MOAIGfxScript > mGfxScript;
	
	MOAI_LUA_OBJECT_VISITOR_FRIEND
	
	//----------------------------------------------------------------//
	static int			_getGfxScript				( lua_State* L );
	static int			_gfx						( lua_State* L );
	static int			_setGfxScript				( lua_State* L );
	
	//----------------------------------------------------------------//
	void							_RegisterLuaClass						( RTTIVisitorHistory& history, MOAILuaState& state );
	void							_RegisterLuaFuncs						( RTTIVisitorHistory& history, MOAILuaState& state );
	virtual MOAIGfxScript&			MOAIHasGfxScript_AffirmGfxScript		();

public:

	//----------------------------------------------------------------//
	MOAIGfxScript&		AffirmGfxScript				();
	MOAIGfxScript*		GetGfxScript				();
						MOAIHasGfxScript			();
						~MOAIHasGfxScript			();
	void				SetGfxScript				( MOAIGfxScript* gfxScript );
	
	//----------------------------------------------------------------//
	template < typename API_TYPE >
	MOAICmdMediumWithAPI < API_TYPE >& _ ( MOAIAbstractCmdHandler& handler ) {
		return this->AffirmGfxScript ()._ < API_TYPE >( handler );
	}
};

#endif
