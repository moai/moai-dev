// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTHASGFXSCRIPTBATCH_H
#define	MOAIABSTRACTHASGFXSCRIPTBATCH_H

#include <moai-gfx/MOAIAbstractHasGfxScript.h>

class MOAIAbstractGfxScriptBatch;

//================================================================//
// MOAIAbstractHasGfxScriptBatch
//================================================================//
// TODO: doxygen
class MOAIAbstractHasGfxScriptBatch :
	public virtual MOAIAbstractHasGfxScript {
protected:
	
	MOAI_LUA_OBJECT_VISITOR_FRIEND
	
	//----------------------------------------------------------------//
	static int						_getGfxScript						( lua_State* L );
	static int						_getIndexBatchSize					( lua_State* L );
	static int						_gfxScript							( lua_State* L );
	static int						_reserveGfxScripts					( lua_State* L );
	static int						_setGfxScript						( lua_State* L );
	static int						_setIndexBatchSize					( lua_State* L );
	
	//----------------------------------------------------------------//
	void							_RegisterLuaClass									( RTTIVisitorHistory& history, MOAILuaState& state );
	void							_RegisterLuaFuncs									( RTTIVisitorHistory& history, MOAILuaState& state );
	MOAIAbstractGfxScript&			MOAIAbstractHasGfxScript_AffirmGfxScript			();
	
	//----------------------------------------------------------------//
	virtual MOAIAbstractGfxScriptBatch&		MOAIAbstractHasGfxScriptBatch_AffirmGfxScriptBatch		() = 0;
	
public:

	//----------------------------------------------------------------//
	MOAIAbstractGfxScript&			AffirmGfxScript						( ZLIndex index );
	MOAIAbstractGfxScriptBatch&		AffirmGfxScriptBatch				();
	MOAIAbstractGfxScript*			GetGfxScript						( ZLIndex index );
	ZLSize							GetIndexBatchSize					();
									MOAIAbstractHasGfxScriptBatch		();
									~MOAIAbstractHasGfxScriptBatch		();
	void							ReserveGfxScripts					( ZLSize size );
	void							SetGfxScript						( ZLIndex index, MOAIAbstractGfxScript* gfxScript );
	void							SetIndexBatchSize					( ZLSize size );
};

#endif
