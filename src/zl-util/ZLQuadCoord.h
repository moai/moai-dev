// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLQUADCOORD_H
#define ZLQUADCOORD_H

#include <zl-util/ZLRect.h>

//================================================================//
// ZLQuadCoord
//================================================================//
class ZLQuadCoord {
public:

	u32		mLevel;
	u32		mXTile;
	u32		mYTile;
	u32		mResolution;
	
	//----------------------------------------------------------------//
	bool operator < ( const ZLQuadCoord& mapQuad ) const {
		
		if ( this->mLevel < mapQuad.mLevel ) return true;
		if ( this->mLevel > mapQuad.mLevel ) return false;
		
		if ( this->mXTile < mapQuad.mXTile ) return true;
		if ( this->mXTile > mapQuad.mXTile ) return false;
		
		if ( this->mYTile < mapQuad.mYTile ) return true;
		if ( this->mYTile > mapQuad.mYTile ) return false;
		
		// equal
		return false;
	}
	
	//----------------------------------------------------------------//
	inline void Clear () {
		this->mLevel		= 0;;
		this->mXTile		= 0;
		this->mYTile		= 0;
		this->mResolution	= 1;
	}
	
	//----------------------------------------------------------------//
	inline u32 GetIndex () const {
		return ( u32 )(( this->mYTile * this->mResolution ) + this->mXTile );
	}
	
	//----------------------------------------------------------------//
	inline u8 GetMask () const {

		return ( u8 )((( this->mXTile & 0x01 ) + 1 ) << (( this->mYTile & 0x01 ) << 1 ));
	}
	
	//----------------------------------------------------------------//
	ZLRect GetRect ( float frameWidth = 1.0f, float frameHeight = 1.0f ) const {
		
		float tileSize = 1.0f / ( 0x01 << this->mLevel );
		
		float tileWidth = frameWidth * tileSize;
		float tileHeight = frameHeight * tileSize;
		
		ZLRect rect;
		
		rect.mXMin = this->mXTile * tileWidth;
		rect.mXMax = rect.mXMin + tileWidth;
		
		rect.mYMin = this->mYTile * tileHeight;
		rect.mYMax = rect.mYMin + tileHeight;
		
		return rect;
	}

	//----------------------------------------------------------------//
	ZLRect GetRect ( const ZLRect& frame ) const {
		
		float tileSize = 1.0f / ( 0x01 << this->mLevel );
		
		float tileWidth = frame.Width () * tileSize;
		float tileHeight = frame.Height () * tileSize;
		
		ZLRect rect;
		
		rect.mXMin = frame.mXMin + ( this->mXTile * tileWidth );
		rect.mXMax = rect.mXMin + tileWidth;
		
		rect.mYMax = frame.mYMax + ( this->mYTile * tileHeight );
		rect.mYMin = rect.mYMax + tileHeight;
		
		return rect;
	}
	
	//----------------------------------------------------------------//
	float GetTileSize () const {
		
		return 1.0f / ( 0x01 << this->mLevel );
	}
	
	//----------------------------------------------------------------//
	inline void Offset ( u32 xOff, u32 yOff ) {
	
		this->mXTile += xOff;
		this->mYTile += yOff;
	}
	
	//----------------------------------------------------------------//
	inline void Set ( u32 xOff, u32 yOff ) {
	
		this->mXTile = xOff;
		this->mYTile = yOff;
	}
	
	//----------------------------------------------------------------//
	inline void StepDown () {
	
		this->mLevel++;
		this->mXTile = this->mXTile << 1;
		this->mYTile = this->mYTile << 1;
		this->mResolution = this->mResolution << 1;
	}
	
	//----------------------------------------------------------------//
	inline void StepUp () {
	
		assert ( this->mLevel > 0 );
	
		this->mLevel--;
		this->mXTile = this->mXTile >> 1;
		this->mYTile = this->mYTile >> 1;
		this->mResolution = this->mResolution >> 1;
	}
};

#endif
