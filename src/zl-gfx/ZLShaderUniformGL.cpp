// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-gfx/ZLGfx.h>
#include <zl-gfx/ZLShaderUniformGL.h>

//================================================================//
// ZLShaderUniformBaseGL
//================================================================//

//----------------------------------------------------------------//
size_t ZLShaderUniformBaseGL::GetSize () const {

	return this->mWidth * ELEMENT_SIZE;
}

//----------------------------------------------------------------//
bool ZLShaderUniformBaseGL::Init ( u32 type, u32 width ) {
	
	this->mType		= type;
	this->mWidth	= width;
	
	return true;
}

//----------------------------------------------------------------//
ZLShaderUniformBaseGL::ZLShaderUniformBaseGL () :
	mType ( UNIFORM_TYPE_FLOAT ),
	mWidth ( 1 ) {
}

//----------------------------------------------------------------//
ZLShaderUniformBaseGL::~ZLShaderUniformBaseGL () {
}

//================================================================//
// ZLShaderUniformGL
//================================================================//

//----------------------------------------------------------------//
void ZLShaderUniformGL::Bind ( ZLGfx&gfx, const void* buffer ) const {

	if ( this->mGPUBase == ZGL_INVALID_UNIFORM_ADDR ) return;

	if ( this->mType == UNIFORM_TYPE_FLOAT ) {

		gfx.UniformFloat ( this->mGPUBase, 0, this->mWidth, this->mCount, ( float* )buffer );
	}
	else if ( this->mType == UNIFORM_TYPE_INT ) {
		
		gfx.UniformInt ( this->mGPUBase, 0, this->mWidth, this->mCount, ( s32* )buffer );
	}
}

//----------------------------------------------------------------//
size_t ZLShaderUniformGL::GetSize () const {

	return this->ZLShaderUniformBaseGL::GetSize () * this->mCount;
}

//----------------------------------------------------------------//
void ZLShaderUniformGL::Init ( u32 type, u32 width, u32 count ) {
	
	this->ZLShaderUniformBaseGL::Init ( type, width );
	this->mCount = count;
	this->mCPUOffset = 0;
}

//----------------------------------------------------------------//
ZLShaderUniformGL::ZLShaderUniformGL () :
	mCount ( 0 ),
	mGPUBase (( u32 )-1 ) {
}

//----------------------------------------------------------------//
ZLShaderUniformGL::~ZLShaderUniformGL () {
}
