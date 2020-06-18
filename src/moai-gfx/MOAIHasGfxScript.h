// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHASGFXSCRIPT_H
#define	MOAIHASGFXSCRIPT_H

#include <moai-gfx/MOAIAbstractGfxScript.h>

class MOAIGfxScript;

//================================================================//
// MOAIHasGfxScript
//================================================================//
// TODO: doxygen
class MOAIHasGfxScript :
	public virtual MOAIAbstractHasGfxScript {
protected:

	ZLStrongPtr < MOAIGfxScript > mGfxScript;
	
	MOAI_LUA_OBJECT_VISITOR_FRIEND
	
	//----------------------------------------------------------------//
	static int			_getGfxScript				( lua_State* L );
	static int			_setGfxScript				( lua_State* L );
	
	//----------------------------------------------------------------//
	void						_RegisterLuaClass							( RTTIVisitorHistory& history, MOAILuaState& state );
	void						_RegisterLuaFuncs							( RTTIVisitorHistory& history, MOAILuaState& state );
	MOAIAbstractGfxScript&		MOAIAbstractHasGfxScript_AffirmGfxScript	();

public:

	//----------------------------------------------------------------//
	MOAIGfxScript*		GetGfxScript				();
						MOAIHasGfxScript			();
						~MOAIHasGfxScript			();
	void				SetGfxScript				( MOAIGfxScript* gfxScript );
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE& AffirmGfxScriptWithType () {
				
		TYPE* gfxScriptWithType = MOAICast < TYPE >( this->GetGfxScript ());
		if ( !gfxScriptWithType ) {
			gfxScriptWithType = new TYPE ();
			this->SetGfxScript ( gfxScriptWithType );
		}
		assert ( gfxScriptWithType );
		return *gfxScriptWithType;
	}
};

#endif
