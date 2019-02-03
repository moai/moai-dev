// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAILayoutFrame.h>
#include <moai-sim/MOAIRenderMgr.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionHolder.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIPartitionHull.h>
#include <moai-sim/MOAIScissorRect.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAISurfaceSampler2D.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAITextureBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getPartition
	@text	Returns the partition prop is currently held in.
	
	@in		MOAIPartitionHull self
	@out	MOAIPartition partition
*/
int	MOAIPartitionHull::_getPartition ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHull, "U" )

	if ( self->mPartition ) {
		self->mPartition->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getPriority
	@text	Returns the current priority of the node or 'nil' if the
			priority is uninitialized.
	
	@in		MOAIPartitionHull self
	@out	number priority		The node's priority or nil.
*/
int MOAIPartitionHull::_getPriority ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHull, "U" )
	
	if ( self->mPriority != UNKNOWN_PRIORITY ) {
		lua_pushnumber ( state, self->mPriority );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getWorldBounds
	@text	Return the prop's world bounds or 'nil' if prop bounds is
			global or missing.
	
	@in		MOAIPartitionHull self
	@out	number xMin
	@out	number yMin
	@out	number zMin
	@out	number xMax
	@out	number yMax
	@out	number zMax
*/
int MOAIPartitionHull::_getWorldBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHull, "U" )
	
	self->ForceUpdate ();
	
	if ( self->mPartition->IsGlobal ( *self )) return 0;
	if ( self->mPartition->IsEmpty ( *self )) return 0;
	
	state.Push ( self->mWorldBounds.mAABB );
	return 6;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPartitionHull::_getWorldBoundsCenter ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHull, "U" )
	
	self->ForceUpdate ();
	
	if ( self->mPartition->IsGlobal ( *self )) return 0;
	if ( self->mPartition->IsEmpty ( *self )) return 0;
	
	ZLVec3D center;
	self->mWorldBounds.mAABB.GetCenter ( center );

	state.Push ( center );
	return 3;
}

//----------------------------------------------------------------//
/**	@lua	insid
	@text	Returns true if the given world space point falls inside
			the prop's bounds.
	
	@in		MOAIPartitionHull self
	@in		number x
	@in		number y
	@in		number z
	@opt	number pad			Pad the hit bounds (in the prop's local space)
	@out	boolean isInside
*/
int	MOAIPartitionHull::_inside ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHull, "U" )

	ZLVec3D vec;
	vec.mX	= state.GetValue < float >( 2, 0.0f );
	vec.mY	= state.GetValue < float >( 3, 0.0f );
	vec.mZ	= state.GetValue < float >( 4, 0.0f );

	float pad = state.GetValue < float >( 5, 0.0f );

	bool result = self->Inside ( vec, pad );
	lua_pushboolean ( state, result );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setHitGranularity
	@text	Specify the granularity to use when performing a hit test. This is a hint to the implementation
			as to how much processing to allocate to a given test. The default value is MOAIPartitionHull.HIT_TEST_COARSE,
			which will cause only the deck or prop bounds to be used. A setting of MOAIPartitionHull.HIT_TEST_MEDIUM or
			MOAIPartitionHull.HIT_TEST_FINE is implementation dependent on the deck, but 'medium' usually means to test
			against the geometry of the deck and 'fine' means to test against both the geometry and the pixels
			of the hit mask (if any).
	
	@in		MOAIPartitionHull self
	@opt	int granularity		One of MOAIPartitionHull.HIT_TEST_COARSE, MOAIPartitionHull.HIT_TEST_MEDIUM, MOAIPartitionHull.HIT_TEST_FINE. Default is MOAIPartitionHull.HIT_TEST_COARSE.
	@out	nil
*/
int MOAIPartitionHull::_setHitGranularity ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHull, "U" )

	self->mHitGranularity = state.GetValue < u32 >( 2, HIT_TEST_COARSE );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPartitionHull::_setPartition ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHull, "U" )
	
	MOAIPartitionHolder* holder = state.GetLuaObject < MOAIPartitionHolder >( 2, false );
	MOAIPartition* partition = holder ? holder->GetPartition () : state.GetLuaObject < MOAIPartition >( 2, true );
	
	self->SetPartition ( partition );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPriority
	@text	Sets or clears the node's priority. Clear the priority
			to have MOAIPartition automatically assign a priority
			to a node when it is added.
	
	@in		MOAIPartitionHull self
	@opt	number priority		Default value is nil.
	@out	nil
*/
int MOAIPartitionHull::_setPriority ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHull, "U" )
	
	if ( state.IsType ( 2, LUA_TNUMBER )) {
		self->mPriority = ( s32 )state.GetValue < int >( 2, 0 );
	}
	else {
		self->mPriority = UNKNOWN_PRIORITY;
		if ( self->mPartition ) {
			self->mPartition->AffirmPriority ( *self );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPartitionHull::_setQueryMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHull, "U" )

	self->mQueryMask = state.GetValue < u32 >( 2, 0 );
	return 0;
}

//================================================================//
// MOAIPartitionHull
//================================================================//

//----------------------------------------------------------------//
void MOAIPartitionHull::AddToSortBuffer ( MOAIPartitionResultBuffer& buffer, u32 key ) {

	this->MOAIPartitionHull_AddToSortBuffer ( buffer, key );
}

//----------------------------------------------------------------//
void MOAIPartitionHull::BoundsDidChange () {

	this->MOAIPartitionHull_BoundsDidChange ();
}

//----------------------------------------------------------------//
//void MOAIPartitionHull::GatherSurfaces ( MOAISurfaceSampler2D& sampler ) {
//	UNUSED ( sampler );

	//if ( !this->mDeck ) return;
	//
	//sampler.SetSourcePrim ( this );
	//
	//if ( this->mGrid ) {
	//	
	//	ZLRect localRect = sampler.GetLocalRect ();
	//	
	//	MOAICellCoord c0;
	//	MOAICellCoord c1;
	//	
	//	ZLRect deckBounds = this->mDeck->GetBounds ().GetRect( ZLBox::PLANE_XY );

	//	this->mGrid->GetBoundsInRect ( localRect, c0, c1, deckBounds );
	//	//this->mDeck->GatherSurfaces ( *this->mGrid, this->mRemapper, this->mGridScale, c0, c1, sampler );
	//}
	//else {
	//	//this->mDeck->GatherSurfaces ( MOAIDeckRemapper::Remap ( this->mRemapper, this->mIndex ), sampler );
	//}
//}

//----------------------------------------------------------------//
bool MOAIPartitionHull::GetCellRect ( ZLRect* cellRect, ZLRect* paddedRect ) {

	if ( !( cellRect || paddedRect )) return false;
	
	if ( this->mLevel ) {
	
		ZLVec3D center;
		this->mWorldBounds.mAABB.GetCenter ( center );
		
		MOAICellCoord coord = this->mLevel->mGridSpace.GetCellCoord ( center.mX, center.mY );
		ZLRect rect = this->mLevel->mGridSpace.GetCellRect ( coord );
		
		if ( cellRect ) {
			*cellRect = rect;
		}
		
		if ( paddedRect ) {
			rect.Inflate ( this->mLevel->mCellSize * 0.5f );
			*paddedRect = rect;
		}
		return true;
	}
	
	if ( cellRect ) {
		*cellRect = ZLRect::EMPTY;
	}
	
	if ( paddedRect ) {
		*paddedRect = ZLRect::EMPTY;
	}
	
	return false;
}

//----------------------------------------------------------------//
MOAIPartition* MOAIPartitionHull::GetPartitionTrait () {

	return this->mPartition;
}

//----------------------------------------------------------------//
bool MOAIPartitionHull::Inside ( ZLVec3D vec, float pad ) {
	
	return this->MOAIPartitionHull_Inside ( vec, pad );
}

//----------------------------------------------------------------//
MOAIPartitionHull::MOAIPartitionHull () :
	mPartition ( 0 ),
	mCell ( 0 ),
	mLevel ( 0 ),
	mNextResult ( 0 ),
	mQueryMask ( 0xffffffff ),
	mPriority ( UNKNOWN_PRIORITY ),
	mWorldBounds ( ZLBounds::EMPTY ),
	mHitGranularity ( HIT_TEST_COARSE ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAINode )
	RTTI_END
	
	this->mLinkInCell.Data ( this );
}

//----------------------------------------------------------------//
MOAIPartitionHull::~MOAIPartitionHull () {

	// MOAIPartition retains member props so it *should* be impossible
	// to destruct a prop without first removing it
	assert ( !this->mPartition );
	assert ( !this->mCell );
}

//----------------------------------------------------------------//
bool MOAIPartitionHull::PrepareForInsertion ( const MOAIPartition& partition ) {

	return this->MOAIPartitionHull_PrepareForInsertion ( partition );
}

//----------------------------------------------------------------//
void MOAIPartitionHull::RegisterLuaClass ( MOAILuaState& state ) {
	MOAINode::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_PARTITION",				AttrID::Pack ( ATTR_PARTITION ).ToRaw ());
	state.SetField ( -1, "ATTR_WORLD_BOUNDS_TRAIT",		AttrID::Pack ( ATTR_WORLD_BOUNDS_TRAIT ).ToRaw ());
	
	state.SetField ( -1, "HIT_TEST_COARSE",				( u32 )HIT_TEST_COARSE );
	state.SetField ( -1, "HIT_TEST_MEDIUM",				( u32 )HIT_TEST_MEDIUM );
	state.SetField ( -1, "HIT_TEST_FINE",				( u32 )HIT_TEST_FINE );
}

//----------------------------------------------------------------//
void MOAIPartitionHull::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAINode::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "getPartition",			_getPartition },
		{ "getPriority",			_getPriority },
		{ "getWorldBounds",			_getWorldBounds },
		{ "getWorldBoundsCenter",	_getWorldBoundsCenter },
		{ "inside",					_inside },
		{ "setHitGranularity",		_setHitGranularity },
		{ "setPartition",			_setPartition },
		{ "setPriority",			_setPriority },
		{ "setQueryMask",			_setQueryMask },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIPartitionHull::SetPartition ( MOAIPartition* partition ) {

	if ( partition != this->mPartition ) {
		if ( partition ) {
			partition->InsertHull ( *this );
		}
		else if ( this->mPartition ) {
			this->mPartition->RemoveHull ( *this );
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionHull::UpdateWorldBounds ( const ZLBounds& bounds ) {

	MOAIPartitionCell* prevCell = this->mCell;
	ZLBox prevBounds = this->mWorldBounds.mAABB;

	this->mWorldBounds = bounds;

	if ( bounds.mStatus == ZLBounds::ZL_BOUNDS_OK ) {
		this->mWorldBounds.UpdateStatus ();
	}

	if ( this->mPartition ) {
		
		this->mPartition->UpdateHull ( *this );
		
		if (( prevCell != this->mCell ) || ( !prevBounds.IsSame ( this->mWorldBounds.mAABB ))) {
			this->BoundsDidChange ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionHull::WasRemovedFromPartition () {

	this->MOAIPartitionHull_WasRemovedFromPartition ();
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
bool MOAIPartitionHull::MOAINode_ApplyAttrOp ( MOAIAttrID attrID, MOAIAttribute& attr, u32 op ) {

	if ( AttrID::Check ( attrID )) {
		
		switch ( attrID.Unpack ()) {;
		
			case ATTR_PARTITION:
				this->SetPartition ( attr.ApplyVariantNoAdd < MOAIPartition* >( this->GetPartition (), op, MOAIAttribute::ATTR_READ_WRITE ));
				return true;
			
			case ATTR_WORLD_BOUNDS_TRAIT:
				attr.ApplyVariantNoAdd < const ZLBounds* >( &this->mWorldBounds, op, MOAIAttribute::ATTR_READ );
				return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIPartitionHull::MOAIPartitionHull_AddToSortBuffer ( MOAIPartitionResultBuffer& buffer, u32 key ) {

	ZLVec3D center;
	this->mWorldBounds.mAABB.GetCenter ( center );

	buffer.PushResult ( *this, key, NO_SUBPRIM_ID, this->GetPriority (), center, this->mWorldBounds.mAABB, ZLVec3D::ORIGIN );
}

//----------------------------------------------------------------//
void MOAIPartitionHull::MOAIPartitionHull_BoundsDidChange () {
}

//----------------------------------------------------------------//
bool MOAIPartitionHull::MOAIPartitionHull_Inside ( ZLVec3D vec, float pad ) {
	
	ZLBounds bounds = this->mWorldBounds;
	
	if ( bounds.mStatus == ZLBounds::ZL_BOUNDS_EMPTY ) return false;
	if ( bounds.mStatus == ZLBounds::ZL_BOUNDS_GLOBAL ) return true;
	
	bounds.mAABB.Inflate ( pad );
	bounds.mAABB.Bless ();
	return bounds.mAABB.Contains ( vec );
}

//----------------------------------------------------------------//
bool MOAIPartitionHull::MOAIPartitionHull_PrepareForInsertion ( const MOAIPartition& partition ) {
	UNUSED ( partition );
	return true;
}

//----------------------------------------------------------------//
void MOAIPartitionHull::MOAIPartitionHull_WasRemovedFromPartition () {
}
