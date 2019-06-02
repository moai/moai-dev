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
#include <moai-sim/MOAIScissorRect.h>
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
	if ( bounds.mStatus != ZLBounds::ZL_BOUNDS_OK ) return 0;
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
	if ( bounds.mStatus != ZLBounds::ZL_BOUNDS_OK ) return 0;
	
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

	return 0;
}

//----------------------------------------------------------------//
// TODO: macro
int MOAIAbstractProp::_setFlag ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractProp, "U" )

	u32 flag		= state.GetValue < u32 >( 2, 0 );
	bool set		= state.GetValue < bool >( 3, true );

	self->mFlags = set ? self->mFlags |= flag : self->mFlags &= flag;

	return 0;
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
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractProp::_setModelBoundsPad ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractProp, "U" )

	self->mModelBoundsPad	= state.GetValue < ZLVec3D >( 2, ZLVec3D::ORIGIN );

	bool pad = (( self->mModelBoundsPad.mX != 0.0f ) || ( self->mModelBoundsPad.mY != 0.0f ) || ( self->mModelBoundsPad.mZ != 0.0f ));
	self->mFlags = pad ? ( self->mFlags | FLAGS_PAD_BOUNDS ) : ( self->mFlags & ~FLAGS_PAD_BOUNDS );

	self->ScheduleUpdate ();

	return 0;
}

//================================================================//
// MOAIAbstractProp
//================================================================//

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
	
	if ( bounds.mStatus == ZLBounds::ZL_BOUNDS_OK ) {
		bounds.mAABB.Pad ( this->mModelBoundsPad.mX, this->mModelBoundsPad.mY, this->mModelBoundsPad.mZ );
	}

	return bounds;
}

//----------------------------------------------------------------//
bool MOAIAbstractProp::InsideModelBounds ( const ZLVec3D& vec, float pad ) {

	ZLBounds bounds = this->GetModelBounds ();
	
	if ( bounds.mStatus == ZLBounds::ZL_BOUNDS_EMPTY ) return false;
	if ( bounds.mStatus == ZLBounds::ZL_BOUNDS_GLOBAL ) return true;
	
	bounds.mAABB.Inflate ( pad );
	bounds.mAABB.Bless ();
	return bounds.mAABB.Contains ( vec );
}

//----------------------------------------------------------------//
MOAIAbstractProp::MOAIAbstractProp () :
		mFlags ( 0 ),
		mModelBoundsOverride ( ZLBox::EMPTY ),
		mModelBoundsPad ( ZLVec3D::ORIGIN ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIPartitionHull )
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
void MOAIAbstractProp::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIPartitionHull, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAITransform, MOAILuaObject_RegisterLuaClass ( composer, state ));
	
	state.SetField ( -1, "FLAGS_EXPAND_FOR_SORT",		( u32 )FLAGS_EXPAND_FOR_SORT );
	state.SetField ( -1, "FLAGS_PARTITION_GLOBAL",		( u32 )FLAGS_PARTITION_GLOBAL );
}

//----------------------------------------------------------------//
void MOAIAbstractProp::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIPartitionHull, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAITransform, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	
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
void MOAIAbstractProp::MOAILuaObject_SerializeIn ( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIPartitionHull, MOAILuaObject_SerializeIn ( composer, state, serializer ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAITransform, MOAILuaObject_SerializeIn ( composer, state, serializer ));
}

//----------------------------------------------------------------//
void MOAIAbstractProp::MOAILuaObject_SerializeOut ( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIPartitionHull, MOAILuaObject_SerializeOut ( composer, state, serializer ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAITransform, MOAILuaObject_SerializeOut ( composer, state, serializer ));
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
	if ( bounds.mStatus == ZLBounds::ZL_BOUNDS_OK ) {
	
		ZLPrism obb = bounds.mOBB;
		obb.Transform ( this->mLocalToWorldMtx );
	
		bounds.Init ( obb );
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

//----------------------------------------------------------------//
bool MOAIAbstractProp::MOAIPartitionHull_Inside ( ZLVec3D vec, float pad ) {

	ZLAffine3D worldToLocal = this->GetWorldToLocalMtx ();
	worldToLocal.Transform ( vec );
	
	return this->InsideModelBounds ( vec, pad );
}
