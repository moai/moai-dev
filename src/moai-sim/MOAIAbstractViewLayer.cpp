// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAbstractViewLayer.h>
#include <moai-sim/MOAICamera.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIPartitionResultMgr.h>
#include <moai-sim/MOAITransform.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getCamera
	@text	Get the camera associated with the layer.
	
	@in		MOAIAbstractViewLayer self
	@out	MOAICamera camera
*/
int MOAIAbstractViewLayer::_getCamera ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractViewLayer, "U" )
	state.Push (( MOAILuaObject* )self->mCamera );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getFitting
	@text	Computes a camera fitting for a given world rect along with
			an optional screen space padding. To do a fitting, compute
			the world rect based on whatever you are fitting to, use
			this method to get the fitting, then animate the camera
			to match.
	
	@in		MOAIAbstractViewLayer self
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
int MOAIAbstractViewLayer::_getFitting ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractViewLayer, "UNNNN" )

	ZLRect worldRect;
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
/**	@lua	getFitting3D
	@text	Find a position for the camera where all given locations or
			props will be visible without changing the camera's orientation
			(i.e. orient the camera first, then call this to derive the
			correct position).
	
	@in		MOAIAbstractViewLayer self
	@in		table targets		A table of either props or locations. Locations are tables containing {x, y, z, r}.
	@out	number x
	@out	number y
	@out	number z
*/
int MOAIAbstractViewLayer::_getFitting3D ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractViewLayer, "UT" )

	if (( !self->mViewport ) || ( !self->mCamera ) || ( self->mCamera->GetType () != MOAICamera::CAMERA_TYPE_3D )) return 0;
	
	ZLRect fitRect = state.GetValue < ZLRect >( 3, *self->mViewport );
	
	self->mCamera->ForceUpdate ();
	
	ZLFrustumFitter fitter;
	
	fitter.Init (
		*self->mViewport,
		fitRect,
		self->mCamera->GetFieldOfView (),
		self->mCamera->GetLocalToWorldMtx ()
	);

	u32 itr = state.PushTableItr ( 2 );
	while ( state.TableItrNext ( itr )) {
	
		int type = lua_type ( state, -1 );
		
		switch ( type ) {
		
			case LUA_TTABLE: {
			
				ZLVec3D loc;
				
				loc.mX = state.GetFieldValue < cc8*, float >( -1, "x", 0.0 );
				loc.mY = state.GetFieldValue < cc8*, float >( -1, "y", 0.0 );
				loc.mZ = state.GetFieldValue < cc8*, float >( -1, "z", 0.0 );
				
				float r = state.GetFieldValue < cc8*, float >( -1, "r", 0.0 );
				
				fitter.FitPoint( loc, r );
				
				break;
			}
			
			case LUA_TUSERDATA: {
			
				MOAIPartitionHull* hull = state.GetLuaObject < MOAIPartitionHull >( -1, true );
		
				if ( hull ) {					
					ZLVec3D center;
					hull->GetWorldBounds ().mAABB.GetCenter ( center );
					fitter.FitBox ( center, 0.0f );
				}
				break;
			}
		}
	}
	
	ZLVec3D position = fitter.GetPosition ();
	
	state.Push ( position.mX );
	state.Push ( position.mY );
	state.Push ( position.mZ );

	return 3;
}

//----------------------------------------------------------------//
/**	@lua	getViewport
	@text	Return the viewport currently associated with the layer.
	
	@in		MOAIAbstractViewLayer self
	@out	MOAILuaObject viewport
*/
int MOAIAbstractViewLayer::_getViewport ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractViewLayer, "U" )
	state.Push (( MOAILuaObject* )self->mViewport );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setCamera
	@text	Sets a camera for the layer. If no camera is supplied,
			layer will render using the identity matrix as view/proj.
	
	@overload
	
		@in		MOAIAbstractViewLayer self
		@opt	MOAICamera camera		Default value is nil.
		@out	nil
	
	@overload
	
		@in		MOAIAbstractViewLayer self
		@opt	MOAICamera2D camera		Default value is nil.
		@out	nil
*/
int MOAIAbstractViewLayer::_setCamera ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractViewLayer, "U" )

	self->mCamera.Set ( *self, state.GetLuaObject < MOAICamera >( 2, true ));

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractViewLayer::_setDebugCamera ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractViewLayer, "U" )

	self->mDebugCamera.Set ( *self, state.GetLuaObject < MOAICamera >( 2, true ));

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setParallax
	@text	Sets the parallax scale for this layer. This is simply a
			scalar applied to the view transform before rendering.
	
	@in		MOAIAbstractViewLayer self
	@opt	number xParallax	Default value is 1.
	@opt	number yParallax	Default value is 1.
	@opt	number zParallax	Default value is 1.
	@out	nil
*/
int MOAIAbstractViewLayer::_setParallax ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractViewLayer, "U" )

	self->mParallax.mX = state.GetValue < float >( 2, 1.0f );
	self->mParallax.mY = state.GetValue < float >( 3, 1.0f );
	self->mParallax.mZ = state.GetValue < float >( 4, 1.0f );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setViewport
	@text	Set the layer's viewport.
	
	@in		MOAIAbstractViewLayer self
	@in		MOAIViewport viewport
	@out	nil
*/
int MOAIAbstractViewLayer::_setViewport ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractViewLayer, "UU" )

	self->mViewport.Set ( *self, state.GetLuaObject < MOAIViewport >( 2, true ));

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	showDebugLines
	@text	Display debug lines for props in this layer.
	
	@in		MOAIAbstractViewLayer self
	@opt	boolean showDebugLines		Default value is 'true'.
	@out	nil
*/
int	MOAIAbstractViewLayer::_showDebugLines ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractViewLayer, "U" )
	
	self->mShowDebugLines = state.GetValue < bool >( 2, true );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	wndToWorld
	@text	Project a point from window space into world space.
	
	@in		MOAIAbstractViewLayer self
	@in		number x
	@in		number y
	@out	number x
	@out	number y
	@out	number z
*/
int MOAIAbstractViewLayer::_wndToWorld ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractViewLayer, "UNN" )

	ZLMatrix4x4 worldToWnd = self->GetWorldToWndMtx ();

	ZLMatrix4x4 wndToWorld = worldToWnd;
	wndToWorld.Inverse ();

	ZLVec4D loc;
	loc.mX = state.GetValue < float >( 2, 0.0f );
	loc.mY = state.GetValue < float >( 3, 0.0f );
	loc.mZ = worldToWnd.m [ ZLMatrix4x4::C3_R2 ] / worldToWnd.m [ ZLMatrix4x4::C3_R3 ];
	loc.mW = 1.0f;

	if ( self->mCamera && ( self->mCamera->GetType () == MOAICamera::CAMERA_TYPE_3D )) {
		wndToWorld.Project ( loc );
	}
	else {
		wndToWorld.Transform ( loc );
	}

	lua_pushnumber ( state, loc.mX );
	lua_pushnumber ( state, loc.mY );
	lua_pushnumber ( state, loc.mZ );

	return 3;
}

//----------------------------------------------------------------//
/**	@lua	wndToWorldRay
	@text	Project a point from window space into world space and return
			a normal vector representing a ray cast from the point into
			the world away from the camera (suitable for 3D picking).
	
	@in		MOAIAbstractViewLayer self
	@in		number x
	@in		number y
	@in		number d	If non-zero, scale normal by dist to plane d units away from camera. Default is zero.
	@out	number x
	@out	number y
	@out	number z
	@out	number xn
	@out	number yn
	@out	number zn
*/
int MOAIAbstractViewLayer::_wndToWorldRay ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractViewLayer, "UNN" )

	if ( self->mCamera ) {
		self->mCamera->ForceUpdate ();
	}

	ZLMatrix4x4 wndToWorld = self->GetWndToWorldMtx ();

	ZLVec4D loc;
	loc.mX = state.GetValue < float >( 2, 0.0f );
	loc.mY = state.GetValue < float >( 3, 0.0f );
	loc.mZ = 0.0f;
	loc.mW = 1.0f;

	float d = state.GetValue < float >( 4, 0.0f );

	ZLVec4D origin;

	if ( self->mCamera  && ( self->mCamera->GetType () == MOAICamera::CAMERA_TYPE_3D )) {
		const ZLAffine3D& localToWorldMtx = self->mCamera->GetLocalToWorldMtx ();
		ZLVec3D cameraLoc = localToWorldMtx.GetTranslation ();
		origin.mX = cameraLoc.mX;
		origin.mY = cameraLoc.mY;
		origin.mZ = cameraLoc.mZ;
	}
	else {
		origin = loc;
		wndToWorld.Project ( origin );
	}
	
	lua_pushnumber ( state, origin.mX );
	lua_pushnumber ( state, origin.mY );
	lua_pushnumber ( state, origin.mZ );

	ZLVec3D norm;

	if ( self->mCamera  && ( self->mCamera->GetType () == MOAICamera::CAMERA_TYPE_3D )) {
	
		wndToWorld.Project ( loc );
	
		norm.mX = loc.mX - origin.mX;
		norm.mY = loc.mY - origin.mY;
		norm.mZ = loc.mZ - origin.mZ;
		norm.Norm ();
	}
	else {
		
		norm.mX = 0.0f;
		norm.mY = 0.0f;
		norm.mZ = -1.0f;
	}

	float ns = 1.0f;
	
	if ( d != 0.0f ) {
	
		if ( self->mCamera  && ( self->mCamera->GetType () == MOAICamera::CAMERA_TYPE_3D )) {
			const ZLAffine3D& localToWorldMtx = self->mCamera->GetLocalToWorldMtx ();
			ZLVec3D zAxis = localToWorldMtx.GetZAxis ();
			ns = -( d * zAxis.Dot ( norm ));
		}
		else {
			ns = d;
		}
	}
	
	lua_pushnumber ( state, norm.mX * ns );
	lua_pushnumber ( state, norm.mY * ns );
	lua_pushnumber ( state, norm.mZ * ns );

	return 6;
}

//----------------------------------------------------------------//
/**	@lua	worldToWnd
	@text	Transform a point from world space to window space.
	
	@in		MOAIAbstractViewLayer self
	@in		number x
	@in		number y
	@in		number Z
	@out	number x
	@out	number y
	@out	number z
*/
int MOAIAbstractViewLayer::_worldToWnd ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractViewLayer, "UNN" )

	ZLVec4D loc;
	loc.mX = state.GetValue < float >( 2, 0.0f );
	loc.mY = state.GetValue < float >( 3, 0.0f );
	loc.mZ = state.GetValue < float >( 4, 0.0f );
	loc.mW = 1.0f;

	ZLMatrix4x4 worldToWnd = self->GetWorldToWndMtx ();
	worldToWnd.Project ( loc );

	lua_pushnumber ( state, loc.mX );
	lua_pushnumber ( state, loc.mY );
	lua_pushnumber ( state, loc.mZ );

	return 3;
}

//================================================================//
// MOAIAbstractViewLayer
//================================================================//

//----------------------------------------------------------------//
float MOAIAbstractViewLayer::GetFitting ( ZLRect& worldRect, float hPad, float vPad ) {

	if ( !( this->mCamera && this->mViewport )) return 1.0f;

	ZLRect viewRect = this->mViewport->GetRect ();
	
	float hFit = ( viewRect.Width () - ( hPad * 2 )) / worldRect.Width ();
	float vFit = ( viewRect.Height () - ( vPad * 2 )) / worldRect.Height ();
	
	return ( hFit < vFit ) ? hFit : vFit;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIAbstractViewLayer::GetWndToWorldMtx () const {

	return ZLViewProj::GetWndToWorldMtx ( this->mViewport, this->mCamera, this->mLocalToWorldMtx, this->mParallax );
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIAbstractViewLayer::GetWorldToWndMtx () const {

	return ZLViewProj::GetWorldToWndMtx ( this->mViewport, this->mCamera, this->mLocalToWorldMtx, this->mParallax );
}

//----------------------------------------------------------------//
MOAIAbstractViewLayer::MOAIAbstractViewLayer () :
	mParallax ( 1.0f, 1.0f, 1.0f ),
	mShowDebugLines ( true ) {
	
	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAIAbstractViewLayer )
		RTTI_EXTEND ( MOAIGraphicsProp )
		RTTI_EXTEND ( MOAIAbstractLayer )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractViewLayer::~MOAIAbstractViewLayer () {

	this->mCamera.Set ( *this, 0 );
	this->mDebugCamera.Set ( *this, 0 );
	this->mViewport.Set ( *this, 0 );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractViewLayer::MOAIDrawable_Draw ( int subPrimID ) {
	UNUSED ( subPrimID );
    
   	if ( !this->IsVisible ()) return;
	if ( !this->mViewport ) return;
	if ( this->IsClear ()) return;
	
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	gfxMgr.SetFrameBuffer ( this->GetFrameBuffer ());
	
	MOAIViewport& viewport = *this->mViewport;
	ZLRect viewportRect = viewport;

	ZLMatrix4x4 mtx ( this->mLocalToWorldMtx );
	mtx.Transform ( viewportRect );

	gfxMgr.SetViewRect ( viewportRect );
	gfxMgr.SetScissorRect ( viewportRect );
	
	this->ClearSurface ();
	
	gfxMgr.SetViewProj ( this->mViewport, this->mCamera, this->mDebugCamera, this->mParallax );
	gfxMgr.SetMtx ( MOAIGfxMgr::MODEL_TO_WORLD_MTX );
	
	// set up the ambient color
	gfxMgr.SetAmbientColor ( this->mColor );
	
	this->MOAIAbstractViewLayer_Draw ();
	
	if ( MOAIDebugLinesMgr::Get ().IsVisible () && this->mShowDebugLines ) {
		if ( this->mCamera ) {
			this->mCamera->DrawDebug ();
		}
	}
	gfxMgr.SetFrameBuffer ( this->GetFrameBuffer ());
}

//----------------------------------------------------------------//
void MOAIAbstractViewLayer::MOAIDrawable_DrawDebug ( int subPrimID ) {
	UNUSED ( subPrimID );
}

//----------------------------------------------------------------//
ZLBounds MOAIAbstractViewLayer::MOAIAbstractProp_GetModelBounds () {
	
	if ( this->mViewport ) {
		ZLBounds bounds ( this->mViewport->GetRect ());
		return bounds;
	}
	return ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
void MOAIAbstractViewLayer::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAIGraphicsProp, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIAbstractLayer, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIAbstractViewLayer::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGraphicsProp, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIAbstractLayer, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	
	luaL_Reg regTable [] = {
		{ "getCamera",				_getCamera },
		{ "getFitting",				_getFitting },
		{ "getFitting3D",			_getFitting3D },
		{ "getViewport",			_getViewport },
		{ "setDebugCamera",			_setDebugCamera },
		{ "setCamera",				_setCamera },
		{ "setParallax",			_setParallax },
		{ "setViewport",			_setViewport },
		{ "showDebugLines",			_showDebugLines },
		{ "wndToWorld",				_wndToWorld },
		{ "wndToWorldRay",			_wndToWorldRay },
		{ "worldToWnd",				_worldToWnd },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
