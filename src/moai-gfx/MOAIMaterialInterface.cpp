// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>
#include <moai-gfx/MOAILight.h>
#include <moai-gfx/MOAIMaterial.h>
#include <moai-gfx/MOAIMaterialInterface.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialInterface::_getBlendMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialInterface, "U" )

	MOAIMaterial* material = self->GetMaterial ();
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
int MOAIMaterialInterface::_getCullMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialInterface, "U" )
	
	MOAIMaterial* material = self->GetMaterial ();
	if ( material ) {
		state.Push ( material->mCullMode );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialInterface::_getDepthMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialInterface, "U" )
	
	MOAIMaterial* material = self->GetMaterial ();
	if ( material ) {
		state.Push ( material->mDepthMask );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialInterface::_getDepthTest ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialInterface, "U" )
	
	MOAIMaterial* material = self->GetMaterial ();
	if ( material ) {
	
		state.Push ( material->mDepthTest );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialInterface::_getLight ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialInterface, "U" )
	
	MOAIMaterial* material = self->GetMaterial ();
	if ( material ) {
		u32 name = MOAIMaterial::GetNamedGlobalID ( state, 2 );
		state.Push ( material->GetLight ( name ));
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialInterface::_getShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialInterface, "U" )

	MOAIMaterial* material = self->GetMaterial ();
	if ( material ) {
		state.Push (( MOAILuaObject* )material->mShader );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialInterface::_getTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialInterface, "U" )

	MOAIMaterial* material = self->GetMaterial ();
	if ( material ) {
		u32 name = MOAIMaterial::GetNamedGlobalID ( state, 2 );
		state.Push ( material->GetTexture ( name ));
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/** @lua	Set blend mode using OpenGL source and dest factors. OpenGl blend factor constants are exposed as members of MOAIMaterialInterface.
			See the OpenGL documentation for an explanation of blending constants.

	@in		MOAIMaterialInterface self
	@in		number srcFactor
	@in		number dstFactor
	@out	nil
*/
int MOAIMaterialInterface::_setBlendMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialInterface, "U" )
	
	u32 equation	= state.GetValue < u32 >( 2, ZGL_BLEND_MODE_ADD );
	u32 srcFactor	= state.GetValue < u32 >( 3, ZGL_BLEND_FACTOR_ONE );
	u32 dstFactor	= state.GetValue < u32 >( 4, ZGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA );

	MOAIBlendMode blendMode;
	blendMode.SetBlend ( equation, srcFactor, dstFactor );
	self->AffirmMaterial ().SetBlendMode ( blendMode );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setCullMode
	@text	Sets and enables face culling.
 
	@in		MOAIMaterialInterface self
	@opt	number cullMode			Default value is MOAIMaterialBatchHolder.CULL_NONE.
	@out	nil
*/
int MOAIMaterialInterface::_setCullMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialInterface, "U" )

	self->AffirmMaterial ().SetCullMode ( state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDepthMask
	@text	Disables or enables depth writing.
 
	@in		MOAIMaterialInterface self
	@opt	boolean depthMask		Default value is true.
	@out	nil
*/
int MOAIMaterialInterface::_setDepthMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialInterface, "U" )
	
	self->AffirmMaterial ().SetDepthMask ( state.GetValue < bool >( 2, false ));
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDepthTest
	@text	Sets and enables depth testing (assuming depth buffer is present).
 
	@in		MOAIMaterialInterface self
	@opt	number depthFunc		Default value is MOAIMaterialBatchHolder.DEPTH_TEST_DISABLE.
	@out	nil
*/
int MOAIMaterialInterface::_setDepthTest ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialInterface, "U" )

	self->AffirmMaterial ().SetDepthTest ( state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialInterface::_setLight ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialInterface, "U" )

	u32 name = MOAIMaterial::GetNamedGlobalID ( state, 2 );
	MOAILight* light = state.GetLuaObject < MOAILight >( 3, true );
	self->AffirmMaterial ().SetLight ( name, light );
	state.Push ( light );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setShader
	@text	Sets a shader in the associated material.
 
	@in		MOAIMaterialInterface self
	@in		variant shader			Overloaded to accept a MOAIShaderGL or a shader preset.
	@out	MOAIShaderGL shader		The shader that was set or created.
*/
int MOAIMaterialInterface::_setShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialInterface, "U" )
	
	MOAIShader* shader = MOAIGfxMgr::Get ().AffirmShader ( state, 2 );
	self->AffirmMaterial ().SetShader ( shader );
	state.Push ( shader );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setTexture
	@text	Sets a texture in the associated material.
 
	@in		MOAIMaterialInterface self
	@in		variant texture				Overloaded to accept a filename, MOAITextureGL, MOAIImage, MOAIStream or MOAIDataBuffer.
	@out	MOAITextureGL texture		The texture that was set or created.
*/
int MOAIMaterialInterface::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialInterface, "U" )

	u32 name = MOAIMaterial::GetNamedGlobalID ( state, 2 );
	MOAITexture* texture = NULL;
	
	MOAIMaterial& material = self->AffirmMaterial ();
	
	if ( name != MOAI_UNKNOWN_MATERIAL_GLOBAL ) {
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
// MOAIMaterialInterface
//================================================================//

//----------------------------------------------------------------//
MOAIMaterial& MOAIMaterialInterface::AffirmMaterial () {

	return this->MOAIMaterialInterface_AffirmMaterial ();
}

//----------------------------------------------------------------//
MOAIMaterial* MOAIMaterialInterface::GetMaterial () {

	return this->MOAIMaterialInterface_GetMaterial ();
}

//----------------------------------------------------------------//
MOAIMaterialInterface::MOAIMaterialInterface () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIMaterialInterface::~MOAIMaterialInterface () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIMaterialInterface::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIMaterialInterface::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	
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
