// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAITexture2DGL.h>
#include <moai-gfx-gl/ZLTextureFormat.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAITexture2DGL::_init ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITexture2DGL, "U" )

	self->mWidth				= state.GetValue < u32 >( 2, 0 );
	self->mHeight				= state.GetValue < u32 >( 3, 0 );
	self->mGLInternalFormat		= ( ZLGfxEnum::_ )state.GetValue < u32 >( 4, ZLGfxEnum::PIXEL_FORMAT_RGBA );
	self->mGLPixelType			= ( ZLGfxEnum::_ )state.GetValue < u32 >( 5, ZLGfxEnum::PIXEL_TYPE_UNSIGNED_BYTE );

	self->ScheduleForGPUUpdate ();

	return 0;
}

//================================================================//
// MOAITexture2DGL
//================================================================//

//----------------------------------------------------------------//
MOAITexture2DGL::MOAITexture2DGL () {

	RTTI_BEGIN ( MOAITexture2DGL )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAITexture2DGL >)
		RTTI_EXTEND ( MOAITexture2D )
		RTTI_EXTEND ( MOAITextureGL )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITexture2DGL::~MOAITexture2DGL () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAITexture2DGL::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAITexture2DGL::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "init",					_init },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
bool MOAITexture2DGL::MOAIGfxResourceGL_OnGPUCreate () {
	
	bool success = false;
	
	if ( this->mImage && this->mImage->IsOK ()) {
		success = this->CreateTextureFromImage ( *this->mImage );
	}
	else if ( this->mTextureDataFormat && this->mTextureData ) {
		success = this->mTextureDataFormat->CreateTexture ( *this, this->mTextureData, this->mTextureDataSize );
	}
	else {
		success = this->CreateTexture ();
	}
	
	if ( !success ) {
		this->Finalize ();
		return false;
	}
	
	return this->MOAIGfxResourceGL_OnGPUUpdate ();
}
