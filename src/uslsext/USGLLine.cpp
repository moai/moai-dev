// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USGLLine.h>

//================================================================//
// USGLLine
//================================================================//

//----------------------------------------------------------------//
void USGLLine::Draw () {

	glVertexPointer ( 2, GL_FLOAT, 0, this->mVtx );
	glEnableClientState ( GL_VERTEX_ARRAY );
	
	glDisable ( GL_TEXTURE_2D );
	glDisableClientState ( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState ( GL_COLOR_ARRAY );
	
	glDrawArrays ( GL_LINES, 0, 2 );
}

//----------------------------------------------------------------//
void USGLLine::SetVerts ( USVec2D& v0, USVec2D& v1 ) {

	this->mVtx [ 0 ] = v0.mX;
	this->mVtx [ 1 ] = v0.mY;
	
	this->mVtx [ 2 ] = v1.mX;
	this->mVtx [ 3 ] = v1.mY;
}

//----------------------------------------------------------------//
void USGLLine::SetVerts ( float x0, float y0, float x1, float y1 ) {

	this->mVtx [ 0 ] = x0;
	this->mVtx [ 1 ] = y0;
	
	this->mVtx [ 2 ] = x1;
	this->mVtx [ 3 ] = y1;
}
