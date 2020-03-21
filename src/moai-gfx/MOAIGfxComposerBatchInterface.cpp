// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>
#include <moai-gfx/MOAILight.h>
#include <moai-gfx/MOAIGfxComposer.h>
#include <moai-gfx/MOAIGfxComposerBatch.h>
#include <moai-gfx/MOAIGfxComposerBatchInterface.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxComposerBatchInterface::_affirmComposer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxComposerBatchInterface, "U" );
	state.Push ( &self->AffirmComposer ( state.GetValue < MOAILuaIndex >( 2, 0 )));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getIndexBatchSize
	@text	Get the index batch size.
 
	@in		MOAIGfxComposerBatch self
	@out	number indexBatchSize
*/
int MOAIGfxComposerBatchInterface::_getIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxComposerBatchInterface, "U" );
	
	MOAIGfxComposerBatch* materialBatch = self->GetComposerBatch ();
	if ( materialBatch ) {
		state.Push ( MOAILuaSize ( materialBatch->mIndexBatchSize ));
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxComposerBatchInterface::_getComposer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxComposerBatchInterface, "U" );
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
int MOAIGfxComposerBatchInterface::_reserveComposers ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxComposerBatchInterface, "U" )
	
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
int MOAIGfxComposerBatchInterface::_setIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxComposerBatchInterface, "U" )
	
	self->AffirmComposerBatch ().mIndexBatchSize = state.GetValue < u32 >( 2, 1 );
	return 0;
}

//================================================================//
// MOAIGfxComposerBatchInterface
//================================================================//

//----------------------------------------------------------------//
MOAIGfxComposer& MOAIGfxComposerBatchInterface::AffirmComposer () {

	return this->MOAIGfxComposerInterface::AffirmComposer ();
}

//----------------------------------------------------------------//
MOAIGfxComposer& MOAIGfxComposerBatchInterface::AffirmComposer ( ZLIndex index ) {

	return this->MOAIGfxComposerBatchInterface_AffirmComposer ( index );
}

//----------------------------------------------------------------//
MOAIGfxComposerBatch& MOAIGfxComposerBatchInterface::AffirmComposerBatch () {

	return this->MOAIGfxComposerBatchInterface_AffirmComposerBatch ();
}

//----------------------------------------------------------------//
MOAIGfxComposer* MOAIGfxComposerBatchInterface::GetComposer () {

	return this->MOAIGfxComposerInterface::GetComposer ();
}

//----------------------------------------------------------------//
MOAIGfxComposer* MOAIGfxComposerBatchInterface::GetComposer ( ZLIndex index ) {

	return this->MOAIGfxComposerBatchInterface_GetComposer ( index );
}

//----------------------------------------------------------------//
MOAIGfxComposerBatch* MOAIGfxComposerBatchInterface::GetComposerBatch () {

	return this->MOAIGfxComposerBatchInterface_GetComposerBatch ();
}

//----------------------------------------------------------------//
MOAIGfxComposerBatchInterface::MOAIGfxComposerBatchInterface () {

	RTTI_BEGIN ( MOAIGfxComposerBatchInterface )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIGfxComposerBatchInterface >)
		RTTI_EXTEND ( MOAIGfxComposerInterface )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxComposerBatchInterface::~MOAIGfxComposerBatchInterface () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxComposerBatchInterface::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIGfxComposerBatchInterface::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
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
MOAIGfxComposer& MOAIGfxComposerBatchInterface::MOAIGfxComposerInterface_AffirmComposer () {

	return this->AffirmComposer ( 0 );
}

//----------------------------------------------------------------//
MOAIGfxComposer* MOAIGfxComposerBatchInterface::MOAIGfxComposerInterface_GetComposer () {

	return this->GetComposer ( 0 );
}
