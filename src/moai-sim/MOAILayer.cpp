// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICamera.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIFrameBufferTexture.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAILayer.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIPartitionResultMgr.h>
#include <moai-sim/MOAIProp.h>
#include <moai-sim/MOAIRenderMgr.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAITransform.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	clear
	@text	Remove all props from the layer's partition.
	
	@in		MOAILayer self
	@out	nil
*/
int MOAILayer::_clear ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )

	if ( self->mPartition ) {
		self->mPartition->Clear ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getFitting
	@text	Computes a camera fitting for a given world rect along with
			an optional screen space padding. To do a fitting, compute
			the world rect based on whatever you are fitting to, use
			this method to get the fitting, then animate the camera
			to match.
	
	@in		MOAILayer self
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
int MOAILayer::_getFitting ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "UNNNN" )

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
/**	@name	getPartition
	@text	Returns the partition currently attached to this layer.
	
	@in		MOAILayer self
	@out	MOAIPartition partition
*/
int	MOAILayer::_getPartition ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )

	self->AffirmPartition ();
	self->mPartition->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getSortMode
	@text	Get the sort mode for rendering.
	
	@in		MOAILayer self
	@out	number sortMode
*/
int MOAILayer::_getSortMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )
	
	lua_pushnumber ( state, self->mSortMode );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getSortScale
	@text	Return the scalar applied to axis sorts.
	
	@in		MOAILayer self
	@out	number x
	@out	number y
	@out	number priority
*/
int	MOAILayer::_getSortScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )

	lua_pushnumber ( state, self->mSortScale [ 0 ]);
	lua_pushnumber ( state, self->mSortScale [ 1 ]);
	lua_pushnumber ( state, self->mSortScale [ 3 ]);

	return 3;
}

//----------------------------------------------------------------//
/**	@name	insertProp
	@text	Adds a prop to the layer's partition.
	
	@in		MOAILayer self
	@in		MOAIProp prop
	@out	nil
*/
int	MOAILayer::_insertProp ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "UU" )

	MOAIProp* prop = state.GetLuaObject < MOAIProp >( 2, true );
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
	
	@in		MOAILayer self
	@in		MOAIProp prop
	@out	nil
*/
int	MOAILayer::_removeProp ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "UU" )

	MOAIProp* prop = state.GetLuaObject < MOAIProp >( 2, true );
	if ( !prop ) return 0;
	if ( prop == self ) return 0;

	if ( self->mPartition ) {
		self->mPartition->RemoveProp ( *prop );
		prop->ScheduleUpdate ();
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setCamera
	@text	Sets a camera for the layer. If no camera is supplied,
			layer will render using the identity matrix as view/proj.
	
	@overload
	
		@in		MOAILayer self
		@opt	MOAICamera camera		Default value is nil.
		@out	nil
	
	@overload
	
		@in		MOAILayer self
		@opt	MOAICamera2D camera		Default value is nil.
		@out	nil
*/
int MOAILayer::_setCamera ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )

	self->mCamera.Set ( *self, state.GetLuaObject < MOAICamera >( 2, true ));

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAILayer::_setLODMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )
	
	self->mLODMode		= state.GetValue < u32 >( 2, LOD_CONSTANT );
	self->mLODFactor	= state.GetValue < float >( 3, 0.0f );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAILayer::_setOverlayTable ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )
	
	self->mOverlayTable.SetRef ( state, 2 );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setParallax
	@text	Sets the parallax scale for this layer. This is simply a
			scalar applied to the view transform before rendering.
	
	@in		MOAILayer self
	@opt	number xParallax	Default value is 1.
	@opt	number yParallax	Default value is 1.
	@opt	number zParallax	Default value is 1.
	@out	nil
*/
int MOAILayer::_setParallax ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )

	self->mParallax.mX = state.GetValue < float >( 2, 1.0f );
	self->mParallax.mY = state.GetValue < float >( 3, 1.0f );
	self->mParallax.mZ = state.GetValue < float >( 4, 1.0f );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setPartition
	@text	Sets a partition for the layer to use. The layer will automatically
			create a partition when the first prop is added if no partition
			has been set.
	
	@in		MOAILayer self
	@in		MOAIPartition partition
	@out	nil
*/
int MOAILayer::_setPartition ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "UU" )

	self->mPartition.Set ( *self, state.GetLuaObject < MOAIPartition >( 2, true ));

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setPartitionCull2D
	@text	Enables 2D partition cull (projection of frustum AABB will
			be used instead of AABB or frustum).
	
	@in		MOAILayer self
	@in		boolean partitionCull2D		Default value is false.
	@out	nil
*/
int	MOAILayer::_setPartitionCull2D ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )

	self->mPartitionCull2D = state.GetValue < bool >( 2, false );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setSortMode
	@text	Set the sort mode for rendering.
	
	@in		MOAILayer self
	@in		number sortMode				One of MOAILayer.SORT_NONE, MOAILayer.SORT_PRIORITY_ASCENDING,
										MOAILayer.SORT_PRIORITY_DESCENDING, MOAILayer.SORT_X_ASCENDING,
										MOAILayer.SORT_X_DESCENDING, MOAILayer.SORT_Y_ASCENDING,
										MOAILayer.SORT_Y_DESCENDING, MOAILayer.SORT_Z_ASCENDING,
										MOAILayer.SORT_Z_DESCENDING
	@in		boolean sortInViewSpace		Default value is 'false'.
	@out	nil
*/
int MOAILayer::_setSortMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )
	
	self->mSortMode			= state.GetValue < u32 >( 2, MOAIPartitionResultBuffer::SORT_PRIORITY_ASCENDING );
	self->mSortInViewSpace	= state.GetValue < bool >( 3, false );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setSortScale
	@text	Set the scalar applied to axis sorts.
	
	@in		MOAILayer self
	@opt	number x			Default value is 0.
	@opt	number y			Default value is 0.
	@opt	number z			Default value is 0.
	@opt	number priority		Default value is 1.
	@out	nil
*/
int	MOAILayer::_setSortScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )

	self->mSortScale [ 0 ] = state.GetValue < float >( 2, 0.0f );
	self->mSortScale [ 1 ] = state.GetValue < float >( 3, 0.0f );
	self->mSortScale [ 2 ] = state.GetValue < float >( 4, 0.0f );
	self->mSortScale [ 3 ] = state.GetValue < float >( 5, 1.0f );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAILayer::_setUnderlayTable ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )
	
	self->mUnderlayTable.SetRef ( state, 2 );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setViewport
	@text	Set the layer's viewport.
	
	@in		MOAILayer self
	@in		MOAIViewport viewport
	@out	nil
*/
int MOAILayer::_setViewport ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "UU" )

	self->mViewport.Set ( *self, state.GetLuaObject < MOAIViewport >( 2, true ));

	return 0;
}

//----------------------------------------------------------------//
/**	@name	showDebugLines
	@text	Display debug lines for props in this layer.
	
	@in		MOAILayer self
	@opt	boolean showDebugLines		Default value is 'true'.
	@out	nil
*/
int	MOAILayer::_showDebugLines ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "U" )
	
	self->mShowDebugLines = state.GetValue < bool >( 2, true );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	wndToWorld
	@text	Project a point from window space into world space.
	
	@in		MOAILayer self
	@in		number x
	@in		number y
	@out	number x
	@out	number y
	@out	number z
*/
int MOAILayer::_wndToWorld ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "UNN" )

	ZLMatrix4x4 worldToWnd = self->GetWorldToWndMtx ();

	ZLMatrix4x4 wndToWorld = worldToWnd;
	wndToWorld.Inverse ();

	ZLVec4D loc;
	loc.mX = state.GetValue < float >( 2, 0.0f );
	loc.mY = state.GetValue < float >( 3, 0.0f );
	loc.mZ = worldToWnd.m [ ZLMatrix4x4::C3_R2 ] / worldToWnd.m [ ZLMatrix4x4::C3_R3 ];
	loc.mW = 1.0f;

	if ( self->mCamera && ( self->mCamera->GetType () == MOAICamera::CAMERA_TYPE_3D )) {
		loc.mZ = worldToWnd.m [ ZLMatrix4x4::C3_R2 ] / worldToWnd.m [ ZLMatrix4x4::C3_R3 ];
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
/**	@name	wndToWorldRay
	@text	Project a point from window space into world space and return
			a normal vector representing a ray cast from the point into
			the world away from the camera (suitable for 3D picking).
	
	@in		MOAILayer self
	@in		number x
	@in		number y
	@out	number x
	@out	number y
	@out	number z
	@out	number xn
	@out	number yn
	@out	number zn
*/
int MOAILayer::_wndToWorldRay ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "UNN" )

	ZLMatrix4x4 wndToWorld = self->GetWndToWorldMtx ();

	ZLVec4D loc;
	loc.mX = state.GetValue < float >( 2, 0.0f );
	loc.mY = state.GetValue < float >( 3, 0.0f );
	loc.mZ = 0.0f;
	loc.mW = 1.0f;

	ZLVec4D origin = loc;
	origin.mZ = -1.0f;

	if ( self->mCamera  && ( self->mCamera->GetType () == MOAICamera::CAMERA_TYPE_3D )) {
		ZLVec3D cameraLoc = self->mCamera->GetLoc ();
		origin.mX = cameraLoc.mX;
		origin.mY = cameraLoc.mY;
		origin.mZ = cameraLoc.mZ;
	}
	else {
		wndToWorld.Project ( origin );
	}
		
	lua_pushnumber ( state, origin.mX );
	lua_pushnumber ( state, origin.mY );
	lua_pushnumber ( state, origin.mZ );

	ZLVec4D vec = loc;
	wndToWorld.Project ( vec );

	ZLVec3D norm;

	norm.mX = vec.mX - origin.mX;
	norm.mY = vec.mY - origin.mY;
	norm.mZ = vec.mZ - origin.mZ;
	
	norm.Norm ();
	
	lua_pushnumber ( state, norm.mX );
	lua_pushnumber ( state, norm.mY );
	lua_pushnumber ( state, norm.mZ );

	return 6;
}

//----------------------------------------------------------------//
/**	@name	worldToWnd
	@text	Transform a point from world space to window space.
	
	@in		MOAILayer self
	@in		number x
	@in		number y
	@in		number Z
	@out	number x
	@out	number y
	@out	number z
*/
int MOAILayer::_worldToWnd ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayer, "UNN" )

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
// MOAINode
//================================================================//

//----------------------------------------------------------------//
void MOAILayer::AffirmPartition () {

	if ( !this->mPartition ) {
		this->mPartition.Set ( *this, new MOAIPartition ());
	}
}

//----------------------------------------------------------------//
void MOAILayer::Draw ( int subPrimID ) {
	UNUSED ( subPrimID );
    
   	if ( !this->IsVisible () ) return;
	if ( !this->mViewport ) return;
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	MOAIRenderMgr& renderMgr = MOAIRenderMgr::Get ();
	
	MOAIViewport& viewport = *this->mViewport;
	ZLRect viewportRect = viewport;
	
	renderMgr.SetCamera ( this->mCamera );
	renderMgr.SetViewport ( this->mViewport );
	
	gfxDevice.ResetState ();

	// TODO:
	ZLMatrix4x4 mtx;
	mtx.Init ( this->mLocalToWorldMtx );
	// TODO:
	//mtx.Append ( gfxDevice.GetWorldToWndMtx ( 1.0f, 1.0f ));
	mtx.Transform ( viewportRect );

	gfxDevice.SetViewRect ( viewportRect );
	gfxDevice.SetScissorRect ( viewportRect );
	this->ClearSurface ();
	
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM );
	
	ZLMatrix4x4 view = this->GetViewMtx ();
	ZLMatrix4x4 proj = this->GetProjectionMtx ();
	
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM );
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_VIEW_TRANSFORM, view );
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_PROJ_TRANSFORM, proj );
	
	// recompute the frustum
	gfxDevice.UpdateViewVolume ();
	
	this->RenderTable ( this->mUnderlayTable );
	
	if ( this->mPartition ) {
		
		MOAIPartitionResultBuffer& buffer = MOAIPartitionResultMgr::Get ().GetBuffer ();
		const ZLFrustum& viewVolume = gfxDevice.GetViewVolume ();
		
		u32 totalResults = 0;
		
		if ( this->mPartitionCull2D ) {
			totalResults = this->mPartition->GatherProps ( buffer, 0, viewVolume.mAABB, MOAIProp::CAN_DRAW | MOAIProp::CAN_DRAW_DEBUG );
		}
		else {
			totalResults = this->mPartition->GatherProps ( buffer, 0, viewVolume, MOAIProp::CAN_DRAW | MOAIProp::CAN_DRAW_DEBUG );
		}
		
		if ( !totalResults ) return;
		
		if ( this->mSortInViewSpace ) {
			buffer.Transform ( view, false );
		}
		
		buffer.GenerateKeys (
			this->mSortMode,
			this->mSortScale [ 0 ],
			this->mSortScale [ 1 ],
			this->mSortScale [ 2 ],
			this->mSortScale [ 3 ]
		);
		
		totalResults = buffer.Sort ( this->mSortMode );
		
		// set up the ambient color
		gfxDevice.SetAmbientColor ( this->mColor );
		
		// figure out the correct LOD factor
		float lod = this->mLODFactor;
		
		if (( this->mLODMode == LOD_FROM_CAMERA_LOCAL_Z ) && ( this->mCamera )) {
			MOAICamera* camera = this->mCamera;
			ZLVec3D loc = camera->GetLoc ();
			lod = loc.mZ;
		}
		
		if (( this->mLODMode == LOD_FROM_CAMERA_WORLD_Z ) && ( this->mCamera )) {
			MOAICamera* camera = this->mCamera;
			const ZLAffine3D& mtx = camera->GetLocalToWorldMtx ();
			ZLVec3D worldLoc = mtx.GetTranslation ();
			lod = worldLoc.mZ;
		}
		
		this->DrawProps ( buffer, lod );
		
		if ( this->mShowDebugLines ) {
			
			// clear the ambient color and bind vector drawing
			gfxDevice.SetAmbientColor ( 1.0f, 1.0f, 1.0f, 1.0f );
			MOAIDraw::Get ().Bind ();
			this->DrawPropsDebug ( buffer, lod );
		}
	}
	
	this->RenderTable ( this->mOverlayTable );
	gfxDevice.Flush ();
}

//----------------------------------------------------------------//
void MOAILayer::DrawProps ( MOAIPartitionResultBuffer& buffer, float lod ) {

	u32 totalResults = buffer.GetTotalResults ();

	if ( this->mLODMode == LOD_FROM_PROP_SORT_Z ) {
		for ( u32 i = 0; i < totalResults; ++i ) {
			MOAIPartitionResult* result = buffer.GetResultUnsafe ( i );
			MOAIProp* prop = result->mProp;
			prop->Draw ( result->mSubPrimID, result->mLoc.mZ );
		}
	}
	else {
		for ( u32 i = 0; i < totalResults; ++i ) {
			MOAIPartitionResult* result = buffer.GetResultUnsafe ( i );
			MOAIProp* prop = result->mProp;
			prop->Draw ( result->mSubPrimID, lod );
		}
	}
}

//----------------------------------------------------------------//
void MOAILayer::DrawPropsDebug ( MOAIPartitionResultBuffer& buffer, float lod ) {

	u32 totalResults = buffer.GetTotalResults ();

	if ( this->mLODMode == LOD_FROM_PROP_SORT_Z ) {
		for ( u32 i = 0; i < totalResults; ++i ) {
			MOAIPartitionResult* result = buffer.GetResultUnsafe ( i );
			MOAIProp* prop = result->mProp;
			prop->DrawDebug ( result->mSubPrimID, result->mLoc.mZ );
		}
	}
	else {
		for ( u32 i = 0; i < totalResults; ++i ) {
			MOAIPartitionResult* result = buffer.GetResultUnsafe ( i );
			MOAIProp* prop = result->mProp;
			prop->DrawDebug ( result->mSubPrimID, lod );
		}
	}
}

//----------------------------------------------------------------//
float MOAILayer::GetFitting ( ZLRect& worldRect, float hPad, float vPad ) {

	if ( !( this->mCamera && this->mViewport )) return 1.0f;

	ZLRect viewRect = this->mViewport->GetRect ();
	
	float hFit = ( viewRect.Width () - ( hPad * 2 )) / worldRect.Width ();
	float vFit = ( viewRect.Height () - ( vPad * 2 )) / worldRect.Height ();
	
	return ( hFit < vFit ) ? hFit : vFit;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAILayer::GetProjectionMtx () const {
	
	return this->mCamera ? this->mCamera->GetProjMtx ( *this->mViewport ) : this->mViewport->GetProjMtx ();
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAILayer::GetViewMtx () const {
	
	ZLMatrix4x4 view;
	
	if ( this->mCamera ) {
		
		view = this->mCamera->GetViewMtx ();
		view.m [ ZLMatrix4x4::C3_R0 ] *= this->mParallax.mX;
		view.m [ ZLMatrix4x4::C3_R1 ] *= this->mParallax.mY;
		view.m [ ZLMatrix4x4::C3_R2 ] *= this->mParallax.mZ;
	}
	else {
		view.Ident ();
	}
	
	return view;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAILayer::GetWndToWorldMtx () const {

	ZLMatrix4x4 wndToWorld = this->GetWorldToWndMtx ();
	wndToWorld.Inverse ();
	return wndToWorld;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAILayer::GetWorldToWndMtx () const {

	//ZLMatrix4x4 worldToWnd = this->mCamera ? this->mCamera->GetWorldToWndMtx ( *this->mViewport ) : this->mViewport->GetNormToWndMtx ();
	
	ZLMatrix4x4 worldToWnd;
	
	if ( this->mViewport ) {
			worldToWnd = this->GetViewMtx ();
			worldToWnd.Append ( this->GetProjectionMtx ());
			worldToWnd.Append ( this->mViewport->GetNormToWndMtx ());
	}
	else {
			worldToWnd.Ident ();
	}
	
	ZLMatrix4x4 mtx;
	mtx.Init ( this->mLocalToWorldMtx );
	worldToWnd.Append ( mtx );
	
	return worldToWnd;
}

//----------------------------------------------------------------//
MOAILayer::MOAILayer () :
	mParallax ( 1.0f, 1.0f, 1.0f ),
	mShowDebugLines ( true ),
	mSortMode ( MOAIPartitionResultBuffer::SORT_PRIORITY_ASCENDING ),
	mSortInViewSpace ( false ),
	mLODMode ( LOD_FROM_PROP_SORT_Z ),
	mLODFactor ( 0.0f ),
	mPartitionCull2D ( true ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGraphicsProp )
		RTTI_EXTEND ( MOAIClearableView )
	RTTI_END
	
	this->SetMask ( MOAIProp::CAN_DRAW | MOAIProp::CAN_DRAW_DEBUG );
	this->SetClearFlags ( 0 );
}

//----------------------------------------------------------------//
MOAILayer::~MOAILayer () {

	this->mCamera.Set ( *this, 0 );
	this->mViewport.Set ( *this, 0 );
	this->mPartition.Set ( *this, 0 );
}

//----------------------------------------------------------------//
u32 MOAILayer::OnGetModelBounds ( ZLBox& bounds ) {
	
	if ( this->mViewport ) {
		ZLRect frame = this->mViewport->GetRect ();
		bounds.Init ( frame.mXMin, frame.mYMax, frame.mXMax, frame.mYMin, 0.0f, 0.0f );
		return MOAIProp::BOUNDS_OK;
	}
	return MOAIProp::BOUNDS_EMPTY;
}

//----------------------------------------------------------------//
void MOAILayer::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIGraphicsProp::RegisterLuaClass ( state );
	MOAIClearableView::RegisterLuaClass ( state );
	
	state.SetField ( -1, "SORT_NONE",					( u32 )MOAIPartitionResultBuffer::SORT_NONE );
	state.SetField ( -1, "SORT_ISO",					( u32 )MOAIPartitionResultBuffer::SORT_ISO );
	state.SetField ( -1, "SORT_PRIORITY_ASCENDING",		( u32 )MOAIPartitionResultBuffer::SORT_PRIORITY_ASCENDING );
	state.SetField ( -1, "SORT_PRIORITY_DESCENDING",	( u32 )MOAIPartitionResultBuffer::SORT_PRIORITY_DESCENDING );
	state.SetField ( -1, "SORT_X_ASCENDING",			( u32 )MOAIPartitionResultBuffer::SORT_X_ASCENDING );
	state.SetField ( -1, "SORT_X_DESCENDING",			( u32 )MOAIPartitionResultBuffer::SORT_X_DESCENDING );
	state.SetField ( -1, "SORT_Y_ASCENDING",			( u32 )MOAIPartitionResultBuffer::SORT_Y_ASCENDING );
	state.SetField ( -1, "SORT_Y_DESCENDING",			( u32 )MOAIPartitionResultBuffer::SORT_Y_DESCENDING );
	state.SetField ( -1, "SORT_Z_ASCENDING",			( u32 )MOAIPartitionResultBuffer::SORT_Z_ASCENDING );
	state.SetField ( -1, "SORT_Z_DESCENDING",			( u32 )MOAIPartitionResultBuffer::SORT_Z_DESCENDING );
	state.SetField ( -1, "SORT_VECTOR_ASCENDING",		( u32 )MOAIPartitionResultBuffer::SORT_VECTOR_ASCENDING );
	state.SetField ( -1, "SORT_VECTOR_DESCENDING",		( u32 )MOAIPartitionResultBuffer::SORT_VECTOR_DESCENDING );

	state.SetField ( -1, "LOD_CONSTANT",				( u32 )MOAILayer::LOD_CONSTANT );
	state.SetField ( -1, "LOD_FROM_PROP_SORT_Z",		( u32 )MOAILayer::LOD_FROM_PROP_SORT_Z );
	state.SetField ( -1, "LOD_FROM_CAMERA_LOCAL_Z",		( u32 )MOAILayer::LOD_FROM_CAMERA_LOCAL_Z );
	state.SetField ( -1, "LOD_FROM_CAMERA_WORLD_Z",		( u32 )MOAILayer::LOD_FROM_CAMERA_WORLD_Z );
}

//----------------------------------------------------------------//
void MOAILayer::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIGraphicsProp::RegisterLuaFuncs ( state );
	MOAIClearableView::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "clear",					_clear },
		{ "getFitting",				_getFitting },
		{ "getPartition",			_getPartition },
		{ "getSortMode",			_getSortMode },
		{ "getSortScale",			_getSortScale },
		{ "insertProp",				_insertProp },
		{ "removeProp",				_removeProp },
		{ "setCamera",				_setCamera },
		{ "setLODMode",				_setLODMode },
		{ "setOverlayTable",		_setOverlayTable },
		{ "setParallax",			_setParallax },
		{ "setPartition",			_setPartition },
		{ "setPartitionCull2D",		_setPartitionCull2D },
		{ "setSortMode",			_setSortMode },
		{ "setSortScale",			_setSortScale },
		{ "setUnderlayTable",		_setUnderlayTable },
		{ "setViewport",			_setViewport },
		{ "showDebugLines",			_showDebugLines },
		{ "wndToWorld",				_wndToWorld },
		{ "wndToWorldRay",			_wndToWorldRay },
		{ "worldToWnd",				_worldToWnd },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAILayer::Render () {
	
	this->Draw ( MOAIProp::NO_SUBPRIM_ID );
}

//----------------------------------------------------------------//
void MOAILayer::RenderTable ( MOAILuaRef& ref ) {

	if ( ref ) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		state.Push ( ref );
		this->RenderTable ( state, -1 );
		state.Pop ( 1 );
	}
}

//----------------------------------------------------------------//
void MOAILayer::RenderTable ( MOAILuaState& state, int idx ) {

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
void MOAILayer::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAIGraphicsProp::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAILayer::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAIGraphicsProp::SerializeOut ( state, serializer );
}