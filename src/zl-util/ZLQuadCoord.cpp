// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLMathConsts.h>
#include <zl-util/ZLQuadCoord.h>

//================================================================//
// ZLQuadCoord
//================================================================//

float ZLQuadCoord::mXOff = 0.0f;
float ZLQuadCoord::mYOff = 0.0f;
float ZLQuadCoord::mScale = 0.0f;

//----------------------------------------------------------------//
ZLQuadCoord ZLQuadCoord::GetParent () {

	ZLQuadCoord parent;
	
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
ZLRect ZLQuadCoord::GetRect () {
	
	float tileSize = 1.0f / ( 0x01 << this->mLevel );
	
	ZLRect rect;
	
	rect.mXMin = this->mXTile * tileSize;
	rect.mXMax = rect.mXMin + tileSize;
	
	rect.mYMax = -( this->mYTile * tileSize );
	rect.mYMin = rect.mYMax - tileSize;
	
	rect.Offset ( mXOff, mYOff );
	
	return rect;
}

//----------------------------------------------------------------//
ZLRect ZLQuadCoord::GetRect ( const ZLRect& frame ) {
	
	float tileSize = 1.0f / ( 0x01 << this->mLevel );
	
	float tileWidth = frame.Width () * tileSize;
	float tileHeight = frame.Height () * tileSize;
	
	ZLRect rect;
	
	rect.mXMin = frame.mXMin + ( this->mXTile * tileWidth );
	rect.mXMax = rect.mXMin + tileWidth;
	
	rect.mYMax = frame.mYMax - ( this->mYTile * tileHeight );
	rect.mYMin = rect.mYMax - tileHeight;
	
	rect.Offset ( mXOff, mYOff );
	
	return rect;
}
