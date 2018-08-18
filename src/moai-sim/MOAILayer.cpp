// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGfxResourceClerk.h>
#include <moai-sim/MOAILayer.h>
#include <moai-sim/MOAIRenderMgr.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAILayer::_draw ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionViewLayer, "U" )

	self->Draw ( MOAIPartitionHull::NO_SUBPRIM_ID );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAILayer::_getClearMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )
	state.Push ( self->mClearMode );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAILayer::_getFrameBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )
	state.Push ( self->GetFrameBuffer ());
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAILayer::_pushRenderPass ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )
	MOAIRenderMgr::Get ().PushDrawable ( self );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setClearColor
	@text	At the start of each frame the device will by default automatically
			render a background color.  Using this function you can set the
			background color that is drawn each frame.  If you specify no arguments
			to this function, then automatic redraw of the background color will
			be turned off (i.e. the previous render will be used as the background).

	@overload

		@in		MOAILayer self
		@opt	number red			The red value of the color.
		@opt	number green		The green value of the color.
		@opt	number blue			The blue value of the color.
		@opt	number alpha		The alpha value of the color.
		@out	nil
	
	@overload
		
		@in		MOAILayer self
		@in		MOAIColor color
		@out	nil
*/
int MOAILayer::_setClearColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )
	
	MOAIColor* color = state.GetLuaObject < MOAIColor >( 2, true );
	if ( color ) {
		self->SetClearColor ( color );
		self->mClearFlags |= ZGL_CLEAR_COLOR_BUFFER_BIT;
		return 0;
	}
	
	// don't clear the color
	self->mClearFlags &= ~ZGL_CLEAR_COLOR_BUFFER_BIT;
	self->SetClearColor ( 0 );

	if ( state.GetTop () > 1 ) {
	
		float r = state.GetValue < float >( 2, 0.0f );
		float g = state.GetValue < float >( 3, 0.0f );
		float b = state.GetValue < float >( 4, 0.0f );
		float a = state.GetValue < float >( 5, 1.0f );
		
		self->mClearColor = ZLColor::PackRGBA ( r, g, b, a );
		self->mClearFlags |= ZGL_CLEAR_COLOR_BUFFER_BIT;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setClearDepth
	@text	At the start of each frame the buffer will by default automatically
			clear the depth buffer.  This function sets whether or not the depth
			buffer should be cleared at the start of each frame.

	@in		MOAILayer self
	@in		boolean clearDepth	Whether to clear the depth buffer each frame.
	@out	nil
*/
int MOAILayer::_setClearDepth ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )
	
	bool clearDepth = state.GetValue < bool >( 2, false );
	
	if ( clearDepth ) {
		self->mClearFlags |= ZGL_CLEAR_DEPTH_BUFFER_BIT;
	}
	else {
		self->mClearFlags &= ~ZGL_CLEAR_DEPTH_BUFFER_BIT;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAILayer::_setClearMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )
	self->mClearMode = state.GetValue < u32 >( 2, CLEAR_ON_BUFFER_FLAG );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAILayer::_setFrameBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )
	
	MOAIFrameBuffer* frameBuffer = state.GetLuaObject < MOAIFrameBuffer >( 2, true );
	self->SetFrameBuffer ( frameBuffer );
	return 0;
}

//================================================================//
// MOAILayer
//================================================================//

//----------------------------------------------------------------//
void MOAILayer::ClearSurface () {

	MOAIFrameBuffer* frameBuffer = this->GetFrameBuffer ();
	assert ( frameBuffer );

	if (( this->mClearMode == CLEAR_NEVER ) || (( this->mClearMode == CLEAR_ON_BUFFER_FLAG ) && ( !frameBuffer->NeedsClear ()))) return;

	MOAIGfxState& gfxState = MOAIGfxMgr::Get ().mGfxState;

	if ( this->mClearFlags & ZGL_CLEAR_COLOR_BUFFER_BIT ) {
	
		ZLColorVec clearColor;
		
		if ( this->mClearColorNode ) {
			clearColor = this->mClearColorNode->GetColorTrait ();
		}
		else {
			clearColor.SetRGBA ( this->mClearColor );
		}
		
		gfxState.SetClearColor ( clearColor );
	}
	
	gfxState.SetClearFlags ( this->mClearFlags );
	MOAIGfxMgr::Get ().mGfxState.ClearSurface ();
	
	frameBuffer->NeedsClear ( false );
}

//----------------------------------------------------------------//
MOAIFrameBuffer* MOAILayer::GetFrameBuffer () {

	return this->mFrameBuffer ? this->mFrameBuffer : MOAIGfxMgr::Get ().mGfxState.GetDefaultFrameBuffer ();
}

//----------------------------------------------------------------//
MOAILayer::MOAILayer () :
	mClearFlags ( ZGL_CLEAR_COLOR_BUFFER_BIT | ZGL_CLEAR_DEPTH_BUFFER_BIT ),
	mClearColor ( ZLColor::PackRGBA ( 0.0f, 0.0f, 0.0f, 1.0f )),
	mClearMode ( CLEAR_ON_BUFFER_FLAG ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
		RTTI_EXTEND ( MOAIDrawable )
	RTTI_END
}

//----------------------------------------------------------------//
MOAILayer::~MOAILayer () {

	this->mClearColorNode.Set ( *this, 0 );
	this->mFrameBuffer.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAILayer::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "CLEAR_ALWAYS",			( u32 )CLEAR_ALWAYS );
	state.SetField ( -1, "CLEAR_NEVER",				( u32 )CLEAR_NEVER );
	state.SetField ( -1, "CLEAR_ON_BUFFER_FLAG",	( u32 )CLEAR_ON_BUFFER_FLAG );
}

//----------------------------------------------------------------//
void MOAILayer::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "draw",						_draw },
		{ "getClearMode",				_getClearMode },
		{ "getFrameBuffer",				_getFrameBuffer },
		{ "pushRenderPass",				_pushRenderPass },
		{ "setClearColor",				_setClearColor },
		{ "setClearDepth",				_setClearDepth },
		{ "setClearMode",				_setClearMode },
		{ "setFrameBuffer",				_setFrameBuffer },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAILayer::SetClearColor ( MOAIColor* color ) {

	if ( this->mClearColorNode != color ) {
		this->mClearColorNode.Set ( *this, color );
	}
}

//----------------------------------------------------------------//
void MOAILayer::SetFrameBuffer ( MOAIFrameBuffer* frameBuffer ) {

	//this->mFrameBuffer.Set ( *this, frameBuffer ? frameBuffer : MOAIGfxMgr::Get ().mGfxState.GetDefaultFrameBuffer ());
	this->mFrameBuffer.Set ( *this, frameBuffer );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
void MOAILayer::MOAIDrawable_Draw ( int subPrimID ) {
	UNUSED ( subPrimID );

	MOAIGfxState& gfxState = MOAIGfxMgr::Get ().mGfxState;

	gfxState.SetFrameBuffer ( this->GetFrameBuffer ());
	
	//disable scissor rect for clear
	gfxState.SetScissorRect ();
	this->ClearSurface ();
}
