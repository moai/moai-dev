// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USMathConsts.h>
#include <uslscore/USQuadCoord.h>

//================================================================//
// USQuadCoord
//================================================================//

float USQuadCoord::mXOff = 0.0f;
float USQuadCoord::mYOff = 0.0f;
float USQuadCoord::mScale = 0.0f;

//----------------------------------------------------------------//
USQuadCoord USQuadCoord::GetParent () {

	USQuadCoord parent;
	
	if ( this->mLevel > 0 ) {
	
		parent.mLevel = this->mLevel - 1;
		parent.mXTile = this->mXTile >> 1;
		parent.mYTile = this->mYTile >> 1;
	}
	else {
		parent = *this;
	}
	
	return parent;
}

//----------------------------------------------------------------//
USRect USQuadCoord::GetRect () {
	
	float tileSize = 1.0f / ( 0x01 << this->mLevel );
	
	USRect rect;
	
	rect.mXMin = this->mXTile * tileSize;
	rect.mXMax = rect.mXMin + tileSize;
	
	rect.mYMax = -( this->mYTile * tileSize );
	rect.mYMin = rect.mYMax - tileSize;
	
	rect.Offset ( mXOff, mYOff );
	
	return rect;
}

//----------------------------------------------------------------//
USRect USQuadCoord::GetRect ( const USRect& frame ) {
	
	float tileSize = 1.0f / ( 0x01 << this->mLevel );
	
	float tileWidth = frame.Width () * tileSize;
	float tileHeight = frame.Height () * tileSize;
	
	USRect rect;
	
	rect.mXMin = frame.mXMin + ( this->mXTile * tileWidth );
	rect.mXMax = rect.mXMin + tileWidth;
	
	rect.mYMax = frame.mYMax - ( this->mYTile * tileHeight );
	rect.mYMin = rect.mYMax - tileHeight;
	
	rect.Offset ( mXOff, mYOff );
	
	return rect;
}
