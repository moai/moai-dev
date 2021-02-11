// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-gl/MOAIFrameBufferAttachableGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL.h>

//================================================================//
// MOAIFrameBufferAttachableGL
//================================================================//

//----------------------------------------------------------------//
bool MOAIFrameBufferAttachableGL::Attach ( ZLGfx& gfx, ZLGfxEnum::_ attachment, ZLGfxEnum::_ target, s32 level, s32 layer ) {

	return this->MOAIFrameBufferAttachableGL_Attach ( gfx, attachment, target, level, layer );
}

//----------------------------------------------------------------//
MOAIFrameBufferAttachableGL::MOAIFrameBufferAttachableGL () {
	
	RTTI_BEGIN ( MOAIFrameBufferAttachableGL )
		RTTI_EXTEND ( MOAIGfxResourceGL )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFrameBufferAttachableGL::~MOAIFrameBufferAttachableGL () {
}

//================================================================//
// virtual
//================================================================//

////----------------------------------------------------------------//
//void MOAIFrameBufferAttachableGL::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
//	if ( history.DidVisit ( *this )) return;
//}
//
////----------------------------------------------------------------//
//void MOAIFrameBufferAttachableGL::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
//	if ( history.DidVisit ( *this )) return;
//
//	luaL_Reg regTable [] = {
//		{ "init",						_init },
//		{ NULL, NULL }
//	};
//
//	luaL_register ( state, 0, regTable );
//}
//
////----------------------------------------------------------------//
//void MOAIFrameBufferAttachableGL::MOAIGfxResourceGL_OnGPUBind () {
//}
//
////----------------------------------------------------------------//
//bool MOAIFrameBufferAttachableGL::MOAIGfxResourceGL_OnGPUCreate () {
//	
//	if ( !( this->mWidth && this->mHeight && this->mPixelFormat )) {
//		return false;
//	}
//	
//	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
//	
//	if ( this->mPixelFormat ) {
//		this->mBuffer = gfx.CreateRenderbuffer ();
//		gfx.BindRenderbuffer ( this->mBuffer );
//		gfx.RenderbufferStorage ( this->mPixelFormat, this->mWidth, this->mHeight );
//	}
//	return true;
//}
//
////----------------------------------------------------------------//
//void MOAIFrameBufferAttachableGL::MOAIGfxResourceGL_OnGPUDeleteOrDiscard ( bool shouldDelete ) {
//
//	this->mGfxMgr->DeleteOrDiscard ( this->mBuffer, shouldDelete );
//}
