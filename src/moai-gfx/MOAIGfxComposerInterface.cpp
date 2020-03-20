// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIGfxComposer.h>
#include <moai-gfx/MOAIGfxComposerCmd.h>
#include <moai-gfx/MOAIGfxComposerInterface.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIGfxComposerInterface::_pushCallCmd ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxComposerInterface, "U" )
	self->PushCallCmd ();
	return 0;
}

//----------------------------------------------------------------//
int MOAIGfxComposerInterface::_pushCallFromShaderCmd ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxComposerInterface, "U" )
	self->PushCallFromShaderCmd ();
	return 0;
}

//----------------------------------------------------------------//
int MOAIGfxComposerInterface::_pushShaderCmd ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxComposerInterface, "U" )
	
	MOAIGfxComposerAddrModeEnum::_ from 	= ( MOAIGfxComposerAddrModeEnum::_ )state.GetValue < u32 >( 2, ( u32 )MOAIGfxComposerAddrModeEnum::NONE );
	MOAIGfxComposerAddrModeEnum::_ to 		= ( MOAIGfxComposerAddrModeEnum::_ )state.GetValue < u32 >( 3, ( u32 )MOAIGfxComposerAddrModeEnum::NONE );
	MOAIShader* shader 						= MOAIGfxMgr::Get ().AffirmShader ( state, 3 );

	self->PushShaderCmd ( from, to, shader );
	return 0;
}

//----------------------------------------------------------------//
int MOAIGfxComposerInterface::_pushTextureCmd ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxComposerInterface, "U" )
	
	MOAIGfxComposerAddrModeEnum::_ from 	= ( MOAIGfxComposerAddrModeEnum::_ )state.GetValue < u32 >( 2, ( u32 )MOAIGfxComposerAddrModeEnum::NONE );
	MOAIGfxComposerAddrModeEnum::_ to 		= ( MOAIGfxComposerAddrModeEnum::_ )state.GetValue < u32 >( 3, ( u32 )MOAIGfxComposerAddrModeEnum::NONE );
	MOAITexture* texture 					= MOAIGfxMgr::Get ().AffirmTexture ( state, 3 );
	ZLIndex textureUnit						= state.GetValue < ZLIndex >( 4, 0 );

	self->PushTextureCmd ( from, to, texture, textureUnit );
	return 0;
}

//----------------------------------------------------------------//
int MOAIGfxComposerInterface::_pushUniformCmd ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxComposerInterface, "U" )
	self->PushCallCmd ();
	return 0;
}

//================================================================//
// MOAIGfxComposerInterface
//================================================================//

//----------------------------------------------------------------//
MOAIGfxComposer& MOAIGfxComposerInterface::AffirmComposer () {

	return this->MOAIGfxComposerInterface_AffirmComposer ();
}

//----------------------------------------------------------------//
MOAIGfxComposer* MOAIGfxComposerInterface::GetComposer () {

	return this->MOAIGfxComposerInterface_GetComposer ();
}

//----------------------------------------------------------------//
MOAIGfxComposerInterface::MOAIGfxComposerInterface () {
	
	RTTI_BEGIN ( MOAIGfxComposerInterface )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIGfxComposerInterface >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxComposerInterface::~MOAIGfxComposerInterface () {
}

//----------------------------------------------------------------//
void MOAIGfxComposerInterface::PushCallCmd () {

	MOAIGfxComposerCmd* cmd = new MOAIGfxComposerCmd ();
	cmd->mType = MOAIGfxComposerCmdEnum::CALL;
	this->AffirmComposer ().mCommands.Push ( cmd );
}

//----------------------------------------------------------------//
void MOAIGfxComposerInterface::PushCallFromShaderCmd () {

	MOAIGfxComposerCmd* cmd = new MOAIGfxComposerCmd ();
	cmd->mType = MOAIGfxComposerCmdEnum::CALL_FROM_SHADER;
	this->AffirmComposer ().mCommands.Push ( cmd );
}

//----------------------------------------------------------------//
void MOAIGfxComposerInterface::PushShaderCmd ( MOAIGfxComposerAddrModeEnum::_ from, MOAIGfxComposerAddrModeEnum::_ to, MOAIShader* shader ) {

	assert ( from == MOAIGfxComposerAddrModeEnum::FROM_OBJECT );
	assert ( to == MOAIGfxComposerAddrModeEnum::TO_GFX_STATE );
	
	MOAIGfxComposerCmd* cmd = new MOAIGfxComposerCmd ( MOAIGfxComposerCmdEnum::SHADER, from, to );
	cmd->mShader = shader;
	this->AffirmComposer ().mCommands.Push ( cmd );
}

//----------------------------------------------------------------//
void MOAIGfxComposerInterface::PushTextureCmd ( MOAIGfxComposerAddrModeEnum::_ from, MOAIGfxComposerAddrModeEnum::_ to, MOAITexture* texture, ZLIndex textureUnit ) {

	assert ( from == MOAIGfxComposerAddrModeEnum::FROM_OBJECT );
	assert ( to == MOAIGfxComposerAddrModeEnum::TO_GFX_STATE );
	
	MOAIGfxComposerCmd* cmd = new MOAIGfxComposerCmd ( MOAIGfxComposerCmdEnum::TEXTURE, from, to );
	cmd->mTexture = texture;
	cmd->mTextureUnit = textureUnit;
	this->AffirmComposer ().mCommands.Push ( cmd );
}

//----------------------------------------------------------------//
void MOAIGfxComposerInterface::PushUniformCmd ( MOAIGfxComposerAddrModeEnum::_ from, MOAIGfxComposerAddrModeEnum::_ to, ZLIndex globalID, ZLIndex uniformID, ZLIndex index ) {

	assert ( from == MOAIGfxComposerAddrModeEnum::FROM_PIPELINE_GLOBALS );
	assert ( to == MOAIGfxComposerAddrModeEnum::TO_SHADER_BODY );
	
	MOAIGfxComposerCmd* cmd = new MOAIGfxComposerCmd ( MOAIGfxComposerCmdEnum::UNIFORM, from, to );
	cmd->mPipelineGlobalID = globalID;
	cmd->mTargetUniformID = uniformID;
	cmd->mTargetUniformIndex = index;
	this->AffirmComposer ().mCommands.Push ( cmd );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxComposerInterface::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
	
	state.SetField ( -1, "FROM_MATERIAL_MGR",			( u32 )MOAIGfxComposerAddrModeEnum::FROM_MATERIAL_MGR );
	state.SetField ( -1, "FROM_OBJECT",					( u32 )MOAIGfxComposerAddrModeEnum::FROM_OBJECT );
	state.SetField ( -1, "FROM_PIPELINE_GLOBALS",		( u32 )MOAIGfxComposerAddrModeEnum::FROM_PIPELINE_GLOBALS );
	state.SetField ( -1, "TO_GFX_STATE",				( u32 )MOAIGfxComposerAddrModeEnum::TO_GFX_STATE );
	state.SetField ( -1, "TO_MATERIAL_MGR",				( u32 )MOAIGfxComposerAddrModeEnum::TO_MATERIAL_MGR );
	state.SetField ( -1, "TO_SHADER_BODY",				( u32 )MOAIGfxComposerAddrModeEnum::TO_SHADER_BODY );
	
//	state.SetField ( -1, "UNIFORM_WIDTH_VEC_2",						( u32 )MOAIUniformDescriptor::UNIFORM_WIDTH_VEC_2 );
//	state.SetField ( -1, "UNIFORM_WIDTH_VEC_3",						( u32 )MOAIUniformDescriptor::UNIFORM_WIDTH_VEC_3 );
//	state.SetField ( -1, "UNIFORM_WIDTH_VEC_4",						( u32 )MOAIUniformDescriptor::UNIFORM_WIDTH_VEC_4 );
//	state.SetField ( -1, "UNIFORM_WIDTH_MATRIX_3X3",				( u32 )MOAIUniformDescriptor::UNIFORM_WIDTH_MATRIX_3X3 );
//	state.SetField ( -1, "UNIFORM_WIDTH_MATRIX_4X4",				( u32 )MOAIUniformDescriptor::UNIFORM_WIDTH_MATRIX_4X4 );
//
//	state.SetField ( -1, "GLOBAL_CLIP_TO_DISPLAY_MTX",				( u32 )MOAIGfxMgr::CLIP_TO_DISPLAY_MTX );
//	state.SetField ( -1, "GLOBAL_CLIP_TO_MODEL_MTX",				( u32 )MOAIGfxMgr::CLIP_TO_MODEL_MTX );
//	state.SetField ( -1, "GLOBAL_CLIP_TO_VIEW_MTX",					( u32 )MOAIGfxMgr::CLIP_TO_VIEW_MTX );
//	state.SetField ( -1, "GLOBAL_CLIP_TO_WINDOW_MTX",				( u32 )MOAIGfxMgr::CLIP_TO_WINDOW_MTX );
//	state.SetField ( -1, "GLOBAL_CLIP_TO_WORLD_MTX",				( u32 )MOAIGfxMgr::CLIP_TO_WORLD_MTX );
//
//	state.SetField ( -1, "GLOBAL_DISPLAY_TO_CLIP_MTX",				( u32 )MOAIGfxMgr::DISPLAY_TO_CLIP_MTX );
//	state.SetField ( -1, "GLOBAL_DISPLAY_TO_MODEL_MTX",				( u32 )MOAIGfxMgr::DISPLAY_TO_MODEL_MTX );
//	state.SetField ( -1, "GLOBAL_DISPLAY_TO_VIEW_MTX",				( u32 )MOAIGfxMgr::DISPLAY_TO_VIEW_MTX );
//	state.SetField ( -1, "GLOBAL_DISPLAY_TO_WORLD_MTX",				( u32 )MOAIGfxMgr::DISPLAY_TO_WORLD_MTX );
//
//	state.SetField ( -1, "GLOBAL_MODEL_TO_CLIP_MTX",				( u32 )MOAIGfxMgr::MODEL_TO_CLIP_MTX );
//	state.SetField ( -1, "GLOBAL_MODEL_TO_DISPLAY_MTX",				( u32 )MOAIGfxMgr::MODEL_TO_DISPLAY_MTX );
//	state.SetField ( -1, "GLOBAL_MODEL_TO_UV_MTX",					( u32 )MOAIGfxMgr::MODEL_TO_UV_MTX );
//	state.SetField ( -1, "GLOBAL_MODEL_TO_VIEW_MTX",				( u32 )MOAIGfxMgr::MODEL_TO_VIEW_MTX );
//	state.SetField ( -1, "GLOBAL_MODEL_TO_WORLD_MTX",				( u32 )MOAIGfxMgr::MODEL_TO_WORLD_MTX );
//
//	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_DISPLAY_MTX",		( u32 )MOAIGfxMgr::NORMAL_CLIP_TO_DISPLAY_MTX );
//	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_MODEL_MTX",			( u32 )MOAIGfxMgr::NORMAL_CLIP_TO_MODEL_MTX );
//	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_VIEW_MTX",			( u32 )MOAIGfxMgr::NORMAL_CLIP_TO_VIEW_MTX );
//	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_WINDOW_MTX",		( u32 )MOAIGfxMgr::NORMAL_CLIP_TO_WINDOW_MTX );
//	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_WORLD_MTX",			( u32 )MOAIGfxMgr::NORMAL_CLIP_TO_WORLD_MTX );
//
//	state.SetField ( -1, "GLOBAL_NORMAL_DISPLAY_TO_CLIP_MTX",		( u32 )MOAIGfxMgr::NORMAL_DISPLAY_TO_CLIP_MTX );
//	state.SetField ( -1, "GLOBAL_NORMAL_DISPLAY_TO_MODEL_MTX",		( u32 )MOAIGfxMgr::NORMAL_DISPLAY_TO_MODEL_MTX );
//	state.SetField ( -1, "GLOBAL_NORMAL_DISPLAY_TO_VIEW_MTX",		( u32 )MOAIGfxMgr::NORMAL_DISPLAY_TO_VIEW_MTX );
//	state.SetField ( -1, "GLOBAL_NORMAL_DISPLAY_TO_WORLD_MTX",		( u32 )MOAIGfxMgr::NORMAL_DISPLAY_TO_WORLD_MTX );
//
//	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_CLIP_MTX",			( u32 )MOAIGfxMgr::NORMAL_MODEL_TO_CLIP_MTX );
//	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_DISPLAY_MTX",		( u32 )MOAIGfxMgr::NORMAL_MODEL_TO_DISPLAY_MTX );
//	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_UV_MTX",			( u32 )MOAIGfxMgr::NORMAL_MODEL_TO_UV_MTX );
//	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_VIEW_MTX",			( u32 )MOAIGfxMgr::NORMAL_MODEL_TO_VIEW_MTX );
//	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_WORLD_MTX",		( u32 )MOAIGfxMgr::NORMAL_MODEL_TO_WORLD_MTX );
//
//	state.SetField ( -1, "GLOBAL_NORMAL_WORLD_TO_DISPLAY_MTX",		( u32 )MOAIGfxMgr::NORMAL_WORLD_TO_DISPLAY_MTX );
//	state.SetField ( -1, "GLOBAL_NORMAL_WORLD_TO_VIEW_MTX",			( u32 )MOAIGfxMgr::NORMAL_WORLD_TO_VIEW_MTX );
//
//	state.SetField ( -1, "GLOBAL_NORMAL_UV_TO_MODEL_MTX",			( u32 )MOAIGfxMgr::NORMAL_UV_TO_MODEL_MTX );
//
//	state.SetField ( -1, "GLOBAL_NORMAL_VIEW_TO_CLIP_MTX",			( u32 )MOAIGfxMgr::NORMAL_VIEW_TO_CLIP_MTX );
//	state.SetField ( -1, "GLOBAL_NORMAL_VIEW_TO_DISPLAY_MTX",		( u32 )MOAIGfxMgr::NORMAL_VIEW_TO_DISPLAY_MTX );
//	state.SetField ( -1, "GLOBAL_NORMAL_VIEW_TO_MODEL_MTX",			( u32 )MOAIGfxMgr::NORMAL_VIEW_TO_MODEL_MTX );
//	state.SetField ( -1, "GLOBAL_NORMAL_VIEW_TO_WORLD_MTX",			( u32 )MOAIGfxMgr::NORMAL_VIEW_TO_WORLD_MTX );
//
//	state.SetField ( -1, "GLOBAL_NORMAL_WINDOW_TO_CLIP_MTX",		( u32 )MOAIGfxMgr::NORMAL_WINDOW_TO_CLIP_MTX );
//	state.SetField ( -1, "GLOBAL_NORMAL_WORLD_TO_CLIP_MTX",			( u32 )MOAIGfxMgr::NORMAL_WORLD_TO_CLIP_MTX );
//	state.SetField ( -1, "GLOBAL_NORMAL_WORLD_TO_MODEL_MTX",		( u32 )MOAIGfxMgr::NORMAL_WORLD_TO_MODEL_MTX );
//
//	state.SetField ( -1, "GLOBAL_PEN_COLOR",						( u32 )MOAIGfxMgr::PEN_COLOR );
//
//	state.SetField ( -1, "GLOBAL_UV_TO_MODEL_MTX",					( u32 )MOAIGfxMgr::UV_TO_MODEL_MTX );
//
//	state.SetField ( -1, "GLOBAL_VIEW_TO_CLIP_MTX",					( u32 )MOAIGfxMgr::VIEW_TO_CLIP_MTX );
//	state.SetField ( -1, "GLOBAL_VIEW_TO_DISPLAY_MTX",				( u32 )MOAIGfxMgr::VIEW_TO_DISPLAY_MTX );
//	state.SetField ( -1, "GLOBAL_VIEW_TO_MODEL_MTX",				( u32 )MOAIGfxMgr::VIEW_TO_MODEL_MTX );
//	state.SetField ( -1, "GLOBAL_VIEW_TO_WORLD_MTX",				( u32 )MOAIGfxMgr::VIEW_TO_WORLD_MTX );
//
//	state.SetField ( -1, "GLOBAL_WINDOW_TO_CLIP_MTX",				( u32 )MOAIGfxMgr::WINDOW_TO_CLIP_MTX );
//
//	state.SetField ( -1, "GLOBAL_WORLD_TO_CLIP_MTX",				( u32 )MOAIGfxMgr::WORLD_TO_CLIP_MTX );
//	state.SetField ( -1, "GLOBAL_WORLD_TO_DISPLAY_MTX",				( u32 )MOAIGfxMgr::WORLD_TO_DISPLAY_MTX );
//	state.SetField ( -1, "GLOBAL_WORLD_TO_MODEL_MTX",				( u32 )MOAIGfxMgr::WORLD_TO_MODEL_MTX );
//	state.SetField ( -1, "GLOBAL_WORLD_TO_VIEW_MTX",				( u32 )MOAIGfxMgr::WORLD_TO_VIEW_MTX );
//
//	state.SetField ( -1, "GLOBAL_VIEW_HEIGHT",						( u32 )MOAIGfxMgr::VIEW_HEIGHT );
//	state.SetField ( -1, "GLOBAL_VIEW_WIDTH",						( u32 )MOAIGfxMgr::VIEW_WIDTH );
//
//	state.SetField ( -1, "GLOBAL_VIEW_HALF_HEIGHT",					( u32 )MOAIGfxMgr::VIEW_HALF_HEIGHT );
//	state.SetField ( -1, "GLOBAL_VIEW_HALF_WIDTH",					( u32 )MOAIGfxMgr::VIEW_HALF_WIDTH );
}

//----------------------------------------------------------------//
void MOAIGfxComposerInterface::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;

//	luaL_Reg regTable [] = {
//		{ "reserveGlobals",				_reserveGlobals },
//		{ "reserveTextures",			_reserveTextures },
//		{ "setGlobal",					_setGlobal },
//		{ "setTexture",					_setTexture },
//		{ NULL, NULL }
//	};
//	luaL_register ( state, 0, regTable );
}
