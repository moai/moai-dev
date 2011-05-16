// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USVertexFormat.h>

//================================================================//
// USVertexFormatElem
//================================================================//

//----------------------------------------------------------------//
void USVertexFormatElem::Bind ( void* buffer, u32 stride ) const {

	if ( this->mIsEnabled ) {
		
		buffer = ( void* )(( size_t )buffer + this->mOffset );
	
		switch ( this->mUse ) {
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
		
		glEnableClientState ( this->mUse );
	}
	else {
		
		glDisableClientState ( this->mUse );
	}
}

//----------------------------------------------------------------//
void USVertexFormatElem::Init ( GLenum use ) {

	this->mUse = use;
	this->mIsEnabled = false;
}

//----------------------------------------------------------------//
bool USVertexFormatElem::IsMatch ( const USVertexFormatElem& elem ) const {

	if ( this->mIsEnabled == elem.mIsEnabled ) {
		if ( !this->mIsEnabled ) return true;
	
		return (
			( this->mSize == elem.mSize ) &&
			( this->mType == elem.mType ) &&
			( this->mOffset == elem.mOffset )
		);
	}
	return false;
}

//----------------------------------------------------------------//
USVertexFormatElem::USVertexFormatElem () :
	mSize ( 0 ),
	mType ( 0 ),
	mOffset ( 0 ),
	mUse ( 0 ),
	mIsEnabled ( false ) {
}

//----------------------------------------------------------------//
void USVertexFormatElem::Set ( GLint size, GLenum type, u32 offset ) {

	this->mSize = size;
	this->mType = type;
	this->mOffset = offset;
	this->mIsEnabled = true;
}

//================================================================//
// USVertexFormat
//================================================================//

//----------------------------------------------------------------//
void USVertexFormat::Bind ( void* buffer ) const {

	this->mElements [ ARRAY_COLOR ].Bind ( buffer, this->mVertexSize );
	this->mElements [ ARRAY_NORMAL ].Bind ( buffer, this->mVertexSize );
	this->mElements [ ARRAY_TEX_COORD ].Bind ( buffer, this->mVertexSize );
	this->mElements [ ARRAY_VERTEX ].Bind ( buffer, this->mVertexSize );
}

//----------------------------------------------------------------//
void USVertexFormat::Clear () {

	this->mVertexSize = 0;
	
	this->mElements [ ARRAY_COLOR ].Init ( GL_COLOR_ARRAY );
	this->mElements [ ARRAY_NORMAL ].Init ( GL_NORMAL_ARRAY );
	this->mElements [ ARRAY_TEX_COORD ].Init ( GL_TEXTURE_COORD_ARRAY );
	this->mElements [ ARRAY_VERTEX ].Init ( GL_VERTEX_ARRAY );
}

//----------------------------------------------------------------//
bool USVertexFormat::ComputeBounds ( void* buffer, u32 size, USRect& bounds ) {

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
bool USVertexFormat::IsMatch ( const USVertexFormat& prevState ) const {

	if ( this->mVertexSize != prevState.mVertexSize ) {
		return false;
	}

	if ( !this->mElements [ ARRAY_VERTEX ].IsMatch ( prevState.mElements [ ARRAY_VERTEX ])) return false;
	if ( !this->mElements [ ARRAY_TEX_COORD ].IsMatch ( prevState.mElements [ ARRAY_TEX_COORD ])) return false;
	if ( !this->mElements [ ARRAY_COLOR ].IsMatch ( prevState.mElements [ ARRAY_COLOR ])) return false;
	if ( !this->mElements [ ARRAY_NORMAL ].IsMatch ( prevState.mElements [ ARRAY_NORMAL ])) return false;
	
	return true;
}

//----------------------------------------------------------------//
USVertexFormat::USVertexFormat () {
}

//----------------------------------------------------------------//
USVertexFormat::~USVertexFormat () {
}
