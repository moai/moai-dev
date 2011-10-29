// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIBox2DWorld.h>
#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAICpSpace.h>
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIFrameBuffer.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAILayer2D.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIPartitionResultBuffer.h>
#include <moaicore/MOAIPartitionResultMgr.h>
#include <moaicore/MOAIProp2D.h>
#include <moaicore/MOAITexture.h>
#include <moaicore/MOAITransform.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	clear
	@text	Remove all props from the layer's partition.
	
	@in		MOAILayer2D self
	@out	nil
*/
int MOAILayer2D::_clear ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer2D, "U" )

	if ( self->mPartition ) {
		self->mPartition->Clear ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getFitting
	@text	Computes a camera fitting for a given world rect along with
			an optional screen space padding. To do a fitting, computer
			the world rect based on whatever you are fitting to, use
			this method to get the fitting, then animate the camera
			to match.
	
	@in		MOAILayer2D self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@opt	number xPad
	@opt	number yPad
	@out	number x		X center of fitting (use for camera location).
	@out	number y		Y center of fitting (use for camera location).
	@out	number s		Scale of fitting (use for camera scale).
*/
int MOAILayer2D::_getFitting ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer2D, "UNNNN" )

	USRect worldRect;
	worldRect.mXMin = state.GetValue < float >( 2, 0.0f );
	worldRect.mYMin = state.GetValue < float >( 3, 0.0f );
	worldRect.mXMax = state.GetValue < float >( 4, 0.0f );
	worldRect.mYMax = state.GetValue < float >( 5, 0.0f );

	worldRect.Bless ();

	float hPad = state.GetValue < float >( 6, 0.0f );
	float vPad = state.GetValue < float >( 7, 0.0f );

	float x = worldRect.mXMin + (( worldRect.mXMax - worldRect.mXMin ) * 0.5f );
	float y = worldRect.mYMin + (( worldRect.mYMax - worldRect.mYMin ) * 0.5f );

	lua_pushnumber ( state, x );
	lua_pushnumber ( state, y );

	float fitting = self->GetFitting ( worldRect, hPad, vPad );
	lua_pushnumber ( state, fitting );

	return 3;
}

//----------------------------------------------------------------//
/**	@name	getPartition
	@text	Returns the partition (if any) currently attached to this layer.
	
	@in		MOAILayer2D self
	@out	MOAIPartition partition
*/
int	MOAILayer2D::_getPartition ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer2D, "U" )

	if ( self->mPartition ) {
		self->mPartition->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}


//----------------------------------------------------------------//
/**	@name	insertProp
	@text	Adds a prop to the layer's partition.
	
	@in		MOAILayer2D self
	@in		MOAIProp prop
	@out	nil
*/
int	MOAILayer2D::_insertProp ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer2D, "UU" )

	MOAIProp2D* prop = state.GetLuaObject < MOAIProp2D >( 2 );
	if ( !prop ) return 0;
	if ( prop == self ) return 0;

	self->AffirmPartition ();
	self->mPartition->InsertProp ( *prop );
	prop->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	removeProp
	@text	Removes a prop from the layer's partition.
	
	@in		MOAILayer2D self
	@in		MOAIProp prop
	@out	nil
*/
int	MOAILayer2D::_removeProp ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer2D, "UU" )

	MOAIProp2D* prop = state.GetLuaObject < MOAIProp2D >( 2 );
	if ( !prop ) return 0;
	if ( prop == self ) return 0;

	if ( self->mPartition ) {
		self->mPartition->RemoveProp ( *prop );
		prop->ScheduleUpdate ();
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setBox2DWorld
	@text	Sets a Box2D world for debug drawing.
	
	@in		MOAILayer2D self
	@in		MOAIBox2DWorld world
	@out	nil
*/
int MOAILayer2D::_setBox2DWorld ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer2D, "UU" )
	
	#if USE_BOX2D
		self->mBox2DWorld.Set ( *self, state.GetLuaObject < MOAIBox2DWorld >( 2 ));
	#endif
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setCamera
	@text	Sets a camera transform. This is just a regular transform.
			Its inverse is used as the view transform. If no camera
			transform is set, the layer will render using the
			identity transform.
	
	@in		MOAILayer2D self
	@in		MOAITransformBase camera
	@out	nil
*/
int MOAILayer2D::_setCamera ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer2D, "UU" )

	self->mCamera.Set ( *self, state.GetLuaObject < MOAITransformBase >( 2 ));

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setCpSpace
	@text	Sets a Chipmunk space for debug drawing.
	
	@in		MOAILayer2D self
	@in		MOAICpSpace space
	@out	nil
*/
int MOAILayer2D::_setCpSpace ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer2D, "UU" )
	
	#if USE_CHIPMUNK
		self->mCpSpace.Set ( *self, state.GetLuaObject < MOAICpSpace >( 2 ));
	#endif
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setFrameBuffer
	@text	Attach a frame buffer. Layer will render to frame buffer
			instead of the main view.
	
	@in		MOAILayer2D self
	@in		MOAITexture frameBuffer
	@out	nil
*/
int MOAILayer2D::_setFrameBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer2D, "UU" )

	self->mFrameBuffer.Set ( *self, state.GetLuaObject < MOAITexture >( 2 ));

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setParallax
	@text	Sets the parallax scale for this layer. This is simply a
			scalar applied to the view transform before rendering.
	
	@in		MOAILayer2D self
	@in		number xParallax
	@in		number yParallax
	@out	nil
*/
int MOAILayer2D::_setParallax ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer2D, "UNN" )

	self->mParallax.mX = state.GetValue < float >( 2, self->mParallax.mX );
	self->mParallax.mY = state.GetValue < float >( 3, self->mParallax.mY );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setPartition
	@text	Sets a partition for the layer to use. The layer will automatically
			create a partition when the first prop is added if no partition
			has been set.
	
	@in		MOAILayer2D self
	@in		MOAIPartition partition
	@out	nil
*/
int MOAILayer2D::_setPartition ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer2D, "UU" )

	self->mPartition.Set ( *self, state.GetLuaObject < MOAIPartition >( 2 ));

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setViewport
	@text	Set the layer's viewport.
	
	@in		MOAILayer2D self
	@in		MOAIViewport viewport
	@out	nil
*/
int MOAILayer2D::_setViewport ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer2D, "UU" )

	self->mViewport.Set ( *self, state.GetLuaObject < MOAIViewport >( 2 ));

	return 0;
}

//----------------------------------------------------------------//
/**	@name	showDebugLines
	@text	Display debug lines for props in this layer.
	
	@in		MOAILayer2D self
	@opt	bool showDebugLines		Default value is 'true'.
	@out	nil
*/
int	MOAILayer2D::_showDebugLines ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer2D, "U" )
	
	self->mShowDebugLines = state.GetValue < bool >( 2, true );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	wndToWorld
	@text	Transform a point from window space to world space.
	
	@in		MOAILayer2D self
	@in		number x
	@in		number y
	@out	number x
	@out	number y
*/
int MOAILayer2D::_wndToWorld ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer2D, "UNN" )

	USVec2D loc;
	loc.mX = state.GetValue < float >( 2, 0.0f );
	loc.mY = state.GetValue < float >( 3, 0.0f );

	USAffine2D wndToWorld;
	self->GetWndToWorldMtx ( wndToWorld );
	wndToWorld.Transform ( loc );

	lua_pushnumber ( state, loc.mX );
	lua_pushnumber ( state, loc.mY );

	return 2;
}

//----------------------------------------------------------------//
/**	@name	wndToWorld
	@text	Transform a point from world space to window space.
	
	@in		MOAILayer2D self
	@in		number x
	@in		number y
	@out	number x
	@out	number y
*/
int MOAILayer2D::_worldToWnd ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer2D, "UNN" )

	USVec2D loc;
	loc.mX = state.GetValue < float >( 2, 0.0f );
	loc.mY = state.GetValue < float >( 3, 0.0f );

	USAffine2D worldToWnd;
	self->GetWorldToWndMtx ( worldToWnd );
	worldToWnd.Transform ( loc );

	lua_pushnumber ( state, loc.mX );
	lua_pushnumber ( state, loc.mY );

	return 2;
}

//================================================================//
// MOAINode
//================================================================//

//----------------------------------------------------------------//
void MOAILayer2D::AffirmPartition () {

	if ( !this->mPartition ) {
		this->mPartition.Set ( *this, new MOAIPartition ());
		
		USLuaStateHandle state = USLuaRuntime::Get ().State ();
		this->mPartition->PushLuaUserdata ( state );
		state.Pop ( 1 );
	}
}

//----------------------------------------------------------------//
void MOAILayer2D::Draw () {

	if ( !this->mViewport ) return;
	
	MOAIViewport& viewport = *this->mViewport;
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	gfxDevice.ResetState ();
	gfxDevice.SetFrameBuffer ( this->mFrameBuffer );
	
	USRect viewportRect = viewport;
	
	if ( !this->IsOffscreen ()) {
		USMatrix4x4 mtx;
		mtx.Init ( this->mLocalToWorldMtx );
		mtx.Append (gfxDevice. GetWorldToWndMtx ( 1.0f, 1.0f ));
		mtx.Transform ( viewportRect );
	}
	gfxDevice.SetViewport ( viewportRect );
	
	USAffine2D camera;
	this->GetCameraMtx ( camera );
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_VIEW_TRANSFORM, camera );
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_PROJ_TRANSFORM, viewport.GetProjMtx ());
	
	if ( this->mShowDebugLines ) {
		
		#if USE_CHIPMUNK
			if ( this->mCpSpace ) {
				this->mCpSpace->DrawDebug ();
				gfxDevice.Flush ();
			}
		#endif
		
		#if USE_BOX2D
			if ( this->mBox2DWorld ) {
				this->mBox2DWorld->DrawDebug ();
				gfxDevice.Flush ();
			}
		#endif
	}
	
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM );
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_VIEW_TRANSFORM, camera );
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_PROJ_TRANSFORM, viewport.GetProjMtx ());
	
	if ( this->mPartition ) {
		
		USQuad viewQuad = gfxDevice.GetViewQuad ();
		USRect viewBounds = viewQuad.GetBounds ();
		
		MOAIPartitionResultBuffer& buffer = MOAIPartitionResultMgr::Get ().GetBuffer ();
		
		u32 totalResults = this->mPartition->GatherProps ( buffer, viewBounds, 0, MOAIProp::CAN_DRAW | MOAIProp::CAN_DRAW_DEBUG );
		if ( !totalResults ) return;
		
		// TODO: allow configurable sort
		buffer.Sort ( MOAIPartitionResultBuffer::SORT_PRIORITY );

		// render the sorted list
		for ( u32 i = 0; i < totalResults; ++i ) {
			MOAIProp* prop = buffer.GetResultUnsafe ( i );
			prop->Draw ();
			prop->DrawDebug ();
		}
	}
	
	// render the debug lines
	if ( this->mShowDebugLines ) {
		MOAIDebugLines::Get ().Draw ();
	}
	gfxDevice.Flush ();
}

//----------------------------------------------------------------//
void MOAILayer2D::GetCameraMtx ( USAffine2D& camera ) {

	if ( this->mCamera ) {
		camera = this->mCamera->GetLocalToWorldMtx ();
		
		camera.m [ USAffine2D::C2_R0 ] *= this->mParallax.mX;
		camera.m [ USAffine2D::C2_R1 ] *= this->mParallax.mY;
		
		camera.Inverse ();
	}
	else {
		camera.Ident ();
	}
}

//----------------------------------------------------------------//
float MOAILayer2D::GetFitting ( USRect& worldRect, float hPad, float vPad ) {

	if ( !( this->mCamera && this->mViewport )) return 1.0f;

	USRect viewRect = this->mViewport->GetRect ();
	
	float hFit = ( viewRect.Width () - ( hPad * 2 )) / worldRect.Width ();
	float vFit = ( viewRect.Height () - ( vPad * 2 )) / worldRect.Height ();
	
	return ( hFit < vFit ) ? hFit : vFit;
}

//----------------------------------------------------------------//
u32 MOAILayer2D::GetLocalFrame ( USRect& frame ) {
	
	if ( this->mViewport ) {
		frame = this->mViewport->GetRect ();
		return MOAIProp::BOUNDS_OK;
	}
	return MOAIProp::BOUNDS_EMPTY;
}

//----------------------------------------------------------------//
void MOAILayer2D::GetWndToWorldMtx ( USAffine2D& wndToWorld ) {

	if ( this->mViewport ) {
		
		USAffine2D camera;
		this->GetCameraMtx ( camera );
		wndToWorld = this->mViewport->GetWndToWorldMtx ( camera );
	}
	else {
		wndToWorld.Ident ();
	}
}

//----------------------------------------------------------------//
void MOAILayer2D::GetWorldToWndMtx ( USAffine2D& worldToWnd ) {

	if ( this->mViewport ) {
		
		USAffine2D camera;
		this->GetCameraMtx ( camera );
		worldToWnd = this->mViewport->GetWorldToWndMtx ( camera );
	}
	else {
		worldToWnd.Ident ();
	}
}

//----------------------------------------------------------------//
bool MOAILayer2D::IsOffscreen () {

	return this->mFrameBuffer ? this->mFrameBuffer->GetFrameBuffer () != 0 : false;
}

//----------------------------------------------------------------//
MOAILayer2D::MOAILayer2D () :
	mParallax ( 1.0f, 1.0f ),
	mShowDebugLines ( true ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIProp2D )
	RTTI_END
	
	this->SetMask ( MOAIProp::CAN_DRAW | MOAIProp::CAN_DRAW_DEBUG );
}

//----------------------------------------------------------------//
MOAILayer2D::~MOAILayer2D () {

	this->mCamera.Set ( *this, 0 );
	this->mViewport.Set ( *this, 0 );
	this->mPartition.Set ( *this, 0 );
	this->mFrameBuffer.Set ( *this, 0 );

	#if USE_CHIPMUNK
		this->mCpSpace.Set ( *this, 0 );
	#endif
	
	#if USE_BOX2D
		this->mBox2DWorld.Set ( *this, 0 );
	#endif
}

//----------------------------------------------------------------//
void MOAILayer2D::RegisterLuaClass ( USLuaState& state ) {

	MOAIProp2D::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAILayer2D::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAIProp2D::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "clear",					_clear },
		{ "getFitting",				_getFitting },
		{ "getPartition",			_getPartition },
		{ "insertProp",				_insertProp },
		{ "removeProp",				_removeProp },
		{ "setBox2DWorld",			_setBox2DWorld },
		{ "setCamera",				_setCamera },
		{ "setCpSpace",				_setCpSpace },
		{ "setFrameBuffer",			_setFrameBuffer },
		{ "setParallax",			_setParallax },
		{ "setPartition",			_setPartition },
		{ "setViewport",			_setViewport },
		{ "showDebugLines",			_showDebugLines },
		{ "wndToWorld",				_wndToWorld },
		{ "worldToWnd",				_worldToWnd },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
