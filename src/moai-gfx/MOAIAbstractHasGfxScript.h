// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTHASGFXSCRIPT_H
#define	MOAIABSTRACTHASGFXSCRIPT_H

#include <moai-gfx/MOAIAbstractDrawingLuaAPI.h>
#include <moai-gfx/MOAIGfxConsts.h>
#include <moai-gfx/MOAIDrawingAPI.h>

class MOAIAbstractGfxScript;
class MOAIBlendMode;
class MOAIGfxScriptRetained;
class MOAIShader;
class MOAITexture;

//================================================================//
// MOAIAbstractHasGfxScript
//================================================================//
// TODO: doxygen
class MOAIAbstractHasGfxScript :
	public virtual MOAILuaObject {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND
	
	//----------------------------------------------------------------//
	static int					_getGfxScript				( lua_State* L );
	static int					_setGfxScript				( lua_State* L );
	
	//----------------------------------------------------------------//
	void								_RegisterLuaClass							( RTTIVisitorHistory& history, MOAILuaState& state );
	void								_RegisterLuaFuncs							( RTTIVisitorHistory& history, MOAILuaState& state );
	virtual MOAIGfxScriptRetained&		MOAIAbstractHasGfxScript_AffirmGfxScript	() = 0;
	virtual MOAIAbstractGfxScript*		MOAIAbstractHasGfxScript_GetGfxScript		() = 0;
	virtual void						MOAIAbstractHasGfxScript_SetGfxScript		( MOAIAbstractGfxScript* gfxScript ) = 0;

public:

	//----------------------------------------------------------------//
	MOAIGfxScriptRetained&		AffirmGfxScript				();
	MOAIAbstractGfxScript*		GetGfxScript				();
								MOAIAbstractHasGfxScript	();
								~MOAIAbstractHasGfxScript	();
	void						SetGfxScript				( MOAIAbstractGfxScript* gfxScript );
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE& AffirmGfxScriptWithType () {
	
		MOAIAbstractGfxScript* gfxScript = this->GetGfxScript ();
		TYPE* gfxScriptWithType = MOAICast < TYPE >( gfxScript );
		if ( !gfxScriptWithType ) {
			gfxScriptWithType = new TYPE ();
			this->SetGfxScript ( gfxScriptWithType );
		}
		assert ( gfxScriptWithType );
		return *gfxScriptWithType;
	}
};

#endif
