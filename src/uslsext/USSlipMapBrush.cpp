// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <math.h>
#include <uslsext/USCanvas.h>
#include <uslsext/USGLQuad.h>
#include <uslsext/USMathConsts.h>
#include <uslsext/USMercator.h>
#include <uslsext/USQuadCoord.h>
#include <uslsext/USSlipMapBrush.h>
#include <uslsext/USSlipMapTile.h>
#include <uslsext/USSlipMapTileSource.h>
#include <uslsext/USUrlMgr.h>
#include <uslsext/USViewQuad.h>

//================================================================//
// USSlipMapBrush
//================================================================//

//----------------------------------------------------------------//
USSlipMapTile& USSlipMapBrush::AffirmQuad ( USQuadCoord quadID, bool zoomed ) {

	USSlipMapTile& quad = this->mQuads [ quadID ];
	if ( quad.mMap ) return quad;
	
	quad.Init ( *this, quadID, zoomed );
	
	//if ( quadID.mLevel == this->mZoom ) {
	//
	//	u32 minZoom = this->GetMinZoom ();
	//	
	//	for ( u32 i = 0; i < 2; ++i ) {
	//		quadID = quadID.GetParent ();
	//		if ( quadID.mLevel < minZoom ) break;
	//		this->AffirmQuad ( quadID, true );
	//	}
	//}
	
	return quad;
}

//----------------------------------------------------------------//
bool USSlipMapBrush::BindTex ( kTexID texID ) {

	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();

	bool ok = false;

	switch ( texID ) {
		case LOADING_TEX: {
			drawBuffer.BindTexture ( &this->mLoadingTex );
			ok = this->mLoadingTex.IsOK ();
			break;
		}
		case LOADING_BG_TEX: {
			drawBuffer.BindTexture ( &this->mLoadingBGTex );
			ok = this->mLoadingBGTex.IsOK ();
			break;
		}
	}
	
	return ok;
}

//----------------------------------------------------------------//
u32 USSlipMapBrush::ClampZoom ( u32 zoom ) {

	u32 min = this->GetMinZoom ();
	u32 max = this->GetMaxZoom ();
	
	if ( zoom < min ) zoom = min;
	if ( zoom > max ) zoom = max;
	
	return zoom;
}	

//----------------------------------------------------------------//
void USSlipMapBrush::Clear() {

	this->PurgeQuads();
	this->mLoadingTex.Release();
	this->mLoadingBGTex.Release();
}

//----------------------------------------------------------------//
u32 USSlipMapBrush::ComputeZoom () {

	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
	USAffine2D modelToWnd = drawBuffer.GetVtxTransform () ;
	
	USAffine2D mtx;
	USCanvas::GetWorldToWndMtx ( mtx );
	modelToWnd.Append ( mtx );
	
	USVec2D xAxis ( this->mFrame.Width (), 0.0f );
	USVec2D yAxis ( 0.0f, this->mFrame.Height ());
	
	modelToWnd.TransformVec ( xAxis );
	modelToWnd.TransformVec ( yAxis );
	
	float xLength = xAxis.Length ();
	float yLength = yAxis.Length ();
	
	float length = MAX ( xLength, yLength );
	float tileSize = ( float )this->GetTileSize ();
	
	//u32 total = ( u32 )(( length / tileSize ) * 0.75f );
	u32 total = ( u32 )( length / tileSize );
	u32 zoom = ( u32 )Log2 (( float )total );

	return zoom;
}

//----------------------------------------------------------------//
void USSlipMapBrush::Draw ( u32 hint, bool zoomed ) {
	
	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
	USGLQuad::BindVertexFormat ( drawBuffer );
	
	USAffine2D invWorldMtx = drawBuffer.GetVtxTransform ();
	invWorldMtx.Inverse ();
	
	USViewQuad viewQuad;
	viewQuad.Init ();
	viewQuad.Transform ( invWorldMtx );
	
	USRect viewRect = viewQuad.mBounds;
	viewRect.Offset ( -this->mFrame.mXMin, -this->mFrame.mYMax );
	viewRect.Scale ( 1.0f / this->mFrame.Width (), 1.0f / this->mFrame.Height ());
	
	USRect bounds = viewRect;
	
	viewRect.Offset ( -USQuadCoord::mXOff, -USQuadCoord::mYOff );
	
	USQuadCoord quadID;
	quadID.mLevel = this->mZoom;
	
	int total = 1 << this->mZoom;
	
	int left = ( int )( viewRect.mXMin * total );
	int right = ( int )( viewRect.mXMax * total );
	int top = -( int )( viewRect.mYMax * total );
	int bottom = -( int )( viewRect.mYMin * total );
	
	if ( left < 0 ) left = 0;
	if ( right >= total ) right = total - 1;
	
	if ( top < 0 ) top = 0;
	if ( bottom >= total ) bottom = total - 1;
	
	for ( int y = top; y <= bottom; ++y ) {
		for ( int x = left; x <= right; ++x ) {
			
			quadID.mXTile = ( u32 )x;
			quadID.mYTile = ( u32 )y;
			
			USSlipMapTile& quad = this->AffirmQuad ( quadID, zoomed );
			quad.Draw ( this->mTime, this->mFrame, hint );
			
			// tiles get reused, so cannot check state change by texture ID...
			drawBuffer.Flush ();
		}
	}

	this->UpdateQuads ( bounds );
}

//----------------------------------------------------------------//
float USSlipMapBrush::GetCamScaleFromZoom ( u32 zoom ) {

	zoom = this->ClampZoom ( zoom );
	return ( float )( 1 << zoom );
}

//----------------------------------------------------------------//
u32 USSlipMapBrush::GetMaxZoom () {

	return USSlipMapTileSource::GetMaxZoom ( this->mSourceID );
}

//----------------------------------------------------------------//
u32 USSlipMapBrush::GetMinZoom () {

	return USSlipMapTileSource::GetMinZoom ( this->mSourceID );
}

//----------------------------------------------------------------//
u32 USSlipMapBrush::GetTileSize () {

	return USSlipMapTileSource::GetTileSize ( this->mSourceID );
}

//----------------------------------------------------------------//
STLString USSlipMapBrush::GetTileStoreKey ( USQuadCoord& quadID ) {

	return USSlipMapTileSource::GetTileStoreKey ( this->mSourceID, quadID );
}

//----------------------------------------------------------------//
STLString USSlipMapBrush::GetTileURL ( USQuadCoord& quadID ) {

	return USSlipMapTileSource::GetTileURL ( this->mSourceID, quadID );
}

//----------------------------------------------------------------//
u32 USSlipMapBrush::GetZoom () {

	return this->mZoom;
}

//----------------------------------------------------------------//
u32 USSlipMapBrush::GetZoomFromCamScale ( float scale ) {

	int zoom = ( int )Log2 ( scale );
	return this->ClampZoom ( zoom );
}

//----------------------------------------------------------------//
USVec2D64 USSlipMapBrush::ProjectMapToPolar ( double x, double y ) {
	
	USVec2D64 loc2d;
	
	loc2d.mX = ( double )((( x - this->mFrame.mXMin ) / this->mFrame.Width ()) - 0.5 );
	loc2d.mY = ( double )((( y - this->mFrame.mYMin ) / this->mFrame.Height ()) - 0.5 );
	
	loc2d.Scale ( TWOPI );
	
	loc2d = USMercator::ProjectInv ( loc2d.mX, loc2d.mY );
	
	return loc2d;
}

//----------------------------------------------------------------//
USVec2D USSlipMapBrush::ProjectPolarToMap ( double lon, double lat ) {
	
	USVec2D64 loc2d = USMercator::Project ( lon, lat );
	loc2d.Scale ( 1.0f / TWOPI );
	
	USVec2D loc2f;
	
	loc2f.mX = this->mFrame.mXMin + (( float )( loc2d.mX + 0.5 ) * this->mFrame.Width ());
	loc2f.mY = this->mFrame.mYMin + (( float )( loc2d.mY + 0.5 ) * this->mFrame.Height ());
	
	return loc2f;
}

//----------------------------------------------------------------//
void USSlipMapBrush::PurgeQuads () {

	this->mQuads.clear ();
}

//----------------------------------------------------------------//
void USSlipMapBrush::SetFileCache ( USFileCache* fileCache, int cachePriority ) {

	this->mFileCache = fileCache;
	this->mCachePriority = cachePriority;
}

//----------------------------------------------------------------//
void USSlipMapBrush::SetLoadingTiles ( cc8* loadingTile, cc8* bgTile ) {

	this->mLoadingTex.Init ( loadingTile );
	this->mLoadingBGTex.Init ( bgTile );
}

//----------------------------------------------------------------//
void USSlipMapBrush::SetSourceID ( u32 sourceID ) {

	if ( this->mSourceID != sourceID ) {
		this->PurgeQuads ();
		this->mSourceID = sourceID;
	}
}

//----------------------------------------------------------------//
void USSlipMapBrush::SetZoom ( u32 zoom ) {

	this->mZoom = this->ClampZoom ( zoom );
}

//----------------------------------------------------------------//
USSlipMapBrush::USSlipMapBrush () :
	mSourceID ( USSlipMapTileSource::OSM_STREET ),
	mTime ( 0.0f ),
	mQueueSize ( 12 ),
	mZoom ( 0 ),
	mExtraLevels ( 2 ),
	mPrecacheLevels ( 2 ),
	mViewPad ( 2.5f ),
	mMaxTiles ( 28 ),
	mFileCache ( 0 ),
	mCachePriority ( 0 ) {
	
	this->mFrame.Init ( 0.0f, -1.0f, 1.0f, 0.0f );
}

//----------------------------------------------------------------//
USSlipMapBrush::~USSlipMapBrush () {
}

//----------------------------------------------------------------//
void USSlipMapBrush::Update ( float step ) {

	this->mTime += step;
	USUrlMgr::Get ().Process ();
}

//----------------------------------------------------------------//
void USSlipMapBrush::UpdateQuads ( USRect& bounds ) {
	
	bounds.Bless ();

	u32 loading = 0;
	USSlipMapTile* loadQuad = NULL;
	
	if ( this->mQuads.size () <= this->mMaxTiles ) {
		bounds.Inflate ( bounds.Width () * this->mViewPad );
	}
	
	QuadIt quadIt = this->mQuads.begin ();
	while ( quadIt != this->mQuads.end ()) {
		
		QuadIt eraseIt = quadIt;
		++quadIt;
		
		USQuadCoord quadID = eraseIt->first;
		USSlipMapTile& quad = eraseIt->second;
		
		u32 state = quad.GetState ();
		
		// if we're loading, count and skip
		if ( state == USSlipMapTile::LOADING ) {
			++loading;
			continue;
		}
		
		if ( state == USSlipMapTile::KILL_ME ) {
			this->mQuads.erase ( eraseIt );
			continue;
		}
		
		// purge if depth too great
		if ( quadID.mLevel > ( this->mZoom + this->mExtraLevels )) {
			this->mQuads.erase ( eraseIt );
			continue;
		}
		
		// purge if depth too low
		if (( int )quadID.mLevel < (( int )this->mZoom - ( int )this->mPrecacheLevels )) {
			this->mQuads.erase ( eraseIt );
			continue;
		}
		
		// purge if out of bounds
		USRect quadRect = quadID.GetRect ();
		if ( !bounds.Overlap ( quadRect )) {
			this->mQuads.erase ( eraseIt );
			continue;
		}
		
		if ( state == USSlipMapTile::PRE_LOAD ) {
			
			if ( loadQuad == NULL ) {
				loadQuad = &quad;
			}
			else if ( quad.mWaitCounter > loadQuad->mWaitCounter ) {
				loadQuad = &quad;
			}
		}
	}
	
	if (( loadQuad != NULL ) && ( loading < this->mQueueSize )) {
		loadQuad->LoadTile ();
	}
}
