// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIAbstractGfxScript.h>
#include <moai-gfx/MOAIAbstractGfxScriptBatch.h>
#include <moai-gfx/MOAIAbstractHasGfxScriptBatch.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getIndexBatchSize
	@text	Get the index batch size.
 
	@in		MOAIGfxScriptBatch self
	@out	number indexBatchSize
*/
int MOAIAbstractHasGfxScriptBatch::_getIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractHasGfxScriptBatch, "U" );
	
	state.Push ( MOAILuaSize ( self->GetIndexBatchSize ()));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractHasGfxScriptBatch::_getGfxScript ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractHasGfxScriptBatch, "U" );
	state.Push ( self->GetGfxScript ( state.GetValue < MOAILuaIndex >( 2, 0 )));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractHasGfxScriptBatch::_gfxScript ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractHasGfxScriptBatch, "U" );
	state.Push ( &self->AffirmGfxScript ( state.GetValue < MOAILuaIndex >( 2, 0 )));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	reserveGfxScripts
	@text	Reserve material indices.
 
	@in		MOAIGfxScriptBatch self
	@opt	number count
	@out	nil
*/
int MOAIAbstractHasGfxScriptBatch::_reserveGfxScripts ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractHasGfxScriptBatch, "U" )
	
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
int MOAIAbstractHasGfxScriptBatch::_setIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractHasGfxScriptBatch, "U" )
	
	self->SetIndexBatchSize ( state.GetValue < u32 >( 2, 1 ));
	return 0;
}

//================================================================//
// MOAIAbstractHasGfxScriptBatch
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractGfxScript& MOAIAbstractHasGfxScriptBatch::AffirmGfxScript ( ZLIndex index ) {

	return this->AffirmGfxScriptBatch ().MOAIAbstractGfxScriptBatch_AffirmGfxScript ( index );
}

//----------------------------------------------------------------//
MOAIAbstractGfxScriptBatch& MOAIAbstractHasGfxScriptBatch::AffirmGfxScriptBatch () {

	return this->MOAIAbstractHasGfxScriptBatch_AffirmGfxScriptBatch ();
}

//----------------------------------------------------------------//
MOAIAbstractGfxScript* MOAIAbstractHasGfxScriptBatch::GetGfxScript ( ZLIndex index ) {

	return this->AffirmGfxScriptBatch ().MOAIAbstractGfxScriptBatch_GetGfxScript ( index );
}

//----------------------------------------------------------------//
ZLSize MOAIAbstractHasGfxScriptBatch::GetIndexBatchSize () {

	return this->AffirmGfxScriptBatch ().MOAIAbstractGfxScriptBatch_GetIndexBatchSize ();
}

//----------------------------------------------------------------//
MOAIAbstractHasGfxScriptBatch::MOAIAbstractHasGfxScriptBatch () {

	RTTI_BEGIN ( MOAIAbstractHasGfxScriptBatch )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractHasGfxScriptBatch >)
		RTTI_EXTEND ( MOAIAbstractHasGfxScript )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractHasGfxScriptBatch::~MOAIAbstractHasGfxScriptBatch () {
}

//----------------------------------------------------------------//
void MOAIAbstractHasGfxScriptBatch::ReserveGfxScripts ( ZLSize size ) {

	return this->AffirmGfxScriptBatch ().MOAIAbstractGfxScriptBatch_ReserveGfxScripts ( size );
}

//----------------------------------------------------------------//
void MOAIAbstractHasGfxScriptBatch::SetGfxScript ( ZLIndex index, MOAIAbstractGfxScript* gfxScript ) {

	return this->AffirmGfxScriptBatch ().MOAIAbstractGfxScriptBatch_SetGfxScript ( index, gfxScript );
}

//----------------------------------------------------------------//
void MOAIAbstractHasGfxScriptBatch::SetIndexBatchSize ( ZLSize size ) {

	return this->AffirmGfxScriptBatch ().MOAIAbstractGfxScriptBatch_SetIndexBatchSize ( size );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractHasGfxScriptBatch::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAbstractHasGfxScriptBatch::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
	
	luaL_Reg regTable [] = {
		{ "getGfxScript",			_getGfxScript },
		{ "getIndexBatchSize",		_getIndexBatchSize },
		{ "gfxScript",				_gfxScript },
		{ "reserveGfxScripts",		_reserveGfxScripts },
		{ "setIndexBatchSize",		_setIndexBatchSize },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
MOAIAbstractGfxScript& MOAIAbstractHasGfxScriptBatch::MOAIAbstractHasGfxScript_AffirmGfxScript () {

	return this->AffirmGfxScript ( 0 );
}
