// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIPropWithDeckAndGrid.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getGrid
	@text	Get the grid currently connected to the prop.
	
	@in		MOAIPropWithDeckAndGrid self
	@out	MOAIGrid grid		Current grid or nil.
*/
int MOAIPropWithDeckAndGrid::_getGrid ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPropWithDeckAndGrid, "U" )
	
	if ( self->mGrid ) {
		self->mGrid->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setGrid
	@text	Sets or clears the prop's grid indexer. The grid indexer (if any)
			will override the standard indexer.
	
	@in		MOAIPropWithDeckAndGrid self
	@opt	MOAIGrid grid		Default value is nil.
	@out	nil
*/
int MOAIPropWithDeckAndGrid::_setGrid ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPropWithDeckAndGrid, "U" )
	
	MOAIGrid* grid = state.GetLuaObject < MOAIGrid >( 2, true );
	if ( !grid ) return 0;
	
	self->mGrid.Set ( *self, grid );
	self->ScheduleUpdate ();
	
	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
/**	@lua	setGridScale
	@text	Scale applied to deck items before rendering to grid cell.
	
	@in		MOAIPropWithDeckAndGrid self
	@opt	number xScale		Default value is 1.
	@opt	number yScale		Default value is 1.
	@out	nil
*/
int MOAIPropWithDeckAndGrid::_setGridScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPropWithDeckAndGrid, "U" )

	self->mGridScale.mX = state.GetValue < float >( 2, 1.0f );
	self->mGridScale.mY = state.GetValue < float >( 3, 1.0f );
	
	self->ScheduleUpdate ();
	
	MOAI_LUA_RETURN_SELF
}

//================================================================//
// MOAIPropWithDeckAndGrid
//================================================================//

//----------------------------------------------------------------//
void MOAIPropWithDeckAndGrid::DrawGrid ( MOAIRenderPhaseEnum::_ renderPhase, const ZLGridCoord &c0, const ZLGridCoord &c1 ) {

	MOAIGfxMgr& gfxMgr = this->Get < MOAIGfxMgr >();

	ZLVec3D offset	= ZLVec3D::ORIGIN;
	ZLVec3D scale	= ZLVec3D::AXIS;
	
	assert ( this->mGrid );
	MOAIGrid& grid = *this->mGrid;
	
	float tileWidth = grid.GetTileWidth ();
	float tileHeight = grid.GetTileHeight ();
	
	// is this grid fancy?
	MOAIFancyGrid* fancyGrid = this->mGrid->AsType < MOAIFancyGrid >();
	
	const ZLAffine3D& modelToWorldMtx = this->GetWorldDrawingMtx ();
	ZLColorVec penColor = gfxMgr.GetPenColor ();
	
	for ( int y = c0.mY; y <= c1.mY; ++y ) {
		for ( int x = c0.mX; x <= c1.mX; ++x ) {
			
			ZLIndex addr = grid.GetCellAddr ( x, y );
			u32 idx = grid.GetTile ( addr );
			
			if ( !idx || ( idx & ZLTileFlags::HIDDEN )) continue;
			
			ZLGridCoord coord ( x, y );
			ZLVec2D loc = grid.GetTilePoint ( coord, MOAIGridSpace::TILE_CENTER );

			float xScale = ( idx & ZLTileFlags::XFLIP ) ? -tileWidth : tileWidth;
			float yScale = ( idx & ZLTileFlags::YFLIP ) ? -tileHeight : tileHeight;

			ZLAffine3D mtx = modelToWorldMtx;

			if ( idx & ZLTileFlags::ROT_90 ) {
				mtx.PrependRot90SclTr2D ( xScale, yScale, loc.mX, loc.mY );
			}
			else {
				mtx.PrependSclTr2D ( xScale, yScale, loc.mX, loc.mY );
			}

			gfxMgr.SetMtx ( MOAIGfxMgr::MODEL_TO_WORLD_MTX, mtx );
			
			if ( fancyGrid ) {
				gfxMgr.SetPenColor ( penColor * fancyGrid->GetTileColor ( addr ));
			}
			
			this->mDeck->Render (( idx & ZLTileFlags::CODE_MASK ) - 1, renderPhase );
		}
	}
}

//----------------------------------------------------------------//
//void MOAIPropWithDeckAndGrid::GatherSurfaces ( MOAISurfaceSampler2D& sampler ) {
//	UNUSED ( sampler );

	//if ( !this->mDeck ) return;
	//
	//sampler.SetSourcePrim ( this );
	//
	//if ( this->mGrid ) {
	//
	//	ZLRect localRect = sampler.GetLocalRect ();
	//
	//	ZLGridCoord c0;
	//	ZLGridCoord c1;
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
void MOAIPropWithDeckAndGrid::GetGridFrameInView ( const ZLAffine3D& worldToLocalMtx, ZLGridCoord& c0, ZLGridCoord& c1 ) {

	const ZLFrustum& frustum = this->Get < MOAIGfxMgr >().GetViewVolume ();
	
	ZLRect viewRect;
	//if ( frustum.GetXYSectRect ( this->GetWorldToLocalMtx (), viewRect )) {
	if ( frustum.GetXYSectRect ( worldToLocalMtx, viewRect )) {
	
		// TODO: need to take into account perspective and truncate rect based on horizon
		// TODO: consider bringing back poly to tile scanline converter...

		ZLRect deckFrame = this->mDeck->GetBounds ().mAABB.GetRect ( ZLBox::PLANE_XY );

		this->mGrid->GetFrameInRect ( viewRect, c0, c1, deckFrame );
	}
}

//----------------------------------------------------------------//
MOAIPropWithDeckAndGrid::MOAIPropWithDeckAndGrid ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	MOAIEventSource ( context ),
	MOAIInstanceEventSource ( context ),
	MOAINode ( context ),
	MOAIAbstractPickable ( context ),
	MOAIPartitionHull ( context ),
	MOAIHasGfxScriptsForPhases ( context ),
	MOAIAbstractRenderable ( context ),
	MOAIAbstractBaseTransform ( context ),
	MOAIAbstractChildTransform ( context ),
	MOAITransform ( context ),
	MOAIAbstractProp ( context ),
	MOAIHasDeck ( context ),
	mGridScale ( 1.0f, 1.0f ) {
	
	RTTI_BEGIN ( MOAIPropWithDeckAndGrid )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIPropWithDeckAndGrid >)
		RTTI_VISITOR ( MOAIAbstractLuaSerializationVisitor, MOAILuaSerializationVisitor < MOAIPropWithDeckAndGrid >)
		RTTI_EXTEND ( MOAIHasDeck )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIPropWithDeckAndGrid::~MOAIPropWithDeckAndGrid () {
	
	this->mGrid.Set ( *this, 0 );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIPropWithDeckAndGrid::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIPropWithDeckAndGrid::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getGrid",				_getGrid },
		{ "setGrid",				_setGrid },
		{ "setGridScale",			_setGridScale },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIPropWithDeckAndGrid::_SerializeIn ( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer ) {
	if ( history.Visit ( *this )) return;

	this->mGrid.Set ( *this, serializer.MemberIDToObject < MOAIGrid >( state.GetFieldValue < cc8*, MOAISerializerBase::ObjID >( -1, "mGrid", 0 )));
}

//----------------------------------------------------------------//
void MOAIPropWithDeckAndGrid::_SerializeOut ( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer ) {
	if ( history.Visit ( *this )) return;

	state.SetField ( -1, "mGrid", serializer.AffirmMemberID ( this->mGrid ));
}

//----------------------------------------------------------------//
ZLBounds MOAIPropWithDeckAndGrid::MOAIAbstractProp_GetModelBounds () {
	
	if ( this->mGrid ) {
		if ( this->mGrid->GetRepeat ()) {
			return ZLBounds::GLOBAL;
		}
		return ZLBounds ( this->mGrid->GetFrame ());
	}
	return ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
MOAIPickResult MOAIPropWithDeckAndGrid::MOAIAbstractProp_PickByPoint ( ZLVec3D loc ) {
	UNUSED ( loc );
	
	// TODO:
	
	return MOAIPickResult ();
}

//----------------------------------------------------------------//
MOAIPickResult MOAIPropWithDeckAndGrid::MOAIAbstractProp_PickByRay ( ZLVec3D loc, ZLVec3D normal ) {
	UNUSED ( loc );
	UNUSED ( normal );
	
	// TODO:
	
	return MOAIPickResult ();
}

//----------------------------------------------------------------//
void MOAIPropWithDeckAndGrid::MOAIAbstractRenderNode_Render ( MOAIRenderPhaseEnum::_ renderPhase ) {
	
	if ( renderPhase == MOAIRenderPhaseEnum::RENDER_PHASE_DRAW_DEBUG ) {
		this->DrawDebug ();
		return;
	}
	
	ZLGridCoord c0, c1;

	MOAIGfxMgr& gfxMgr = this->Get < MOAIGfxMgr >();
	u32 subPrimID = gfxMgr.GetIndex ();

	if ( subPrimID == MOAIPartitionHull::NO_SUBPRIM_ID ) {
		this->GetGridFrameInView ( this->GetWorldToLocalMtx (), c0, c1 );
	}
	else {
		c0 = c1 = this->mGrid->GetCellCoord ( subPrimID );
	}
	this->DrawGrid ( renderPhase, c0, c1 );
}

//----------------------------------------------------------------//
bool MOAIPropWithDeckAndGrid::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {
	
	if ( MOAIHasDeck::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	if ( MOAIAbstractProp::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	return false;
}

//----------------------------------------------------------------//
void MOAIPropWithDeckAndGrid::MOAIPartitionHull_AddToSortBuffer ( MOAIPartitionResultBuffer& buffer, u32 key ) {

	if (( this->mFlags & FLAGS_EXPAND_FOR_SORT ) && this->mGrid && this->mDeck ) {

		// add a sub-prim for each visible grid cell
		const ZLAffine3D& mtx = this->GetLocalToWorldMtx ();

		MOAIGrid& grid = *this->mGrid;

		ZLGridCoord c0;
		ZLGridCoord c1;

		// TODO: this needs to be pushed up one level to GatherHulls
		// should not assume anything about the view or rendering
		// only need to do this if we have a frustum - will break
		// expected results for other queries
		this->GetGridFrameInView ( this->GetWorldToLocalMtx (), c0, c1 );

		for ( int y = c0.mY; y <= c1.mY; ++y ) {
			for ( int x = c0.mX; x <= c1.mX; ++x ) {

				ZLGridCoord wrap = grid.WrapCellCoord ( x, y );
				u32 idx = grid.GetTile ( wrap.mX, wrap.mY );
				if ( !idx || ( idx & ZLTileFlags::HIDDEN )) continue;

				ZLGridCoord coord ( x, y );
				int subPrimID = grid.GetCellAddr ( coord );

				ZLVec3D loc;
				loc.Init ( grid.GetTilePoint ( coord, MOAIGridSpace::TILE_CENTER ));

				ZLBox aabb = this->mDeck->GetBounds ( idx ).mAABB;
				aabb.Offset ( loc );

				mtx.Transform ( loc );
				aabb.Transform ( mtx );

				buffer.PushResult ( *this, key, subPrimID, this->GetPriority (), loc, aabb, this->GetPiv ());
			}
		}
	}
	else {
		buffer.PushResult ( *this, key, NO_SUBPRIM_ID, this->GetPriority (), this->GetWorldLoc (), this->GetWorldBounds ().mAABB, this->GetPiv ());
	}
}

