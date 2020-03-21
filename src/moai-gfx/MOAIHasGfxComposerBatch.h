// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHASGFXCOMPOSERBATCH_H
#define	MOAIHASGFXCOMPOSERBATCH_H

#include <moai-gfx/MOAIGfxComposerBatchInterface.h>

//================================================================//
// MOAIHasComposerBatch
//================================================================//
// TODO: doxygen
class MOAIHasGfxComposerBatch :
	public virtual MOAIGfxComposerBatchInterface {
protected:

	MOAILuaSharedPtr < MOAIGfxComposerBatch > mComposerBatch;

	//----------------------------------------------------------------//
	static int					_affirmComposerBatch		( lua_State* L );
	static int					_getComposerBatch			( lua_State* L );
	static int					_setComposerBatch			( lua_State* L );

	//----------------------------------------------------------------//
	void						_RegisterLuaClass										( RTTIVisitorHistory& history, MOAILuaState& state );
	void						_RegisterLuaFuncs										( RTTIVisitorHistory& history, MOAILuaState& state );
	MOAIGfxComposer&			MOAIGfxComposerBatchInterface_AffirmComposer			( ZLIndex index );
	MOAIGfxComposerBatch&		MOAIGfxComposerBatchInterface_AffirmComposerBatch		();
	MOAIGfxComposer*			MOAIGfxComposerBatchInterface_GetComposer				( ZLIndex index );
	MOAIGfxComposerBatch*		MOAIGfxComposerBatchInterface_GetComposerBatch 			();

public:

	DECL_LUA_FACTORY ( MOAIHasGfxComposerBatch )

	//----------------------------------------------------------------//
								MOAIHasGfxComposerBatch		();
								~MOAIHasGfxComposerBatch	();
};

#endif
