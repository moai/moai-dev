// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USGLQuad.h>
#include <uslsext/USMathConsts.h>
#include <uslsext/USViewQuad.h>
#include <uslsext/USQuadCoord.h>
#include <uslsext/USSlipMapBrush.h>
#include <uslsext/USSlipMapTile.h>
#include <uslsext/USSlipMapTileSource.h>

//================================================================//
// USSlipMapTile
//================================================================//

//----------------------------------------------------------------//
bool USSlipMapTile::BindTexture () {

	if ( this->mLoadingState == LOADED ) {
	
		USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
		drawBuffer.BindTexture ( &this->mTexture );
		
		if ( !this->mTexture.IsOK ()) {
			this->mLoadingState = FAILED;
			return false;
		}
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void USSlipMapTile::Clear () {

	if ( this->mImageFetcher ) {
		this->mImageFetcher->Cancel ();
		this->mImageFetcher = 0;
	}
}

//----------------------------------------------------------------//
void USSlipMapTile::Draw ( float time, const USRect& frame, u32 hint ) {
	
	if ( !this->mMap ) return;
	
	USRect rect = this->mQuadID.GetRect ( frame );
	
	// draw the tile
	if ( this->BindTexture ()) {
		
		this->mStandIn = false;
		
		USGLQuad glQuad;
	
		glQuad.SetVerts ( rect );
		glQuad.SetUVs ( 0.0f, 1.0f, 1.0f, 0.0f );
		
		glQuad.Draw ();
		return;
	}
	
	// draw the standin
	if (( hint & USSlipMapRenderHint::DRAW_STANDINS ) && ( this->mStandIn )) {
		USSlipMapTile* parent = this->GetParent ();
		if ( parent ) {
			parent->DrawSubQuad ( this->mQuadID, time, frame, hint );
			return;
		}
	}

	// draw the bg
	this->DrawBackground ( rect );
	
	// draw the spinner
	if ( hint & USSlipMapRenderHint::DRAW_SPINNER ) {
		this->DrawSpinner ( rect, time );
	}
}

//----------------------------------------------------------------//
void USSlipMapTile::DrawBackground ( USRect& rect ) {

	if ( !this->mMap->BindTex ( USSlipMapBrush::LOADING_BG_TEX )) return;

	USGLQuad glQuad;
	glQuad.SetVerts ( rect );
	glQuad.SetUVs ( 0.0f, 1.0f, 1.0f, 0.0f );
	
	glQuad.Draw ();
}

//----------------------------------------------------------------//
void USSlipMapTile::DrawSpinner ( USRect& rect, float time ) {

	if ( !this->mMap->BindTex ( USSlipMapBrush::LOADING_TEX )) return;

	USGLQuad glQuad;
	glQuad.SetVerts ( rect );
	glQuad.SetUVs ( 0.0f, 1.0f, 1.0f, 0.0f );
	
	USVec2D loc;
	rect.GetCenter ( loc );
	float width = rect.Width ();
	float height = rect.Height ();
	
	int ticks = (( int )( 16.0f * time ) % 12 );
	float degrees = -30.0f * ( float )ticks;
	
	USMatrix2D transform;
	USMatrix2D append;
	
	transform.Rotate ( degrees * ( float )D2R );
	
	append.Scale ( width, height );
	transform.Append ( append );
	
	append.Translate ( loc.mX, loc.mY );
	transform.Append ( append );
	
	glQuad.SetVerts ( -0.25f, 0.25f, 0.25f, -0.25f );
	glQuad.TransformVerts ( transform );
	
	glQuad.Draw ();
}

//----------------------------------------------------------------//
void USSlipMapTile::DrawSubQuad ( USQuadCoord& subQuadID, float time, const USRect& frame, u32 hint ) {
	
	if ( this->mQuadID.mLevel > subQuadID.mLevel ) return;
	
	USRect subRect = subQuadID.GetRect ( frame );
	int levelDiff = subQuadID.mLevel - this->mQuadID.mLevel;
	
	if ( this->BindTexture () ) {
		
		USRect rect = this->mQuadID.GetRect ( frame );
		
		float uScale = 1.0f / rect.Width ();
		float vScale = 1.0f / rect.Height ();
		
		float u = ( subRect.mXMin - rect.mXMin ) * uScale;
		float v = 1.0f - (( subRect.mYMin - rect.mYMin ) * vScale );
		
		USGLQuad glQuad;
		
		glQuad.SetVerts ( subRect );
		glQuad.SetUVs (
			u,
			v,
			u + ( subRect.Width () * uScale ),
			v - ( subRect.Height () * vScale )
		);
		
		glQuad.Draw ();
		return;
	}
	
	USSlipMapTile* parent = this->GetParent ();
	if ( parent && ( levelDiff <= 1 )) {
		parent->DrawSubQuad ( subQuadID, time, frame, hint );
		return;
	}

	// draw the bg
	this->DrawBackground ( subRect );
	
	// draw the spinner
	if ( hint & USSlipMapRenderHint::DRAW_SPINNER ) {
		this->DrawSpinner ( subRect, time );
	}
}

//----------------------------------------------------------------//
USSlipMapTile* USSlipMapTile::GetParent () {

	if ( !this->mMap ) return 0;
	if ( this->mQuadID.mLevel == this->mMap->GetMinZoom ()) return 0;

	USQuadCoord stackUSQuadCoord = this->mQuadID.GetParent ();
	USSlipMapTile& parent = this->mMap->AffirmQuad ( stackUSQuadCoord, true );

	return &parent;
}

//----------------------------------------------------------------//
u32 USSlipMapTile::GetState () {

	return this->mLoadingState;
}

//----------------------------------------------------------------//
STLString USSlipMapTile::GetVEQuadKey () {
	
	u32 cursor = 0;
	char quadKey [ 32 ];
	
	for ( int i = this->mQuadID.mLevel; i > 0; i-- ) {
	
		int mask = 1 << (i - 1);
		int cell = 0;
		
		if (( this->mQuadID.mXTile & mask ) != 0) {
			cell++;
		}
		
		if (( this->mQuadID.mYTile & mask ) != 0) {
			cell += 2;
		}
		
		sprintf ( &quadKey [ cursor++ ], "%d", cell );
	}
	
	quadKey [ cursor ] = 0;
	
	STLString result ( quadKey );
	return result;
}

//----------------------------------------------------------------//
void USSlipMapTile::Init ( USSlipMapBrush& map, USQuadCoord quadID, bool standIn ) {
	
	if ( this->mMap ) return;
	
	this->mMap = &map;
	this->mQuadID = quadID;
	this->mStandIn = standIn;
}

//----------------------------------------------------------------//
void USSlipMapTile::LoadTile () {
	
	if ( this->mLoadingState != PRE_LOAD ) return;
	
	this->Clear ();
	
	this->mLoadingState = LOADING;
	this->mWaitCounter = 0;
	
	STLString tileURL = this->mMap->GetTileURL ( this->mQuadID );
	STLString storeKey = this->mMap->GetTileStoreKey ( this->mQuadID );
	
	if ( !tileURL.size ()) {
		this->mLoadingState = FAILED;
		return;
	}
	
	this->mImageFetcher = new USHttpTask ();
	this->mImageFetcher->SetDelegate < USSlipMapTile >( this, &USSlipMapTile::LoadTileFinish );
	this->mImageFetcher->SetFileCache ( this->mMap->GetFileCache (), this->mMap->GetCachePriority ());
	this->mImageFetcher->HttpGet ( tileURL, storeKey );
}

//----------------------------------------------------------------//
void USSlipMapTile::LoadTileFinish ( USHttpTask* fetcher ) {
	
	void* data = fetcher->GetData ();
	u32 size = fetcher->GetSize ();

	if ( data ) {
		this->mTexture.Init ( data, size );
		this->mTexture.SetFilter ( GL_LINEAR );
		this->mTexture.SetWrap ( GL_CLAMP_TO_EDGE );
		this->mLoadingState = LOADED;
		this->mFailed = false;
	}
	else {
		this->mLoadingState = PRE_LOAD;
		this->mFailed = true;
	}
	
	this->Clear ();
}

//----------------------------------------------------------------//
USSlipMapTile::USSlipMapTile () :
	mImageFetcher ( 0 ),
	mMap ( 0 ),
	mParent ( 0 ),
	mLoadingState ( PRE_LOAD ),
	mFailed ( false ),
	mStandIn ( false ),
	mWaitCounter ( 0 ) {
}

//----------------------------------------------------------------//
USSlipMapTile::~USSlipMapTile () {

	this->Clear ();
}

