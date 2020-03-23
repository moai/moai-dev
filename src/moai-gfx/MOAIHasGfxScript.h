// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHASGFXSCRIPT_H
#define	MOAIHASGFXSCRIPT_H

#include <moai-gfx/MOAIAbstractGfxScriptInterface.h>

//================================================================//
// MOAIHasGfxScript
//================================================================//
// TODO: doxygen
class MOAIHasGfxScript :
	public virtual MOAIAbstractGfxScriptInterface {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	ZLStrongPtr < MOAIAbstractGfxScript > mComposer;
	
	//----------------------------------------------------------------//
	static int					_getComposer				( lua_State* L );
	static int					_setComposer				( lua_State* L );
	
	//----------------------------------------------------------------//
	void						_RegisterLuaClass									( RTTIVisitorHistory& history, MOAILuaState& state );
	void						_RegisterLuaFuncs									( RTTIVisitorHistory& history, MOAILuaState& state );
	MOAIAbstractGfxScript&		MOAIAbstractGfxScriptInterface_AffirmComposer		();
	MOAIAbstractGfxScript*		MOAIAbstractGfxScriptInterface_GetComposer			();

public:

	//----------------------------------------------------------------//
	bool						HasComposer					();
								MOAIHasGfxScript			();
								~MOAIHasGfxScript			();
};

#endif
