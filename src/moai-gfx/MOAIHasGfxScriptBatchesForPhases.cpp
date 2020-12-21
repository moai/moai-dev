// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIAbstractRenderNode.h>
#include <moai-gfx/MOAIHasGfxScriptBatchesForPhases.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHasGfxScriptBatchesForPhases::_getGfxScript ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScriptBatchesForPhases, "U" );
	
	ZLSize index							= state.GetValue < MOAILuaIndex >( 2, 0 );
	MOAIRenderPhaseEnum::_ renderPhase		= state.GetEnum ( 3, MOAIRenderPhaseEnum::RENDER_PHASE_DRAW );
	
	state.Push ( self->GetGfxScript ( index, renderPhase ));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHasGfxScriptBatchesForPhases::_gfx ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScriptBatchesForPhases, "U" )
	
	ZLSize index							= state.GetValue < MOAILuaIndex >( 2, 0 );
	MOAIRenderPhaseEnum::_ renderPhase		= state.GetEnum ( 3, MOAIRenderPhaseEnum::RENDER_PHASE_DRAW );
	
	MOAIGfxScript& gfxScript = self->AffirmGfxScript ( index, renderPhase );
	gfxScript.Reset ();
	gfxScript.AffirmMedium ().PushCmdInterfaceWithHandler ( state, MOAIDraw::Get ());
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHasGfxScriptBatchesForPhases::_gfxScript ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScriptBatchesForPhases, "U" );
	
	ZLSize index							= state.GetValue < MOAILuaIndex >( 2, 0 );
	MOAIRenderPhaseEnum::_ renderPhase		= state.GetEnum ( 3, MOAIRenderPhaseEnum::RENDER_PHASE_DRAW );
	
	state.Push ( &self->AffirmGfxScript ( index, renderPhase ));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHasGfxScriptBatchesForPhases::_reserveGfxScripts ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScriptBatchesForPhases, "UN" )
	
	ZLSize totalScripts						= state.GetValue < MOAILuaSize >( 2, 0 );
	MOAIRenderPhaseEnum::_ renderPhase 		= state.GetEnum ( 3, MOAIRenderPhaseEnum::RENDER_PHASE_DRAW );
	
	self->ReserveGfxScripts ( totalScripts, renderPhase );
	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
int MOAIHasGfxScriptBatchesForPhases::_setGfxScript ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScriptBatchesForPhases, "UN" )

	ZLSize index							= state.GetValue < MOAILuaIndex >( 2, 0 );
	MOAIGfxScript* gfxScript				= state.GetLuaObject < MOAIGfxScript >( 3, false );
	MOAIRenderPhaseEnum::_ renderPhase		= state.GetEnum ( 4, MOAIRenderPhaseEnum::RENDER_PHASE_DRAW );
	
	self->SetGfxScript ( index, gfxScript, renderPhase );
	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
/**	@lua	setIndexBatchSize
	@text	Set the index batch size. When a prop or deck is drawing,
			the index is divided by the material batch's index batch size
			to get the material index. In this way sets of deck indices
			may be assigned to material indices. For example, an index
			batch size of 256 for a set of 4 materials would distribute
			1024 deck indices across materials 1 though 4. An index batch
			size of 1 would create a 1 to 1 mapping between deck indices
			and materials.
 
	@in		MOAIGfxScriptBatch self
	@opt	number renderPhase			Default value is RENDER_PHASE_DRAW.
	@opt	number indexBatchSize		Default value is 1.
	@out	nil
*/
int MOAIHasGfxScriptBatchesForPhases::_setIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScriptBatchesForPhases, "U" )
	
	ZLSize batchSize						= state.GetValue < MOAILuaSize >( 2, 1 );
	MOAIRenderPhaseEnum::_ renderPhase 		= ( MOAIRenderPhaseEnum::_ )state.GetValue < u32 >( 3, MOAIRenderPhaseEnum::RENDER_PHASE_DRAW );
	
	self->SetIndexBatchSize ( batchSize, renderPhase );
	MOAI_LUA_RETURN_SELF
}

//================================================================//
// MOAIHasGfxScriptBatchesForPhases
//================================================================//

//----------------------------------------------------------------//
MOAIGfxScript& MOAIHasGfxScriptBatchesForPhases::AffirmGfxScript ( ZLIndex index, MOAIRenderPhaseEnum::_ renderPhase ) {

	MOAIGfxScriptBatch& batch = this->AffirmGfxScriptBatch ( renderPhase );

	batch.mGfxScripts.Grow (( ZLSize )index + 1 );
	MOAIGfxScript* gfxScript = batch.mGfxScripts [ index ];
	
	if ( !gfxScript ) {
		gfxScript = new MOAIGfxScript ();
		batch.mGfxScripts [ index ] = gfxScript;
	}
	return *gfxScript;
}

//----------------------------------------------------------------//
MOAIGfxScriptBatch& MOAIHasGfxScriptBatchesForPhases::AffirmGfxScriptBatch ( MOAIRenderPhaseEnum::_ renderPhase ) {

	return this->mGfxScriptBatches [ renderPhase ];
}

//----------------------------------------------------------------//
MOAIGfxScript* MOAIHasGfxScriptBatchesForPhases::GetGfxScript ( ZLIndex index, MOAIRenderPhaseEnum::_ renderPhase ) {

	MOAIGfxScriptBatch* batch = this->GetGfxScriptBatch ( renderPhase );
	if ( !batch ) return NULL;

	ZLSize size = batch->mGfxScripts.Size ();
	if ( !size ) return NULL;
	
	index = batch->WrapIndex ( index );
	return ( index < size ) ? ( MOAIGfxScript* )batch->mGfxScripts [ index ] : NULL;
}

//----------------------------------------------------------------//
MOAIGfxScriptBatch* MOAIHasGfxScriptBatchesForPhases::GetGfxScriptBatch ( MOAIRenderPhaseEnum::_ renderPhase ) {

	return this->mGfxScriptBatches.contains ( renderPhase ) ? &this->mGfxScriptBatches [ renderPhase ] : NULL;
}

//----------------------------------------------------------------//
MOAIHasGfxScriptBatchesForPhases::MOAIHasGfxScriptBatchesForPhases () {

	RTTI_BEGIN ( MOAIHasGfxScriptBatchesForPhases )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIHasGfxScriptBatchesForPhases >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHasGfxScriptBatchesForPhases::~MOAIHasGfxScriptBatchesForPhases () {
}

//----------------------------------------------------------------//
void MOAIHasGfxScriptBatchesForPhases::ReserveGfxScripts ( ZLSize size, MOAIRenderPhaseEnum::_ renderPhase ) {

	this->AffirmGfxScriptBatch ( renderPhase ).mGfxScripts.Init ( size );
}

//----------------------------------------------------------------//
void MOAIHasGfxScriptBatchesForPhases::SetGfxScript ( ZLIndex index, MOAIGfxScript* gfxScript, MOAIRenderPhaseEnum::_ renderPhase ) {

	MOAIGfxScriptBatch& batch = this->AffirmGfxScriptBatch ( renderPhase );

	batch.mGfxScripts.Grow ( index + 1 );
	batch.mGfxScripts [ index ] = gfxScript;
}

//----------------------------------------------------------------//
void MOAIHasGfxScriptBatchesForPhases::SetIndexBatchSize ( ZLSize size, MOAIRenderPhaseEnum::_ renderPhase ) {

	MOAIGfxScriptBatch& batch = this->AffirmGfxScriptBatch ( renderPhase );

	batch.mIndexBatchSize = size;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIHasGfxScriptBatchesForPhases::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIHasGfxScriptBatchesForPhases::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
	
	luaL_Reg regTable [] = {
		{ "getGfxScript",			_getGfxScript },
		{ "gfx",					_gfx },
		{ "gfxScript",				_gfxScript },
		{ "reserveGfxScripts",		_reserveGfxScripts },
		{ "setGfxScript",			_setGfxScript },
		{ "setIndexBatchSize",		_setIndexBatchSize },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
