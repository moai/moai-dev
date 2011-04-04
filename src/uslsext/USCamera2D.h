// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef GFX_CAMERA2D_H
#define GFX_CAMERA2D_H

#include <uslsext/USMatrix2D.h>
#include <uslsext/USViewport.h>

class USViewQuad;

//================================================================//
// USCamera2D
//================================================================//
class USCamera2D {
public:

	USVec2D		mLoc;
	float		mRoll;
	float		mScale;
	
	//----------------------------------------------------------------//
	void			GetViewQuad				( USViewport& viewport, USViewQuad& viewQuad );
	void			SerializeIn				( USLuaState& state );
	void			SerializeOut			( USLuaState& state );
	void			SetScale				( USVec2D& zoomLoc, float scale );
					USCamera2D				();
};

#endif
