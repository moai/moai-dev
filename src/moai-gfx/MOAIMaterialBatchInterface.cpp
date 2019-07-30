// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>
#include <moai-gfx/MOAILight.h>
#include <moai-gfx/MOAIMaterial.h>
#include <moai-gfx/MOAIMaterialBatch.h>
#include <moai-gfx/MOAIMaterialBatchInterface.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatchInterface::_affirmMaterial ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchInterface, "U" );
	state.Push ( &self->AffirmMaterial ( state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO )));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getIndexBatchSize
	@text	Get the index batch size.
 
	@in		MOAIMaterialBatch self
	@out	number indexBatchSize
*/
int MOAIMaterialBatchInterface::_getIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchInterface, "U" );
	
	MOAIMaterialBatch* materialBatch = self->GetMaterialBatch ();
	if ( materialBatch ) {
		state.Push ( MOAILuaSize ( materialBatch->mIndexBatchSize ));
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatchInterface::_getMaterial ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchInterface, "U" );
	state.Push ( self->GetMaterial ( state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO )));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	reserveMaterials
	@text	Reserve material indices.
 
	@in		MOAIMaterialBatch self
	@opt	number count
	@out	nil
*/
int MOAIMaterialBatchInterface::_reserveMaterials ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchInterface, "U" )
	
	self->AffirmMaterialBatch ().Reserve ( state.GetValue ( 2, 0 ));
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
 
	@in		MOAIMaterialBatch self
	@opt	number indexBatchSize		Default value is 1.
	@out	nil
*/
int MOAIMaterialBatchInterface::_setIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchInterface, "U" )
	
	self->AffirmMaterialBatch ().mIndexBatchSize = state.GetValue < u32 >( 2, 1 );
	return 0;
}

//================================================================//
// MOAIMaterialBatchInterface
//================================================================//

//----------------------------------------------------------------//
MOAIMaterial& MOAIMaterialBatchInterface::AffirmMaterial () {

	return this->MOAIMaterialInterface::AffirmMaterial ();
}

//----------------------------------------------------------------//
MOAIMaterial& MOAIMaterialBatchInterface::AffirmMaterial ( ZLIndex index ) {

	return this->MOAIMaterialBatchInterface_AffirmMaterial ( index );
}

//----------------------------------------------------------------//
MOAIMaterialBatch& MOAIMaterialBatchInterface::AffirmMaterialBatch () {

	return this->MOAIMaterialBatchInterface_AffirmMaterialBatch ();
}

//----------------------------------------------------------------//
MOAIMaterial* MOAIMaterialBatchInterface::GetMaterial () {

	return this->MOAIMaterialInterface::GetMaterial ();
}

//----------------------------------------------------------------//
MOAIMaterial* MOAIMaterialBatchInterface::GetMaterial ( ZLIndex index ) {

	return this->MOAIMaterialBatchInterface_GetMaterial ( index );
}

//----------------------------------------------------------------//
MOAIMaterialBatch* MOAIMaterialBatchInterface::GetMaterialBatch () {

	return this->MOAIMaterialBatchInterface_GetMaterialBatch ();
}

//----------------------------------------------------------------//
MOAIMaterialBatchInterface::MOAIMaterialBatchInterface () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIMaterialInterface )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIMaterialBatchInterface::~MOAIMaterialBatchInterface () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIMaterialBatchInterface::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAIMaterialInterface::MOAILuaObject_RegisterLuaClass ( composer, state );
}

//----------------------------------------------------------------//
void MOAIMaterialBatchInterface::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAIMaterialInterface::MOAILuaObject_RegisterLuaFuncs ( composer, state );
	
	luaL_Reg regTable [] = {
		{ "affirmMaterial",			_affirmMaterial },
		{ "getIndexBatchSize",		_getIndexBatchSize },
		{ "getMaterial",			_getMaterial },
		{ "reserveMaterials",		_reserveMaterials },
		{ "setIndexBatchSize",		_setIndexBatchSize },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
MOAIMaterial& MOAIMaterialBatchInterface::MOAIMaterialInterface_AffirmMaterial () {

	return this->AffirmMaterial ( ZLIndexOp::ZERO );
}

//----------------------------------------------------------------//
MOAIMaterial* MOAIMaterialBatchInterface::MOAIMaterialInterface_GetMaterial () {

	return this->GetMaterial ( ZLIndexOp::ZERO );
}
