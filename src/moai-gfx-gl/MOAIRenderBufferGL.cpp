// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-gl/MOAIGfxConstsGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL.h>
#include <moai-gfx-gl/MOAIRenderBufferGL.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRenderBufferGL::_init ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRenderBufferGL, "U" )

	self->mWidth				= state.GetValue < u32 >( 2, 0 );
	self->mHeight				= state.GetValue < u32 >( 3, 0 );
	self->mGLPixelFormat		= ( ZLGfxEnum::_ )state.GetValue < u32 >( 4, ZLGfxEnum::DEFAULT_RGBA_PIXEL_FORMAT );

	self->ScheduleForGPUUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getSize
	@text	Returns the width and height of the texture's source image.
			Avoid using the texture width and height to compute UV
			coordinates from pixels, as this will prevent texture
			resolution swapping.
 
	@in		MOAIRenderBufferGL self
	@out	number width
	@out	number height
*/
int MOAIRenderBufferGL::_getSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRenderBufferGL, "U" )
		
	lua_pushnumber ( state, self->mWidth );
	lua_pushnumber ( state, self->mHeight );
	
	return 2;
}

//================================================================//
// MOAIRenderBufferGL
//================================================================//

//----------------------------------------------------------------//
MOAIRenderBufferGL::MOAIRenderBufferGL () :
	mWidth ( 0 ),
	mHeight ( 0 ),
	mGLPixelFormat ( ZLGfxEnum::NONE ) {
	
	RTTI_BEGIN ( MOAIRenderBufferGL )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIRenderBufferGL >)
		RTTI_EXTEND ( MOAIFrameBufferAttachmentGL )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIRenderBufferGL::~MOAIRenderBufferGL () {

	this->ScheduleForGPUDestroy ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIRenderBufferGL::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIRenderBufferGL::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "init",					_init },
		{ "getSize",				_getSize },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
bool MOAIRenderBufferGL::MOAIFrameBufferAttachmentGL_Attach ( ZLGfx& gfx, ZLGfxEnum::_ target, ZLGfxEnum::_ attachment, s32 level, s32 layer ) {
	UNUSED ( layer );

	gfx.FramebufferRenderbuffer ( target, attachment, this->mGLRenderBuffer );
	return true;
}

//----------------------------------------------------------------//
bool MOAIRenderBufferGL::MOAIGfxResourceGL_OnGPUCreate () {
	
	if ( this->mGLPixelFormat && this->mWidth && this->mHeight ) {
		
		ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
	
		this->mGLRenderBuffer = gfx.CreateRenderbuffer ();
		gfx.BindRenderbuffer ( this->mGLRenderBuffer );
		gfx.RenderbufferStorage ( this->mGLPixelFormat, this->mWidth, this->mHeight );
	}
	return true;
}

//----------------------------------------------------------------//
void MOAIRenderBufferGL::MOAIGfxResourceGL_OnGPUBind () {
}

//----------------------------------------------------------------//
void MOAIRenderBufferGL::MOAIGfxResourceGL_OnGPUDeleteOrDiscard ( bool shouldDelete ) {

	this->mGfxMgr->DeleteOrDiscard ( this->mGLRenderBuffer, shouldDelete );
}

//----------------------------------------------------------------//
void MOAIRenderBufferGL::MOAIGfxResourceGL_OnGPUUnbind () {
}

//----------------------------------------------------------------//
bool MOAIRenderBufferGL::MOAIGfxResourceGL_OnGPUUpdate () {
	return true;
}
