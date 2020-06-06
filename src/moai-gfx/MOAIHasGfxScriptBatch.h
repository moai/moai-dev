// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHASGFXSCRIPTBATCH_H
#define	MOAIHASGFXSCRIPTBATCH_H

#include <moai-gfx/MOAIAbstractGfxScriptBatch.h>

//================================================================//
// MOAIHasComposerBatch
//================================================================//
// TODO: doxygen
class MOAIHasGfxScriptBatch :
	public virtual MOAIAbstractGfxScriptBatch,
	public virtual MOAIAbstractDrawingLuaAPI {
protected:

	ZLStrongPtr < MOAIGfxScriptBatch > mGfxScriptBatch;

	//----------------------------------------------------------------//
	static int					_affirmGfxScriptBatch		( lua_State* L );
	static int					_getGfxScriptBatch			( lua_State* L );
	static int					_setGfxScriptBatch			( lua_State* L );

	//----------------------------------------------------------------//
	void						_RegisterLuaClass								( RTTIVisitorHistory& history, MOAILuaState& state );
	void						_RegisterLuaFuncs								( RTTIVisitorHistory& history, MOAILuaState& state );
	void						MOAIAbstractDrawingAPI_RetainObject				( ZLRefCountedObject* object );
	void						MOAIAbstractDrawingAPI_SubmitCommand			( MOAIDrawingAPIEnum::_ cmd, const void* param, ZLSize size );
	MOAIGfxScriptRetained&		MOAIAbstractGfxScriptBatch_AffirmGfxScript		( ZLIndex index );
	MOAIAbstractGfxScript*		MOAIAbstractGfxScriptBatch_GetGfxScript			( ZLIndex index );
	ZLSize						MOAIAbstractGfxScriptBatch_GetIndexBatchSize	();
	void						MOAIAbstractGfxScriptBatch_ReserveGfxScripts	( ZLSize size );
	void						MOAIAbstractGfxScriptBatch_SetGfxScript			( ZLSize index, MOAIAbstractGfxScript* gfxScript );
	void						MOAIAbstractGfxScriptBatch_SetIndexBatchSize	( ZLSize size );

public:

	DECL_LUA_FACTORY ( MOAIHasGfxScriptBatch )

	//----------------------------------------------------------------//
	MOAIGfxScriptBatch&			AffirmGfxScriptBatch 		();
	MOAIGfxScriptBatch*			GetGfxScriptBatch 			();
								MOAIHasGfxScriptBatch		();
								~MOAIHasGfxScriptBatch		();
	void						SetGfxScriptBatch			( MOAIGfxScriptBatch* batch );
};

#endif
