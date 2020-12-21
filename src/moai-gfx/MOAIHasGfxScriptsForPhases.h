// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHASGFXSCRIPTSFORPHASES_H
#define	MOAIHASGFXSCRIPTSFORPHASES_H

#include <moai-gfx/MOAIDraw.h>

//================================================================//
// MOAIHasGfxScriptsForPhases
//================================================================//
class MOAIHasGfxScriptsForPhases :
	public virtual MOAILuaObject {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND
	
	STLMap < u32, ZLStrongPtr < MOAIGfxScript > > mGfxScripts;

	//----------------------------------------------------------------//
	static int			_getGfxScript				( lua_State* L );
	static int			_gfx						( lua_State* L );
	static int			_gfxScript					( lua_State* L );
	static int			_setGfxScript				( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	//----------------------------------------------------------------//
	MOAIGfxScript&		AffirmGfxScript					( MOAIRenderPhaseEnum::_ renderPhase );
	MOAIGfxScript*		GetGfxScript					( MOAIRenderPhaseEnum::_ renderPhase );
						MOAIHasGfxScriptsForPhases		();
	virtual 			~MOAIHasGfxScriptsForPhases		();
	void				SetGfxScript					( MOAIRenderPhaseEnum::_ renderPhase, MOAIGfxScript* gfxScript );
	
	//----------------------------------------------------------------//
	template < typename API_TYPE >
	MOAICmdMediumWithAPI < API_TYPE >& _ ( MOAIAbstractCmdHandler& handler, MOAIRenderPhaseEnum::_ renderPhase ) {
		return this->AffirmGfxScript ( renderPhase )._ < API_TYPE >( handler );
	}
};

#endif
