// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USVertexFormat.h>

//================================================================//
// USVertexFormatElem
//================================================================//

//----------------------------------------------------------------//
void USVertexFormatElem::Bind ( void* buffer, u32 stride, GLenum use ) const {

	if ( this->mIsEnabled ) {
		
		buffer = ( void* )(( size_t )buffer + this->mOffset );
	
		switch ( use ) {
			case GL_COLOR_ARRAY:
				glColorPointer ( this->mSize, this->mType, stride, buffer );
				break;
			case GL_NORMAL_ARRAY:
				glNormalPointer ( this->mType, stride, buffer );
				break;
			case GL_TEXTURE_COORD_ARRAY:
				glTexCoordPointer ( this->mSize, this->mType, stride, buffer );
				break;
			case GL_VERTEX_ARRAY:
				glVertexPointer ( this->mSize, this->mType, stride, buffer );
				break;
			default:
				break;
		}
		
		glEnableClientState ( use );
	}
	else {
		
		glDisableClientState ( use );
	}
}

//----------------------------------------------------------------//
void USVertexFormatElem::Set () {

	this->mIsEnabled = false;
}

//----------------------------------------------------------------//
void USVertexFormatElem::Set ( GLint size, GLenum type, u32 offset ) {

	this->mSize = size;
	this->mType = type;
	this->mOffset = offset;
	this->mIsEnabled = true;
}

//----------------------------------------------------------------//
void USVertexFormatElem::Unbind ( GLenum use ) const {

	glDisableClientState ( use );
}


//----------------------------------------------------------------//
USVertexFormatElem::USVertexFormatElem () :
	mSize ( 0 ),
	mType ( 0 ),
	mOffset ( 0 ),
	mIsEnabled ( false ) {
}

//================================================================//
// USVertexFormatAttr
//================================================================//
 	
//----------------------------------------------------------------//
void USVertexFormatAttr::Bind ( void* buffer, u32 stride ) const {

	buffer = ( void* )(( size_t )buffer + this->mOffset );

	glVertexAttribPointer (	this->mIndex, this->mSize, this->mType, this->mNormalized, stride, buffer );
	glEnableVertexAttribArray ( this->mIndex );
}

//----------------------------------------------------------------//
void USVertexFormatAttr::Set ( GLint index, GLint size, GLenum type, GLboolean normalized, u32 offset ) {

	this->mIndex = index;
	this->mSize = size;
	this->mType = type;
	this->mNormalized = normalized;
	this->mOffset = offset;
}

//----------------------------------------------------------------//
void USVertexFormatAttr::Unbind () const {

	glDisableVertexAttribArray ( this->mIndex );
}

//----------------------------------------------------------------//
USVertexFormatAttr::USVertexFormatAttr () {
}

//================================================================//
// USVertexFormat
//================================================================//

//----------------------------------------------------------------//
void USVertexFormat::Bind ( void* buffer ) const {

	if ( this->mTotalAttributes ) {
	
		for ( u32 i = 0; i < this->mTotalAttributes; ++i ) {
			this->mAttributes [ i ].Bind ( buffer, this->mVertexSize );
		}
	}
	else {
		this->mElements [ ARRAY_COLOR ].Bind ( buffer, this->mVertexSize, GL_COLOR_ARRAY );
		this->mElements [ ARRAY_NORMAL ].Bind ( buffer, this->mVertexSize, GL_NORMAL_ARRAY );
		this->mElements [ ARRAY_TEX_COORD ].Bind ( buffer, this->mVertexSize, GL_TEXTURE_COORD_ARRAY );
		this->mElements [ ARRAY_VERTEX ].Bind ( buffer, this->mVertexSize, GL_VERTEX_ARRAY );
	}
}

//----------------------------------------------------------------//
void USVertexFormat::Clear () {

	this->mVertexSize = 0;
	this->mTotalAttributes = 0;
	
	this->mElements [ ARRAY_COLOR ].Set ();
	this->mElements [ ARRAY_NORMAL ].Set ();
	this->mElements [ ARRAY_TEX_COORD ].Set ();
	this->mElements [ ARRAY_VERTEX ].Set ();
}

//----------------------------------------------------------------//
bool USVertexFormat::ComputeBounds ( void* buffer, u32 size, USRect& bounds ) {

	// TODO: GLES2

	u32 total = ( u32 )( size / this->mVertexSize );
	if ( !total ) return false;

	USVertexFormatElem& coordElem = this->mElements [ ARRAY_VERTEX ];
	if ( coordElem.mType != GL_FLOAT ) return false; // TODO: handle other types
	if ( coordElem.mSize < 2 ) return false;
	
	buffer = ( void* )(( size_t )buffer + coordElem.mOffset );
	
	USVec2D* coord = ( USVec2D* )buffer;
	bounds.Init ( *coord );
	bounds.Inflate ( 0.0000001f ); // prevent 'empty' bounds on cardinal direction lines or single vertex objects
	
	for ( u32 i = 1; i < total; ++i ) {
		
		buffer = ( void* )(( size_t )buffer + this->mVertexSize );
		coord = ( USVec2D* )buffer;
		bounds.Grow ( *coord );
	}
	
	return true;
}

//----------------------------------------------------------------//
void USVertexFormat::DeclareAttribute ( GLint index, GLint size, GLenum type, GLboolean normalized ) {

	this->mAttributes.Grow ( this->mTotalAttributes + 1 );

	USVertexFormatAttr& attr = this->mAttributes [ this->mTotalAttributes++ ];
	
	u32 offset = this->mVertexSize;
	attr.Set ( index, size, type, normalized, offset );
	this->mVertexSize += this->GetSize ( size, type );
}

//----------------------------------------------------------------//
void USVertexFormat::DeclareColorArray ( GLenum type ) {

	USVertexFormatElem& elem = this->mElements [ ARRAY_COLOR ];
	
	u32 offset = this->mVertexSize;
	elem.Set ( COLOR_SIZE, type, offset );
	this->mVertexSize += this->GetSize ( COLOR_SIZE, type );
}

//----------------------------------------------------------------//
void USVertexFormat::DeclareNormalArray ( GLenum type ) {

	USVertexFormatElem& elem = this->mElements [ ARRAY_NORMAL ];
	
	u32 offset = this->mVertexSize;
	elem.Set ( NORMAL_SIZE, type, offset );
	this->mVertexSize += this->GetSize ( NORMAL_SIZE, type );
}

//----------------------------------------------------------------//
void USVertexFormat::DeclareTexCoordArray ( GLenum type, GLint size ) {

	USVertexFormatElem& elem = this->mElements [ ARRAY_TEX_COORD ];
	
	u32 offset = this->mVertexSize;
	elem.Set ( size, type, offset );
	this->mVertexSize += this->GetSize ( size, type );
}

//----------------------------------------------------------------//
void USVertexFormat::DeclareVertexArray ( GLenum type, GLint size ) {

	USVertexFormatElem& elem = this->mElements [ ARRAY_VERTEX ];
	
	u32 offset = this->mVertexSize;
	elem.Set ( size, type, offset );
	this->mVertexSize += this->GetSize ( size, type );
}

//----------------------------------------------------------------//
u32 USVertexFormat::GetSize ( GLint size, GLenum type ) {

	u32 bytes;
	switch ( type ) {
	
		case GL_BYTE:
		case GL_UNSIGNED_BYTE:
			bytes = 1;
			break;
		
		case GL_SHORT:
		case GL_UNSIGNED_SHORT:
			bytes = 2;
			break;
		
		//case GL_FIXED:
		case GL_FLOAT:
			bytes = 4;
			break;
		
		default:
			bytes = 0;
			break;		
	}
	
	return size * bytes;
}

//----------------------------------------------------------------//
GLenum USVertexFormat::GetColorType () const {

	return this->mElements [ ARRAY_COLOR ].mType;
}

//----------------------------------------------------------------//
void USVertexFormat::ReserveAttributes ( u32 total ) {

	this->mAttributes.Init ( total );
	this->mTotalAttributes = 0;
}

//----------------------------------------------------------------//
void USVertexFormat::Unbind () const {

	if ( this->mTotalAttributes ) {
	
		for ( u32 i = 0; i < this->mTotalAttributes; ++i ) {
			this->mAttributes [ i ].Unbind ();
		}
	}
	else {
		this->mElements [ ARRAY_COLOR ].Unbind ( GL_COLOR_ARRAY );
		this->mElements [ ARRAY_NORMAL ].Unbind ( GL_NORMAL_ARRAY );
		this->mElements [ ARRAY_TEX_COORD ].Unbind ( GL_TEXTURE_COORD_ARRAY );
		this->mElements [ ARRAY_VERTEX ].Unbind ( GL_VERTEX_ARRAY );
	}
}

//----------------------------------------------------------------//
USVertexFormat::USVertexFormat () :
	mVertexSize ( 0 ),
	mTotalAttributes ( 0 ) {
}

//----------------------------------------------------------------//
USVertexFormat::~USVertexFormat () {
}
