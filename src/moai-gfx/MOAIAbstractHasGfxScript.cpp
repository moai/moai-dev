// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractGfxScript.h>
#include <moai-gfx/MOAIAbstractHasGfxScript.h>
#include <moai-gfx/MOAIBlendMode.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIGfxScriptRetained.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractHasGfxScript::_getGfxScript ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractHasGfxScript, "U" )
	
	state.Push (( MOAIAbstractGfxScript* )self->GetGfxScript ());
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractHasGfxScript::_setBlendMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractHasGfxScript, "U" )

	MOAIBlendFuncEnum::_ equation			= ( MOAIBlendFuncEnum::_ )state.GetValue < u32 >( 2, MOAIBlendFuncEnum::ADD );
	MOAIBlendFactorEnum::_ srcFactor		= ( MOAIBlendFactorEnum::_ )state.GetValue < u32 >( 3, MOAIBlendFactorEnum::ONE );
	MOAIBlendFactorEnum::_ dstFactor		= ( MOAIBlendFactorEnum::_ )state.GetValue < u32 >( 4, MOAIBlendFactorEnum::ONE_MINUS_SRC_ALPHA );

	MOAIBlendMode blendMode;
	blendMode.SetBlend ( equation, srcFactor, dstFactor );
	self->SetBlendMode ( blendMode );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractHasGfxScript::_setCullFunc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractHasGfxScript, "U" )

	self->SetCullFunc (( MOAICullFuncEnum::_ )state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractHasGfxScript::_setDepthFunc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractHasGfxScript, "U" )

	self->SetDepthFunc (( MOAIDepthFuncEnum::_ )state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractHasGfxScript::_setDepthMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractHasGfxScript, "U" )

	self->SetDepthMask ( state.GetValue < bool >( 2, false ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractHasGfxScript::_setGfxScript ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractHasGfxScript, "U" )

	self->SetGfxScript ( state.GetLuaObject < MOAIAbstractGfxScript >( 2, true ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractHasGfxScript::_setShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractHasGfxScript, "U" )

	MOAIShader* shader 		= MOAIGfxMgr::Get ().AffirmShader ( state, 2 );
	self->SetShader ( shader );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractHasGfxScript::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractHasGfxScript, "U" )

	MOAITexture* texture	= MOAIGfxMgr::Get ().AffirmTexture ( state, 2 );
	ZLIndex textureUnit		= state.GetValue < ZLIndex >( 3, 0 );

	self->SetTexture ( texture, textureUnit );
	return 0;
}

//================================================================//
// MOAIAbstractHasGfxScript
//================================================================//

//----------------------------------------------------------------//
MOAIGfxScriptRetained& MOAIAbstractHasGfxScript::AffirmGfxScript () {

	return this->MOAIAbstractHasGfxScript_AffirmGfxScript ();
}

//----------------------------------------------------------------//
MOAIAbstractGfxScript* MOAIAbstractHasGfxScript::GetGfxScript () {

	return this->MOAIAbstractHasGfxScript_GetGfxScript ();
}

//----------------------------------------------------------------//
MOAIAbstractHasGfxScript::MOAIAbstractHasGfxScript () {

	RTTI_BEGIN ( MOAIAbstractHasGfxScript )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractHasGfxScript >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractHasGfxScript::~MOAIAbstractHasGfxScript () {
}

//----------------------------------------------------------------//
void MOAIAbstractHasGfxScript::SetBlendMode ( const MOAIBlendMode& blendMode ) {

	this->AffirmGfxScript ().SetBlendMode ( blendMode );
}

//----------------------------------------------------------------//
void MOAIAbstractHasGfxScript::SetCullFunc ( MOAICullFuncEnum::_ cullFunc ) {

	this->AffirmGfxScript ().SetCullFunc ( cullFunc );
}

//----------------------------------------------------------------//
void MOAIAbstractHasGfxScript::SetDepthFunc ( MOAIDepthFuncEnum::_ depthFunc ) {

	this->AffirmGfxScript ().SetDepthFunc ( depthFunc );
}

//----------------------------------------------------------------//
void MOAIAbstractHasGfxScript::SetDepthMask ( bool depthMask ) {

	this->AffirmGfxScript ().SetDepthMask ( depthMask );
}

//----------------------------------------------------------------//
void MOAIAbstractHasGfxScript::SetGfxScript ( MOAIAbstractGfxScript* gfxScript ) {

	this->MOAIAbstractHasGfxScript_SetGfxScript ( gfxScript );
}

//----------------------------------------------------------------//
void MOAIAbstractHasGfxScript::SetShader ( MOAIShader* shader ) {

	this->AffirmGfxScript ().SetShader ( shader );
}

//----------------------------------------------------------------//
void MOAIAbstractHasGfxScript::SetTexture ( MOAITexture* texture, ZLIndex textureUnit ) {

	this->AffirmGfxScript ().SetTexture ( texture, textureUnit );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractHasGfxScript::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAbstractHasGfxScript::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getGfxScript",				_getGfxScript },
		{ "setGfxScript",				_setGfxScript },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}
