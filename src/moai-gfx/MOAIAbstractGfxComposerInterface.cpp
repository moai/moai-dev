// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIGfxComposerRetained.h>
#include <moai-gfx/MOAIAbstractGfxComposerInterface.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIAbstractGfxComposerInterface::_call ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxComposerInterface, "U" )
	self->Call ();
	return 0;
}

//----------------------------------------------------------------//
int MOAIAbstractGfxComposerInterface::_callFromShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxComposerInterface, "U" )
	self->CallFromShader ();
	return 0;
}

//----------------------------------------------------------------//
int MOAIAbstractGfxComposerInterface::_setShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxComposerInterface, "U" )
	
	MOAIShader* shader		= MOAIGfxMgr::Get ().AffirmShader ( state, 2 );

	self->SetShader ( shader );
	return 0;
}

//----------------------------------------------------------------//
int MOAIAbstractGfxComposerInterface::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxComposerInterface, "U" )
	
	MOAITexture* texture	= MOAIGfxMgr::Get ().AffirmTexture ( state, 2 );
	ZLIndex textureUnit		= state.GetValue < ZLIndex >( 3, 0 );

	self->SetTexture ( texture, textureUnit );
	return 0;
}

//----------------------------------------------------------------//
int MOAIAbstractGfxComposerInterface::_setUniform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxComposerInterface, "U" )
	
	ZLIndex globalID		= state.GetValue < ZLIndex >( 3, 0 );
	ZLIndex uniformID		= state.GetValue < ZLIndex >( 3, 0 );
	ZLIndex index			= state.GetValue < ZLIndex >( 3, 0 );
	
	self->SetUniform ( globalID, uniformID, index );
	return 0;
}

//================================================================//
// MOAIAbstractGfxComposerInterface
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractGfxComposer& MOAIAbstractGfxComposerInterface::AffirmComposer () {

	return this->MOAIAbstractGfxComposerInterface_AffirmComposer ();
}

//----------------------------------------------------------------//
void MOAIAbstractGfxComposerInterface::Call () {

	this->AffirmComposer ().MOAIAbstractGfxComposer_SubmitCommand ( MOAIGfxComposerCmdEnum::CALL, 0, 0 );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxComposerInterface::CallFromShader() {

	this->AffirmComposer ().MOAIAbstractGfxComposer_SubmitCommand ( MOAIGfxComposerCmdEnum::CALL_FROM_SHADER, 0, 0 );
}

//----------------------------------------------------------------//
MOAIAbstractGfxComposer* MOAIAbstractGfxComposerInterface::GetComposer () {

	return this->MOAIAbstractGfxComposerInterface_GetComposer ();
}

//----------------------------------------------------------------//
MOAIAbstractGfxComposerInterface::MOAIAbstractGfxComposerInterface () {
	
	RTTI_BEGIN ( MOAIAbstractGfxComposerInterface )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractGfxComposerInterface >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractGfxComposerInterface::~MOAIAbstractGfxComposerInterface () {
}

//----------------------------------------------------------------//
void MOAIAbstractGfxComposerInterface::Optimize () {

	MOAIAbstractGfxComposer* composer = this->GetComposer ();
	if ( composer ) {
		composer->Optimize ();
	}
}

//----------------------------------------------------------------//
void MOAIAbstractGfxComposerInterface::RetainObject ( ZLRefCountedObject* object ) {

	this->AffirmComposer ().MOAIAbstractGfxComposer_RetainObject ( object );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxComposerInterface::SetShader ( MOAIShader* shader ) {

	MOAIAbstractGfxComposer& composer = this->AffirmComposer ();

	MOAIGfxComposerParam::SetShader param;
	param.mShader = shader;
	composer.SubmitCommand < MOAIGfxComposerParam::SetShader >( MOAIGfxComposerCmdEnum::SET_SHADER, param );
	composer.RetainObject ( shader );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxComposerInterface::SetTexture ( MOAITexture* texture, ZLIndex textureUnit ) {

	MOAIAbstractGfxComposer& composer = this->AffirmComposer ();

	MOAIGfxComposerParam::SetTexture param;
	param.mTexture = texture;
	param.mTextureUnit = textureUnit;
	composer.SubmitCommand < MOAIGfxComposerParam::SetTexture >( MOAIGfxComposerCmdEnum::SET_TEXTURE, param );
	composer.RetainObject ( texture );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxComposerInterface::SetUniform ( ZLIndex globalID, ZLIndex uniformID, ZLIndex index ) {

	MOAIAbstractGfxComposer& composer = this->AffirmComposer ();

	MOAIGfxComposerParam::SetUniform param;
	param.mPipelineGlobalID = globalID;
	param.mTargetUniformID = uniformID;
	param.mTargetUniformIndex = index;
	composer.SubmitCommand < MOAIGfxComposerParam::SetUniform >( MOAIGfxComposerCmdEnum::SET_UNIFORM, param );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractGfxComposerInterface::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
	
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
void MOAIAbstractGfxComposerInterface::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
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
