// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef VIEWQUAD_H
#define VIEWQUAD_H

#include <uslsext/USMatrix3D.h>

//================================================================//
// USViewQuad
//================================================================//
class USViewQuad {
public:
	
	USRect	mBounds;
	USVec2D	mPoints [ 4 ];
	
	//----------------------------------------------------------------//
	void	Init		();
	void	Init		( USAffine2D& viewProjMtx );
	void	Transform	( const USAffine2D& mtx );
};

#endif
