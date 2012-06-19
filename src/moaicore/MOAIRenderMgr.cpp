// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAIRenderable.h>
#include <moaicore/MOAIRenderMgr.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getPerformanceDrawCount	
	@text	Returns the number of draw calls last frame.	

	@out	number count Number of underlying graphics "draw" calls last frame.	
*/	
int MOAIRenderMgr::_getPerformanceDrawCount ( lua_State* L ) {

	MOAIRenderMgr& device = MOAIRenderMgr::Get ();
	lua_pushnumber ( L, device.mLastDrawCount );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	getRenderTable
	@text	Returns the table currently being used for rendering.
	
	@out	table renderTable
*/
int MOAIRenderMgr::_getRenderTable ( lua_State* L ) {
	MOAILuaState state ( L );
	state.Push ( MOAIRenderMgr::Get ().mRenderTable );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	grabNextFrame
	@text	Save the next frame rendered to 

	@in		MOAIImage image			Image to save the backbuffer to
	@in		function callback		The function to execute when the frame has been saved into the image specified

	@out	table renderTable
*/
int MOAIRenderMgr::_grabNextFrame ( lua_State* L ) {
	MOAILuaState state ( L );

	MOAIImage* image = state.GetLuaObject < MOAIImage >( 1, true );
	if ( image ) {
		MOAIRenderMgr::Get ().mFrameImage = image;
	}

	MOAIRenderMgr::Get().SetLocal ( state, 2, MOAIRenderMgr::Get ().mOnFrameFinish );

	MOAIRenderMgr::Get ().mGrabNextFrame = true;

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRenderTable
	@text	Sets the table to be used for rendering. This should be
			an array indexed from 1 consisting of MOAIRenderable objects
			and sub-tables. Objects will be rendered in order starting
			from index 1 and continuing until 'nil' is encountered.
	
	@in		table renderTable
	@out	nil
*/
int MOAIRenderMgr::_setRenderTable ( lua_State* L ) {
	MOAILuaState state ( L );
	MOAIRenderMgr::Get ().mRenderTable.SetStrongRef ( state, 1 );
	return 0;
}

//================================================================//
// DOXYGEN
//================================================================//

#ifdef DOXYGEN

	//----------------------------------------------------------------//
	/**	@name	clearRenderStack
		@text	Sets the render stack to nil. THIS METHOD
				IS DEPRECATED AND WILL BE REMOVED IN A FUTURE RELEASE.

		@out	nil
	*/
	int MOAIRenderMgr::_clearRenderStack ( lua_State* L ) {
	}
	
	//----------------------------------------------------------------//
	/**	@name	popRenderPass
		@text	Pops the top renderable from the render stack. THIS METHOD
				IS DEPRECATED AND WILL BE REMOVED IN A FUTURE RELEASE.

		@out	nil
	*/
	int MOAIRenderMgr::_popRenderPass ( lua_State* L ) {
	}
	
	//----------------------------------------------------------------//
	/**	@name	pushRenderPass
		@text	Pushes a renderable onto the render stack. THIS METHOD
				IS DEPRECATED AND WILL BE REMOVED IN A FUTURE RELEASE.

		@in		MOAIRenderable renderable
		@out	nil
	*/
	int MOAIRenderMgr::_pushRenderPass ( lua_State* L ) {
	}
	
	//----------------------------------------------------------------//
	/**	@name	removeRenderPass
		@text	Removes a renderable from the render stack. THIS METHOD
				IS DEPRECATED AND WILL BE REMOVED IN A FUTURE RELEASE.

		@in		MOAIRenderable renderable
		@out	nil
	*/
	int MOAIRenderMgr::_removeRenderPass ( lua_State* L ) {
	}

#endif

//================================================================//
// MOAIRenderMgr
//================================================================//

//----------------------------------------------------------------//
MOAIRenderMgr::MOAIRenderMgr () :
	mGrabNextFrame ( false ),
	mLastDrawCount( 0 ),
	mRenderCounter ( 0 ) {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIRenderMgr::~MOAIRenderMgr () {
}

//----------------------------------------------------------------//
void MOAIRenderMgr::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getRenderTable",				_getRenderTable },
		{ "setRenderTable",				_setRenderTable },
		{ "grabNextFrame",				_grabNextFrame },
		{ "getPerformanceDrawCount",	_getPerformanceDrawCount },
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

	this->mRenderCounter++;

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	gfxDevice.BeginDrawing ();
	
	if ( this->mRenderTable ) {
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		state.Push ( this->mRenderTable );
		this->RenderTable ( state, -1 );
		state.Pop ( 1 );
	}

	gfxDevice.Flush ();
	gfxDevice.ProcessDeleters ();

	if ( mGrabNextFrame ) {

		MOAIGfxDevice::Get().ReadFrameBuffer ( mFrameImage );
		mGrabNextFrame = false;

		if ( this->mOnFrameFinish ) {
		
			MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
			this->PushLocal ( state, this->mOnFrameFinish );
			state.DebugCall ( 0, 0 );
		}
	}

	this->mLastDrawCount = MOAIGfxDevice::Get().GetDrawCount();
}

//----------------------------------------------------------------//
void MOAIRenderMgr::RenderTable ( MOAILuaState& state, int idx ) {

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