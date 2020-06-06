// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIGfxScriptBatch.h>
#include <moai-gfx/MOAIHasGfxScriptBatch.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHasGfxScriptBatch::_affirmGfxScriptBatch ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScriptBatch, "U" )
	
	state.Push ( &self->AffirmGfxScriptBatch ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getGfxScriptBatch
	@text	Return the material batch attached to the prop.
	
	@in		MOAIHasGfxScriptBatch self
	@out	MOAIGfxScriptBatch materialBatch
*/
int MOAIHasGfxScriptBatch::_getGfxScriptBatch ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScriptBatch, "U" )
	
	MOAIGfxScriptBatch* batch = self->GetGfxScriptBatch ();
	if ( batch ) {
		state.Push ( batch );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setGfxScriptBatch
	@text	Sets the prop's material batch.
	
	@in		MOAIHasGfxScriptBatch self
	@opt	MOAIGfxScriptBatch materialBatch
	@out	nil
*/
int MOAIHasGfxScriptBatch::_setGfxScriptBatch ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasGfxScriptBatch, "U" )
	
	self->SetGfxScriptBatch ( state.GetLuaObject < MOAIGfxScriptBatch >( 2, true ));
	return 0;
}

//================================================================//
// MOAIHasGfxScriptBatch
//================================================================//

//----------------------------------------------------------------//
MOAIGfxScriptBatch& MOAIHasGfxScriptBatch::AffirmGfxScriptBatch () {

	if ( !this->mGfxScriptBatch ) {
		this->mGfxScriptBatch = new MOAIGfxScriptBatch ();
	}
	return *this->mGfxScriptBatch;
}

//----------------------------------------------------------------//
MOAIGfxScriptBatch* MOAIHasGfxScriptBatch::GetGfxScriptBatch () {

	return this->mGfxScriptBatch;
}

//----------------------------------------------------------------//
MOAIHasGfxScriptBatch::MOAIHasGfxScriptBatch () {

	RTTI_BEGIN ( MOAIHasGfxScriptBatch )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIHasGfxScriptBatch >)
		RTTI_EXTEND ( MOAIAbstractGfxScriptBatch )
		RTTI_EXTEND ( MOAIAbstractDrawingObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHasGfxScriptBatch::~MOAIHasGfxScriptBatch () {
}

//----------------------------------------------------------------//
void MOAIHasGfxScriptBatch::SetGfxScriptBatch ( MOAIGfxScriptBatch* batch ) {

	this->mGfxScriptBatch = batch;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIHasGfxScriptBatch::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIHasGfxScriptBatch::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "affirmGfxScriptBatch",	_affirmGfxScriptBatch },
		{ "getGfxScriptBatch",		_getGfxScriptBatch },
		{ "setGfxScriptBatch",		_setGfxScriptBatch },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIHasGfxScriptBatch::MOAIAbstractDrawingAPI_RetainObject ( ZLRefCountedObject* object ) {

	if ( object && this->mGfxScriptBatch ) {
		this->mGfxScriptBatch->RetainObject ( object );
	}
}

//----------------------------------------------------------------//
void MOAIHasGfxScriptBatch::MOAIAbstractDrawingAPI_SubmitCommand ( MOAIDrawingAPIEnum::_ cmd, const void* param, ZLSize size ) {

	if ( this->mGfxScriptBatch ) {
		this->mGfxScriptBatch->SubmitCommand ( cmd, param, size );
	}
}

//----------------------------------------------------------------//
MOAIGfxScriptRetained& MOAIHasGfxScriptBatch::MOAIAbstractGfxScriptBatch_AffirmGfxScript ( ZLIndex index ) {

	return this->AffirmGfxScriptBatch ().AffirmGfxScript ( index );
}

//----------------------------------------------------------------//
MOAIAbstractGfxScript* MOAIHasGfxScriptBatch::MOAIAbstractGfxScriptBatch_GetGfxScript ( ZLIndex index ) {

	return this->AffirmGfxScriptBatch ().GetGfxScript ( index );
}

//----------------------------------------------------------------//
ZLSize MOAIHasGfxScriptBatch::MOAIAbstractGfxScriptBatch_GetIndexBatchSize () {

	return this->AffirmGfxScriptBatch ().GetIndexBatchSize ();
}

//----------------------------------------------------------------//
void MOAIHasGfxScriptBatch::MOAIAbstractGfxScriptBatch_ReserveGfxScripts ( ZLSize size ) {

	this->AffirmGfxScriptBatch ().ReserveGfxScripts ( size );
}

//----------------------------------------------------------------//
void MOAIHasGfxScriptBatch::MOAIAbstractGfxScriptBatch_SetGfxScript ( ZLIndex index, MOAIAbstractGfxScript* gfxScript ) {

	this->AffirmGfxScriptBatch ().SetGfxScript ( index, gfxScript );
}

//----------------------------------------------------------------//
void MOAIHasGfxScriptBatch::MOAIAbstractGfxScriptBatch_SetIndexBatchSize ( ZLSize size ) {

	this->AffirmGfxScriptBatch ().SetIndexBatchSize ( size );
}
