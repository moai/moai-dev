// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIAbstractGfxScript.h>
#include <moai-gfx/MOAIAbstractGfxScriptBatchInterface.h>
#include <moai-gfx/MOAIGfxScriptBatch.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>
#include <moai-gfx/MOAILight.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractGfxScriptBatchInterface::_affirmComposer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxScriptBatchInterface, "U" );
	state.Push ( &self->AffirmComposer ( state.GetValue < MOAILuaIndex >( 2, 0 )));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getIndexBatchSize
	@text	Get the index batch size.
 
	@in		MOAIGfxScriptBatch self
	@out	number indexBatchSize
*/
int MOAIAbstractGfxScriptBatchInterface::_getIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxScriptBatchInterface, "U" );
	
	MOAIGfxScriptBatch* batch = self->GetComposerBatch ();
	if ( batch ) {
		state.Push ( MOAILuaSize ( batch->mIndexBatchSize ));
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractGfxScriptBatchInterface::_getComposer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxScriptBatchInterface, "U" );
	state.Push ( self->GetComposer ( state.GetValue < MOAILuaIndex >( 2, 0 )));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	reserveComposers
	@text	Reserve material indices.
 
	@in		MOAIGfxScriptBatch self
	@opt	number count
	@out	nil
*/
int MOAIAbstractGfxScriptBatchInterface::_reserveComposers ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxScriptBatchInterface, "U" )
	
	self->AffirmComposerBatch ().Reserve ( state.GetValue ( 2, 0 ));
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
int MOAIAbstractGfxScriptBatchInterface::_setIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxScriptBatchInterface, "U" )
	
	self->AffirmComposerBatch ().mIndexBatchSize = state.GetValue < u32 >( 2, 1 );
	return 0;
}

//================================================================//
// MOAIAbstractGfxScriptBatchInterface
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractGfxScript& MOAIAbstractGfxScriptBatchInterface::AffirmComposer () {

	return this->MOAIAbstractGfxScript::AffirmComposer ();
}

//----------------------------------------------------------------//
MOAIAbstractGfxScript& MOAIAbstractGfxScriptBatchInterface::AffirmComposer ( ZLIndex index ) {

	return this->MOAIAbstractGfxScriptBatchInterface_AffirmComposer ( index );
}

//----------------------------------------------------------------//
MOAIGfxScriptBatch& MOAIAbstractGfxScriptBatchInterface::AffirmComposerBatch () {

	return this->MOAIAbstractGfxScriptBatchInterface_AffirmComposerBatch ();
}

//----------------------------------------------------------------//
MOAIAbstractGfxScript* MOAIAbstractGfxScriptBatchInterface::GetComposer () {

	return this->MOAIAbstractGfxScript::GetComposer ();
}

//----------------------------------------------------------------//
MOAIAbstractGfxScript* MOAIAbstractGfxScriptBatchInterface::GetComposer ( ZLIndex index ) {

	return this->MOAIAbstractGfxScriptBatchInterface_GetComposer ( index );
}

//----------------------------------------------------------------//
MOAIGfxScriptBatch* MOAIAbstractGfxScriptBatchInterface::GetComposerBatch () {

	return this->MOAIAbstractGfxScriptBatchInterface_GetComposerBatch ();
}

//----------------------------------------------------------------//
MOAIAbstractGfxScriptBatchInterface::MOAIAbstractGfxScriptBatchInterface () {

	RTTI_BEGIN ( MOAIAbstractGfxScriptBatchInterface )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractGfxScriptBatchInterface >)
		RTTI_EXTEND ( MOAIAbstractGfxScript )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractGfxScriptBatchInterface::~MOAIAbstractGfxScriptBatchInterface () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractGfxScriptBatchInterface::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAbstractGfxScriptBatchInterface::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
	
	luaL_Reg regTable [] = {
		{ "affirmComposer",			_affirmComposer },
		{ "getIndexBatchSize",		_getIndexBatchSize },
		{ "getComposer",			_getComposer },
		{ "reserveComposers",		_reserveComposers },
		{ "setIndexBatchSize",		_setIndexBatchSize },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
MOAIAbstractGfxScript& MOAIAbstractGfxScriptBatchInterface::MOAIAbstractGfxScriptInterface_AffirmComposer () {

	return this->AffirmComposer ( 0 );
}

//----------------------------------------------------------------//
MOAIAbstractGfxScript* MOAIAbstractGfxScriptBatchInterface::MOAIAbstractGfxScriptInterface_GetComposer () {

	return this->GetComposer ( 0 );
}
