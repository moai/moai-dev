// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIMaterialBatch.h>
#include <moai-gfx/MOAIMaterialBatchHolder.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatchHolder::_affirmMaterialBatch ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchHolder, "U" )
	
	state.Push ( &self->AffirmMaterialBatch ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getMaterialBatch
	@text	Return the material batch attached to the prop.
	
	@in		MOAIMaterialBatchHolder self
	@out	MOAIMaterialBatch materialBatch
*/
int MOAIMaterialBatchHolder::_getMaterialBatch ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchHolder, "U" )
	
	if ( self->mMaterialBatch ) {
		self->mMaterialBatch.PushRef ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setMaterialBatch
	@text	Sets the prop's material batch.
	
	@in		MOAIMaterialBatchHolder self
	@opt	MOAIMaterialBatch materialBatch
	@out	nil
*/
int MOAIMaterialBatchHolder::_setMaterialBatch ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatchHolder, "U" )
	
	self->mMaterialBatch.Set ( *self, state.GetLuaObject < MOAIMaterialBatch >( 2, true ));
	
	return 0;
}

//================================================================//
// MOAIMaterialBatchHolder
//================================================================//

//----------------------------------------------------------------//
MOAIMaterialBatchHolder::MOAIMaterialBatchHolder () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIMaterialBatchInterface )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIMaterialBatchHolder::~MOAIMaterialBatchHolder () {

	this->mMaterialBatch.Set ( *this, 0 );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIMaterialBatchHolder::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAIMaterialBatchInterface::MOAILuaObject_RegisterLuaClass ( composer, state );
}

//----------------------------------------------------------------//
void MOAIMaterialBatchHolder::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAIMaterialBatchInterface::MOAILuaObject_RegisterLuaFuncs ( composer, state );

	luaL_Reg regTable [] = {
		{ "affirmMaterialBatch",	_affirmMaterialBatch },
		{ "getMaterialBatch",		_getMaterialBatch },
		{ "setMaterialBatch",		_setMaterialBatch },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIMaterialBatchHolder::MOAILuaObject_SerializeIn ( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIMaterialBatchHolder::MOAILuaObject_SerializeOut ( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
MOAIMaterial& MOAIMaterialBatchHolder::MOAIMaterialBatchInterface_AffirmMaterial ( ZLIndex index ) {

	return this->AffirmMaterialBatch ().AffirmMaterial ( index );
}

//----------------------------------------------------------------//
MOAIMaterialBatch& MOAIMaterialBatchHolder::MOAIMaterialBatchInterface_AffirmMaterialBatch () {

	if ( !this->mMaterialBatch ) {
		this->mMaterialBatch.Set ( *this, new MOAIMaterialBatch ());
	}
	return *this->mMaterialBatch;
}

//----------------------------------------------------------------//
MOAIMaterial* MOAIMaterialBatchHolder::MOAIMaterialBatchInterface_GetMaterial ( ZLIndex index ) {

	return this->mMaterialBatch ? this->mMaterialBatch->GetMaterial ( index ) : NULL;
}

//----------------------------------------------------------------//
MOAIMaterialBatch* MOAIMaterialBatchHolder::MOAIMaterialBatchInterface_GetMaterialBatch () {

	return this->mMaterialBatch;
}

