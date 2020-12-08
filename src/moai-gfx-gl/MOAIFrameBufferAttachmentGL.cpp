// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-gl/MOAIFrameBufferAttachmentGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL.h>

//================================================================//
// MOAIFrameBufferAttachmentGL
//================================================================//

//----------------------------------------------------------------//
bool MOAIFrameBufferAttachmentGL::Attach ( ZLGfx& gfx, ZLGfxEnum::_ target, ZLGfxEnum::_ attachment, s32 level, s32 layer ) {

	return this->MOAIFrameBufferAttachmentGL_Attach ( gfx, target, attachment, level, layer );
}

//----------------------------------------------------------------//
MOAIFrameBufferAttachmentGL::MOAIFrameBufferAttachmentGL () {
	
	RTTI_BEGIN ( MOAIFrameBufferAttachmentGL )
		RTTI_EXTEND ( MOAIRenderResource )
		RTTI_EXTEND ( MOAIGfxResourceGL )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFrameBufferAttachmentGL::~MOAIFrameBufferAttachmentGL () {
}

//================================================================//
// virtual
//================================================================//

////----------------------------------------------------------------//
//void MOAIFrameBufferAttachmentGL::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
//	if ( history.DidVisit ( *this )) return;
//}
//
////----------------------------------------------------------------//
//void MOAIFrameBufferAttachmentGL::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
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
//void MOAIFrameBufferAttachmentGL::MOAIGfxResourceGL_OnGPUBind () {
//}
//
////----------------------------------------------------------------//
//bool MOAIFrameBufferAttachmentGL::MOAIGfxResourceGL_OnGPUCreate () {
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
//void MOAIFrameBufferAttachmentGL::MOAIGfxResourceGL_OnGPUDeleteOrDiscard ( bool shouldDelete ) {
//
//	this->mGfxMgr->DeleteOrDiscard ( this->mBuffer, shouldDelete );
//}
