// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTGFXSCRIPTBATCH_H
#define	MOAIABSTRACTGFXSCRIPTBATCH_H

#include <moai-gfx/MOAIAbstractHasGfxScript.h>

class MOAIGfxScriptBatch;

//================================================================//
// MOAIAbstractGfxScriptBatch
//================================================================//
// TODO: doxygen
class MOAIAbstractGfxScriptBatch :
	public virtual MOAIAbstractHasGfxScript {
protected:
	
	MOAI_LUA_OBJECT_VISITOR_FRIEND
	
	//----------------------------------------------------------------//
	static int					_affirmGfxScript				( lua_State* L );
	static int					_getGfxScript					( lua_State* L );
	static int					_getIndexBatchSize				( lua_State* L );
	static int					_reserveGfxScripts				( lua_State* L );
	static int					_setGfxScript					( lua_State* L );
	static int					_setIndexBatchSize				( lua_State* L );
	
	//----------------------------------------------------------------//
	void						_RegisterLuaClass							( RTTIVisitorHistory& history, MOAILuaState& state );
	void						_RegisterLuaFuncs							( RTTIVisitorHistory& history, MOAILuaState& state );
	MOAIGfxScriptRetained&		MOAIAbstractHasGfxScript_AffirmGfxScript	();
	MOAIAbstractGfxScript*		MOAIAbstractHasGfxScript_GetGfxScript		();
	void						MOAIAbstractHasGfxScript_SetGfxScript		( MOAIAbstractGfxScript* gfxScript );
	
	//----------------------------------------------------------------//
	virtual MOAIGfxScriptRetained&		MOAIAbstractGfxScriptBatch_AffirmGfxScript		( ZLIndex index ) = 0;
	virtual MOAIAbstractGfxScript*		MOAIAbstractGfxScriptBatch_GetGfxScript			( ZLIndex index ) = 0;
	virtual ZLSize						MOAIAbstractGfxScriptBatch_GetIndexBatchSize	() = 0;
	virtual void						MOAIAbstractGfxScriptBatch_ReserveGfxScripts	( ZLSize size ) = 0;
	virtual void						MOAIAbstractGfxScriptBatch_SetGfxScript			( ZLIndex index, MOAIAbstractGfxScript* gfxScript ) = 0;
	virtual void						MOAIAbstractGfxScriptBatch_SetIndexBatchSize	( ZLSize size ) = 0;
	
public:

	//----------------------------------------------------------------//
	MOAIGfxScriptRetained&		AffirmGfxScript					( ZLIndex index );
	MOAIAbstractGfxScript*		GetGfxScript					( ZLIndex index );
	ZLSize						GetIndexBatchSize				();
								MOAIAbstractGfxScriptBatch		();
								~MOAIAbstractGfxScriptBatch		();
	void						ReserveGfxScripts				( ZLSize size );
	void						SetGfxScript					( ZLIndex index, MOAIAbstractGfxScript* gfxScript );
	void						SetIndexBatchSize				( ZLSize size );
};

#endif
