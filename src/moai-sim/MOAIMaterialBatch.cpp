// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIImage.h>
#include <moai-sim/MOAIMaterial.h>
#include <moai-sim/MOAIMaterialBatch.h>
#include <moai-sim/MOAIPartitionHull.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAITextureBase.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatch::_clearBlendMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	return self->ClearBlendMode ( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatch::_clearCullMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	return self->ClearCullMode ( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatch::_clearDepthMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	return self->ClearDepthMask ( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatch::_clearDepthTest ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	return self->ClearDepthTest ( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatch::_clearShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	return self->ClearShader ( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatch::_clearTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	return self->ClearTexture ( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatch::_getBlendMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	return self->GetBlendMode ( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatch::_getCullMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	return self->GetCullMode ( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatch::_getDepthMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	return self->GetDepthMask ( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatch::_getDepthTest ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	return self->GetDepthTest ( state, 2 );
}

//----------------------------------------------------------------//
/**	@lua	getIndexBatchSize
	@text	Get the index batch size.
	
	@in		MOAIMaterialBatch self
	@out	number indexBatchSize
*/
int MOAIMaterialBatch::_getIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" );
	state.Push ( self->mIndexBatchSize );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getShader
	@text	Get the shader for the given index. Index batch size
			is ignored.
	
	@in		MOAIMaterialBatch self
	@opt	number idx					Default value is 1.
	@out	MOAIShader shader
*/
int MOAIMaterialBatch::_getShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	return self->GetShader ( state, 2 );
}

//----------------------------------------------------------------//
/**	@lua	getTexture
	@text	Get the texture for the given index. Index batch size
			is ignored.
	
	@in		MOAIMaterialBatch self
	@opt	number idx					Default value is 1.
	@out	MOAITexture shader
*/
int MOAIMaterialBatch::_getTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	return self->GetTexture ( state, 2 );
}

//----------------------------------------------------------------//
/**	@lua	reserveMaterials
	@text	Reserve material indices.
	
	@in		MOAIMaterialBatch self
	@opt	number count
	@out	nil
*/
int MOAIMaterialBatch::_reserveMaterials ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	self->Reserve ( state.GetValue ( 2, 0 ));
	return 1;
}

//----------------------------------------------------------------//
/** @lua	setBlendMode
	@text	Set the blend mode.

	@overload	Reset the blend mode to MOAIMaterialBatch.BLEND_NORMAL (equivalent to src = GL_ONE, dst = GL_ONE_MINUS_SRC_ALPHA). This will reset the blend function to GL_FUNC_ADD.

		@in		MOAIMaterialBatch self
		@out	nil

	@overload	Set blend mode using one of the Moai presets. This will reset the blend function to GL_FUNC_ADD.

		@in		MOAIMaterialBatch self
		@in		number mode					One of MOAIMaterialBatch.BLEND_NORMAL, MOAIMaterialBatch.BLEND_ADD, MOAIMaterialBatch.BLEND_MULTIPLY.
		@out	nil
	
	@overload	Set blend mode using OpenGL source and dest factors. OpenGl blend factor constants are exposed as members of MOAIMaterialBatch.
				See the OpenGL documentation for an explanation of blending constants.

		@in		MOAIMaterialBatch self
		@in		number srcFactor
		@in		number dstFactor
		@out	nil
*/
int MOAIMaterialBatch::_setBlendMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	self->SetBlendMode ( state, 2 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setCullMode
	@text	Sets and enables face culling.
	
	@in		MOAIMaterialBatch self
	@opt	number cullMode			Default value is MOAIMaterialBatch.CULL_NONE.
	@out	nil
*/
int MOAIMaterialBatch::_setCullMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	self->SetCullMode ( state, 2 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDepthMask
	@text	Disables or enables depth writing.
	
	@in		MOAIMaterialBatch self
	@opt	boolean depthMask		Default value is true.
	@out	nil
*/
int MOAIMaterialBatch::_setDepthMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	self->SetDepthMask ( state, 2 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDepthTest
	@text	Sets and enables depth testing (assuming depth buffer is present).
	
	@in		MOAIMaterialBatch self
	@opt	number depthFunc		Default value is MOAIMaterialBatch.DEPTH_TEST_DISABLE.
	@out	nil
*/
int MOAIMaterialBatch::_setDepthTest ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	self->SetDepthTest ( state, 2 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setHitMask
	@text	Set or load an image to act as the hit mask for this deck.
	
	@in		MOAIDeck self
	@in		variant mask		A MOAIImage or a path to an image file
	@out	MOAIImage mask
*/
//int MOAIMaterialBatch::_setHitMask ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
//	state.Push ( self->SetHitMask ( state, 2 ));
//	return 1;
//}

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
int MOAIMaterialBatch::_setIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	self->mIndexBatchSize = state.GetValue < u32 >( 2, 1 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setShader
	@text	Sets a shader in the material batch. Index batch size is ignored.
			
			If no value for 'idx' is provided, then the shader or shader
			preset is expected as the first paramater, and idx defaults to 1.
	
	@overload
	
		@in		MOAIMaterialBatch self
		@in		number idx
		@in		variant shader			Overloaded to accept a MOAIShader or a shader preset.
		@out	MOAIShader shader		The shader that was set or created.
	
	@overload
	
		@in		MOAIMaterialBatch self
		@in		variant shader			Overloaded to accept a MOAIShader or a shader preset.
		@out	MOAIShader shader		The shader that was set or created.
*/
int MOAIMaterialBatch::_setShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	state.Push ( self->SetShader ( state, 2 ));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setTexture
	@text	Sets a texture in material batch. Index batch size is ignored.
			
			If no value for 'idx' is provided, then the texture or filename
			is expected as the first paramater, and idx defaults to 1.
	
	@overload
	
		@in		MOAIMaterialBatch self
		@in		number idx
		@in		variant texture			Overloaded to accept a filename, MOAITexture, MOAIImage, MOAIStream or MOAIDataBuffer.
		@out	MOAITexture texture		The texture that was set or created.
	
	@overload
	
		@in		MOAIMaterialBatch self
		@in		variant texture			Overloaded to accept a filename, MOAITexture, MOAIImage, MOAIStream or MOAIDataBuffer.
		@out	MOAITexture texture		The texture that was set or created.
*/
int MOAIMaterialBatch::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	state.Push ( self->SetTexture ( state, 2 ));
	return 1;
}

//================================================================//
// MOAIMaterialBatch
//================================================================//

//----------------------------------------------------------------//
MOAIMaterial& MOAIMaterialBatch::AffirmMaterial ( u32 idx ) {

	this->mMaterials.Grow ( idx, 1 );
	return this->mMaterials [ idx ];
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::Clear () {

	for ( u32 i = 0; i < this->mMaterials.Size (); ++i ) {
		MOAIMaterial& material = this->mMaterials [ i ];
	
		this->LuaRelease ( material.mShader );
		this->LuaRelease ( material.mTexture );
		this->LuaRelease ( material.mHitMask );
	}
	this->mMaterials.Clear ();
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::ClearBlendMode ( u32 idx ) {

	if ( idx < this->mMaterials.Size ()) {
		this->mMaterials [ idx ].ClearBlendMode ();
	}
}

//----------------------------------------------------------------//
int MOAIMaterialBatch::ClearBlendMode ( MOAILuaState& state, int idx ) {

	this->ClearBlendMode ( state.GetValue ( idx, 1 ) - 1 );
	return 0;
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::ClearCullMode ( u32 idx ) {

	if ( idx < this->mMaterials.Size ()) {
		this->mMaterials [ idx ].ClearCullMode ();
	}
}

//----------------------------------------------------------------//
int MOAIMaterialBatch::ClearCullMode ( MOAILuaState& state, int idx ) {

	this->ClearCullMode ( state.GetValue ( idx, 1 ) - 1 );
	return 0;
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::ClearDepthMask ( u32 idx ) {

	if ( idx < this->mMaterials.Size ()) {
		this->mMaterials [ idx ].ClearDepthMask ();
	}
}

//----------------------------------------------------------------//
int MOAIMaterialBatch::ClearDepthMask ( MOAILuaState& state, int idx ) {

	this->ClearDepthMask ( state.GetValue ( idx, 1 ) - 1 );
	return 0;
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::ClearDepthTest ( u32 idx ) {

	if ( idx < this->mMaterials.Size ()) {
		this->mMaterials [ idx ].ClearDepthTest ();
	}
}

//----------------------------------------------------------------//
int MOAIMaterialBatch::ClearDepthTest ( MOAILuaState& state, int idx ) {

	this->ClearDepthTest ( state.GetValue ( idx, 1 ) - 1 );
	return 0;
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::ClearHitMask ( u32 idx ) {
	UNUSED ( idx );

//	if ( idx < this->mMaterials.Size ()) {
//		this->mMaterials [ idx ].ClearHitMask ();
//	}
}

//----------------------------------------------------------------//
int MOAIMaterialBatch::ClearHitMask ( MOAILuaState& state, int idx ) {

	this->ClearHitMask ( state.GetValue ( idx, 1 ) - 1 );
	return 0;
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::ClearShader ( u32 idx ) {

	if ( idx < this->mMaterials.Size ()) {
		this->mMaterials [ idx ].ClearShader ();
	}
}

//----------------------------------------------------------------//
int MOAIMaterialBatch::ClearShader ( MOAILuaState& state, int idx ) {

	this->ClearShader ( state.GetValue ( idx, 1 ) - 1 );
	return 0;
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::ClearTexture ( u32 idx ) {

	if ( idx < this->mMaterials.Size ()) {
		this->mMaterials [ idx ].ClearTexture ();
	}
}

//----------------------------------------------------------------//
int MOAIMaterialBatch::ClearTexture ( MOAILuaState& state, int idx ) {

	this->ClearTexture ( state.GetValue ( idx, 1 ) - 1 );
	return 0;
}

//----------------------------------------------------------------//
int MOAIMaterialBatch::GetBlendMode ( MOAILuaState& state, int idx ) {

	MOAIMaterial* material = this->RawGetMaterial ( state.GetValue < u32 >( idx, 1 ) - 1 );
	if ( material ) {
		return material->mBlendMode.Push ( state );
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIMaterialBatch::GetCullMode ( MOAILuaState& state, int idx ) {

	MOAIMaterial* material = this->RawGetMaterial ( state.GetValue < u32 >( idx, 1 ) - 1 );
	if ( material ) {
		state.Push ( material->mCullMode );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIMaterialBatch::GetDepthMask ( MOAILuaState& state, int idx ) {

	MOAIMaterial* material = this->RawGetMaterial ( state.GetValue < u32 >( idx, 1 ) - 1 );
	if ( material ) {
		state.Push ( material->mDepthMask );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIMaterialBatch::GetDepthTest ( MOAILuaState& state, int idx ) {

	MOAIMaterial* material = this->RawGetMaterial ( state.GetValue < u32 >( idx, 1 ) - 1 );
	if ( material ) {
		state.Push ( material->mDepthTest );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIMaterialBatch::GetHitMask ( MOAILuaState& state, int idx ) {
	UNUSED ( state );
	UNUSED ( idx );

//	MOAIMaterial* material = this->RawGetMaterial ( state.GetValue < u32 >( idx, 1 ) - 1 );
//	if ( material && material->mHitMask ) {
//		state.Push (( MOAIHitMask* )material->mHitMask );
//		return 1;
//	}
	return 0;
}

//----------------------------------------------------------------//
MOAIMaterial* MOAIMaterialBatch::GetMaterial ( u32 idx ) {

	return this->RawGetMaterial ( this->GetRawIndex ( idx ));
}

//----------------------------------------------------------------//
int MOAIMaterialBatch::GetShader ( MOAILuaState& state, int idx ) {

	MOAIMaterial* material = this->RawGetMaterial ( state.GetValue < u32 >( idx, 1 ) - 1 );
	if ( material && material->mShader ) {
		state.Push (( MOAIShader* )material->mShader );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIMaterialBatch::GetTexture ( MOAILuaState& state, int idx ) {

	MOAIMaterial* material = this->RawGetMaterial ( state.GetValue < u32 >( idx, 1 ) - 1 );
	if ( material && material->mTexture ) {
		state.Push (( MOAITextureBase* )material->mTexture );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
MOAIMaterialBatch::MOAIMaterialBatch () :
	mIndexBatchSize ( 1 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIMaterialBatch::~MOAIMaterialBatch () {

	this->Clear ();
}

//----------------------------------------------------------------//
MOAIMaterial* MOAIMaterialBatch::RawGetMaterial ( u32 idx ) {

	size_t size = this->mMaterials.Size ();
	return size ? &this->mMaterials [ idx % size ] : 0;
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "clearBlendMode",			_clearBlendMode },
		{ "clearCullMode",			_clearCullMode },
		{ "clearDepthMask",			_clearDepthMask },
		{ "clearDepthTest",			_clearDepthTest },
		{ "clearShader",			_clearShader },
		{ "clearTexture",			_clearTexture },
		{ "getBlendMode",			_getBlendMode },
		{ "getCullMode",			_getCullMode },
		{ "getDepthMask",			_getDepthMask },
		{ "getDepthTest",			_getDepthTest },
		//{ "getHitMask",				_getHitMask },
		{ "getShader",				_getShader },
		{ "getTexture",				_getTexture },
		{ "reserveMaterials",		_reserveMaterials },
		{ "setBlendMode",			_setBlendMode },
		{ "setCullMode",			_setCullMode },
		{ "setDepthMask",			_setDepthMask },
		//{ "setHitMask",				_setHitMask },
		{ "setIndexBatchSize",		_setIndexBatchSize },
		{ "setShader",				_setShader },
		{ "setTexture",				_setTexture },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::Reserve ( u32 n ) {

	this->Clear ();
	this->mMaterials.Init ( n );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetBlendMode ( u32 idx, const MOAIBlendMode& blendMode ) {

	this->AffirmMaterial ( idx ).SetBlendMode ( blendMode );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetBlendMode ( MOAILuaState& state, int idx ) {

	u32 materialID = 0;
	u32 stackSize = state.GetStackSize ( idx );

	if (( stackSize == 2 ) || ( stackSize == 4 )) {
		materialID = state.GetValue < u32 >( idx++, 1 ) - 1;
	}

	MOAIBlendMode blendMode;
	blendMode.Init ( state, idx );

	this->SetBlendMode ( materialID, blendMode );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetCullMode ( u32 idx, int cullMode ) {

	this->AffirmMaterial ( idx ).SetCullMode ( cullMode );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetCullMode ( MOAILuaState& state, int idx ) {

	u32 materialID = 0;
	if ( state.GetStackSize ( idx ) == 2 ) {
		materialID = state.GetValue < u32 >( idx++, 1 ) - 1;
	}
	this->SetCullMode ( materialID, state.GetValue < u32 >( idx, 0 ));
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetDepthMask ( u32 idx, bool depthMask ) {

	this->AffirmMaterial ( idx ).SetDepthMask ( depthMask );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetDepthMask ( MOAILuaState& state, int idx ) {

	u32 materialID = 0;
	if ( state.GetStackSize ( idx ) == 2 ) {
		materialID = state.GetValue < u32 >( idx++, 1 ) - 1;
	}
	this->SetDepthMask ( materialID, state.GetValue < u32 >( idx, 0 ));
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetDepthTest ( u32 idx, int depthTest ) {

	this->AffirmMaterial ( idx ).SetDepthTest ( depthTest );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetDepthTest ( MOAILuaState& state, int idx ) {

	u32 materialID = 0;
	if ( state.GetStackSize ( idx ) == 2 ) {
		materialID = state.GetValue < u32 >( idx++, 1 ) - 1;
	}
	this->SetDepthTest ( materialID, state.GetValue < u32 >( idx, 0 ));
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetHitMask ( u32 idx, MOAIImage* mask ) {
	UNUSED ( idx );
	UNUSED ( mask );

	//this->AffirmMaterial ( idx ).SetHitMask ( mask );
}

//----------------------------------------------------------------//
MOAIImage* MOAIMaterialBatch::SetHitMask ( MOAILuaState& state, int idx ) {

	u32 materialIdx = 0;
	if ( state.IsType ( idx, LUA_TNUMBER )) {
		materialIdx = state.GetValue < u32 >( idx++, 1 ) - 1;
	}

	MOAIImage* mask = MOAIImage::AffirmImage ( state, idx );
	this->SetHitMask ( materialIdx, mask );
	return mask;
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetShader ( u32 idx, u32 shaderID ) {

	this->SetShader ( idx, MOAIShaderMgr::Get ().GetShader ( shaderID ));
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetShader ( u32 idx, MOAIShader* shader ) {

	this->AffirmMaterial ( idx ).SetShader ( shader );
}

//----------------------------------------------------------------//
MOAIShader* MOAIMaterialBatch::SetShader ( MOAILuaState& state, int idx ) {
	
	u32 materialIdx = 0;
	if ( state.IsType ( idx, LUA_TNUMBER ) && ( state.AbsIndex ( idx ) < state.GetTop ())) {
		materialIdx = state.GetValue < u32 >( idx++, 1 ) - 1;
	}

	MOAIShader* shader = MOAIShader::AffirmShader ( state, idx );
	this->SetShader ( materialIdx, shader );
	return shader;
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetTexture ( u32 idx, MOAITextureBase* texture ) {

	this->AffirmMaterial ( idx ).SetTexture ( texture );
}

//----------------------------------------------------------------//
MOAITextureBase* MOAIMaterialBatch::SetTexture ( MOAILuaState& state, int idx ) {
	
	u32 materialIdx = 0;
	if ( state.IsType ( idx, LUA_TNUMBER )) {
		materialIdx = state.GetValue < u32 >( idx++, 1 ) - 1;
	}
	
	MOAITextureBase* texture = MOAITexture::AffirmTexture ( state, idx );
	this->SetTexture ( materialIdx, texture );
	return texture;
}

//----------------------------------------------------------------//
size_t MOAIMaterialBatch::Size () {

	return this->mMaterials.Size ();
}
