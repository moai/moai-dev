// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USQUADCOORD_H
#define USQUADCOORD_H

#include <uslscore/USRect.h>

//================================================================//
// USQuadCoord
//================================================================//
class USQuadCoord {
public:

	static float mXOff;
	static float mYOff;
	static float mScale;

	u32		mLevel;
	u32		mXTile;
	u32		mYTile;
	
	//----------------------------------------------------------------//
	USQuadCoord		GetParent			();
	USRect			GetRect				();
	USRect			GetRect				( const USRect& frame );
	
	//----------------------------------------------------------------//
	bool operator < ( const USQuadCoord& mapQuad ) const {
		
		if ( this->mLevel < mapQuad.mLevel ) return true;
		if ( this->mLevel > mapQuad.mLevel ) return false;
		
		if ( this->mXTile < mapQuad.mXTile ) return true;
		if ( this->mXTile > mapQuad.mXTile ) return false;
		
		if ( this->mYTile < mapQuad.mYTile ) return true;
		if ( this->mYTile > mapQuad.mYTile ) return false;
		
		// equal
		return false;
	}
	
	// back compat w/ quad tree code
	
	u8		mResolution;
	
	//----------------------------------------------------------------//
	inline u32 GetIndex () {
		return ( u32 )(( this->mYTile * this->mResolution ) + this->mXTile );
	}
	
	//----------------------------------------------------------------//
	inline u8 GetMask () {

		return (( this->mXTile & 0x01 ) + 1 ) << (( this->mYTile & 0x01 ) << 1 );
	}
	
	//----------------------------------------------------------------//
	inline bool IsNull () {
		return ( this->mResolution == 0 );
	}
	
	//----------------------------------------------------------------//
	inline void MakeNull () {
		this->mResolution = 0;
	}
	
	//----------------------------------------------------------------//
	inline void Offset ( u8 xOff, u8 yOff ) {
	
		this->mXTile += xOff;
		this->mYTile += yOff;
	}
	
	//----------------------------------------------------------------//
	inline void StepDown () {
	
		assert ( this->mLevel > 0 );
	
		this->mLevel--;
		this->mXTile = ( this->mXTile << 1 );
		this->mYTile = ( this->mYTile << 1 );
		this->mResolution = this->mResolution << 1;
	}
	
	//----------------------------------------------------------------//
	inline void StepUp () {
	
		this->mLevel++;
		this->mXTile = this->mXTile >> 1;
		this->mYTile = this->mYTile >> 1;
		this->mResolution = this->mResolution >> 1;
	}
};

#endif
