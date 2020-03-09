// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIFrameBufferGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getGrabbedImage
	@text	Returns the image into which frame(s) will be (or were) grabbed (if any).

	@in		MOAIFrameBufferGL self
	@opt	boolean discard			If true, image will be discarded from the frame buffer.
	@out	MOAIImage image			The frame grab image, or nil if none exists.
*/
int MOAIFrameBufferGL::_getGrabbedImage ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFrameBufferGL, "U" )
	
	bool discard = state.GetValue < bool >( 2, false );
	
	self->mFrameImage.PushRef ( state );
	
	if ( discard ) {
		self->mFrameImage.Set ( *self, 0 );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	grabNextFrame
	@text	Save the next frame rendered to an image. If no image is
			provided, one will be created tp match the size of the frame
			buffer.

	@in		MOAIFrameBufferGL self
	@opt	MOAIImage image			Image to save the backbuffer to
	@opt	function callback		The function to execute when the frame has been saved into the image specified
	@out	nil
*/
int MOAIFrameBufferGL::_grabNextFrame ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFrameBufferGL, "U" )

	MOAIImage* image = state.GetLuaObject < MOAIImage >( 2, false );
	
	if ( image ) {
		self->mFrameImage.Set ( *self, image );
	}
	else if ( !self->mFrameImage ) {
	
		image = new MOAIImage ();
		image->Init ( self->mBufferWidth, self->mBufferHeight, ZLColor::RGBA_8888, MOAIImage::TRUECOLOR );
		self->mFrameImage.Set ( *self, image );
	}
	
	self->mGrabNextFrame = self->mFrameImage != 0;
	
	if ( self->mGrabNextFrame ) {
		self->mOnFrameFinish.SetRef ( *self, state, 3 );
	}
	else{
		self->mOnFrameFinish.Clear ();
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	isPendingGrab
	@text	True if a frame grab has been requested but not yet grabbed.
 
	@in		MOAIFrameBufferGL self
	@out	table renderTable
*/
int MOAIFrameBufferGL::_isPendingGrab ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFrameBufferGL, "U" )
	state.Push ( self->mGrabNextFrame );
	return 1;
}

//================================================================//
// MOAIFrameBufferGL
//================================================================//

//----------------------------------------------------------------//
void MOAIFrameBufferGL::AffirmBuffers () {

	this->MOAIFrameBufferGL_AffirmBuffers ();
}

//----------------------------------------------------------------//
void MOAIFrameBufferGL::DetectGLFrameBufferID ( MOAIGfxMgrGL& gfxMgr ) {

	this->SetGLFrameBuffer ( gfxMgr, gfxMgr.GetDrawingAPI ().GetCurrentFramebuffer ());
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
void MOAIFrameBufferGL::GrabImage ( MOAIImage* image ) {
	UNUSED ( image ); // TODO: doesn't work now?

	// TODO: all this is extremely hinky. this assumes that the framebuffer is RGBA_8888, which it
	// may not be. it also does two extra allocations and copies. what *should* happen is that we
	// grab the pixels directly into the image if the format matches, and create an extra buffer
	// only if we need to convert. we should also implement/use a mirror operation inside of MOAIImage
	// so we don't have to do it here.

//	unsigned char* buffer = ( unsigned char* ) malloc ( this->mBufferWidth * this->mBufferHeight * 4 );
//
//	zglReadPixels ( 0, 0, this->mBufferWidth, this->mBufferHeight, buffer );
//
//	//image is flipped vertically, flip it back
//	int index,indexInvert;
//	for ( u32 y = 0; y < ( this->mBufferHeight / 2 ); ++y ) {
//		for ( u32 x = 0; x < this->mBufferWidth; ++x ) {
//			for ( u32 i = 0; i < 4; ++i ) {
//
//				index = i + ( x * 4 ) + ( y * this->mBufferWidth * 4 );
//				indexInvert = i + ( x * 4 ) + (( this->mBufferHeight - 1 - y ) * this->mBufferWidth * 4 );
//
//				unsigned char temp = buffer [ indexInvert ];
//				buffer [ indexInvert ] = buffer [ index ];
//				buffer [ index ] = temp;
//			}
//		}
//	}
//
//	image->Init ( buffer, this->mBufferWidth, this->mBufferHeight, ZLColor::RGBA_8888 );
//	free ( buffer );
}

//----------------------------------------------------------------//
MOAIFrameBufferGL::MOAIFrameBufferGL () :
	mGrabNextFrame ( false ) {
	
	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAIFrameBufferGL )
		RTTI_EXTEND ( MOAIFrameBuffer )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFrameBufferGL::~MOAIFrameBufferGL () {

	this->mFrameImage.Set ( *this, 0 );
}

//----------------------------------------------------------------//
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

//----------------------------------------------------------------//
void MOAIFrameBufferGL::SetGLFrameBuffer ( MOAIGfxMgrGL& gfxMgr, const ZLGfxHandle& frameBuffer ){

	gfxMgr.DeleteOrDiscard ( this->mGLFrameBuffer, true );
	this->mGLFrameBuffer = frameBuffer;
}

////----------------------------------------------------------------//
//ZLRect MOAIFrameBufferGL::WndRectToDevice ( ZLRect rect ) const {
//
//	rect.Bless ();
//
//	if ( this->mLandscape ) {
//	
//		float width = ( float )this->mBufferWidth;
//		
//		float xMin = rect.mYMin;
//		float yMin = width - rect.mXMax;
//		float xMax = rect.mYMax;
//		float yMax = width - rect.mXMin;
//		
//		rect.mXMin = xMin;
//		rect.mYMin = yMin;
//		rect.mXMax = xMax;
//		rect.mYMax = yMax;
//	}
//	else {
//	
//		float height = ( float )this->mBufferHeight;
//		
//		float xMin = rect.mXMin;
//		float yMin = height - rect.mYMax;
//		float xMax = rect.mXMax;
//		float yMax = height - rect.mYMin;
//		
//		rect.mXMin = xMin;
//		rect.mYMin = yMin;
//		rect.mXMax = xMax;
//		rect.mYMax = yMax;
//	}
//
//	rect.Scale ( this->mBufferScale, this->mBufferScale );
//	return rect;
//}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIFrameBufferGL::MOAIFrameBufferGL_AffirmBuffers () {
}

//----------------------------------------------------------------//
void MOAIFrameBufferGL::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAIFrameBuffer, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIFrameBufferGL::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAIFrameBuffer, MOAILuaObject_RegisterLuaFuncs ( composer, state ));

	luaL_Reg regTable [] = {
		{ "getGrabbedImage",			_getGrabbedImage },
		{ "grabNextFrame",				_grabNextFrame },
		{ "isPendingGrab",				_isPendingGrab },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFrameBufferGL::ZLGfxListener_OnReadPixels ( const ZLCopyOnWrite& buffer, void * userdata ) {
	UNUSED ( userdata );

	this->mGrabNextFrame = false;
	MOAIImage* image = this->mFrameImage;
	
	if ( image ) {

		image->Init ( buffer.GetConstBuffer (), this->mBufferWidth, this->mBufferHeight, ZLColor::RGBA_8888 );

		if ( this->mOnFrameFinish ) {
			MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
			if ( this->mOnFrameFinish.PushRef ( state )) {
				this->mFrameImage.PushRef ( state );
				state.DebugCall ( 1, 0 );
			}
		}
	}
}
