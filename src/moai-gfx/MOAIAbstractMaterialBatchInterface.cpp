// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>
#include <moai-gfx/MOAILight.h>
#include <moai-gfx/MOAIMaterial.h>
#include <moai-gfx/MOAIMaterialBatch.h>
#include <moai-gfx/MOAIAbstractMaterialBatchInterface.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractMaterialBatchInterface::_affirmMaterial ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractMaterialBatchInterface, "U" );
	state.Push ( &self->AffirmMaterial ( state.GetValue < MOAILuaIndex >( 2, 0 )));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getIndexBatchSize
	@text	Get the index batch size.
 
	@in		MOAIMaterialBatch self
	@out	number indexBatchSize
*/
int MOAIAbstractMaterialBatchInterface::_getIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractMaterialBatchInterface, "U" );
	
	MOAIMaterialBatch* materialBatch = self->GetMaterialBatch ();
	if ( materialBatch ) {
		state.Push ( MOAILuaSize ( materialBatch->mIndexBatchSize ));
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractMaterialBatchInterface::_getMaterial ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractMaterialBatchInterface, "U" );
	state.Push ( self->GetMaterial ( state.GetValue < MOAILuaIndex >( 2, 0 )));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	reserveMaterials
	@text	Reserve material indices.
 
	@in		MOAIMaterialBatch self
	@opt	number count
	@out	nil
*/
int MOAIAbstractMaterialBatchInterface::_reserveMaterials ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractMaterialBatchInterface, "U" )
	
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
int MOAIAbstractMaterialBatchInterface::_setIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractMaterialBatchInterface, "U" )
	
	self->AffirmMaterialBatch ().mIndexBatchSize = state.GetValue < u32 >( 2, 1 );
	return 0;
}

//================================================================//
// MOAIAbstractMaterialBatchInterface
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractMaterial& MOAIAbstractMaterialBatchInterface::AffirmMaterial () {

	return this->MOAIAbstractMaterialInterface::AffirmMaterial ();
}

//----------------------------------------------------------------//
MOAIMaterial& MOAIAbstractMaterialBatchInterface::AffirmMaterial ( ZLIndex index ) {

	return this->MOAIAbstractMaterialBatchInterface_AffirmMaterial ( index );
}

//----------------------------------------------------------------//
MOAIMaterialBatch& MOAIAbstractMaterialBatchInterface::AffirmMaterialBatch () {

	return this->MOAIAbstractMaterialBatchInterface_AffirmMaterialBatch ();
}

//----------------------------------------------------------------//
MOAIAbstractMaterial* MOAIAbstractMaterialBatchInterface::GetMaterial () {

	return this->MOAIAbstractMaterialInterface::GetMaterial ();
}

//----------------------------------------------------------------//
MOAIMaterial* MOAIAbstractMaterialBatchInterface::GetMaterial ( ZLIndex index ) {

	return this->MOAIAbstractMaterialBatchInterface_GetMaterial ( index );
}

//----------------------------------------------------------------//
MOAIMaterialBatch* MOAIAbstractMaterialBatchInterface::GetMaterialBatch () {

	return this->MOAIAbstractMaterialBatchInterface_GetMaterialBatch ();
}

//----------------------------------------------------------------//
MOAIAbstractMaterialBatchInterface::MOAIAbstractMaterialBatchInterface () {

	RTTI_BEGIN ( MOAIAbstractMaterialBatchInterface )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractMaterialBatchInterface >)
		RTTI_EXTEND ( MOAIAbstractMaterialInterface )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractMaterialBatchInterface::~MOAIAbstractMaterialBatchInterface () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractMaterialBatchInterface::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialBatchInterface::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
	
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
MOAIAbstractMaterial& MOAIAbstractMaterialBatchInterface::MOAIAbstractMaterialInterface_AffirmMaterial () {

	return this->AffirmMaterial ( 0 );
}

//----------------------------------------------------------------//
MOAIAbstractMaterial* MOAIAbstractMaterialBatchInterface::MOAIAbstractMaterialInterface_GetMaterial () {

	return this->GetMaterial ( 0 );
}
