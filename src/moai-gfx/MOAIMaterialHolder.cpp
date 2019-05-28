// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIMaterialHolder.h>
#include <moai-gfx/MOAIShaderGL.h>
#include <moai-gfx/MOAITextureGL.h>
#include <moai-gfx/MOAITextureBaseGL.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialHolder::_getBlendMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialHolder, "U" )
	
	const ZLBlendMode& blendMode = self->mMaterial.GetBlendMode ();
	
	state.Push ( blendMode.mEquation );
	state.Push ( blendMode.mSourceFactor );
	state.Push ( blendMode.mDestFactor );
	
	return 3;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialHolder::_getCullMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialHolder, "U" )
	state.Push ( self->mMaterial.GetCullMode ());
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialHolder::_getDepthMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialHolder, "U" )
	state.Push ( self->mMaterial.GetDepthMask ());
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialHolder::_getDepthTest ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialHolder, "U" )
	state.Push ( self->mMaterial.GetDepthTest ());
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialHolder::_getLight ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialHolder, "UN" )
	
	u32 name = state.GetValue < u32 >( 2,  MOAI_UNKNOWN_MATERIAL_GLOBAL );
	state.Push ( self->mMaterial.GetLight ( name ));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialHolder::_getShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialHolder, "U" )
	state.Push ( self->mMaterial.GetShader ());
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialHolder::_getTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialHolder, "U" )
	
	u32 name = state.GetValue < u32 >( 2,  MOAI_UNKNOWN_MATERIAL_GLOBAL );
	state.Push ( self->mMaterial.GetTexture ( name ));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialHolder::_setBlendMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialHolder, "U" )

	if ( state.IsNil ( 2 )) {
		self->mMaterial.SetBlendMode ();
	}
	else {
		ZLBlendMode blendMode;
		blendMode.mEquation			= ( ZLColor::BlendEquation )state.GetValue < u32 >( 2, ( u32 )ZLColor::BLEND_EQ_NONE );
		blendMode.mSourceFactor		= ( ZLColor::BlendFactor )state.GetValue < u32 >( 3, ( u32 )ZLColor::BLEND_FACTOR_ZERO );
		blendMode.mDestFactor		= ( ZLColor::BlendFactor )state.GetValue < u32 >( 4, ( u32 )ZLColor::BLEND_FACTOR_ZERO );
		self->mMaterial.SetBlendMode ( blendMode );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialHolder::_setCullMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialHolder, "U" )
	
	if ( state.IsNil ( 2 )) {
		self->mMaterial.SetCullMode ();
	}
	else {
		self->mMaterial.SetCullMode ( state.GetValue < int >( 2, 0 ));
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialHolder::_setDepthMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialHolder, "U" )
	
	if ( state.IsNil ( 2 )) {
		self->mMaterial.SetDepthMask ();
	}
	else {
		self->mMaterial.SetDepthMask ( state.GetValue < bool >( 2, false ));
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialHolder::_setDepthTest ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialHolder, "U" )
	
	if ( state.IsNil ( 2 )) {
		self->mMaterial.SetDepthTest ();
	}
	else {
		self->mMaterial.SetDepthTest ( state.GetValue < int >( 2, 0 ));
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialHolder::_setLight ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialHolder, "U" )
	
	u32 name = state.GetValue < u32 >( 2, MOAI_UNKNOWN_MATERIAL_GLOBAL );
	MOAILight* light = state.GetLuaObject < MOAILight >( 3, true );
	self->mMaterial.SetLight ( name, light );
	state.Push ( light );
	
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialHolder::_setShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialHolder, "U" )
	
	ZLAbstractShader* shader = MOAIAbstractGfxMgr::Get ().AffirmShader ( state, 2 );
	self->mMaterial.SetShader ( shader );
	state.Push ( shader->AsType < MOAILuaObject >());
	
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMaterialHolder::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMaterialHolder, "U" )
	
	u32 idx = 2;
	u32 name = MOAI_UNKNOWN_MATERIAL_GLOBAL;
	
	if ( state.IsType ( 2, LUA_TNUMBER )) {
		name = state.GetValue < u32 >( idx++, name );
	}
	
	ZLAbstractTexture* texture = MOAIAbstractGfxMgr::Get ().AffirmTexture ( state, idx );
	if ( name != MOAI_UNKNOWN_MATERIAL_GLOBAL ) {
		self->mMaterial.SetTexture ( name, texture );
	}
	else {
		self->mMaterial.SetTexture ( texture );
	}
	state.Push ( texture->AsType < MOAILuaObject >());
	return 1;
}

//================================================================//
// MOAIMaterialHolder
//================================================================//

//----------------------------------------------------------------//
MOAIMaterialHolder::MOAIMaterialHolder () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIMaterialHolder )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIMaterialHolder::~MOAIMaterialHolder () {
}

//----------------------------------------------------------------//
void MOAIMaterialHolder::RegisterLuaFuncs ( MOAILuaState& state ) {

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
