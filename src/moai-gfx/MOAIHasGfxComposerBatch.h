// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHASGFXCOMPOSERBATCH_H
#define	MOAIHASGFXCOMPOSERBATCH_H

#include <moai-gfx/MOAIAbstractGfxComposerBatchInterface.h>

//================================================================//
// MOAIHasComposerBatch
//================================================================//
// TODO: doxygen
class MOAIHasGfxComposerBatch :
	public virtual MOAIAbstractGfxComposerBatchInterface {
protected:

	MOAILuaSharedPtr < MOAIGfxComposerBatch > mComposerBatch;

	//----------------------------------------------------------------//
	static int					_affirmComposerBatch		( lua_State* L );
	static int					_getComposerBatch			( lua_State* L );
	static int					_setComposerBatch			( lua_State* L );

	//----------------------------------------------------------------//
	void						_RegisterLuaClass											( RTTIVisitorHistory& history, MOAILuaState& state );
	void						_RegisterLuaFuncs											( RTTIVisitorHistory& history, MOAILuaState& state );
	MOAIAbstractGfxComposer&	MOAIAbstractGfxComposerBatchInterface_AffirmComposer		( ZLIndex index );
	MOAIGfxComposerBatch&		MOAIAbstractGfxComposerBatchInterface_AffirmComposerBatch	();
	MOAIAbstractGfxComposer*	MOAIAbstractGfxComposerBatchInterface_GetComposer			( ZLIndex index );
	MOAIGfxComposerBatch*		MOAIAbstractGfxComposerBatchInterface_GetComposerBatch 		();

public:

	DECL_LUA_FACTORY ( MOAIHasGfxComposerBatch )

	//----------------------------------------------------------------//
								MOAIHasGfxComposerBatch		();
								~MOAIHasGfxComposerBatch	();
};

#endif
