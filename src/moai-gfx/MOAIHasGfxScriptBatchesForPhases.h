// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHASGFXSCRIPTBATCHESFORPHASES_H
#define	MOAIHASGFXSCRIPTBATCHESFORPHASES_H

#include <moai-gfx/MOAIDraw.h>

//================================================================//
// MOAIHasGfxScriptBatchesForPhases
//================================================================//
// TODO: doxygen
class MOAIGfxScriptBatch {
public:

	ZLSize											mIndexBatchSize;
	ZLLeanArray < ZLStrongPtr < MOAIGfxScript > >	mGfxScripts;

	//----------------------------------------------------------------//
	MOAIGfxScriptBatch () :
		mIndexBatchSize ( 1 ) {
	}

	//----------------------------------------------------------------//
	ZLIndex WrapIndex ( ZLIndex index ) {
		ZLSize totalGfxScripts = this->mGfxScripts.Size ();
		return (( this->mIndexBatchSize > 0 ) ? ( index / this->mIndexBatchSize ) : index ) % totalGfxScripts;
	}
};

//================================================================//
// MOAIHasGfxScriptBatchesForPhases
//================================================================//
// TODO: doxygen
class MOAIHasGfxScriptBatchesForPhases :
	public virtual MOAILuaObject {
private:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	STLMap < u32, MOAIGfxScriptBatch > mGfxScriptBatches;

	//----------------------------------------------------------------//
	static int				_getGfxScript					( lua_State* L );
	static int				_gfx							( lua_State* L );
	static int				_gfxScript						( lua_State* L );
	static int				_reserveGfxScripts				( lua_State* L );
	static int				_setGfxScript					( lua_State* L );
	static int				_setIndexBatchSize				( lua_State* L );

	//----------------------------------------------------------------//
	MOAIGfxScriptBatch&		AffirmGfxScriptBatch			( MOAIRenderPhaseEnum::_ renderPhase );
	MOAIGfxScriptBatch*		GetGfxScriptBatch				( MOAIRenderPhaseEnum::_ renderPhase );

	//----------------------------------------------------------------//
	void					_RegisterLuaClass				( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_RegisterLuaFuncs				( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	//----------------------------------------------------------------//
	MOAIGfxScript&			AffirmGfxScript							( ZLIndex index, MOAIRenderPhaseEnum::_ renderPhase );
	MOAIGfxScript*			GetGfxScript							( ZLIndex index, MOAIRenderPhaseEnum::_ renderPhase);
							MOAIHasGfxScriptBatchesForPhases		( ZLContext& context );
							~MOAIHasGfxScriptBatchesForPhases		();
	void					ReserveGfxScripts						( ZLSize size, MOAIRenderPhaseEnum::_ renderPhase );
	void					SetGfxScript							( ZLIndex index, MOAIGfxScript* gfxScript, MOAIRenderPhaseEnum::_ renderPhase );
	void					SetIndexBatchSize						( ZLSize size, MOAIRenderPhaseEnum::_ renderPhase );
	
	//----------------------------------------------------------------//
	template < typename API_TYPE >
	MOAICmdMediumWithAPI < API_TYPE >& _ ( MOAIAbstractCmdHandler& handler, ZLIndex index, MOAIRenderPhaseEnum::_ renderPhase ) {
		return this->AffirmGfxScript ( index, renderPhase )._ < API_TYPE >( handler );
	}
};

#endif
