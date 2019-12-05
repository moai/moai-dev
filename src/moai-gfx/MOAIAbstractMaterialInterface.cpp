// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAILight.h>
#include <moai-gfx/MOAIMaterial.h>
#include <moai-gfx/MOAIAbstractMaterialInterface.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractMaterialInterface::_getBlendMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractMaterialInterface, "U" )

	MOAIAbstractMaterial* material = self->GetMaterial ();
	if ( material ) {

		MOAIBlendMode& blendMode = material->mBlendMode;
		state.Push ( blendMode.mEquation );
		state.Push ( blendMode.mSourceFactor );
		state.Push ( blendMode.mDestFactor );

		return 3;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractMaterialInterface::_getCullMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractMaterialInterface, "U" )
	
	MOAIAbstractMaterial* material = self->GetMaterial ();
	if ( material ) {
		state.Push ( material->mCullMode );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractMaterialInterface::_getDepthMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractMaterialInterface, "U" )
	
	MOAIAbstractMaterial* material = self->GetMaterial ();
	if ( material ) {
		state.Push ( material->mDepthMask );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractMaterialInterface::_getDepthTest ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractMaterialInterface, "U" )
	
	MOAIAbstractMaterial* material = self->GetMaterial ();
	if ( material ) {
	
		state.Push ( material->mDepthTest );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractMaterialInterface::_getLight ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractMaterialInterface, "U" )
	
	MOAIAbstractMaterial* material = self->GetMaterial ();
	if ( material ) {
		u32 name = MOAIMaterialGlobals::GetNamedGlobalID ( state, 2 );
		state.Push ( material->GetLight ( name ));
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractMaterialInterface::_getShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractMaterialInterface, "U" )

	MOAIAbstractMaterial* material = self->GetMaterial ();
	if ( material ) {
		state.Push (( MOAILuaObject* )material->mShader );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractMaterialInterface::_getTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractMaterialInterface, "U" )

	MOAIAbstractMaterial* material = self->GetMaterial ();
	if ( material ) {
		u32 name = MOAIMaterialGlobals::GetNamedGlobalID ( state, 2 );
		state.Push ( material->GetTexture ( name ));
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/** @lua	Set blend mode using OpenGL source and dest factors. OpenGl blend factor constants are exposed as members of MOAIAbstractMaterialInterface.
			See the OpenGL documentation for an explanation of blending constants.

	@in		MOAIAbstractMaterialInterface self
	@in		number srcFactor
	@in		number dstFactor
	@out	nil
*/
int MOAIAbstractMaterialInterface::_setBlendMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractMaterialInterface, "U" )
	
	MOAIBlendFuncEnum::_ equation			= ( MOAIBlendFuncEnum::_ )state.GetValue < u32 >( 2, MOAIBlendFuncEnum::ADD );
	MOAIBlendFactorEnum::_ srcFactor		= ( MOAIBlendFactorEnum::_ )state.GetValue < u32 >( 3, MOAIBlendFactorEnum::ONE );
	MOAIBlendFactorEnum::_ dstFactor		= ( MOAIBlendFactorEnum::_ )state.GetValue < u32 >( 4, MOAIBlendFactorEnum::ONE_MINUS_SRC_ALPHA );

	MOAIBlendMode blendMode;
	blendMode.SetBlend ( equation, srcFactor, dstFactor );
	self->AffirmMaterial ().SetBlendMode ( blendMode );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setCullMode
	@text	Sets and enables face culling.
 
	@in		MOAIAbstractMaterialInterface self
	@opt	number cullMode			Default value is MOAIHasMaterialBatch.CULL_NONE.
	@out	nil
*/
int MOAIAbstractMaterialInterface::_setCullMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractMaterialInterface, "U" )

	self->AffirmMaterial ().SetCullMode (( MOAICullFuncEnum::_ )state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDepthMask
	@text	Disables or enables depth writing.
 
	@in		MOAIAbstractMaterialInterface self
	@opt	boolean depthMask		Default value is true.
	@out	nil
*/
int MOAIAbstractMaterialInterface::_setDepthMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractMaterialInterface, "U" )
	
	self->AffirmMaterial ().SetDepthMask ( state.GetValue < bool >( 2, false ));
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDepthTest
	@text	Sets and enables depth testing (assuming depth buffer is present).
 
	@in		MOAIAbstractMaterialInterface self
	@opt	number depthFunc		Default value is MOAIHasMaterialBatch.DEPTH_TEST_DISABLE.
	@out	nil
*/
int MOAIAbstractMaterialInterface::_setDepthTest ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractMaterialInterface, "U" )

	self->AffirmMaterial ().SetDepthTest (( MOAIDepthFuncEnum::_ )state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractMaterialInterface::_setLight ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractMaterialInterface, "U" )

	u32 name = MOAIMaterialGlobals::GetNamedGlobalID ( state, 2 );
	MOAILight* light = state.GetLuaObject < MOAILight >( 3, true );
	self->AffirmMaterial ().SetLight ( name, light );
	state.Push ( light );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setShader
	@text	Sets a shader in the associated material.
 
	@in		MOAIAbstractMaterialInterface self
	@in		variant shader			Overloaded to accept a MOAIShaderGL or a shader preset.
	@out	MOAIShaderGL shader		The shader that was set or created.
*/
int MOAIAbstractMaterialInterface::_setShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractMaterialInterface, "U" )
	
	MOAIShader* shader = MOAIGfxMgr::Get ().AffirmShader ( state, 2 );
	self->AffirmMaterial ().SetShader ( shader );
	state.Push ( shader );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setTexture
	@text	Sets a texture in the associated material.
 
	@in		MOAIAbstractMaterialInterface self
	@in		variant texture				Overloaded to accept a filename, MOAITextureGL, MOAIImage, MOAIStream or MOAIDataBuffer.
	@out	MOAITextureGL texture		The texture that was set or created.
*/
int MOAIAbstractMaterialInterface::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractMaterialInterface, "U" )

	u32 name = MOAIMaterialGlobals::GetNamedGlobalID ( state, 2 );
	MOAITexture* texture = NULL;
	
	MOAIAbstractMaterial& material = self->AffirmMaterial ();
	
	if ( name != MOAIMaterialGlobals::MOAI_UNKNOWN_MATERIAL_GLOBAL ) {
		texture = MOAIGfxMgr::Get ().AffirmTexture ( state, 3 );
		material.SetTexture ( name, texture );
	}
	else {
		texture = MOAIGfxMgr::Get ().AffirmTexture ( state, 2 );
		material.SetTexture ( texture );
	}
	state.Push ( texture );
	return 1;
}

//================================================================//
// MOAIAbstractMaterialInterface
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractMaterial& MOAIAbstractMaterialInterface::AffirmMaterial () {

	return this->MOAIAbstractMaterialInterface_AffirmMaterial ();
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialInterface::Clear () {

	MOAIAbstractMaterial* material = this->GetMaterial ();
	if ( !material ) return;

	this->Clear ( MOAIMaterial::ALL_FLAGS );
	material->MOAIAbstractMaterial_ClearGlobals ();
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialInterface::Clear ( u32 flags, bool force ) {
	
	MOAIAbstractMaterial* material = this->GetMaterial ();
	if ( !material ) return;
	
	if ( force || ( material->mFlags & flags )) {
		
		if ( flags & MOAIMaterial::BLEND_MODE_FLAG ) {
			material->mBlendMode = MOAIBlendMode ();
		}

		if ( flags & MOAIMaterial::CULL_MODE_FLAG ) {
			material->mCullMode = MOAICullFuncEnum::NONE;
		}
		
		if ( flags & MOAIMaterial::DEPTH_MASK_FLAG ) {
			material->mDepthMask = true;
		}
		
		if ( flags & MOAIMaterial::DEPTH_TEST_FLAG ) {
			material->mDepthTest = MOAIDepthFuncEnum::NONE;
		}

		if ( flags & MOAIMaterial::SHADER_FLAG ) {
			material->mShader = NULL;
		}

		if ( flags & MOAIMaterial::TEXTURE_FLAG ) {
			material->mTexture = NULL;
		}

		material->mFlags &= ~flags;
	}
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialInterface::Compose ( MOAIAbstractMaterial& source ) {

	MOAIAbstractMaterial& material = this->AffirmMaterial ();

	u32 available = ~material.mFlags & source.mFlags;

	// only apply if there are some flags open
	if ( available ) {
	
		if ( available & MOAIMaterial::DRAW_FLAGS ) {
	
			if ( available & MOAIMaterial::BLEND_MODE_FLAG ) {
				material.mBlendMode = source.mBlendMode;
			}
			
			if ( available & MOAIMaterial::CULL_MODE_FLAG ) {
				material.mCullMode = source.mCullMode;
			}
			
			if ( available & MOAIMaterial::DEPTH_MASK_FLAG ) {
				material.mDepthMask = source.mDepthMask;
			}
			
			if ( available & MOAIMaterial::DEPTH_TEST_FLAG ) {
				material.mDepthTest = source.mDepthTest;
			}
		}
		
		if ( available & ( MOAIMaterial::SHADER_FLAG | MOAIMaterial::TEXTURE_FLAG )) {
			
			if ( available & MOAIMaterial::SHADER_FLAG ) {
				material.mShader = source.mShader;
			}
			
			if ( available & MOAIMaterial::TEXTURE_FLAG ) {
				material.mTexture = source.mTexture;
			}
		}
		
		material.mFlags |= available;
	}
	
	source.MOAIAbstractMaterial_ApplyGlobals ( material );
}

//----------------------------------------------------------------//
MOAIBlendMode MOAIAbstractMaterialInterface::GetBlendMode () {

	MOAIAbstractMaterial* material = this->GetMaterial ();
	return material ? material->mBlendMode : MOAIBlendMode ();
}

//----------------------------------------------------------------//
int MOAIAbstractMaterialInterface::GetCullMode () {

	MOAIAbstractMaterial* material = this->GetMaterial ();
	return material ? material->mCullMode : 0;
}

//----------------------------------------------------------------//
bool MOAIAbstractMaterialInterface::GetDepthMask () {

	MOAIAbstractMaterial* material = this->GetMaterial ();
	return material ? material->mDepthMask : false;
}

//----------------------------------------------------------------//
int MOAIAbstractMaterialInterface::GetDepthTest () {

	MOAIAbstractMaterial* material = this->GetMaterial ();
	return material ? material->mDepthTest : 0;
}

//----------------------------------------------------------------//
MOAILight* MOAIAbstractMaterialInterface::GetLight ( u32 name ) {

	MOAIAbstractMaterial* material = this->GetMaterial ();
	return material ? material->MOAIAbstractMaterial_GetLight ( name ) : NULL;
}

//----------------------------------------------------------------//
MOAIAbstractMaterial* MOAIAbstractMaterialInterface::GetMaterial () {

	return this->MOAIAbstractMaterialInterface_GetMaterial ();
}

//----------------------------------------------------------------//
MOAIShader* MOAIAbstractMaterialInterface::GetShader () {

	MOAIAbstractMaterial* material = this->GetMaterial ();
	return material ? ( MOAIShader* )material->mShader : NULL;
}

//----------------------------------------------------------------//
MOAITexture* MOAIAbstractMaterialInterface::GetTexture () {

	MOAIAbstractMaterial* material = this->GetMaterial ();
	return material ? ( MOAITexture* )material->mTexture : NULL;
}

//----------------------------------------------------------------//
MOAITexture* MOAIAbstractMaterialInterface::GetTexture ( u32 name ) {

	MOAIAbstractMaterial* material = this->GetMaterial ();
	return material ? material->MOAIAbstractMaterial_GetTexture ( name ) : NULL;
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialInterface::LoadGfxState () {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	MOAIAbstractMaterial* material = this->GetMaterial ();
	if ( material ) {
	
		gfxMgr.SetBlendMode ( material->mBlendMode );
		gfxMgr.SetCullFunc ( material->mCullMode );
		gfxMgr.SetDepthMask ( material->mDepthMask );
		gfxMgr.SetDepthFunc ( material->mDepthTest );
		gfxMgr.SetTexture ( material->mTexture );
		
		// load shader last!
		gfxMgr.SetShader ( material->mShader );
	}
	else {
	
		gfxMgr.SetBlendMode ( MOAIBlendMode ());
		gfxMgr.SetCullFunc (  );
		gfxMgr.SetDepthMask ( false );
		gfxMgr.SetDepthFunc ( MOAIDepthFuncEnum::NONE );
		gfxMgr.SetTexture ();
		
		// load shader last!
		gfxMgr.SetShader ();
	}
}

//----------------------------------------------------------------//
MOAIAbstractMaterialInterface::MOAIAbstractMaterialInterface () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractMaterialInterface::~MOAIAbstractMaterialInterface () {
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialInterface::SetBlendMode () {

	MOAIAbstractMaterial& material = this->AffirmMaterial ();

	if ( material.mOverwrite ) {
		material.mFlags &= ~MOAIMaterial::BLEND_MODE_FLAG;
	}
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialInterface::SetBlendMode ( const MOAIBlendMode& blendMode ) {
	
	MOAIAbstractMaterial& material = this->AffirmMaterial ();
	
	if ( material.mOverwrite || !( material.mFlags & MOAIMaterial::BLEND_MODE_FLAG )) {
		material.mBlendMode = blendMode;
		material.mFlags |= MOAIMaterial::BLEND_MODE_FLAG;
	}
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialInterface::SetCullMode () {

	MOAIAbstractMaterial& material = this->AffirmMaterial ();

	if ( material.mOverwrite ) {
		material.mFlags &= ~MOAIMaterial::CULL_MODE_FLAG;
	}
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialInterface::SetCullMode ( MOAICullFuncEnum::_ cullMode ) {

	MOAIAbstractMaterial& material = this->AffirmMaterial ();

	if ( material.mOverwrite || !( material.mFlags & MOAIMaterial::CULL_MODE_FLAG )) {
		material.mCullMode = cullMode;
		material.mFlags |= MOAIMaterial::CULL_MODE_FLAG;
	}
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialInterface::SetDepthMask () {

	MOAIAbstractMaterial& material = this->AffirmMaterial ();

	if ( material.mOverwrite ) {
		material.mFlags &= ~MOAIMaterial::DEPTH_MASK_FLAG;
	}
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialInterface::SetDepthMask ( bool depthMask ) {

	MOAIAbstractMaterial& material = this->AffirmMaterial ();

	if ( material.mOverwrite || !( material.mFlags & MOAIMaterial::DEPTH_MASK_FLAG )) {
		material.mDepthMask = depthMask;
		material.mFlags |= MOAIMaterial::DEPTH_MASK_FLAG;
	}
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialInterface::SetDepthTest () {

	MOAIAbstractMaterial& material = this->AffirmMaterial ();

	if ( material.mOverwrite ) {
		material.mFlags &= ~MOAIMaterial::DEPTH_TEST_FLAG;
	}
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialInterface::SetDepthTest ( MOAIDepthFuncEnum::_ depthTest ) {

	MOAIAbstractMaterial& material = this->AffirmMaterial ();

	if ( material.mOverwrite || !( material.mFlags & MOAIMaterial::DEPTH_TEST_FLAG )) {
		material.mDepthTest = depthTest;
		material.mFlags |= MOAIMaterial::DEPTH_TEST_FLAG;
	}
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialInterface::SetLight ( u32 name ) {

	this->SetLight ( name, 0 );
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialInterface::SetLight ( u32 name, MOAILight* light ) {

	if ( name < MOAIMaterialGlobals::MAX_GLOBAL_LIGHTS ) {
		this->AffirmMaterial ().MOAIAbstractMaterial_SetLight ( name, light );
	}
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialInterface::SetShader () {

	this->SetShader ( 0 );
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialInterface::SetShader ( MOAIShaderPresetEnum shaderID ) {

	this->SetShader ( MOAIGfxMgr::Get ().GetShaderPreset ( shaderID ));
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialInterface::SetShader ( MOAIShader* shader ) {

	MOAIAbstractMaterial& material = this->AffirmMaterial ();

	if ( material.mOverwrite || !( material.mFlags & MOAIMaterial::SHADER_FLAG )) {

		material.mShader = shader;
		
		if ( shader ) {
			material.mFlags |= MOAIMaterial::SHADER_FLAG;
		}
		else {
			material.mFlags &= ~MOAIMaterial::SHADER_FLAG;
		}
	}
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialInterface::SetTexture () {

	this->SetTexture (( MOAITexture* )NULL );
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialInterface::SetTexture ( MOAITexture* texture ) {

	MOAIAbstractMaterial& material = this->AffirmMaterial ();

	if ( material.mOverwrite || !( material.mFlags & MOAIMaterial::TEXTURE_FLAG )) {

		material.mTexture = texture;

		if ( texture ) {
			material.mFlags |= MOAIMaterial::TEXTURE_FLAG;
		}
		else {
			material.mFlags &= ~MOAIMaterial::TEXTURE_FLAG;
		}
	}
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialInterface::SetTexture ( u32 name ) {

	this->AffirmMaterial ().SetTexture ( name, NULL );
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialInterface::SetTexture ( u32 name, MOAITexture* texture ) {

	if ( name < MOAIMaterialGlobals::MAX_GLOBAL_LIGHTS ) {
		this->AffirmMaterial ().MOAIAbstractMaterial_SetTexture ( name, texture );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractMaterialInterface::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIAbstractMaterialInterface::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "getBlendMode",			_getBlendMode },
		{ "getCullMode",			_getCullMode },
		{ "getDepthMask",			_getDepthMask },
		{ "getDepthTest",			_getDepthTest },
		{ "getLight",				_getLight },
		{ "getShader",				_getShader },
		{ "getTexture",				_getTexture },
		{ "setBlendMode",			_setBlendMode },
		{ "setCullMode",			_setCullMode },
		{ "setDepthMask",			_setDepthMask },
		{ "setDepthTest",			_setDepthTest },
		{ "setLight",				_setLight },
		{ "setShader",				_setShader },
		{ "setTexture",				_setTexture },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
