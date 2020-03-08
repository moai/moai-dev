// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>
#include <moai-gfx/MOAILight.h>
#include <moai-gfx/MOAIMaterial.h>
#include <moai-gfx/MOAIMaterialBatch.h>

//================================================================//
// MOAIMaterialBatch
//================================================================//

//----------------------------------------------------------------//
void MOAIMaterialBatch::Clear () {

	this->mMaterials.Clear ();
}

//----------------------------------------------------------------//
MOAIMaterialBatch::MOAIMaterialBatch () :
	mIndexBatchSize ( 1 ) {
	
	RTTI_BEGIN ( MOAIMaterialBatch )
		RTTI_EXTEND ( MOAIAbstractMaterialBatchInterface )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIMaterialBatch::~MOAIMaterialBatch () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::Reserve ( ZLSize n ) {

	// TODO: this was ZGL_FIRST_FLAG; unclear why
	assert ( n < 0x70000000 ); // probably don't need more than 0x70000000 materials...

	this->Clear ();
	this->mMaterials.Init ( n );
}

//----------------------------------------------------------------//
size_t MOAIMaterialBatch::Size () {

	return this->mMaterials.Size ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIMaterialBatch::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAIAbstractMaterialBatchInterface::MOAILuaObject_RegisterLuaClass ( composer, state );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAIAbstractMaterialBatchInterface::MOAILuaObject_RegisterLuaFuncs ( composer, state );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::MOAILuaObject_SerializeIn ( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::MOAILuaObject_SerializeOut ( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
MOAIMaterial& MOAIMaterialBatch::MOAIAbstractMaterialBatchInterface_AffirmMaterial ( ZLIndex index ) {

	this->mMaterials.Grow (( ZLSize )index + 1 );

	MOAIMaterial* material = this->mMaterials [ index ];
	if ( !material ) {
		material = new MOAIMaterial ();
		this->mMaterials [ index ] = material;
	}
	return *material;
}

//----------------------------------------------------------------//
MOAIMaterialBatch& MOAIMaterialBatch::MOAIAbstractMaterialBatchInterface_AffirmMaterialBatch () {

	return *this;
}

//----------------------------------------------------------------//
MOAIMaterial* MOAIMaterialBatch::MOAIAbstractMaterialBatchInterface_GetMaterial ( ZLIndex index ) {

	return ( index < this->mMaterials.Size ()) ? ( MOAIMaterial* )this->mMaterials [ index ] : NULL;
}

//----------------------------------------------------------------//
MOAIMaterialBatch* MOAIMaterialBatch::MOAIAbstractMaterialBatchInterface_GetMaterialBatch () {

	return this;
}
