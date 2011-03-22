// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USPartition.h>

//================================================================//
// USPartitionPrimBase
//================================================================//

//----------------------------------------------------------------//
bool USPartitionPrimBase::GetCellRect ( USRect* cellRect, USRect* paddedRect ) {

	if ( !( cellRect || paddedRect )) return false;
	
	if ( this->mLayer ) {
	
		USVec2D center;
		this->mWorldBounds.GetCenter ( center );
		
		USTileCoord coord = this->mLayer->mGridSpace.GetTileCoord ( center );
		USRect rect = this->mLayer->mGridSpace.GetTileRect ( coord );
		
		if ( cellRect ) {
			*cellRect = rect;
		}
		
		if ( paddedRect ) {
			rect.Inflate ( this->mLayer->mCellSize * 0.5f );
			*paddedRect = rect;
		}
		return true;
	}
	
	return false;
}

//----------------------------------------------------------------//
USRect USPartitionPrimBase::GetWorldBounds () {

	return this->mWorldBounds;
}

//----------------------------------------------------------------//
bool USPartitionPrimBase::InCell () {

	return ( this->mCell && this->mLayer );
}

//----------------------------------------------------------------//
void USPartitionPrimBase::PlaceInPartition () {

	this->mWorldBounds.Bless ();

	float width = this->mWorldBounds.Width ();
	float height = this->mWorldBounds.Height ();
	
	float prevSize = this->mCellSize;
	this->mCellSize = ( width > height ) ? width : height;
	
	USPartitionBase* partition = this->GetPartitionBase ();
	
	if ( partition ) {
		
		if ( prevSize != this->mCellSize ) {
			partition->MovePrim ( *this );
		}
		else if ( this->mLayer ) {
			partition->MovePrim ( *this );
		}
	}
}

//----------------------------------------------------------------//
void USPartitionPrimBase::SetWorldBounds ( const USRect& bounds ) {

	this->mWorldBounds = bounds;
	this->PlaceInPartition ();
}

//----------------------------------------------------------------//
void USPartitionPrimBase::SetWorldBoundsEmpty () {

	this->mWorldBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	this->PlaceInPartition ();
}

//----------------------------------------------------------------//
void USPartitionPrimBase::SetWorldBoundsGlobal () {

	this->mWorldBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f );

	USPartitionBase* partition = this->GetPartitionBase ();

	if ( partition ) {
		partition->MakeGlobal ( *this );
	}
}

//----------------------------------------------------------------//
USPartitionPrimBase::USPartitionPrimBase () :
	mData ( 0 ),
	mLayer ( 0 ),
	mCell ( 0 ),
	mQueryMask ( 0xffffffff ),
	mCellSize ( 0.0f ) {
	
	this->mLinkInCell.Data ( this );
	this->mWorldBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
USPartitionPrimBase::~USPartitionPrimBase () {

	if ( this->mCell ) {
		this->mCell->RemovePrim ( *this );
	}
}

//================================================================//
// USPartitionCell
//================================================================//

//----------------------------------------------------------------//
void USPartitionCell::GatherPrims ( USPartitionBase& partition, USPartitionPrimBase* ignore, u32 mask ) {
	
	PrimIt primIt = this->mPrims.Head ();
	for ( ; primIt; primIt = primIt->Next ()) {
		USPartitionPrimBase* prim = primIt->Data ();
		
		if ( ignore ) {
			if ( ignore->mData == prim->mData ) continue;
		}
		
		if (( mask == 0 ) || ( prim->mQueryMask & mask )) {
			partition.PushResult ( prim->mData );
		}
	}
}

//----------------------------------------------------------------//
void USPartitionCell::GatherPrims ( USPartitionBase& partition, USPartitionPrimBase* ignore, USVec2D& point, u32 mask ) {

	PrimIt primIt = this->mPrims.Head ();
	for ( ; primIt; primIt = primIt->Next ()) {
		USPartitionPrimBase* prim = primIt->Data ();
		
		if ( ignore ) {
			if ( ignore->mData == prim->mData ) continue;
		}
		
		if (( mask == 0 ) || ( prim->mQueryMask & mask )) {
			if ( prim->mCellSize > 0.0f ) {
		
				if ( prim->mWorldBounds.Contains ( point )) {
					partition.PushResult ( prim->mData );
				}
			}
			else {
				partition.PushResult ( prim->mData );
			}
		}
	}
}

//----------------------------------------------------------------//
void USPartitionCell::GatherPrims ( USPartitionBase& partition, USPartitionPrimBase* ignore, USRect& rect, u32 mask ) {

	PrimIt primIt = this->mPrims.Head ();
	for ( ; primIt; primIt = primIt->Next ()) {
		USPartitionPrimBase* prim = primIt->Data ();
		
		if ( ignore ) {
			if ( ignore->mData == prim->mData ) continue;
		}
		
		if (( mask == 0 ) || ( prim->mQueryMask & mask )) {
		
			if ( prim->mCellSize > 0.0f ) {
				
				if ( prim->mWorldBounds.Overlap ( rect )) {
					partition.PushResult ( prim->mData );
				}
			}
			else {
				partition.PushResult ( prim->mData );
			}
		}
	}
}

//----------------------------------------------------------------//
void USPartitionCell::InsertPrim ( USPartitionPrimBase& prim ) {

	if ( prim.mCell == this ) return;

	if ( prim.mCell ) {
		prim.mCell->RemovePrim ( prim );
	}
	this->mPrims.PushBack ( prim.mLinkInCell );
	prim.mCell = this;
}

//----------------------------------------------------------------//
void USPartitionCell::RemovePrim ( USPartitionPrimBase& prim ) {

	if ( prim.mCell != this ) return;
	
	this->mPrims.Remove ( prim.mLinkInCell );
	prim.mCell = 0;
}

//----------------------------------------------------------------//
USPartitionCell::USPartitionCell () {
}

//----------------------------------------------------------------//
USPartitionCell::~USPartitionCell () {
}

//================================================================//
// USPartitionLayer
//================================================================//

//----------------------------------------------------------------//
void USPartitionLayer::GatherPrims ( USPartitionBase& partition, USPartitionPrimBase* ignore, u32 mask ) {

	u32 totalCells = this->mCells.Size ();
	for ( u32 i = 0; i < totalCells; ++i ) {
		this->mCells [ i ].GatherPrims ( partition, ignore, mask );
	}
}

//----------------------------------------------------------------//
void USPartitionLayer::GatherPrims ( USPartitionBase& partition, USPartitionPrimBase* ignore, USVec2D point, u32 mask ) {

	float halfSize = this->mCellSize * 0.5f;
	point.mX = point.mX - halfSize;
	point.mY = point.mY - halfSize;

	USTileCoord coord = this->mGridSpace.GetTileCoord ( point );
	
	int width = this->mGridSpace.GetWidth ();
	int height = this->mGridSpace.GetWidth ();
	
	int xTotal = ( width < 2 ) ? 1 : 2;
	int yTotal = ( height < 2 ) ? 1 : 2;
	
	for ( int y = 0; y < yTotal; ++y ) {
		for ( int x = 0; x < xTotal; ++x ) {
			int addr = this->mGridSpace.GetTileAddr ( coord, x, y );
			this->mCells [ addr ].GatherPrims ( partition, ignore, point, mask );
		}
	}
}

//----------------------------------------------------------------//
void USPartitionLayer::GatherPrims ( USPartitionBase& partition, USPartitionPrimBase* ignore, USRect rect, u32 mask ) {

	float halfSize = this->mCellSize * 0.5f;

	rect.Bless ();
	USTileCoord coord = this->mGridSpace.GetTileCoord ( rect.mXMin - halfSize, rect.mYMax + halfSize );

	int xTotal = ( int )( rect.Width () / this->mGridSpace.GetTileWidth ()) + 2;
	int yTotal = ( int )( rect.Height () / this->mGridSpace.GetTileHeight ()) + 2;
	
	int width = this->mGridSpace.GetWidth ();
	int height = this->mGridSpace.GetHeight ();
	
	if ( xTotal > width ) xTotal = width;
	if ( yTotal > height ) yTotal = height;

	for ( int y = 0; y < yTotal; ++y ) {
		for ( int x = 0; x < xTotal; ++x ) {
			u32 addr = this->mGridSpace.GetTileAddr ( coord, x, -y );
			this->mCells [ addr ].GatherPrims ( partition, ignore, rect, mask );
		}
	}
}

//----------------------------------------------------------------//
void USPartitionLayer::Init ( float cellSize, u32 width, u32 height ) {

	this->mCellSize = cellSize;

	this->mGridSpace.SetWidth ( width );
	this->mGridSpace.SetHeight ( height );
	this->mGridSpace.SetTileWidth ( cellSize );
	this->mGridSpace.SetTileHeight ( cellSize );
	
	this->mCells.Init ( this->mGridSpace.GetTotalTiles ());
}

//----------------------------------------------------------------//
USPartitionCell* USPartitionLayer::GetCell ( USPartitionPrimBase& prim ) {

	USVec2D loc;
	prim.mWorldBounds.GetCenter ( loc );
	
	u32 cellAddr = this->mGridSpace.GetTileAddr ( loc );
	return &this->mCells [ cellAddr ];
}

//----------------------------------------------------------------//
void USPartitionLayer::MovePrim ( USPartitionPrimBase& prim ) {

	USVec2D loc;
	prim.mWorldBounds.GetCenter ( loc );
	USTileCoord coord = this->mGridSpace.GetTileCoord ( loc );

	//printf ( "cell %d ( %d, %d )\n", this->mGridSpace.GetTileAddr ( coord ), coord.mX, coord.mY );

	USPartitionCell* cell = this->GetCell ( prim );
	cell->InsertPrim ( prim );
}

//----------------------------------------------------------------//
USPartitionLayer::USPartitionLayer () :
	mCellSize ( 0.0f ) {
}

//----------------------------------------------------------------//
USPartitionLayer::~USPartitionLayer () {
}

//================================================================//
// USPartitionBase
//================================================================//

//----------------------------------------------------------------//
u32 USPartitionBase::GatherPrims ( USPartitionPrimBase* ignore, u32 mask ) {
	
	this->ResetResults ();
	u32 totalLayers = this->mLayers.Size ();
	for ( u32 i = 0; i < totalLayers; ++i ) {
		this->mLayers [ i ].GatherPrims ( *this, ignore, mask );
	}
	this->mGlobals.GatherPrims ( *this, ignore, mask );
	this->mEmpties.GatherPrims ( *this, ignore, mask );
	
	return this->mTotalResults;
}

//----------------------------------------------------------------//
u32 USPartitionBase::GatherPrims ( USVec2D& point, USPartitionPrimBase* ignore, u32 mask ) {
	
	this->ResetResults ();
	u32 totalLayers = this->mLayers.Size ();
	for ( u32 i = 0; i < totalLayers; ++i ) {
		this->mLayers [ i ].GatherPrims ( *this, ignore, point, mask );
	}
	this->mGlobals.GatherPrims ( *this, ignore, point, mask );
	
	return this->mTotalResults;
}

//----------------------------------------------------------------//
u32 USPartitionBase::GatherPrims ( USRect& rect, USPartitionPrimBase* ignore, u32 mask ) {
	
	this->ResetResults ();
	u32 totalLayers = this->mLayers.Size ();
	for ( u32 i = 0; i < totalLayers; ++i ) {
		this->mLayers [ i ].GatherPrims ( *this, ignore, rect, mask );
	}
	this->mGlobals.GatherPrims ( *this, ignore, rect, mask );
	
	return this->mTotalResults;
}

//----------------------------------------------------------------//
void USPartitionBase::Grow () {

	u32 chunks = ( u32 )USFloat::Ceil (( float )this->mTotalPrims / ( float )CHUNK_SIZE );
	u32 size = chunks * CHUNK_SIZE;
	
	if ( size > this->mResults.Size ()) {
		this->mResults.Init ( size );
	}
}

//----------------------------------------------------------------//
void USPartitionBase::InsertPrim ( USPartitionPrimBase& prim, void* data ) {

	prim.mData = data;

	USPartitionBase* partition = prim.GetPartitionBase ();
	if ( partition ) {
		if ( partition == this ) return;
		partition->RemovePrim ( prim );
	}
	
	++this->mTotalPrims;
	this->Grow ();
}

//----------------------------------------------------------------//
void USPartitionBase::MakeGlobal ( USPartitionPrimBase& prim ) {

	//printf ( "global cell\n" );
	prim.mLayer = 0;
	this->mGlobals.InsertPrim ( prim );
}

//----------------------------------------------------------------//
void USPartitionBase::MovePrim ( USPartitionPrimBase& prim ) {

	u32 layerID;

	float cellSize = prim.mCellSize;
	if ( cellSize > 0.0f ) {
		
		USPartitionLayer* layer = 0;
		
		u32 totalLayers = this->mLayers.Size ();
		for ( u32 i = 0; i < totalLayers; ++i ) {
		
			USPartitionLayer* testLayer = &this->mLayers [ i ];
			
			if ( cellSize <= testLayer->mCellSize ) {
				if (( layer == 0 ) || ( testLayer->mCellSize  < layer->mCellSize )) {
					layer = testLayer;
					layerID = i;
				}
			}
		}
		
		prim.mLayer = layer;
		
		if ( layer ) {
			//printf ( "layer %d ", ( int )layerID );
			layer->MovePrim ( prim );
		}
		else {
			//printf ( "global cell\n" );
			this->mGlobals.InsertPrim ( prim );
		}
	}
	else {
		//printf ( "empty cell\n" );
		prim.mLayer = 0;
		this->mEmpties.InsertPrim ( prim );
	}
}

//----------------------------------------------------------------//
void USPartitionBase::RemovePrim ( USPartitionPrimBase& prim ) {

	USPartitionBase* partition = prim.GetPartitionBase ();
	if ( partition != this ) return;
	
	if ( prim.mCell ) {
		prim.mCell->RemovePrim ( prim );
	}
	
	prim.mLayer = 0;
	prim.mCell = 0;
	
	--this->mTotalPrims;
}

//----------------------------------------------------------------//
void USPartitionBase::ReserveLayers ( int totalLayers ) {

	this->mLayers.Init ( totalLayers );
}

//----------------------------------------------------------------//
void USPartitionBase::PushResult ( void* result ) {

	this->mResults [ this->mTotalResults ] = result;
	++this->mTotalResults;
}

//----------------------------------------------------------------//
void USPartitionBase::ResetResults () {

	this->mTotalResults = 0;
}

//----------------------------------------------------------------//
void USPartitionBase::SetLayer ( int layerID, float cellSize, int width, int height ) {

	this->mLayers [ layerID ].Init ( cellSize, width, height );
}

//----------------------------------------------------------------//
USPartitionBase::USPartitionBase () :
	mTotalPrims ( 0 ),
	mTotalResults ( 0 ) {
}

//----------------------------------------------------------------//
USPartitionBase::~USPartitionBase () {
}
