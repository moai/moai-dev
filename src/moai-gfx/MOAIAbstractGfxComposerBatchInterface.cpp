// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIAbstractGfxComposer.h>
#include <moai-gfx/MOAIAbstractGfxComposerBatchInterface.h>
#include <moai-gfx/MOAIGfxComposerBatch.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>
#include <moai-gfx/MOAILight.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractGfxComposerBatchInterface::_affirmComposer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxComposerBatchInterface, "U" );
	state.Push ( &self->AffirmComposer ( state.GetValue < MOAILuaIndex >( 2, 0 )));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getIndexBatchSize
	@text	Get the index batch size.
 
	@in		MOAIGfxComposerBatch self
	@out	number indexBatchSize
*/
int MOAIAbstractGfxComposerBatchInterface::_getIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxComposerBatchInterface, "U" );
	
	MOAIGfxComposerBatch* batch = self->GetComposerBatch ();
	if ( batch ) {
		state.Push ( MOAILuaSize ( batch->mIndexBatchSize ));
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractGfxComposerBatchInterface::_getComposer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxComposerBatchInterface, "U" );
	state.Push ( self->GetComposer ( state.GetValue < MOAILuaIndex >( 2, 0 )));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	reserveComposers
	@text	Reserve material indices.
 
	@in		MOAIGfxComposerBatch self
	@opt	number count
	@out	nil
*/
int MOAIAbstractGfxComposerBatchInterface::_reserveComposers ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxComposerBatchInterface, "U" )
	
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
 
	@in		MOAIGfxComposerBatch self
	@opt	number indexBatchSize		Default value is 1.
	@out	nil
*/
int MOAIAbstractGfxComposerBatchInterface::_setIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxComposerBatchInterface, "U" )
	
	self->AffirmComposerBatch ().mIndexBatchSize = state.GetValue < u32 >( 2, 1 );
	return 0;
}

//================================================================//
// MOAIAbstractGfxComposerBatchInterface
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractGfxComposer& MOAIAbstractGfxComposerBatchInterface::AffirmComposer () {

	return this->MOAIAbstractGfxComposer::AffirmComposer ();
}

//----------------------------------------------------------------//
MOAIAbstractGfxComposer& MOAIAbstractGfxComposerBatchInterface::AffirmComposer ( ZLIndex index ) {

	return this->MOAIAbstractGfxComposerBatchInterface_AffirmComposer ( index );
}

//----------------------------------------------------------------//
MOAIGfxComposerBatch& MOAIAbstractGfxComposerBatchInterface::AffirmComposerBatch () {

	return this->MOAIAbstractGfxComposerBatchInterface_AffirmComposerBatch ();
}

//----------------------------------------------------------------//
MOAIAbstractGfxComposer* MOAIAbstractGfxComposerBatchInterface::GetComposer () {

	return this->MOAIAbstractGfxComposer::GetComposer ();
}

//----------------------------------------------------------------//
MOAIAbstractGfxComposer* MOAIAbstractGfxComposerBatchInterface::GetComposer ( ZLIndex index ) {

	return this->MOAIAbstractGfxComposerBatchInterface_GetComposer ( index );
}

//----------------------------------------------------------------//
MOAIGfxComposerBatch* MOAIAbstractGfxComposerBatchInterface::GetComposerBatch () {

	return this->MOAIAbstractGfxComposerBatchInterface_GetComposerBatch ();
}

//----------------------------------------------------------------//
MOAIAbstractGfxComposerBatchInterface::MOAIAbstractGfxComposerBatchInterface () {

	RTTI_BEGIN ( MOAIAbstractGfxComposerBatchInterface )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractGfxComposerBatchInterface >)
		RTTI_EXTEND ( MOAIAbstractGfxComposer )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractGfxComposerBatchInterface::~MOAIAbstractGfxComposerBatchInterface () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractGfxComposerBatchInterface::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAbstractGfxComposerBatchInterface::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
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
MOAIAbstractGfxComposer& MOAIAbstractGfxComposerBatchInterface::MOAIAbstractGfxComposerInterface_AffirmComposer () {

	return this->AffirmComposer ( 0 );
}

//----------------------------------------------------------------//
MOAIAbstractGfxComposer* MOAIAbstractGfxComposerBatchInterface::MOAIAbstractGfxComposerInterface_GetComposer () {

	return this->GetComposer ( 0 );
}
