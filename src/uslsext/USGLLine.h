// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	GLLINE_H
#define	GLLINE_H

//================================================================//
// USGLLine
//================================================================//
class USGLLine {
private:

	float mVtx [ 4 ];

public:

	//----------------------------------------------------------------//
	void	Draw		();
	void	SetVerts	( USVec2D& v0, USVec2D& v1 );
	void	SetVerts	( float x0, float y0, float x1, float y1 );
};

#endif
