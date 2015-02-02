// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGfxResourceMgr.h>
#include <moai-sim/MOAIRenderMgr.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRenderMgr::_getBufferTable ( lua_State* L ) {
	MOAILuaState state ( L );
	state.Push ( MOAIRenderMgr::Get ().mBufferTable );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getPerformanceDrawCount	
	@text	Returns the number of draw calls last frame.	

	@out	number count		Number of underlying graphics "draw" calls last frame.	
*/	
int MOAIRenderMgr::_getPerformanceDrawCount ( lua_State* L ) {

	MOAIRenderMgr& device = MOAIRenderMgr::Get ();
	lua_pushnumber ( L, device.mLastDrawCount );

	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRenderMgr::_setBufferTable ( lua_State* L ) {
	MOAILuaState state ( L );
	MOAIRenderMgr::Get ().mBufferTable.SetRef ( state, 1 );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRenderMgr::_getRenderCount ( lua_State* L ) {

	MOAIRenderMgr& device = MOAIRenderMgr::Get ();
	lua_pushnumber ( L, device.mRenderCounter );

	return 1;
}

//================================================================//
// DOXYGEN
//================================================================//

#ifdef DOXYGEN

	//----------------------------------------------------------------//
	/**	@lua	clearRenderStack
		@text	Sets the render stack to nil. THIS METHOD
				IS DEPRECATED AND WILL BE REMOVED IN A FUTURE RELEASE.

		@out	nil
	*/
	int MOAIRenderMgr::_clearRenderStack ( lua_State* L ) {
	}
	
	//----------------------------------------------------------------//
	/**	@lua	getRenderTable
		@text	Returns the table currently being used for rendering.
		
		@out	table renderTable
	*/
	int MOAIRenderMgr::_getRenderTable ( lua_State* L ) {
	}

	//----------------------------------------------------------------//
	/**	@lua	grabNextFrame
		@text	Save the next frame rendered to 

		@in		MOAIImage image			Image to save the backbuffer to
		@in		function callback		The function to execute when the frame has been saved into the image specified

		@out	table renderTable
	*/
	int MOAIRenderMgr::_grabNextFrame ( lua_State* L ) {
	}
	
	//----------------------------------------------------------------//
	/**	@lua	popRenderPass
		@text	Pops the top renderable from the render stack. THIS METHOD
				IS DEPRECATED AND WILL BE REMOVED IN A FUTURE RELEASE.

		@out	nil
	*/
	int MOAIRenderMgr::_popRenderPass ( lua_State* L ) {
	}
	
	//----------------------------------------------------------------//
	/**	@lua	pushRenderPass
		@text	Pushes a renderable onto the render stack. THIS METHOD
				IS DEPRECATED AND WILL BE REMOVED IN A FUTURE RELEASE.

		@in		MOAIRenderable renderable
		@out	nil
	*/
	int MOAIRenderMgr::_pushRenderPass ( lua_State* L ) {
	}
	
	//----------------------------------------------------------------//
	/**	@lua	removeRenderPass
		@text	Removes a renderable from the render stack. THIS METHOD
				IS DEPRECATED AND WILL BE REMOVED IN A FUTURE RELEASE.
				Superseded by setRenderTable.

		@in		MOAIRenderable renderable
		@out	nil
	*/
	int MOAIRenderMgr::_removeRenderPass ( lua_State* L ) {
	}

	//----------------------------------------------------------------//
	/**	@lua	setRenderTable
		@text	Sets the table to be used for rendering. This should be
				an array indexed from 1 consisting of MOAIRenderable objects
				and sub-tables. Objects will be rendered in order starting
				from index 1 and continuing until 'nil' is encountered.
		
		@in		table renderTable
		@out	nil
	*/
	int MOAIRenderMgr::_setRenderTable ( lua_State* L ) {
	}

#endif

//================================================================//
// MOAIRenderMgr
//================================================================//

//----------------------------------------------------------------//
MOAIRenderMgr::MOAIRenderMgr () :
	mRenderCounter ( 0 ),
	mRenderDuration ( 1.0 / 60.0 ),
	mRenderTime ( 0.0 ),
	mViewport ( 0 ),
	mCamera ( 0 ),
	mFrameBuffer ( 0 ),
	mRenderable ( 0 ) {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIRenderMgr::~MOAIRenderMgr () {
}

//----------------------------------------------------------------//
void MOAIRenderMgr::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getBufferTable",				_getBufferTable },
		{ "getPerformanceDrawCount",	_getPerformanceDrawCount },
		{ "getRenderCount",				_getRenderCount },
		{ "setBufferTable",				_setBufferTable },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIRenderMgr::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIRenderMgr::Render () {

	zglBegin ();

	MOAIGfxResourceMgr::Get ().Update ();

	// Measure performance
	double startTime = ZLDeviceTime::GetTimeInSeconds ();

	MOAIGfxDevice& device = MOAIGfxDevice::Get ();
	device.ResetDrawCount ();

	if ( this->mBufferTable ) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		state.Push ( this->mBufferTable );
		this->RenderTable ( state, -1 );
		state.Pop ( 1 );
	}	
	
	device.GetDefaultBuffer ()->Render ();
	this->mLastDrawCount = MOAIGfxDevice::Get ().GetDrawCount ();
	this->mRenderCounter++;
	
	// Measure performance
	double endTime = ZLDeviceTime::GetTimeInSeconds ();
	this->mRenderDuration = endTime - startTime;
	this->mRenderTime += this->mRenderDuration;
	
	this->mFrameBuffer = 0;
	
	zglEnd ();
}

//----------------------------------------------------------------//
void MOAIRenderMgr::RenderTable ( MOAILuaState& state, int idx ) {

	// TODO: rewrite this to remove the requirement of using a table as the root
	idx = state.AbsIndex ( idx );

	int n = 1;
	while ( n ) {
		
		lua_rawgeti ( state, idx, n++ );
		
		int valType = lua_type ( state, -1 );
			
		if ( valType == LUA_TUSERDATA ) {
			MOAIFrameBuffer* frameBuffer = state.GetLuaObject < MOAIFrameBuffer >( -1, false );
			if ( frameBuffer ) {
				this->mFrameBuffer = frameBuffer;
				frameBuffer->Render ();
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
