// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHASGFXSCRIPTBATCH_H
#define	MOAIHASGFXSCRIPTBATCH_H

#include <moai-gfx/MOAIAbstractGfxScriptBatchInterface.h>

//================================================================//
// MOAIHasComposerBatch
//================================================================//
// TODO: doxygen
class MOAIHasGfxScriptBatch :
	public virtual MOAIAbstractGfxScriptBatchInterface {
protected:

	MOAILuaSharedPtr < MOAIGfxScriptBatch > mComposerBatch;

	//----------------------------------------------------------------//
	static int					_affirmComposerBatch		( lua_State* L );
	static int					_getComposerBatch			( lua_State* L );
	static int					_setComposerBatch			( lua_State* L );

	//----------------------------------------------------------------//
	void						_RegisterLuaClass											( RTTIVisitorHistory& history, MOAILuaState& state );
	void						_RegisterLuaFuncs											( RTTIVisitorHistory& history, MOAILuaState& state );
	MOAIAbstractGfxScript&		MOAIAbstractGfxScriptBatchInterface_AffirmComposer			( ZLIndex index );
	MOAIGfxScriptBatch&			MOAIAbstractGfxScriptBatchInterface_AffirmComposerBatch		();
	MOAIAbstractGfxScript*		MOAIAbstractGfxScriptBatchInterface_GetComposer				( ZLIndex index );
	MOAIGfxScriptBatch*			MOAIAbstractGfxScriptBatchInterface_GetComposerBatch 		();

public:

	DECL_LUA_FACTORY ( MOAIHasGfxScriptBatch )

	//----------------------------------------------------------------//
								MOAIHasGfxScriptBatch		();
								~MOAIHasGfxScriptBatch		();
};

#endif
