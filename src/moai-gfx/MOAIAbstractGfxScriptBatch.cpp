// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIAbstractGfxScript.h>
#include <moai-gfx/MOAIAbstractGfxScriptBatch.h>
#include <moai-gfx/MOAIGfxScriptBatch.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractGfxScriptBatch::_affirmGfxScript ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxScriptBatch, "U" );
	state.Push ( &self->AffirmGfxScript ( state.GetValue < MOAILuaIndex >( 2, 0 )));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getIndexBatchSize
	@text	Get the index batch size.
 
	@in		MOAIGfxScriptBatch self
	@out	number indexBatchSize
*/
int MOAIAbstractGfxScriptBatch::_getIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxScriptBatch, "U" );
	
	state.Push ( MOAILuaSize (self->GetIndexBatchSize ()));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractGfxScriptBatch::_getGfxScript ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxScriptBatch, "U" );
	state.Push ( self->GetGfxScript ( state.GetValue < MOAILuaIndex >( 2, 0 )));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	reserveGfxScripts
	@text	Reserve material indices.
 
	@in		MOAIGfxScriptBatch self
	@opt	number count
	@out	nil
*/
int MOAIAbstractGfxScriptBatch::_reserveGfxScripts ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxScriptBatch, "U" )
	
	self->ReserveGfxScripts ( state.GetValue ( 2, 0 ));
	return 0;
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
	@opt	number indexBatchSize		Default value is 1.
	@out	nil
*/
int MOAIAbstractGfxScriptBatch::_setIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxScriptBatch, "U" )
	
	self->SetIndexBatchSize ( state.GetValue < u32 >( 2, 1 ));
	return 0;
}

//================================================================//
// MOAIAbstractGfxScriptBatch
//================================================================//

//----------------------------------------------------------------//
MOAIGfxScriptRetained& MOAIAbstractGfxScriptBatch::AffirmGfxScript ( ZLIndex index ) {

	return this->MOAIAbstractGfxScriptBatch_AffirmGfxScript ( index );
}

//----------------------------------------------------------------//
MOAIAbstractGfxScript* MOAIAbstractGfxScriptBatch::GetGfxScript ( ZLIndex index ) {

	return this->MOAIAbstractGfxScriptBatch_GetGfxScript ( index );
}

//----------------------------------------------------------------//
ZLSize MOAIAbstractGfxScriptBatch::GetIndexBatchSize () {

	return this->MOAIAbstractGfxScriptBatch_GetIndexBatchSize ();
}

//----------------------------------------------------------------//
MOAIAbstractGfxScriptBatch::MOAIAbstractGfxScriptBatch () {

	RTTI_BEGIN ( MOAIAbstractGfxScriptBatch )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractGfxScriptBatch >)
		RTTI_EXTEND ( MOAIAbstractHasGfxScript )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractGfxScriptBatch::~MOAIAbstractGfxScriptBatch () {
}

//----------------------------------------------------------------//
void MOAIAbstractGfxScriptBatch::ReserveGfxScripts ( ZLSize size ) {

	return this->MOAIAbstractGfxScriptBatch_ReserveGfxScripts ( size );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxScriptBatch::SetGfxScript ( ZLIndex index, MOAIAbstractGfxScript* gfxScript ) {

	return this->MOAIAbstractGfxScriptBatch_SetGfxScript ( index, gfxScript );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxScriptBatch::SetIndexBatchSize ( ZLSize size ) {

	return this->MOAIAbstractGfxScriptBatch_SetIndexBatchSize ( size );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractGfxScriptBatch::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAbstractGfxScriptBatch::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
	
	luaL_Reg regTable [] = {
		{ "affirmGfxScript",		_affirmGfxScript },
		{ "getGfxScript",			_getGfxScript },
		{ "getIndexBatchSize",		_getIndexBatchSize },
		{ "reserveGfxScripts",		_reserveGfxScripts },
		{ "setIndexBatchSize",		_setIndexBatchSize },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
MOAIGfxScriptRetained& MOAIAbstractGfxScriptBatch::MOAIAbstractHasGfxScript_AffirmGfxScript () {

	return this->AffirmGfxScript ( 0 );
}

//----------------------------------------------------------------//
MOAIAbstractGfxScript* MOAIAbstractGfxScriptBatch::MOAIAbstractHasGfxScript_GetGfxScript () {

	return this->GetGfxScript ( 0 );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxScriptBatch::MOAIAbstractHasGfxScript_SetGfxScript ( MOAIAbstractGfxScript* gfxScript ) {

	this->SetGfxScript ( 0, gfxScript );
}
