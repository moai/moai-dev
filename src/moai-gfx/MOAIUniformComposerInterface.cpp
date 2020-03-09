// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIMaterialMgr.h>
#include <moai-gfx/MOAIUniformComposer.h>
#include <moai-gfx/MOAIUniformComposerInterface.h>
#include <moai-gfx/MOAIUniformDescriptor.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIUniformComposerInterface::_reserveGlobals ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUniformComposerInterface, "U" )
	
	u32 nGlobals = state.GetValue < u32 >( 2, 0 );
	self->ReserveGlobals ( nGlobals );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIUniformComposerInterface::_reserveTextures ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUniformComposerInterface, "U" )

	self->ReserveTextures ( state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIUniformComposerInterface::_setGlobal ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUniformComposerInterface, "UNNN" )

	ZLIndex globalIdx	= state.GetValue < MOAILuaIndex >( 2, 0 );
	u32 globalID		= state.GetValue < u32 >( 3, MOAIGfxMgr::NULL_GLOBAL );
	ZLIndex uniformID	= state.GetValue < MOAILuaIndex >( 4, 0 );
	ZLIndex index		= state.GetValue < MOAILuaIndex >( 5, 0 );
	
	self->SetGlobal ( globalIdx, globalID, uniformID, index );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIUniformComposerInterface::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUniformComposerInterface, "U" )
	
	ZLIndex idx		= state.GetValue < MOAILuaIndex >( 2, 0 );
	ZLIndex unit	= state.GetValue < MOAILuaIndex >( 3, 0 );
	
	if ( state.IsType ( 3, LUA_TUSERDATA )) {
	
		self->SetTexture ( idx, state.GetLuaObject < MOAITexture >( 3, true ), unit );
	}
	else {
	
		self->SetTexture (
			idx,
			state.GetValue < u32 >( 3, MOAIMaterialGlobals::MOAI_UNKNOWN_MATERIAL_GLOBAL + 1 ) - 1,
			unit,
			state.GetLuaObject < MOAITexture >( 5, false )
		);
	}
	return 0;
}

//================================================================//
// MOAIUniformComposerInterface
//================================================================//

//----------------------------------------------------------------//
MOAIUniformComposer& MOAIUniformComposerInterface::AffirmComposer () {

	return this->MOAIUniformComposerInterface_AffirmComposer ();
}

//----------------------------------------------------------------//
MOAIUniformComposer* MOAIUniformComposerInterface::GetComposer () {

	return this->MOAIUniformComposerInterface_GetComposer ();
}

//----------------------------------------------------------------//
MOAIUniformComposerInterface::MOAIUniformComposerInterface () {
	
	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAIUniformComposerInterface )
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIUniformComposerInterface::~MOAIUniformComposerInterface () {
}

//----------------------------------------------------------------//
void MOAIUniformComposerInterface::ReserveGlobals ( ZLSize nGlobals ) {

	MOAIUniformComposer& composer = this->AffirmComposer ();
	composer.mGlobals.Init ( nGlobals );
}

//----------------------------------------------------------------//
void MOAIUniformComposerInterface::ReserveTextures ( ZLSize nTextures ) {

	MOAIUniformComposer& composer = this->AffirmComposer ();
	composer.mTextures.Init ( nTextures );
}

//----------------------------------------------------------------//
void MOAIUniformComposerInterface::SetGlobal ( ZLIndex idx, u32 globalID, ZLIndex uniformID, ZLIndex index ) {
	
	MOAIUniformComposer& composer = this->AffirmComposer ();
	
	if ( idx < composer.mGlobals.Size ()) {
	
		MOAIUniformComposerGlobalRef& global = composer.mGlobals [ idx ];
		
		global.mUniformID	= uniformID;
		global.mIndex		= index;
		global.mGlobalID	= globalID;
	}
}

//----------------------------------------------------------------//
void MOAIUniformComposerInterface::SetTexture ( ZLIndex idx, u32 name, ZLIndex unit, MOAITexture* fallback ) {

	MOAIUniformComposer& composer = this->AffirmComposer ();

	if ( idx < composer.mTextures.Size ()) {
	
		MOAIUniformComposerTextureRef& shaderTexture = composer.mTextures [ idx ];
		
		shaderTexture.mName = name;
		shaderTexture.mUnit = unit;
		shaderTexture.mTexture = fallback;
	}
}

//----------------------------------------------------------------//
void MOAIUniformComposerInterface::SetTexture ( ZLIndex idx, MOAITexture* texture, ZLIndex unit ) {

	MOAIUniformComposer& composer = this->AffirmComposer ();

	if ( idx < composer.mTextures.Size ()) {
	
		MOAIUniformComposerTextureRef& shaderTexture = composer.mTextures [ idx ];
		
		shaderTexture.mName = MOAIMaterialGlobals::MOAI_UNKNOWN_MATERIAL_GLOBAL;
		shaderTexture.mUnit = unit;
		shaderTexture.mTexture = texture;
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIUniformComposerInterface::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	
	state.SetField ( -1, "UNIFORM_WIDTH_VEC_2",						( u32 )MOAIUniformDescriptor::UNIFORM_WIDTH_VEC_2 );
	state.SetField ( -1, "UNIFORM_WIDTH_VEC_3",						( u32 )MOAIUniformDescriptor::UNIFORM_WIDTH_VEC_3 );
	state.SetField ( -1, "UNIFORM_WIDTH_VEC_4",						( u32 )MOAIUniformDescriptor::UNIFORM_WIDTH_VEC_4 );
	state.SetField ( -1, "UNIFORM_WIDTH_MATRIX_3X3",				( u32 )MOAIUniformDescriptor::UNIFORM_WIDTH_MATRIX_3X3 );
	state.SetField ( -1, "UNIFORM_WIDTH_MATRIX_4X4",				( u32 )MOAIUniformDescriptor::UNIFORM_WIDTH_MATRIX_4X4 );
	
	state.SetField ( -1, "GLOBAL_CLIP_TO_DISPLAY_MTX",				( u32 )MOAIGfxMgr::CLIP_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_CLIP_TO_MODEL_MTX",				( u32 )MOAIGfxMgr::CLIP_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_CLIP_TO_VIEW_MTX",					( u32 )MOAIGfxMgr::CLIP_TO_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_CLIP_TO_WINDOW_MTX",				( u32 )MOAIGfxMgr::CLIP_TO_WINDOW_MTX );
	state.SetField ( -1, "GLOBAL_CLIP_TO_WORLD_MTX",				( u32 )MOAIGfxMgr::CLIP_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_DISPLAY_TO_CLIP_MTX",				( u32 )MOAIGfxMgr::DISPLAY_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_DISPLAY_TO_MODEL_MTX",				( u32 )MOAIGfxMgr::DISPLAY_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_DISPLAY_TO_VIEW_MTX",				( u32 )MOAIGfxMgr::DISPLAY_TO_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_DISPLAY_TO_WORLD_MTX",				( u32 )MOAIGfxMgr::DISPLAY_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_MODEL_TO_CLIP_MTX",				( u32 )MOAIGfxMgr::MODEL_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_MODEL_TO_DISPLAY_MTX",				( u32 )MOAIGfxMgr::MODEL_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_MODEL_TO_UV_MTX",					( u32 )MOAIGfxMgr::MODEL_TO_UV_MTX );
	state.SetField ( -1, "GLOBAL_MODEL_TO_VIEW_MTX",				( u32 )MOAIGfxMgr::MODEL_TO_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_MODEL_TO_WORLD_MTX",				( u32 )MOAIGfxMgr::MODEL_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_DISPLAY_MTX",		( u32 )MOAIGfxMgr::NORMAL_CLIP_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_MODEL_MTX",			( u32 )MOAIGfxMgr::NORMAL_CLIP_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_VIEW_MTX",			( u32 )MOAIGfxMgr::NORMAL_CLIP_TO_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_WINDOW_MTX",		( u32 )MOAIGfxMgr::NORMAL_CLIP_TO_WINDOW_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_WORLD_MTX",			( u32 )MOAIGfxMgr::NORMAL_CLIP_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_DISPLAY_TO_CLIP_MTX",		( u32 )MOAIGfxMgr::NORMAL_DISPLAY_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_DISPLAY_TO_MODEL_MTX",		( u32 )MOAIGfxMgr::NORMAL_DISPLAY_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_DISPLAY_TO_VIEW_MTX",		( u32 )MOAIGfxMgr::NORMAL_DISPLAY_TO_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_DISPLAY_TO_WORLD_MTX",		( u32 )MOAIGfxMgr::NORMAL_DISPLAY_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_CLIP_MTX",			( u32 )MOAIGfxMgr::NORMAL_MODEL_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_DISPLAY_MTX",		( u32 )MOAIGfxMgr::NORMAL_MODEL_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_UV_MTX",			( u32 )MOAIGfxMgr::NORMAL_MODEL_TO_UV_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_VIEW_MTX",			( u32 )MOAIGfxMgr::NORMAL_MODEL_TO_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_WORLD_MTX",		( u32 )MOAIGfxMgr::NORMAL_MODEL_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_WORLD_TO_DISPLAY_MTX",		( u32 )MOAIGfxMgr::NORMAL_WORLD_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_WORLD_TO_VIEW_MTX",			( u32 )MOAIGfxMgr::NORMAL_WORLD_TO_VIEW_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_UV_TO_MODEL_MTX",			( u32 )MOAIGfxMgr::NORMAL_UV_TO_MODEL_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_VIEW_TO_CLIP_MTX",			( u32 )MOAIGfxMgr::NORMAL_VIEW_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_VIEW_TO_DISPLAY_MTX",		( u32 )MOAIGfxMgr::NORMAL_VIEW_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_VIEW_TO_MODEL_MTX",			( u32 )MOAIGfxMgr::NORMAL_VIEW_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_VIEW_TO_WORLD_MTX",			( u32 )MOAIGfxMgr::NORMAL_VIEW_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_WINDOW_TO_CLIP_MTX",		( u32 )MOAIGfxMgr::NORMAL_WINDOW_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_WORLD_TO_CLIP_MTX",			( u32 )MOAIGfxMgr::NORMAL_WORLD_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_WORLD_TO_MODEL_MTX",		( u32 )MOAIGfxMgr::NORMAL_WORLD_TO_MODEL_MTX );

	state.SetField ( -1, "GLOBAL_PEN_COLOR",						( u32 )MOAIGfxMgr::PEN_COLOR );

	state.SetField ( -1, "GLOBAL_UV_TO_MODEL_MTX",					( u32 )MOAIGfxMgr::UV_TO_MODEL_MTX );

	state.SetField ( -1, "GLOBAL_VIEW_TO_CLIP_MTX",					( u32 )MOAIGfxMgr::VIEW_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_VIEW_TO_DISPLAY_MTX",				( u32 )MOAIGfxMgr::VIEW_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_VIEW_TO_MODEL_MTX",				( u32 )MOAIGfxMgr::VIEW_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_VIEW_TO_WORLD_MTX",				( u32 )MOAIGfxMgr::VIEW_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_WINDOW_TO_CLIP_MTX",				( u32 )MOAIGfxMgr::WINDOW_TO_CLIP_MTX );

	state.SetField ( -1, "GLOBAL_WORLD_TO_CLIP_MTX",				( u32 )MOAIGfxMgr::WORLD_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_WORLD_TO_DISPLAY_MTX",				( u32 )MOAIGfxMgr::WORLD_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_WORLD_TO_MODEL_MTX",				( u32 )MOAIGfxMgr::WORLD_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_WORLD_TO_VIEW_MTX",				( u32 )MOAIGfxMgr::WORLD_TO_VIEW_MTX );

	state.SetField ( -1, "GLOBAL_VIEW_HEIGHT",						( u32 )MOAIGfxMgr::VIEW_HEIGHT );
	state.SetField ( -1, "GLOBAL_VIEW_WIDTH",						( u32 )MOAIGfxMgr::VIEW_WIDTH );
	
	state.SetField ( -1, "GLOBAL_VIEW_HALF_HEIGHT",					( u32 )MOAIGfxMgr::VIEW_HALF_HEIGHT );
	state.SetField ( -1, "GLOBAL_VIEW_HALF_WIDTH",					( u32 )MOAIGfxMgr::VIEW_HALF_WIDTH );
}

//----------------------------------------------------------------//
void MOAIUniformComposerInterface::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "reserveGlobals",				_reserveGlobals },
		{ "reserveTextures",			_reserveTextures },
		{ "setGlobal",					_setGlobal },
		{ "setTexture",					_setTexture },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}
