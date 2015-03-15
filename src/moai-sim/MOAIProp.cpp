// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICamera.h>
#include <moai-sim/MOAICollisionShape.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDeckRemapper.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAILayer.h>
#include <moai-sim/MOAILayoutFrame.h>
#include <moai-sim/MOAIRenderMgr.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIProp.h>
#include <moai-sim/MOAIScissorRect.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAISurfaceSampler2D.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAIViewport.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getBounds
	@text	Return the prop's local bounds or 'nil' if prop bounds is
			global or missing. The bounds are in model space and will
			be overridden by the prop's bounds if it's been set (using
			setBounds ())
	
	@in		MOAIProp self
	@out	number xMin
	@out	number yMin
	@out	number zMin
	@out	number xMax
	@out	number yMax
	@out	number zMax
*/
int MOAIProp::_getBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )
	
	ZLBox bounds;

	u32 status = self->GetModelBounds ( bounds );
	if ( status != BOUNDS_OK ) return 0;

	state.Push ( bounds.mMin.mX );
	state.Push ( bounds.mMin.mY );
	state.Push ( bounds.mMin.mZ );
	
	state.Push ( bounds.mMax.mX );
	state.Push ( bounds.mMax.mY );
	state.Push ( bounds.mMax.mZ );

	return 6;
}

//----------------------------------------------------------------//
/**	@lua	getDeck
	@text	Get the deck.
               
	@in		MOAIProp self
	@out	MOAIDeck deck
*/
int MOAIProp::_getDeck ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

	if ( self->mDeck ) {
		self->mDeck->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getDims
	@text	Return the prop's width and height or 'nil' if prop rect is global.
               
	@in		MOAIProp self
	@out	number width		X max - X min
	@out	number height		Y max - Y min
	@out	number depth		Z max - Z min
*/
int MOAIProp::_getDims ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

	ZLBox bounds;

	u32 status = self->GetModelBounds ( bounds );
	if ( status != BOUNDS_OK ) return 0;
 
	state.Push ( bounds.mMax.mX - bounds.mMin.mX );
	state.Push ( bounds.mMax.mY - bounds.mMin.mY );
	state.Push ( bounds.mMax.mZ - bounds.mMin.mZ );
 
	return 3;
}

//----------------------------------------------------------------//
/**	@lua	getGrid
	@text	Get the grid currently connected to the prop.
	
	@in		MOAIProp self
	@out	MOAIGrid grid		Current grid or nil.
*/
int MOAIProp::_getGrid ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )
	
	if ( self->mGrid ) {
		self->mGrid->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getIndex
	@text	Gets the value of the deck indexer.
	
	@in		MOAIProp self
	@out	number index
*/
int MOAIProp::_getIndex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

	lua_pushnumber ( state, self->mIndex );

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getPriority
	@text	Returns the current priority of the node or 'nil' if the
			priority is uninitialized.
	
	@in		MOAIProp self
	@out	number priority		The node's priority or nil.
*/
int MOAIProp::_getPriority ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )
	
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
	
	@in		MOAIProp self
	@out	number xMin
	@out	number yMin
	@out	number zMin
	@out	number xMax
	@out	number yMax
	@out	number zMax
*/
int MOAIProp::_getWorldBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )
	
	self->ForceUpdate ();
	
	if ( self->mPartition->IsGlobal ( *self )) return 0;
	if ( self->mPartition->IsEmpty ( *self )) return 0;
	
	ZLBox bounds = self->mWorldBounds;

	state.Push ( bounds.mMin.mX );
	state.Push ( bounds.mMin.mY );
	state.Push ( bounds.mMin.mZ );
	
	state.Push ( bounds.mMax.mX );
	state.Push ( bounds.mMax.mY );
	state.Push ( bounds.mMax.mZ );

	return 6;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIProp::_getWorldBoundsCenter ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )
	
	self->ForceUpdate ();
	
	if ( self->mPartition->IsGlobal ( *self )) return 0;
	if ( self->mPartition->IsEmpty ( *self )) return 0;
	
	ZLBox bounds = self->mWorldBounds;
	
	ZLVec3D center;
	bounds.GetCenter ( center );

	state.Push ( center.mX );
	state.Push ( center.mY );
	state.Push ( center.mZ );

	return 3;
}

//----------------------------------------------------------------//
/**	@lua	inside
	@text	Returns true if the given world space point falls inside
			the prop's bounds.
	
	@in		MOAIProp self
	@in		number x
	@in		number y
	@in		number z
	@opt	number pad			Pad the hit bounds (in the prop's local space)
	@out	boolean isInside
*/
int	MOAIProp::_inside ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

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
/**	@lua	setBounds
	@text	Sets or clears the partition bounds override.
	
	@overload	Clear the bounds override.
	
		@in		MOAIProp self
		@out	nil
	
	@overload	Set the bounds override.
	
		@in		MOAIProp self
		@in		number xMin
		@in		number yMin
		@in		number zMin
		@in		number xMax
		@in		number yMax
		@in		number zMax
		@out	nil
*/
int MOAIProp::_setBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

	if ( state.CheckParams ( 2, "NNNNNN", false )) {

		self->mBoundsOverride = state.GetBox ( 2 );
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
int MOAIProp::_setBoundsPad ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

	self->mBoundsPad.mX	= state.GetValue < float >( 2, 0.0f );
	self->mBoundsPad.mY	= state.GetValue < float >( 3, 0.0f );
	self->mBoundsPad.mZ	= state.GetValue < float >( 4, 0.0f );

	bool pad = (( self->mBoundsPad.mX != 0.0f ) || ( self->mBoundsPad.mY != 0.0f ) || ( self->mBoundsPad.mZ != 0.0f ));
	self->mFlags = pad ? ( self->mFlags | FLAGS_PAD_BOUNDS ) : ( self->mFlags & ~FLAGS_PAD_BOUNDS );

	self->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDeck
	@text	Sets or clears the deck to be indexed by the prop.
	
	@in		MOAIProp self
	@opt	MOAIDeck deck		Default value is nil.
	@out	nil
*/
int MOAIProp::_setDeck ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

	self->mDeck.Set ( *self, state.GetLuaObject < MOAIDeck >( 2, true ));
	self->ScheduleUpdate ();
	return 0;
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
	
	@in		MOAIProp self
	@in		boolean expandForSort	Default value is false.
	@out	nil
*/
int MOAIProp::_setExpandForSort ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

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
/**	@lua	setGrid
	@text	Sets or clears the prop's grid indexer. The grid indexer (if any)
			will override the standard indexer.
	
	@in		MOAIProp self
	@opt	MOAIGrid grid		Default value is nil.
	@out	nil
*/
int MOAIProp::_setGrid ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )
	
	MOAIGrid* grid = state.GetLuaObject < MOAIGrid >( 2, true );
	if ( !grid ) return 0;
	
	self->mGrid.Set ( *self, grid );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setGridScale
	@text	Scale applied to deck items before rendering to grid cell.
	
	@in		MOAIProp self
	@opt	number xScale		Default value is 1.
	@opt	number yScale		Default value is 1.
	@out	nil
*/
int MOAIProp::_setGridScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

	self->mGridScale.mX = state.GetValue < float >( 2, 1.0f );
	self->mGridScale.mY = state.GetValue < float >( 3, 1.0f );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setIndex
	@text	Set the prop's index into its deck.
	
	@in		MOAIProp self
	@opt	number index		Default value is 1.
	@out	nil
*/
int MOAIProp::_setIndex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

	self->mIndex = state.GetValue < u32 >( 2, 1 );
	self->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIProp::_setLayer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )
	
	MOAILayer* layer = state.GetLuaObject < MOAILayer >( 2, true );
	self->SetPartition ( layer ? layer->GetPartition () : 0 );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIProp::_setPartition ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )
	
	MOAIPartition* partition = state.GetLuaObject < MOAIPartition >( 2, true );
	self->SetPartition ( partition );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPriority
	@text	Sets or clears the node's priority. Clear the priority
			to have MOAIPartition automatically assign a priority
			to a node when it is added.
	
	@in		MOAIProp self
	@opt	number priority		Default value is nil.
	@out	nil
*/
int MOAIProp::_setPriority ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )
	
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
int MOAIProp::_setQueryMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

	self->mQueryMask = state.GetValue < u32 >( 2, 0 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setRemapper
	@text	Set a remapper for this prop to use when drawing deck members.
	
	@in		MOAIProp self
	@opt	MOAIDeckRemapper remapper		Default value is nil.
	@out	nil
*/
int MOAIProp::_setRemapper ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProp, "U" )

	MOAIDeckRemapper* remapper = state.GetLuaObject < MOAIDeckRemapper >( 2, true );
	self->SetDependentMember < MOAIDeckRemapper >( self->mRemapper, remapper );
	self->ScheduleUpdate ();
	
	return 0;
}

//================================================================//
// MOAIProp
//================================================================//

//----------------------------------------------------------------//
void MOAIProp::AddToSortBuffer ( MOAIPartitionResultBuffer& buffer, u32 key ) {

	if (( this->mFlags & FLAGS_EXPAND_FOR_SORT ) && this->mGrid && this->mDeck ) {
		
		// add a sub-prim for each visible grid cell
		const ZLAffine3D& mtx = this->GetLocalToWorldMtx ();
		
		MOAIGrid& grid = *this->mGrid;
		
		MOAICellCoord c0;
		MOAICellCoord c1;
		
		// TODO: this needs to be pushed up one level to GatherProps
		// should not assume anything about the view or rendering
		// only need to do this if we have a frustum - will break
		// expected results for other queries
		this->GetGridBoundsInView ( c0, c1 );

		for ( int y = c0.mY; y <= c1.mY; ++y ) {
			for ( int x = c0.mX; x <= c1.mX; ++x ) {
				
				MOAICellCoord wrap = grid.WrapCellCoord ( x, y );
				u32 idx = grid.GetTile ( wrap.mX, wrap.mY );
				if ( !idx || ( idx & MOAITileFlags::HIDDEN )) continue;
				
				MOAICellCoord coord ( x, y );
				int subPrimID = grid.GetCellAddr ( coord );
				
				ZLVec3D loc;
				loc.Init ( grid.GetTilePoint ( coord, MOAIGridSpace::TILE_CENTER ));
				
				ZLBox bounds = this->mDeck->GetBounds ( MOAIDeckRemapper::Remap ( this->mRemapper, this->mIndex ));
				bounds.Offset ( loc );
				
				mtx.Transform ( loc );
				bounds.Transform ( mtx );
				
				buffer.PushResult ( *this, key, subPrimID, this->GetPriority (), loc, this->GetBounds ()); // TODO: should use tile bounds for expand mode
			}
		}
	}
	else {
		buffer.PushResult ( *this, key, NO_SUBPRIM_ID, this->mPriority, this->GetWorldLoc (), this->GetBounds ());
	}
}

//----------------------------------------------------------------//
bool MOAIProp::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {

	if ( MOAIPropAttr::Check ( attrID )) {
		
		switch ( UNPACK_ATTR ( attrID )) {
			case ATTR_INDEX:
				this->mIndex = ZLFloat::ToIndex ( attrOp.Apply (( float )this->mIndex, op, MOAIAttrOp::ATTR_READ_WRITE, MOAIAttrOp::ATTR_TYPE_INT ));
				return true;
			case ATTR_PARTITION:
				this->SetPartition ( attrOp.ApplyNoAdd < MOAIPartition* >( this->GetPartition (), op, MOAIAttrOp::ATTR_READ_WRITE, MOAIAttrOp::ATTR_TYPE_VARIANT ));
				return true;
		}
	}
	
	return MOAITransform::ApplyAttrOp ( attrID, attrOp, op );
}

//----------------------------------------------------------------//
//void MOAIProp::GatherSurfaces ( MOAISurfaceSampler2D& sampler ) {
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
bool MOAIProp::GetCellRect ( ZLRect* cellRect, ZLRect* paddedRect ) {

	if ( !( cellRect || paddedRect )) return false;
	
	if ( this->mLevel ) {
	
		ZLVec3D center;
		this->mWorldBounds.GetCenter ( center );
		
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
		cellRect->Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	}
	
	if ( paddedRect ) {
		paddedRect->Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	}
	
	return false;
}

//----------------------------------------------------------------//
void MOAIProp::GetGridBoundsInView ( MOAICellCoord& c0, MOAICellCoord& c1 ) {

	const ZLFrustum& frustum = MOAIGfxDevice::Get ().GetViewVolume ();
	
	ZLRect viewRect;
	if ( frustum.GetXYSectRect ( this->GetWorldToLocalMtx (), viewRect )) {
	
		// TODO: need to take into account perspective and truncate rect based on horizon
		// TODO: consider bringing back poly to tile scanline converter...

		ZLRect deckBounds = this->mDeck->GetBounds ().GetRect ( ZLBox::PLANE_XY );

		this->mGrid->GetBoundsInRect ( viewRect, c0, c1, deckBounds );
	}
}

//----------------------------------------------------------------//
u32 MOAIProp::GetModelBounds ( ZLBox& bounds ) {

	u32 status = BOUNDS_EMPTY;

	if ( this->mFlags & FLAGS_OVERRIDE_BOUNDS ) {
		bounds = this->mBoundsOverride;
		status = BOUNDS_OK;
	}
	else {
		status = this->OnGetModelBounds ( bounds );
	}
	
	if (( status == BOUNDS_OK ) && ( this->mFlags & FLAGS_PAD_BOUNDS )) {
		bounds.Pad ( this->mBoundsPad.mX, this->mBoundsPad.mY, this->mBoundsPad.mZ );
	}
	
	return status;
}

//----------------------------------------------------------------//
MOAIPartition* MOAIProp::GetPartitionTrait () {

	return this->mPartition;
}

//----------------------------------------------------------------//
bool MOAIProp::Inside ( ZLVec3D vec, float pad ) {

	ZLAffine3D worldToLocal = this->GetWorldToLocalMtx ();
	worldToLocal.Transform ( vec );

	ZLBox bounds;

	u32 status = this->GetModelBounds ( bounds );
	
	if ( status == BOUNDS_EMPTY ) return false;
	
	bool passTrivial = false;
	
	if ( status == BOUNDS_GLOBAL ) {
		passTrivial = true;
	}
	else {
		bounds.Bless ();
		bounds.Inflate ( pad );
		if ( pad != 0 ) bounds.Bless ();
		passTrivial = bounds.Contains ( vec );
	}
	
	// TODO: handle grids
	return ( passTrivial && this->mDeck ) ? this->mDeck->Inside ( MOAIDeckRemapper::Remap ( this->mRemapper, this->mIndex ), vec, pad ) : passTrivial;
}

//----------------------------------------------------------------//
MOAIProp::MOAIProp () :
	mPartition ( 0 ),
	mCell ( 0 ),
	mLevel ( 0 ),
	mNextResult ( 0 ),
	mInterfaceMask ( 0 ),
	mQueryMask ( 0xffffffff ),
	mPriority ( UNKNOWN_PRIORITY ),
	mFlags ( 0 ),
	mIndex ( 1 ),
	mGridScale ( 1.0f, 1.0f ),
	mBoundsPad ( 0.0f, 0.0f, 0.0f ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITransform )
	RTTI_END
	
	this->mLinkInCell.Data ( this );
	this->mWorldBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIProp::~MOAIProp () {

	// MOAIPartition retains member props so it *should* be impossible
	// to destruct a prop without first removing it
	assert ( !this->mPartition );
	assert ( !this->mCell );
	
	this->mDeck.Set ( *this, 0 );
	this->mRemapper.Set ( *this, 0 );
	this->mGrid.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIProp::OnDepNodeUpdate () {
	
	MOAITransform::OnDepNodeUpdate ();
	
	ZLBox propBounds;
	u32 propBoundsStatus = this->GetModelBounds ( propBounds );
	
	// update the prop location in the partition
	propBounds.Transform ( this->mLocalToWorldMtx );
	this->UpdateWorldBounds ( propBounds, propBoundsStatus );
}

//----------------------------------------------------------------//
u32 MOAIProp::OnGetModelBounds ( ZLBox& bounds ) {
	
	if ( this->mGrid ) {
		
		if ( this->mGrid->GetRepeat ()) {
			return BOUNDS_GLOBAL;
		}
		ZLRect rect = this->mGrid->GetBounds ();
		bounds.Init ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMax, 0.0f, 0.0f );
		return this->mGrid->GetRepeat () ? BOUNDS_GLOBAL : BOUNDS_OK;
	}
	else if ( this->mDeck ) {
	
		bounds = this->mDeck->GetBounds ( MOAIDeckRemapper::Remap ( this->mRemapper, this->mIndex ));
		return BOUNDS_OK;
	}
	
	return BOUNDS_EMPTY;
}

//----------------------------------------------------------------//
void MOAIProp::OnBoundsChanged () {
}

//----------------------------------------------------------------//
void MOAIProp::OnRemoved () {
}

//----------------------------------------------------------------//
bool MOAIProp::PrepareForInsertion ( const MOAIPartition& partition ) {
	UNUSED ( partition );
	return true;
}

//----------------------------------------------------------------//
void MOAIProp::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAITransform::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_INDEX",					MOAIPropAttr::Pack ( ATTR_INDEX ));
	state.SetField ( -1, "ATTR_PARTITION",				MOAIPropAttr::Pack ( ATTR_PARTITION ));
}

//----------------------------------------------------------------//
void MOAIProp::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAITransform::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getBounds",				_getBounds },
		{ "getDeck",				_getDeck },
		{ "getDims",				_getDims },
		{ "getGrid",				_getGrid },
		{ "getIndex",				_getIndex },
		{ "getPriority",			_getPriority },
		{ "getWorldBounds",			_getWorldBounds },
		{ "getWorldBoundsCenter",	_getWorldBoundsCenter },
		{ "inside",					_inside },
		{ "setBounds",				_setBounds },
		{ "setBoundsPad",			_setBoundsPad },
		{ "setDeck",				_setDeck },
		{ "setExpandForSort",		_setExpandForSort },
		{ "setGrid",				_setGrid },
		{ "setGridScale",			_setGridScale },
		{ "setIndex",				_setIndex },
		{ "setLayer",				_setLayer },
		{ "setPartition",			_setPartition },
		{ "setPriority",			_setPriority },
		{ "setQueryMask",			_setQueryMask },
		{ "setRemapper",			_setRemapper },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIProp::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	MOAITransform::SerializeIn ( state, serializer );
	
	this->mDeck.Set ( *this, serializer.MemberIDToObject < MOAIDeck >( state.GetField < MOAISerializerBase::ObjID >( -1, "mDeck", 0 )));
	this->mGrid.Set ( *this, serializer.MemberIDToObject < MOAIGrid >( state.GetField < MOAISerializerBase::ObjID >( -1, "mGrid", 0 )));
}

//----------------------------------------------------------------//
void MOAIProp::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	
	MOAITransform::SerializeOut ( state, serializer );
	
	state.SetField ( -1, "mDeck", serializer.AffirmMemberID ( this->mDeck ));
	state.SetField ( -1, "mGrid", serializer.AffirmMemberID ( this->mGrid ));
}

//----------------------------------------------------------------//
void MOAIProp::SetPartition ( MOAIPartition* partition ) {

	if ( partition != this->mPartition ) {
		if ( partition ) {
			partition->InsertProp ( *this );
		}
		else if ( this->mPartition ) {
			this->mPartition->RemoveProp ( *this );
		}
	}
}

//----------------------------------------------------------------//
void MOAIProp::UpdateWorldBounds ( u32 status ) {

	ZLBox bounds;
	bounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );

	if ( status == BOUNDS_OK ) {
		status = BOUNDS_EMPTY;
	}
	this->UpdateWorldBounds ( bounds, status );
}

//----------------------------------------------------------------//
void MOAIProp::UpdateWorldBounds ( const ZLBox& bounds, u32 status ) {

	MOAIPartitionCell* prevCell = this->mCell;
	ZLBox prevBounds = this->mWorldBounds;

	this->mWorldBounds = bounds;
	this->mWorldBounds.Bless ();

	if (( status == BOUNDS_OK ) && this->mWorldBounds.IsPoint ()) {
		status = BOUNDS_EMPTY;
	}

	if ( this->mPartition ) {
		this->mPartition->UpdateProp ( *this, status );
		
		if (( prevCell != this->mCell ) || ( !prevBounds.IsSame ( this->mWorldBounds ))) {
			this->OnBoundsChanged ();
		}
	}
}
