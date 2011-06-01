// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIBox2DWorld.h>
#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAICpSpace.h>
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAILayer2D.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIProp2D.h>
#include <moaicore/MOAITransform.h>

#define MAX_RENDERABLES 512

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
		self->mBox2DWorld = state.GetLuaObject < MOAIBox2DWorld >( 2 );
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

	MOAITransformBase* camera = state.GetLuaObject < MOAITransformBase >( 2 );
	if ( !camera ) return 0;

	self->SetCamera ( camera );

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
		self->mCpSpace = state.GetLuaObject < MOAICpSpace >( 2 );
	#endif
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

	MOAIPartition* partition = state.GetLuaObject < MOAIPartition >( 2 );
	if ( !partition ) return 0;

	self->SetPartition ( partition );

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

	MOAIViewport* viewport = state.GetLuaObject < MOAIViewport >( 2 );
	if ( !viewport ) return 0;

	self->SetViewport ( viewport );

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
		this->mPartition = new MOAIPartition ();
	}
}

//----------------------------------------------------------------//
void MOAILayer2D::Draw () {

	if ( !this->mViewport ) return;
	
	USViewport viewport = *this->mViewport;
	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
	
	drawBuffer.Flush ();
	drawBuffer.Reset ();
	
	USAffine2D mtx;
	USCanvas::GetWorldToWndMtx ( mtx, 1.0f, 1.0f );
	mtx.Prepend ( this->mLocalToWorldMtx );
	mtx.Transform ( viewport );
	
	USAffine2D camera;
	this->GetCameraMtx ( camera );
	
	USCanvas::BeginDrawing ( viewport, camera );
	
	if ( this->mShowDebugLines ) {
		
		#if USE_CHIPMUNK
			if ( this->mCpSpace ) {
				this->mCpSpace->DrawDebug ();
				drawBuffer.Flush ();
				drawBuffer.Reset ();
			}
		#endif
		
		#if USE_BOX2D
			if ( this->mBox2DWorld ) {
				this->mBox2DWorld->DrawDebug ();
				drawBuffer.Flush ();
				drawBuffer.Reset ();
			}
		#endif
	}
	
	if ( this->mPartition ) {
		
		USViewQuad viewQuad;
		viewQuad.Init ();
		
		this->mPartition->GatherProps ( viewQuad.mBounds, 0, MOAIProp::CAN_DRAW | MOAIProp::CAN_DRAW_DEBUG );
		u32 totalResults = this->mPartition->GetTotalResults ();
		if (( !totalResults ) || ( totalResults > MAX_RENDERABLES )) return;
		
		// initialize the sort buffer
		USRadixKey16 < MOAIProp* > key [ MAX_RENDERABLES ];
		USRadixKey16 < MOAIProp* > swap [ MAX_RENDERABLES ];
		
		u32 count = 0;
		while ( MOAIProp* prop = this->mPartition->PopResult ()) {
			s16 priority = ( s16 )prop->GetPriority ();
			key [ count ].mKey = (( priority ^ 0x8000 ) | ( priority & 0x7fff ));
			key [ count ].mData = prop;
			count++;
		}

		// sort
		USRadixKey16 < MOAIProp* >* sort = RadixSort16 < MOAIProp* >( key, swap, count );

		// render the sorted list
		for ( u32 i = 0; i < count; ++i ) {
			MOAIProp* prop = sort [ i ].mData;
			prop->Draw ();
			prop->DrawDebug ();
		}
	}
	
	// render the debug lines
	if ( this->mShowDebugLines ) {
		drawBuffer.Flush ();
		MOAIDebugLines::Get ().Draw ();
		drawBuffer.Flush ();
	}
}

//----------------------------------------------------------------//
void MOAILayer2D::GetCameraMtx ( USAffine2D& camera ) {

	if ( this->mCamera ) {
		camera = this->mCamera->GetLocalToWorldMtx ();
		
		camera.m [ USAffine2D::C2_R0 ] *= this->mParallax.mX;
		camera.m [ USAffine2D::C2_R1 ] *= this->mParallax.mY;
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
		USCanvas::GetWndToWorldMtx ( *this->mViewport, camera, wndToWorld );
	}
	else {
		wndToWorld.Ident ();
	}
}

//----------------------------------------------------------------//
void MOAILayer2D::GetWorldToWndMtx ( USAffine2D& worldToWnd ) {

	if ( this->mCamera && this->mViewport ) {
		
		USAffine2D camera;
		this->GetCameraMtx ( camera );
		USCanvas::GetWorldToWndMtx ( *this->mViewport, camera, worldToWnd );
	}
	else {
		worldToWnd.Ident ();
	}
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

//----------------------------------------------------------------//
STLString MOAILayer2D::ToString () {

	STLString repr( MOAIProp2D::ToString () );

	PRETTY_PRINT ( repr, mCamera )
	PRETTY_PRINT ( repr, mViewport )
	PRETTY_PRINT ( repr, mPartition )

	return repr;
}
