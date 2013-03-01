// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIColor.h>
#include <moaicore/MOAIFrameBuffer.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIImage.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIRenderable.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	setClearColor
	@text	At the start of each frame the device will by default automatically
			render a background color.  Using this function you can set the
			background color that is drawn each frame.  If you specify no arguments
			to this function, then automatic redraw of the background color will
			be turned off (i.e. the previous render will be used as the background).

	@overload

		@in		MOAIClearableView self
		@opt	number red			The red value of the color.
		@opt	number green		The green value of the color.
		@opt	number blue			The blue value of the color.
		@opt	number alpha		The alpha value of the color.
		@out	nil
	
	@overload
		
		@in		MOAIClearableView self
		@in		MOAIColor color
		@out	nil
*/
int MOAIClearableView::_setClearColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIClearableView, "U" )
	
	MOAIColor* color = state.GetLuaObject < MOAIColor >( 2, true );
	if ( color ) {
		self->SetClearColor ( color );
		self->mClearFlags |= GL_COLOR_BUFFER_BIT;
		return 0;
	}
	
	// don't clear the color
	self->mClearFlags &= ~GL_COLOR_BUFFER_BIT;
	self->SetClearColor ( 0 );

	if ( state.GetTop () > 0 ) {
	
		float r = state.GetValue < float >( 2, 0.0f );
		float g = state.GetValue < float >( 3, 0.0f );
		float b = state.GetValue < float >( 4, 0.0f );
		float a = state.GetValue < float >( 5, 1.0f );
		
		self->mClearColor = USColor::PackRGBA ( r, g, b, a );
		self->mClearFlags |= GL_COLOR_BUFFER_BIT;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setClearDepth
	@text	At the start of each frame the buffer will by default automatically
			clear the depth buffer.  This function sets whether or not the depth
			buffer should be cleared at the start of each frame.

	@in		MOAIClearableView self
	@in		boolean clearDepth	Whether to clear the depth buffer each frame.
	@out	nil
*/
int MOAIClearableView::_setClearDepth ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIClearableView, "U" )
	
	bool clearDepth = state.GetValue < bool >( 2, false );
	
	if ( clearDepth ) {
		self->mClearFlags |= GL_DEPTH_BUFFER_BIT;
	}
	else {
		self->mClearFlags &= ~GL_DEPTH_BUFFER_BIT;
	}
	return 0;
}

//================================================================//
// MOAIFrameBuffer
//================================================================//

//----------------------------------------------------------------//
void MOAIClearableView::ClearSurface () {

	if ( this->mClearFlags & GL_COLOR_BUFFER_BIT ) {
	
		USColorVec clearColor;
		
		if ( this->mClearColorNode ) {
			clearColor = this->mClearColorNode->GetColorTrait ();
		}
		else {
			clearColor.SetRGBA ( this->mClearColor );
		}
		
		glClearColor (
			clearColor.mR,
			clearColor.mG,
			clearColor.mB,
			clearColor.mA
		);
	}

	if ( this->mClearFlags ) {
		glClear ( this->mClearFlags );
	}
}

//----------------------------------------------------------------//
MOAIClearableView::MOAIClearableView () :
	mClearFlags ( GL_COLOR_BUFFER_BIT ),
	mClearColor ( 0 ),
	mClearColorNode ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIClearableView::~MOAIClearableView () {

	this->SetClearColor ( 0 );
}

//----------------------------------------------------------------//
void MOAIClearableView::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIClearableView::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "setClearColor",				_setClearColor },
		{ "setClearDepth",				_setClearDepth },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIClearableView::SetClearColor ( MOAIColor* color ) {

	if ( this->mClearColorNode != color ) {
		this->LuaRelease ( this->mClearColorNode );
		this->LuaRetain ( color );
		this->mClearColorNode = color;
	}
}

//----------------------------------------------------------------//
void  MOAIFrameBuffer::SetGLFrameBufferID ( GLuint frameBufferId ){
  this->mGLFrameBufferID = frameBufferId;
}


//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getPerformanceDrawCount	
	@text	Returns the number of draw calls last frame.	

	@in		MOAIFrameBuffer self
	@out	number count Number of underlying graphics "draw" calls last frame.	
*/	
int MOAIFrameBuffer::_getPerformanceDrawCount ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFrameBuffer, "U" )
	lua_pushnumber ( L, self->mLastDrawCount );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getRenderTable
	@text	Returns the table currently being used for rendering.
	
	@in		MOAIFrameBuffer self
	@out	table renderTable
*/
int MOAIFrameBuffer::_getRenderTable ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFrameBuffer, "U" )
	state.Push ( self->mRenderTable );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	grabNextFrame
	@text	Save the next frame rendered to 

	@in		MOAIFrameBuffer self
	@in		MOAIImage image			Image to save the backbuffer to
	@in		function callback		The function to execute when the frame has been saved into the image specified
	@out	nil
*/
int MOAIFrameBuffer::_grabNextFrame ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFrameBuffer, "U" )

	MOAIImage* image = state.GetLuaObject < MOAIImage >( 1, true );
	if ( image ) {
		self->mFrameImage = image;
	}

	self->SetLocal ( state, 2, self->mOnFrameFinish );
	self->mGrabNextFrame = true;

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRenderTable
	@text	Sets the table to be used for rendering. This should be
			an array indexed from 1 consisting of MOAIRenderable objects
			and sub-tables. Objects will be rendered in order starting
			from index 1 and continuing until 'nil' is encountered.
	
	@in		MOAIFrameBuffer self
	@in		table renderTable
	@out	nil
*/
int MOAIFrameBuffer::_setRenderTable ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFrameBuffer, "U" )
	self->mRenderTable.SetStrongRef ( state, 2 );
	return 0;
}

//================================================================//
// MOAIFrameBuffer
//================================================================//

//----------------------------------------------------------------//
USRect MOAIFrameBuffer::GetBufferRect () const {

	USRect rect;
	rect.mXMin = 0;
	rect.mYMin = 0;
	rect.mXMax = ( float )this->mBufferWidth;
	rect.mYMax = ( float )this->mBufferHeight;
	
	return rect;
}

//----------------------------------------------------------------//
void MOAIFrameBuffer::GrabImage ( MOAIImage* image ) {

	unsigned char* buffer = ( unsigned char* ) malloc ( this->mBufferWidth * this->mBufferHeight * 4 );

	glReadPixels ( 0, 0, this->mBufferWidth, this->mBufferHeight, GL_RGBA, GL_UNSIGNED_BYTE, buffer );

	//image is flipped vertically, flip it back
	int index,indexInvert;
	for ( u32 y = 0; y < ( this->mBufferHeight / 2 ); ++y ) {
		for ( u32 x = 0; x < this->mBufferWidth; ++x ) {
			for ( u32 i = 0; i < 4; ++i ) {

				index = i + ( x * 4 ) + ( y * this->mBufferWidth * 4 );
				indexInvert = i + ( x * 4 ) + (( this->mBufferHeight - 1 - y ) * this->mBufferWidth * 4 );

				unsigned char temp = buffer [ indexInvert ];
				buffer [ indexInvert ] = buffer [ index ];
				buffer [ index ] = temp;
			}
		}
	}

	image->Init ( buffer, this->mBufferWidth, this->mBufferHeight, USColor::RGBA_8888 );
	free ( buffer );
}

//----------------------------------------------------------------//
MOAIFrameBuffer::MOAIFrameBuffer () :
	mBufferWidth ( 0 ),
	mBufferHeight ( 0 ),
	mBufferScale ( 1.0f ),
	mLandscape ( false ),
	mGLFrameBufferID ( 0 ),
	mGrabNextFrame ( false ),
	mLastDrawCount( 0 ),
	mRenderCounter ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIClearableView )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFrameBuffer::~MOAIFrameBuffer () {
}

//----------------------------------------------------------------//
void MOAIFrameBuffer::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIClearableView::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIFrameBuffer::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIClearableView::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getPerformanceDrawCount",	_getPerformanceDrawCount },
		{ "getRenderTable",				_getRenderTable },
		{ "grabNextFrame",				_grabNextFrame },
		{ "setRenderTable",				_setRenderTable },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFrameBuffer::Render () {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	this->mLastDrawCount = gfxDevice.GetDrawCount ();

	gfxDevice.SetFrameBuffer ( this );
	
	//disable scissor rect for clear
	gfxDevice.SetScissorRect ();
	this->ClearSurface ();
	
	if ( this->mRenderTable ) {
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		state.Push ( this->mRenderTable );
		this->RenderTable ( state, -1 );
		state.Pop ( 1 );
	}

	gfxDevice.Flush ();
	gfxDevice.ProcessDeleters ();

	if ( this->mGrabNextFrame ) {

		this->GrabImage ( this->mFrameImage );
		mGrabNextFrame = false;

		if ( this->mOnFrameFinish ) {
			MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
			this->PushLocal ( state, this->mOnFrameFinish );
			state.DebugCall ( 0, 0 );
		}
	}
	
	this->mRenderCounter++;
	this->mLastDrawCount = gfxDevice.GetDrawCount () - this->mLastDrawCount;
}

//----------------------------------------------------------------//
void MOAIFrameBuffer::RenderTable ( MOAILuaState& state, int idx ) {

	idx = state.AbsIndex ( idx );

	int n = 1;
	while ( n ) {
		
		lua_rawgeti ( state, idx, n++ );
		
		int valType = lua_type ( state, -1 );
			
		if ( valType == LUA_TUSERDATA ) {
			MOAIRenderable* renderable = state.GetLuaObject < MOAIRenderable >( -1, false );
			if ( renderable ) {
				renderable->Render ();
			}
		}
		else if ( valType == LUA_TTABLE ) {
			this->RenderTable ( state, -1 );
		}
		else {
			n = 0;
		}
		
		lua_pop ( state, 1 );
	}
}

//----------------------------------------------------------------//
USRect MOAIFrameBuffer::WndRectToDevice ( USRect rect ) const {

	rect.Bless ();

	if ( this->mLandscape ) {
	
		float width = ( float )this->mBufferWidth;
		
		float xMin = rect.mYMin;
		float yMin = width - rect.mXMax;
		float xMax = rect.mYMax;
		float yMax = width - rect.mXMin;
		
		rect.mXMin = xMin;
		rect.mYMin = yMin;
		rect.mXMax = xMax;
		rect.mYMax = yMax;
	}
	else {
	
		float height = ( float )this->mBufferHeight;
		
		float xMin = rect.mXMin;
		float yMin = height - rect.mYMax;
		float xMax = rect.mXMax;
		float yMax = height - rect.mYMin;
		
		rect.mXMin = xMin;
		rect.mYMin = yMin;
		rect.mXMax = xMax;
		rect.mYMax = yMax;
	}

	rect.Scale ( this->mBufferScale, this->mBufferScale );
	return rect;
}