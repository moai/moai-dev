// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIAbstractProp.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAILayoutFrame.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAISurfaceSampler2D.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getModelBounds
	@text	Return the prop's local bounds or 'nil' if prop bounds is
			global or missing. The bounds are in model space and will
			be overridden by the prop's bounds if it's been set (using
			setBounds ())
 
	@in		MOAIPartitionHull self
	@out	number xMin
	@out	number yMin
	@out	number zMin
	@out	number xMax
	@out	number yMax
	@out	number zMax
*/
int MOAIAbstractProp::_getModelBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractProp, "U" )
	
	ZLBounds bounds = self->GetModelBounds ();
	if ( !bounds.HasGeometry ()) return 0;
	state.Push ( bounds.mAABB );
	return 6;
}

//----------------------------------------------------------------//
/**	@lua	getModelBoundsSize
	@text	Return the prop's width and height or 'nil' if prop rect is global.
 
	@in		MOAIPartitionHull self
	@out	number width		X max - X min
	@out	number height		Y max - Y min
	@out	number depth		Z max - Z min
*/
int MOAIAbstractProp::_getModelBoundsSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractProp, "U" )

	ZLBounds bounds = self->GetModelBounds ();
	if ( !bounds.HasGeometry ()) return 0;
	
	state.Push ( bounds.mAABB.mMax.mX - bounds.mAABB.mMin.mX );
	state.Push ( bounds.mAABB.mMax.mY - bounds.mAABB.mMin.mY );
	state.Push ( bounds.mAABB.mMax.mZ - bounds.mAABB.mMin.mZ );
	
	return 3;
}

//----------------------------------------------------------------//
/**	@lua	setExpandForSort
	@text	Used when drawing with a layout scheme (i.e. MOAIGrid).
			Expanding for sort causes the prop to emit a sub-prim
			for each component of the layout. For example, when
			attaching a MOAIGrid to a prop, each cell of the grid
			will be added to the render queue for sorting against
			all other props and sub-prims. This is obviously less
			efficient, but still more efficient then using an
			separate prop for each cell or object.
 
	@in		MOAIPartitionHull self
	@in		boolean expandForSort	Default value is false.
	@out	nil
*/
int MOAIAbstractProp::_setExpandForSort ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractProp, "U" )

	bool expandForSort = state.GetValue < bool >( 2, false );

	if ( expandForSort ) {
		self->mFlags |= FLAGS_EXPAND_FOR_SORT;
	}
	else {
		self->mFlags &= ~FLAGS_EXPAND_FOR_SORT;
	}

	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
// TODO: macro
int MOAIAbstractProp::_setFlag ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractProp, "U" )

	u32 flag		= state.GetValue < u32 >( 2, 0 );
	bool set		= state.GetValue < bool >( 3, true );

	self->mFlags = set ? self->mFlags |= flag : self->mFlags &= flag;

	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
/**	@lua	setModelBounds
	@text	Sets or clears the partition bounds override.
 
	@overload	Clear the bounds override.
 
		@in		MOAIPartitionHull self
		@out	nil
 
	@overload	Set the bounds override.
 
		@in		MOAIPartitionHull self
		@in		number xMin
		@in		number yMin
		@in		number zMin
		@in		number xMax
		@in		number yMax
		@in		number zMax
		@out	nil
*/
int MOAIAbstractProp::_setModelBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractProp, "U" )

	if ( state.CheckParams ( 2, "NNNNNN", false )) {
		self->mModelBoundsOverride = state.GetBox ( 2 );
		self->mFlags |= FLAGS_OVERRIDE_BOUNDS;
	}
	else {
		self->mFlags &= ~FLAGS_OVERRIDE_BOUNDS;
	}
	
	self->ScheduleUpdate ();
	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractProp::_setModelBoundsPad ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractProp, "U" )

	self->mModelBoundsPad	= state.GetValue < ZLVec3D >( 2, ZLVec3D::ORIGIN );

	bool pad = (( self->mModelBoundsPad.mX != 0.0f ) || ( self->mModelBoundsPad.mY != 0.0f ) || ( self->mModelBoundsPad.mZ != 0.0f ));
	self->mFlags = pad ? ( self->mFlags | FLAGS_PAD_BOUNDS ) : ( self->mFlags & ~FLAGS_PAD_BOUNDS );

	self->ScheduleUpdate ();

	MOAI_LUA_RETURN_SELF
}

//================================================================//
// MOAIAbstractProp
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractProp::DrawDebug () {

	if ( this->GetWorldBounds ().IsEmpty ()) return;

	MOAIDebugLinesMgr& debugLines = MOAIDebugLinesMgr::Get ();
	if ( !( debugLines.IsVisible () && debugLines.SelectStyleSet < MOAIAbstractGraphicsProp >())) return;

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008

	draw.BindVectorPresets ();

	this->LoadVertexTransform ();

	gfxMgr.SetVertexTransform ( MOAIGfxMgr::MODEL_TO_DISPLAY_MTX );

	ZLBounds modelBounds = this->GetModelBounds ();

	// TODO: check bounds status

	if ( debugLines.Bind ( DEBUG_DRAW_AXIS )) {
		draw.DrawBoxAxis ( modelBounds.mAABB );
	}

	if ( debugLines.Bind ( DEBUG_DRAW_DIAGONALS )) {
		draw.DrawBoxDiagonals ( modelBounds.mAABB );
	}

	if ( debugLines.Bind ( DEBUG_DRAW_MODEL_BOUNDS )) {
		draw.DrawBoxOutline ( modelBounds.mAABB );
	}

	// clear out the world transform (draw in world space)
	gfxMgr.SetVertexTransform ( MOAIGfxMgr::WORLD_TO_DISPLAY_MTX );

	if ( debugLines.Bind ( DEBUG_DRAW_WORLD_BOUNDS )) {
		draw.DrawBoxOutline ( this->GetWorldBounds ().mAABB );
	}

	if ( debugLines.IsVisible ( DEBUG_DRAW_PARTITION_CELLS ) || debugLines.IsVisible ( DEBUG_DRAW_PARTITION_CELLS )) {

		ZLRect cellRect;
		ZLRect paddedRect;

		if ( this->GetCellRect ( &cellRect, &paddedRect )) {

			if ( cellRect.Area () != 0.0f ) {
				if ( debugLines.Bind ( DEBUG_DRAW_PARTITION_CELLS )) {
					draw.DrawRectOutline ( cellRect );
				}
			}

			if ( paddedRect.Area () != 0.0f ) {
				if ( debugLines.Bind ( DEBUG_DRAW_PARTITION_PADDED_CELLS )) {
					draw.DrawRectOutline ( paddedRect );
				}
			}
		}
	}
}

//----------------------------------------------------------------//
ZLBounds MOAIAbstractProp::GetModelBounds () {

	if ( this->mFlags & FLAGS_PARTITION_GLOBAL ) {
		return ZLBounds::GLOBAL;
	}

	ZLBounds bounds = ZLBounds::EMPTY;

	if ( this->mFlags & FLAGS_OVERRIDE_BOUNDS ) {
		bounds.Init ( this->mModelBoundsOverride );
	}
	else {
		bounds = this->MOAIAbstractProp_GetModelBounds ();
	}
	
	if ( bounds.HasGeometry ()) {
		bounds.mAABB.Pad ( this->mModelBoundsPad.mX, this->mModelBoundsPad.mY, this->mModelBoundsPad.mZ );
	}

	return bounds;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIAbstractProp::GetWorldDrawingMtx () const {

	return this->MOAIAbstractProp_GetWorldDrawingMtx ();
}

//----------------------------------------------------------------//
void MOAIAbstractProp::LoadVertexTransform () {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	gfxMgr.SetMtx ( MOAIGfxMgr::MODEL_TO_WORLD_MTX, this->GetWorldDrawingMtx ());
}

//----------------------------------------------------------------//
MOAIAbstractProp::MOAIAbstractProp () :
		mFlags ( 0 ),
		mModelBoundsOverride ( ZLBox::EMPTY ),
		mModelBoundsPad ( ZLVec3D::ORIGIN ) {
	
	RTTI_BEGIN ( MOAIAbstractProp )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractProp >)
		RTTI_EXTEND ( MOAIPartitionHull )
		RTTI_EXTEND ( MOAIAbstractRenderable )
		RTTI_EXTEND ( MOAITransform )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractProp::~MOAIAbstractProp () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractProp::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
	
	MOAIDebugLinesMgr::Get ().ReserveStyleSet < MOAIAbstractProp >( TOTAL_DEBUG_LINE_STYLES );
	
	state.SetField ( -1, "DEBUG_DRAW_GFX_PROP_MASTER",			MOAIDebugLinesMgr::Pack < MOAIAbstractProp >( (u32) -1 ));
	state.SetField ( -1, "DEBUG_DRAW_PARTITION_CELLS",			MOAIDebugLinesMgr::Pack < MOAIAbstractProp >( DEBUG_DRAW_PARTITION_CELLS ));
	state.SetField ( -1, "DEBUG_DRAW_PARTITION_PADDED_CELLS",	MOAIDebugLinesMgr::Pack < MOAIAbstractProp >( DEBUG_DRAW_PARTITION_PADDED_CELLS ));
	state.SetField ( -1, "DEBUG_DRAW_AXIS",						MOAIDebugLinesMgr::Pack < MOAIAbstractProp >( DEBUG_DRAW_AXIS ));
	state.SetField ( -1, "DEBUG_DRAW_DIAGONALS",				MOAIDebugLinesMgr::Pack < MOAIAbstractProp >( DEBUG_DRAW_DIAGONALS ));
	state.SetField ( -1, "DEBUG_DRAW_MODEL_BOUNDS",				MOAIDebugLinesMgr::Pack < MOAIAbstractProp >( DEBUG_DRAW_MODEL_BOUNDS ));
	state.SetField ( -1, "DEBUG_DRAW_WORLD_BOUNDS",				MOAIDebugLinesMgr::Pack < MOAIAbstractProp >( DEBUG_DRAW_WORLD_BOUNDS ));
	
	state.SetField ( -1, "FLAGS_EXPAND_FOR_SORT",				( u32 )FLAGS_EXPAND_FOR_SORT );
	state.SetField ( -1, "FLAGS_PARTITION_GLOBAL",				( u32 )FLAGS_PARTITION_GLOBAL );
}

//----------------------------------------------------------------//
void MOAIAbstractProp::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
	
	luaL_Reg regTable [] = {
		{ "getModelBounds",			_getModelBounds },
		{ "getModelBoundsSize",		_getModelBoundsSize },
		{ "setExpandForSort",		_setExpandForSort },
		{ "setFlag",				_setFlag },
		{ "setModelBounds",			_setModelBounds },
		{ "setModelBoundsPad",		_setModelBoundsPad },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIAbstractProp::MOAIAbstractProp_GetWorldDrawingMtx () const {

	return ZLMatrix4x4::IDENT;
}

//----------------------------------------------------------------//
MOAIPickResult MOAIAbstractProp::MOAIAbstractPickable_PickByPoint ( ZLVec3D loc ) {

	// TODO: handle pad in local space

	ZLAffine3D worldToLocal = this->GetWorldToLocalMtx ();
	worldToLocal.Transform ( loc );

	return this->MOAIAbstractProp_PickByPoint ( loc );
}

//----------------------------------------------------------------//
MOAIPickResult MOAIAbstractProp::MOAIAbstractPickable_PickByRay ( ZLVec3D loc, ZLVec3D normal ) {

	// TODO: handle pad in local space

	ZLAffine3D worldToLocal = this->GetWorldToLocalMtx ();
	worldToLocal.Transform ( loc );
	worldToLocal.TransformVec ( normal );
	
	return this->MOAIAbstractProp_PickByRay ( loc, normal );
}

//----------------------------------------------------------------//
MOAIPickResult MOAIAbstractProp::MOAIAbstractProp_PickByPoint ( ZLVec3D loc ) {

	return MOAIAbstractPickable::PickByPointHelper ( this->GetModelBounds (), loc );
}

//----------------------------------------------------------------//
MOAIPickResult MOAIAbstractProp::MOAIAbstractProp_PickByRay ( ZLVec3D loc, ZLVec3D normal ) {

	return MOAIAbstractPickable::PickByRayHelper ( this->GetModelBounds (), loc, normal );
}

//----------------------------------------------------------------//
bool MOAIAbstractProp::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {
	
	if ( MOAITransform::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	if ( MOAIPartitionHull::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	return false;
}

//----------------------------------------------------------------//
void MOAIAbstractProp::MOAINode_Update () {
	
	MOAITransform::MOAINode_Update ();
	
	ZLBounds bounds = this->GetModelBounds ();
	
	// update the prop location in the partition
	if ( !bounds.IsEmpty ()) {
	
		if ( bounds.HasGeometry ()) {
			ZLPrism obb = bounds.mOBB;
			obb.Transform ( this->mLocalToWorldMtx );
			bounds.Init ( obb );
		}
		this->UpdateWorldBounds ( bounds );
	}
	else {
		this->UpdateWorldBounds ( ZLBounds::EMPTY );
	}
}

//----------------------------------------------------------------//
void MOAIAbstractProp::MOAIPartitionHull_AddToSortBuffer ( MOAIPartitionResultBuffer& buffer, u32 key ) {

	buffer.PushResult ( *this, key, NO_SUBPRIM_ID, this->GetPriority (), this->GetWorldLoc (), this->GetWorldBounds ().mAABB, this->GetPiv ());
}
