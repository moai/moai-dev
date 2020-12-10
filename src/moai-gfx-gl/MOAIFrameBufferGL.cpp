// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIFrameBufferGL.h>
#include <moai-gfx-gl/MOAIFrameBufferAttachmentGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIFrameBufferGL::_getAttachment ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFrameBufferGL, "U" )
	
	if ( self->mColorAttachment ) {
		state.Push (( MOAILuaObject* )self->mColorAttachment );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIFrameBufferGL::_setAttachment ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFrameBufferGL, "U" )
	
	MOAIFrameBufferAttachmentGL* attachment = state.GetLuaObject < MOAIFrameBufferAttachmentGL >( 2, false );
	self->mColorAttachment = attachment;
	self->ScheduleForGPUUpdate ();

	MOAI_LUA_RETURN_SELF
}

//================================================================//
// MOAIFrameBufferGL
//================================================================//

//----------------------------------------------------------------//
void MOAIFrameBufferGL::DetectGLFrameBufferID ( MOAIGfxMgrGL& gfxMgr ) {

	this->SetGLFrameBuffer ( gfxMgr, gfxMgr.GetDrawingAPI ().GetCurrentFramebuffer ());
	this->mState = STATE_READY_TO_BIND;
	this->mIsDefaultBuffer = true;
}

//----------------------------------------------------------------//
ZLRect MOAIFrameBufferGL::GetBufferRect () const {

	ZLRect rect;
	rect.mXMin = 0;
	rect.mYMin = 0;
	rect.mXMax = ( float )this->mBufferWidth;
	rect.mYMax = ( float )this->mBufferHeight;
	
	return rect;
}

//----------------------------------------------------------------//
MOAIFrameBufferGL::MOAIFrameBufferGL () :
	mIsDefaultBuffer ( false ) {
	
	RTTI_BEGIN ( MOAIFrameBufferGL )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIFrameBufferGL >)
		RTTI_EXTEND ( MOAIFrameBuffer )
		RTTI_EXTEND ( MOAIPoolableObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFrameBufferGL::~MOAIFrameBufferGL () {

	if ( !this->mIsDefaultBuffer ) {
		this->ScheduleForGPUDestroy ();
	}
}

//----------------------------------------------------------------//
void MOAIFrameBufferGL::SetGLFrameBuffer ( MOAIGfxMgrGL& gfxMgr, const ZLGfxHandle& frameBuffer ){

	gfxMgr.DeleteOrDiscard ( this->mGLFrameBuffer, true );
	this->mGLFrameBuffer = frameBuffer;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIFrameBufferGL::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIFrameBufferGL::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getAttachment",				_getAttachment },
		{ "setAttachment",				_setAttachment },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFrameBufferGL::MOAIGfxResourceGL_OnGPUBind () {

	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
	gfx.BindFramebuffer ( ZLGfxEnum::FRAMEBUFFER_TARGET_DRAW_READ, this->mGLFrameBuffer );
	
	if ( this->mIsDefaultBuffer ) return;
	
	if ( this->mColorAttachment ) {
		this->mColorAttachment->Attach ( gfx, ZLGfxEnum::FRAMEBUFFER_TARGET_DRAW_READ, ZLGfxEnum::FRAMEBUFFER_ATTACHMENT_COLOR, 0, 0 );
	}
	
	if ( this->mDepthAttachment ) {
		this->mColorAttachment->Attach ( gfx, ZLGfxEnum::FRAMEBUFFER_TARGET_DRAW_READ, ZLGfxEnum::FRAMEBUFFER_ATTACHMENT_DEPTH, 0, 0 );
	}
	
	if ( this->mStencilAttachment ) {
		this->mColorAttachment->Attach ( gfx, ZLGfxEnum::FRAMEBUFFER_TARGET_DRAW_READ, ZLGfxEnum::FRAMEBUFFER_ATTACHMENT_STENCIL, 0, 0 );
	}
	
	// TODO: check buffers OK?
//	gfx.CheckFramebufferStatus ( ZLGfxEnum::FRAMEBUFFER_TARGET_DRAW_READ );
//	bool status = gfx.PushSuccessHandler ();
//	gfx.PopSection ();
}

//----------------------------------------------------------------//
bool MOAIFrameBufferGL::MOAIGfxResourceGL_OnGPUCreate () {
	
	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();

	if ( !( this->mGLFrameBuffer || this->mIsDefaultBuffer )) {
		this->mGLFrameBuffer = gfx.CreateFramebuffer ();
	}
	return true;
}

//----------------------------------------------------------------//
void MOAIFrameBufferGL::MOAIGfxResourceGL_OnGPUDeleteOrDiscard ( bool shouldDelete ) {

	if ( !this->mIsDefaultBuffer ) {
		this->mGfxMgr->DeleteOrDiscard ( this->mGLFrameBuffer, shouldDelete );
	}
}

//----------------------------------------------------------------//
void MOAIFrameBufferGL::MOAIGfxResourceGL_OnGPUUnbind () {
}

//----------------------------------------------------------------//
bool MOAIFrameBufferGL::MOAIGfxResourceGL_OnGPUUpdate () {

	return true;
}

//----------------------------------------------------------------//
void MOAIFrameBufferGL::MOAIRenderResource_OnRemit () {

	MOAIPool::ReleaseIfPooled < MOAIFrameBufferAttachmentGL >( this->mColorAttachment );
	MOAIPool::ReleaseIfPooled < MOAIFrameBufferAttachmentGL >( this->mDepthAttachment );
	MOAIPool::ReleaseIfPooled < MOAIFrameBufferAttachmentGL >( this->mStencilAttachment );
}

//================================================================//
// TODO: this crap needs to be moved someplace sensible (probably MOAIDraw) and generalized for framebuffers and textures
//================================================================//

////----------------------------------------------------------------//
///**	@lua	getGrabbedImage
//	@text	Returns the image into which frame(s) will be (or were) grabbed (if any).
//
//	@in		MOAIFrameBufferGL self
//	@opt	boolean discard			If true, image will be discarded from the frame buffer.
//	@out	MOAIImage image			The frame grab image, or nil if none exists.
//*/
//int MOAIFrameBufferGL::_getGrabbedImage ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIFrameBufferGL, "U" )
//
//	bool discard = state.GetValue < bool >( 2, false );
//
//	self->mFrameImage.PushRef ( state );
//
//	if ( discard ) {
//		self->mFrameImage.Set ( *self, 0 );
//	}
//
//	return 1;
//}
//
////----------------------------------------------------------------//
///**	@lua	grabNextFrame
//	@text	Save the next frame rendered to an image. If no image is
//			provided, one will be created tp match the size of the frame
//			buffer.
//
//	@in		MOAIFrameBufferGL self
//	@opt	MOAIImage image			Image to save the backbuffer to
//	@opt	function callback		The function to execute when the frame has been saved into the image specified
//	@out	nil
//*/
//int MOAIFrameBufferGL::_grabNextFrame ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIFrameBufferGL, "U" )
//
//	MOAIImage* image = state.GetLuaObject < MOAIImage >( 2, false );
//
//	if ( image ) {
//		self->mFrameImage.Set ( *self, image );
//	}
//	else if ( !self->mFrameImage ) {
//
//		image = new MOAIImage ();
//		image->Init ( self->mBufferWidth, self->mBufferHeight, ZLColor::RGBA_8888, MOAIImage::TRUECOLOR );
//		self->mFrameImage.Set ( *self, image );
//	}
//
//	self->mGrabNextFrame = self->mFrameImage != 0;
//
//	if ( self->mGrabNextFrame ) {
//		self->mOnFrameFinish.SetRef ( *self, state, 3 );
//	}
//	else{
//		self->mOnFrameFinish.Clear ();
//	}
//
//	return 0;
//}
//
////----------------------------------------------------------------//
///**	@lua	isPendingGrab
//	@text	True if a frame grab has been requested but not yet grabbed.
//
//	@in		MOAIFrameBufferGL self
//	@out	table renderTable
//*/
//int MOAIFrameBufferGL::_isPendingGrab ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIFrameBufferGL, "U" )
//	state.Push ( self->mGrabNextFrame );
//	return 1;
//}

//	bool								mGrabNextFrame;
//	MOAILuaMemberRef					mOnFrameFinish;
//	MOAILuaSharedPtr < MOAIImage >		mFrameImage;

////----------------------------------------------------------------//
//void MOAIFrameBufferGL::GrabImage ( MOAIImage* image ) {
//	UNUSED ( image ); // TODO: doesn't work now?
//
//	// TODO: all this is extremely hinky. this assumes that the framebuffer is RGBA_8888, which it
//	// may not be. it also does two extra allocations and copies. what *should* happen is that we
//	// grab the pixels directly into the image if the format matches, and create an extra buffer
//	// only if we need to convert. we should also implement/use a mirror operation inside of MOAIImage
//	// so we don't have to do it here.
//
////	unsigned char* buffer = ( unsigned char* ) malloc ( this->mBufferWidth * this->mBufferHeight * 4 );
////
////	zglReadPixels ( 0, 0, this->mBufferWidth, this->mBufferHeight, buffer );
////
////	//image is flipped vertically, flip it back
////	int index,indexInvert;
////	for ( u32 y = 0; y < ( this->mBufferHeight / 2 ); ++y ) {
////		for ( u32 x = 0; x < this->mBufferWidth; ++x ) {
////			for ( u32 i = 0; i < 4; ++i ) {
////
////				index = i + ( x * 4 ) + ( y * this->mBufferWidth * 4 );
////				indexInvert = i + ( x * 4 ) + (( this->mBufferHeight - 1 - y ) * this->mBufferWidth * 4 );
////
////				unsigned char temp = buffer [ indexInvert ];
////				buffer [ indexInvert ] = buffer [ index ];
////				buffer [ index ] = temp;
////			}
////		}
////	}
////
////	image->Init ( buffer, this->mBufferWidth, this->mBufferHeight, ZLColor::RGBA_8888 );
////	free ( buffer );
//}

////----------------------------------------------------------------//
//void MOAIFrameBufferGL::Render () {
//
//	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
//	//this->mLastDrawCount = gfxMgr.GetDrawCount ();
//
//	gfxState.SetFrameBuffer ( this );
//
//	//disable scissor rect for clear
//	gfxState.SetScissorRect ();
//	this->ClearSurface ();
//
//	if ( this->mRenderTable ) {
//		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
//		state.Push ( this->mRenderTable );
//		this->RenderTable ( state, -1 );
//		state.Pop ( 1 );
//	}
//
//	gfxState.FlushVertexCache (); // do we need to do this if we aren't reading pixels?
//
//	// since we're doing this on the render thread, set it every time until we get a callback
//	if ( this->mGrabNextFrame ) {
//
//		// remember, if we're not in immediate mode, this schedules a pixel read for later
//		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
//		gfx.ReadPixels ( 0, 0, this->mBufferWidth, this->mBufferHeight, ZLGfxEnum::PIXEL_FORMAT_RGBA, ZLGfxEnum::PIXEL_TYPE_UNSIGNED_BYTE, 4, this, 0 );
//	}
//
//	this->mRenderCounter++;
//	//this->mLastDrawCount = gfxMgr.GetDrawCount () - this->mLastDrawCount;
//}

////----------------------------------------------------------------//
//void MOAIFrameBufferGL::ZLGfxListener_OnReadPixels ( const ZLCopyOnWrite& buffer, void * userdata ) {
//	UNUSED ( userdata );
//
//	this->mGrabNextFrame = false;
//	MOAIImage* image = this->mFrameImage;
//
//	if ( image ) {
//
//		image->Init ( buffer.GetConstBuffer (), this->mBufferWidth, this->mBufferHeight, ZLColor::RGBA_8888 );
//
//		if ( this->mOnFrameFinish ) {
//			MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
//			if ( this->mOnFrameFinish.PushRef ( state )) {
//				this->mFrameImage.PushRef ( state );
//				state.DebugCall ( 1, 0 );
//			}
//		}
//	}
//}
