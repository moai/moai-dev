// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAILight.h>
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
// TODO: doxygen
int MOAIMaterialBatch::_getLight ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	return self->GetLight ( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatch::_getShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	return self->GetShader ( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
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
// TODO: doxygen
/** @lua	Set blend mode using OpenGL source and dest factors. OpenGl blend factor constants are exposed as members of MOAIMaterialBatch.
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
// TODO: doxygen
int MOAIMaterialBatch::_setCullMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	self->SetCullMode ( state, 2 );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatch::_setDepthMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	self->SetDepthMask ( state, 2 );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatch::_setDepthTest ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	self->SetDepthTest ( state, 2 );
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
int MOAIMaterialBatch::_setIndexBatchSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	self->mIndexBatchSize = state.GetValue < u32 >( 2, 1 );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatch::_setLight ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	state.Push ( self->SetLight ( state, 2 ));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatch::_setShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	state.Push ( self->SetShader ( state, 2 ));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialBatch::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialBatch, "U" )
	state.Push ( self->SetTexture ( state, 2 ));
	return 1;
}

//================================================================//
// MOAIMaterialBatch
//================================================================//

//----------------------------------------------------------------//
MOAIMaterial& MOAIMaterialBatch::AffirmMaterial ( ZLIndex idx ) {

	this->mMaterials.Grow (( ZLSize )idx + 1 );
	return this->mMaterials [ idx ];
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::Clear () {

	this->mMaterials.Clear ();
}

//----------------------------------------------------------------//
int MOAIMaterialBatch::GetBlendMode ( MOAILuaState& state, int idx ) {

	MOAIMaterial* material = this->RawGetMaterial ( state.GetValueAsIndex ( idx ));
	if ( material ) {
		return material->mBlendMode.Push ( state );
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIMaterialBatch::GetCullMode ( MOAILuaState& state, int idx ) {

	MOAIMaterial* material = this->RawGetMaterial ( state.GetValueAsIndex ( idx ));
	if ( material ) {
		state.Push ( material->mCullMode );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIMaterialBatch::GetDepthMask ( MOAILuaState& state, int idx ) {

	MOAIMaterial* material = this->RawGetMaterial ( state.GetValueAsIndex ( idx ));
	if ( material ) {
		state.Push ( material->mDepthMask );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIMaterialBatch::GetDepthTest ( MOAILuaState& state, int idx ) {

	MOAIMaterial* material = this->RawGetMaterial ( state.GetValueAsIndex ( idx ));
	if ( material ) {
		state.Push ( material->mDepthTest );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIMaterialBatch::GetLight ( MOAILuaState& state, int idx ) {

	u32 name;
	ZLIndex materialID = MOAIMaterialBatch::GetNamedGlobalID ( state, idx, name );

	MOAIMaterial* material = this->RawGetMaterial ( materialID );
	state.Push (( MOAILight* )material->GetLight ( name ));
	return 1;
}

//----------------------------------------------------------------//
MOAIMaterial* MOAIMaterialBatch::GetMaterial ( ZLIndex idx ) {

	return this->RawGetMaterial ( this->GetRawIndex ( idx ));
}

//----------------------------------------------------------------//
ZLIndex MOAIMaterialBatch::GetMaterialID ( MOAILuaState& state, int& idx ) {

	if ( state.IsType ( idx, LUA_TNUMBER ) && ( state.AbsIndex ( idx ) < state.GetTop ())) {
		return state.GetValueAsIndex ( idx++ );
	}
	return ZLIndex::ZERO;
}

//----------------------------------------------------------------//
ZLIndex MOAIMaterialBatch::GetMaterialID ( MOAILuaState& state, int& idx, bool& set ) {

	set = false;
	
	if ( state.IsType ( idx, LUA_TNUMBER )) {
	
		u32 value = state.GetValue < u32 >( idx, 0 );
		
		if (( value == 0 ) || ZLGfx::IsFlag ( value )) {
			set = true;
			return ZLIndex::ZERO;
		}
		idx++;
		set = state.IsNil ( idx );
		return ZLIndex ( value - 1, ZLIndex::LIMIT );
	}
	set = state.IsNil ( idx );
	return ZLIndex::ZERO;
}

//----------------------------------------------------------------//
ZLIndex MOAIMaterialBatch::GetNamedGlobalID ( MOAILuaState& state, int& idx, u32& name ) {

	ZLIndex materialID	= ZLIndex::ZERO;
	name				= MOAI_UNKNOWN_MATERIAL_GLOBAL;

	if ( state.IsType ( idx, LUA_TNUMBER )) {
	
		if ( state.IsType ( idx + 1, LUA_TNUMBER )) {
	
			materialID		= state.GetValueAsIndex ( idx++ );
			name			= state.GetValue < u32 >( idx++, name  + 1 ) - 1;
		}
		else {
			
			materialID		= state.GetValueAsIndex ( idx++ );
		}
	}
	return materialID;
}

//----------------------------------------------------------------//
int MOAIMaterialBatch::GetShader ( MOAILuaState& state, int idx ) {

	MOAIMaterial* material = this->RawGetMaterial ( state.GetValueAsIndex ( idx ));
	if ( material && material->mShader ) {
		state.Push (( MOAIShader* )material->mShader );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIMaterialBatch::GetTexture ( MOAILuaState& state, int idx ) {
	
	u32 name;
	ZLIndex materialID = MOAIMaterialBatch::GetNamedGlobalID ( state, idx, name );

	MOAIMaterial* material = this->RawGetMaterial ( materialID );
	state.Push (( MOAITexture* )material->GetTexture ( name ));
	return 1;
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
MOAIMaterial* MOAIMaterialBatch::RawGetMaterial ( ZLIndex idx ) {

	ZLSize size = this->mMaterials.Size ();
	return size ? &this->mMaterials [ ZLIndex::Wrap ( idx, size )] : 0;
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "getBlendMode",			_getBlendMode },
		{ "getCullMode",			_getCullMode },
		{ "getDepthMask",			_getDepthMask },
		{ "getDepthTest",			_getDepthTest },
		{ "getLight",				_getLight },
		{ "getShader",				_getShader },
		{ "getTexture",				_getTexture },
		{ "reserveMaterials",		_reserveMaterials },
		{ "setBlendMode",			_setBlendMode },
		{ "setCullMode",			_setCullMode },
		{ "setDepthMask",			_setDepthMask },
		{ "setDepthTest",			_setDepthTest },
		{ "setIndexBatchSize",		_setIndexBatchSize },
		{ "setLight",				_setLight },
		{ "setShader",				_setShader },
		{ "setTexture",				_setTexture },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::Reserve ( ZLSize n ) {

	assert ( n < ZGL_FIRST_FLAG ); // probably don't need more than 0x70000000 materials...

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
void MOAIMaterialBatch::SetBlendMode ( ZLIndex idx ) {

	if ( idx < this->mMaterials.Size ()) {
		this->mMaterials [ idx ].SetBlendMode ();
	}
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetBlendMode ( ZLIndex idx, const MOAIBlendMode& blendMode ) {

	this->AffirmMaterial ( idx ).SetBlendMode ( blendMode );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetBlendMode ( MOAILuaState& state, int idx ) {

	bool set;
	ZLIndex materialID = MOAIMaterialBatch::GetMaterialID ( state, idx, set );

	if ( set ) {
		MOAIBlendMode blendMode;
		blendMode.Init ( state, idx );
		this->SetBlendMode ( materialID, blendMode );
	}
	else {
		this->SetBlendMode ( materialID );
	}
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetCullMode ( ZLIndex idx ) {

	if ( idx < this->mMaterials.Size ()) {
		this->mMaterials [ idx ].SetCullMode ();
	}
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetCullMode ( ZLIndex idx, int cullMode ) {

	this->AffirmMaterial ( idx ).SetCullMode ( cullMode );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetCullMode ( MOAILuaState& state, int idx ) {

	bool set;
	ZLIndex materialID = MOAIMaterialBatch::GetMaterialID ( state, idx, set );
	
	if ( set ) {
		this->SetCullMode ( materialID, state.GetValue < u32 >( idx, 0 ));
	}
	else {
		this->SetCullMode ( materialID );
	}
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetDepthMask ( ZLIndex idx ) {

	if ( idx < this->mMaterials.Size ()) {
		this->mMaterials [ idx ].SetDepthMask ();
	}
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetDepthMask ( ZLIndex idx, bool depthMask ) {

	this->AffirmMaterial ( idx ).SetDepthMask ( depthMask );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetDepthMask ( MOAILuaState& state, int idx ) {

	bool set;
	ZLIndex materialID = MOAIMaterialBatch::GetMaterialID ( state, idx, set );
	
	if ( set ) {
		this->SetDepthMask ( materialID, state.GetValue < bool >( idx, false ));
	}
	else {
		this->SetDepthMask ( materialID );
	}
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetDepthTest ( ZLIndex idx ) {

	if ( idx < this->mMaterials.Size ()) {
		this->mMaterials [ idx ].SetDepthTest ();
	}
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetDepthTest ( ZLIndex idx, int depthTest ) {

	this->AffirmMaterial ( idx ).SetDepthTest ( depthTest );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetDepthTest ( MOAILuaState& state, int idx ) {

	bool set;
	ZLIndex materialID = MOAIMaterialBatch::GetMaterialID ( state, idx, set );
	
	if ( set ) {
		this->SetDepthTest ( materialID, state.GetValue < u32 >( idx, 0 ));
	}
	else {
		this->SetDepthTest ( materialID );
	}
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetLight ( ZLIndex idx, u32 name ) {

	if ( idx < this->mMaterials.Size ()) {
		this->mMaterials [ idx ].SetLight ( name );
	}
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetLight ( ZLIndex idx, u32 name, MOAILight* light ) {

	this->AffirmMaterial ( idx ).SetLight ( name, light );
}

//----------------------------------------------------------------//
MOAILight* MOAIMaterialBatch::SetLight ( MOAILuaState& state, int idx ) {

	u32 name;
	ZLIndex materialID = MOAIMaterialBatch::GetNamedGlobalID ( state, idx, name );

	MOAILight* light = state.GetLuaObject < MOAILight >( idx, true );
	
	this->SetLight ( materialID, name, light );
	return light;
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetShader ( ZLIndex idx ) {

	if ( idx < this->mMaterials.Size ()) {
		this->mMaterials [ idx ].SetShader ();
	}
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetShader ( ZLIndex idx, u32 shaderID ) {

	this->SetShader ( idx, MOAIShaderMgr::Get ().GetShader ( shaderID ));
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetShader ( ZLIndex idx, MOAIShader* shader ) {

	this->AffirmMaterial ( idx ).SetShader ( shader );
}

//----------------------------------------------------------------//
MOAIShader* MOAIMaterialBatch::SetShader ( MOAILuaState& state, int idx ) {

	ZLIndex materialID = MOAIMaterialBatch::GetMaterialID ( state, idx );
	MOAIShader* shader = MOAIShader::AffirmShader ( state, idx );
	this->SetShader ( materialID, shader );
	return shader;
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetTexture ( ZLIndex idx ) {

	if ( idx < this->mMaterials.Size ()) {
		this->mMaterials [ idx ].SetTexture ();
	}
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetTexture ( ZLIndex idx, MOAITextureBase* texture ) {

	this->AffirmMaterial ( idx ).SetTexture ( texture );
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetTexture ( ZLIndex idx, u32 name ) {

	if ( idx < this->mMaterials.Size ()) {
		this->mMaterials [ idx ].SetTexture ( name );
	}
}

//----------------------------------------------------------------//
void MOAIMaterialBatch::SetTexture ( ZLIndex idx, u32 name, MOAITextureBase* texture ) {

	this->AffirmMaterial ( idx ).SetTexture ( name, texture );
}

//----------------------------------------------------------------//
MOAITextureBase* MOAIMaterialBatch::SetTexture ( MOAILuaState& state, int idx ) {
	
	u32 name;
	ZLIndex materialID = MOAIMaterialBatch::GetNamedGlobalID ( state, idx, name );

	MOAITextureBase* texture = MOAITexture::AffirmTexture ( state, idx );
	if ( name != MOAI_UNKNOWN_MATERIAL_GLOBAL ) {
		this->SetTexture ( materialID, name, texture );
	}
	else {
		this->SetTexture ( materialID, texture );
	}
	return texture;
}

//----------------------------------------------------------------//
size_t MOAIMaterialBatch::Size () {

	return this->mMaterials.Size ();
}
