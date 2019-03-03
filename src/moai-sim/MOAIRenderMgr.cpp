// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAbstractDrawable.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/ZLGfxResourceClerk.h>
#include <moai-sim/MOAIRenderMgr.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRenderMgr::_getRenderCount ( lua_State* L ) {

	MOAIRenderMgr& gfxMgr = MOAIRenderMgr::Get ();
	lua_pushnumber ( L, gfxMgr.mRenderCounter );

	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRenderMgr::_getRender ( lua_State* L ) {
	MOAILuaState state ( L );
	state.Push ( MOAIRenderMgr::Get ().mRenderRoot );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRenderMgr::_setRender ( lua_State* L ) {
	MOAILuaState state ( L );
	MOAIRenderMgr::Get ().mRenderRoot.SetRef ( state, 1 );
	return 0;
}

//================================================================//
// MOAIRenderMgr
//================================================================//

//----------------------------------------------------------------//
MOAIRenderMgr::MOAIRenderMgr () :
	mRenderCounter ( 0 ),
	mRenderDuration ( 1.0 / 60.0 ),
	mRenderTime ( 0.0 ) {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIRenderMgr::~MOAIRenderMgr () {
}

//----------------------------------------------------------------//
void MOAIRenderMgr::PushDrawable ( MOAILuaObject* drawable ) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	state.Push ( this->mRenderRoot );
	
	if ( !state.IsType ( -1, LUA_TTABLE )) {
		state.Pop ();
		lua_newtable ( state );
		this->mRenderRoot.SetRef ( state, -1 );
	}
	
	int top = ( int )state.GetTableSize ( -1 );
	state.SetField ( -1, top + 1, drawable );
}

//----------------------------------------------------------------//
void MOAIRenderMgr::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getRenderCount",				_getRenderCount },
		{ "getRender",					_getRender },
		{ "setRender",					_setRender },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIRenderMgr::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIRenderMgr::Render () {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	gfxMgr.mPipelineMgr.ResetDrawingAPIs ();
	gfxMgr.mResourceMgr.Update ();

	ZLFrameBuffer* frameBuffer = gfxMgr.mGfxState.GetDefaultFrameBuffer ();
	assert ( frameBuffer );
	frameBuffer->NeedsClear ( true );

	// Measure performance
	double startTime = ZLDeviceTime::GetTimeInSeconds ();
	
	ZLGfx* gfx = gfxMgr.mPipelineMgr.SelectDrawingAPI ( ZLGfxPipelineClerk::DRAWING_PIPELINE );
	if ( !gfx ) return;

	ZGL_COMMENT ( *gfx, "RENDER MGR RENDER" );

	if ( this->mRenderRoot ) {
	
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		state.Push ( this->mRenderRoot );
		
		MOAIAbstractDrawable::Draw ( state, -1 );
	}
	
	this->mRenderCounter++;
	
	// Measure performance
	double endTime = ZLDeviceTime::GetTimeInSeconds ();
	this->mRenderDuration = endTime - startTime;
	this->mRenderTime += this->mRenderDuration;
	
	 gfxMgr.mGfxState.FinishFrame ();
}
