// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USDrawBuffer.h>
#include <uslsext/USGfxDevice.h>
#include <uslsext/USTexture.h>

//================================================================//
// USGfxArrayInfo
//================================================================//

//----------------------------------------------------------------//
void USGfxArrayInfo::Bind ( USDrawBuffer& drawBuffer, u32 type, size_t base, u32 stride ) {

	if (( this->mState == STAY_BOUND ) && ( this->mStride != stride )) {
		this->mState = BIND;
	}

	if ( this->mState == UNBIND ) {
		
		drawBuffer.Flush ();
		this->mStride = 0;
		this->mState = STAY_UNBOUND;
		
		glDisableClientState ( type );
	}	
	else if ( this->mState == BIND ) {
		
		drawBuffer.Flush ();
		this->mStride = stride;
		this->mState = STAY_BOUND;
		
		void* buffer = ( void* )( base + this->mOffset );
	
		switch ( type ) {
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
		
		glEnableClientState ( type );
	}
}

//----------------------------------------------------------------//
bool USGfxArrayInfo::Compare ( GLint size, GLenum type, u32 offset ) const {

	return (
		( this->mState != STAY_UNBOUND ) &&
		( this->mSize == size ) &&
		( this->mType == type ) &&
		( this->mOffset == offset )
	);
}

//----------------------------------------------------------------//
USGfxArrayInfo::USGfxArrayInfo () :
	mSize ( 0 ),
	mType ( 0 ),
	mOffset ( 0 ),
	mState ( UNBIND ),
	mStride ( 0 ) {
}

//----------------------------------------------------------------//
void USGfxArrayInfo::Set () {

	if ( this->mState == STAY_BOUND ) {
		this->mState = UNBIND;
	}
}

//----------------------------------------------------------------//
void USGfxArrayInfo::Set ( GLint size, GLenum type, u32 offset ) {

	if ( this->Compare ( size, type, offset )) {
		this->mState = STAY_BOUND;
	}
	else {
		this->mSize = size;
		this->mType = type;
		this->mOffset = offset;
		this->mState = BIND;
	}
}

//----------------------------------------------------------------//
void USGfxArrayInfo::Unbind ( u32 type ) {

	this->mStride = 0;
	this->mState = STAY_UNBOUND;
	
	glDisableClientState ( type );
}

//================================================================//
// USDrawBuffer
//================================================================//

//----------------------------------------------------------------//
void USDrawBuffer::BeginFormat () {

	this->mVertexSize = 0;
	
	this->mArrayInfo [ ARRAY_COLOR ].Set ();
	this->mArrayInfo [ ARRAY_NORMAL ].Set ();
	this->mArrayInfo [ ARRAY_TEX_COORD ].Set ();
	this->mArrayInfo [ ARRAY_VERTEX ].Set ();
}

//----------------------------------------------------------------//
void USDrawBuffer::BeginPrim () {

	if ( this->mPrimSize ) {

		u32 primBytes = this->mPrimSize * this->mVertexSize;

		this->mMaxPrims = ( u32 )( this->mSize / primBytes );
		this->mPrimTop = this->mTop + primBytes;
	}
}

//----------------------------------------------------------------//
void USDrawBuffer::BeginPrim ( u32 primType ) {

	this->SetPrimType ( primType );
	this->BeginPrim ();
}

//----------------------------------------------------------------//
void USDrawBuffer::BindColorArray () {

	USGfxArrayInfo& arrayInfo = this->mArrayInfo [ ARRAY_COLOR ];
	arrayInfo.Set ();
}

//----------------------------------------------------------------//
void USDrawBuffer::BindColorArray ( GLenum type ) {

	USGfxArrayInfo& arrayInfo = this->mArrayInfo [ ARRAY_COLOR ];
	
	u32 offset = this->mVertexSize;
	arrayInfo.Set ( COLOR_SIZE, type, offset );
	this->mVertexSize += this->GetSize ( COLOR_SIZE, type );
}

//----------------------------------------------------------------//
void USDrawBuffer::BindNormalArray () {

	USGfxArrayInfo& arrayInfo = this->mArrayInfo [ ARRAY_NORMAL ];
	arrayInfo.Set ();
}

//----------------------------------------------------------------//
void USDrawBuffer::BindNormalArray ( GLenum type ) {

	USGfxArrayInfo& arrayInfo = this->mArrayInfo [ ARRAY_NORMAL ];
	
	u32 offset = this->mVertexSize;
	arrayInfo.Set ( NORMAL_SIZE, type, offset );
	this->mVertexSize += this->GetSize ( NORMAL_SIZE, type );
}

//----------------------------------------------------------------//
void USDrawBuffer::BindTexCoordArray () {

	USGfxArrayInfo& arrayInfo = this->mArrayInfo [ ARRAY_TEX_COORD ];
	arrayInfo.Set ();
}

//----------------------------------------------------------------//
void USDrawBuffer::BindTexCoordArray ( GLint size, GLenum type ) {

	USGfxArrayInfo& arrayInfo = this->mArrayInfo [ ARRAY_TEX_COORD ];
	
	u32 offset = this->mVertexSize;
	arrayInfo.Set ( size, type, offset );
	this->mVertexSize += this->GetSize ( size, type );
}

//----------------------------------------------------------------//
bool USDrawBuffer::BindTexture ( USTexture* texture ) {
	
	if ( this->mTexture == texture ) {
		return texture ? true : false;
	}
	
	this->Flush ();
	
	if ( texture ) {
		if ( !this->mTexture ) {
			glEnable ( GL_TEXTURE_2D );
		}
		this->mTexture = texture;
		return texture->Bind ();
	}

	if ( this->mTexture ) {
		glDisable ( GL_TEXTURE_2D );
		this->mTexture = 0;
	}
	return false;
}

//----------------------------------------------------------------//
void USDrawBuffer::BindVertexArray () {

	USGfxArrayInfo& arrayInfo = this->mArrayInfo [ ARRAY_VERTEX ];
	arrayInfo.Set ();
}

//----------------------------------------------------------------//
void USDrawBuffer::BindVertexArray ( GLint size, GLenum type ) {

	USGfxArrayInfo& arrayInfo = this->mArrayInfo [ ARRAY_VERTEX ];
	
	u32 offset = this->mVertexSize;
	arrayInfo.Set ( size, type, offset );
	this->mVertexSize += this->GetSize ( size, type );
}

//----------------------------------------------------------------//
void USDrawBuffer::Clear () {

	if ( this->mBuffer ) {
		free ( this->mBuffer );
		this->mBuffer = 0;
		this->mSize = 0;
		this->mTop = 0;
	}
}

//----------------------------------------------------------------//
void USDrawBuffer::DrawPrims () {

	if ( this->mPrimCount && this->mVertexSize ) {
	
		u32 count = this->mPrimSize ? this->mPrimCount * this->mPrimSize : ( u32 )( this->mTop / this->mVertexSize );
		glDrawArrays ( this->mPrimType, 0, count );
	}
}

//----------------------------------------------------------------//
void USDrawBuffer::EndFormat () {

	u32 stride = this->mVertexSize;
	size_t base = ( size_t )this->mBuffer;
	
	this->mArrayInfo [ ARRAY_COLOR ].Bind ( *this, GL_COLOR_ARRAY, base, stride );
	this->mArrayInfo [ ARRAY_NORMAL ].Bind ( *this, GL_NORMAL_ARRAY, base, stride );
	this->mArrayInfo [ ARRAY_TEX_COORD ].Bind ( *this, GL_TEXTURE_COORD_ARRAY, base, stride );
	this->mArrayInfo [ ARRAY_VERTEX ].Bind ( *this, GL_VERTEX_ARRAY, base, stride );
}

//----------------------------------------------------------------//
void USDrawBuffer::EndPrim () {

	if ( this->mPrimSize ) {
		this->mTop = this->mPrimTop;
	}
	++this->mPrimCount;
	
	if (( this->mPrimSize == 0 ) || ( this->mPrimCount >= this->mMaxPrims )) {
		this->Flush ();
	}
}

//----------------------------------------------------------------//
void USDrawBuffer::Flush () {

	this->DrawPrims ();

	this->mTop = 0;
	this->mPrimTop = 0;
	this->mPrimCount = 0;
}

//----------------------------------------------------------------//
const USColorVec& USDrawBuffer::GetPenColor () {

	return this->mPenColor;
}

//----------------------------------------------------------------//
u32 USDrawBuffer::GetSize ( GLint size, GLenum type ) {

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
		
		case GL_FIXED:
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
const USAffine2D& USDrawBuffer::GetUVTransform () {

	return this->mUVTransform;
}

//----------------------------------------------------------------//
const USAffine2D& USDrawBuffer::GetVtxTransform () {

	return this->mVtxTransform;
}

//----------------------------------------------------------------//
void USDrawBuffer::Rebind () {

	this->Flush ();
}

//----------------------------------------------------------------//
void USDrawBuffer::Reserve ( u32 size ) {

	this->mSize = size;
	this->mTop = 0;
	this->mBuffer = malloc ( size );
}

//----------------------------------------------------------------//
void USDrawBuffer::Reset () {

	this->mTop = 0;
	this->mPrimCount = 0;

	// turn off texture
	glDisable ( GL_TEXTURE_2D );
	this->mTexture = 0;
	
	// turn off blending
	glDisable ( GL_BLEND );
	this->mBlendEnabled = false;
	
	// disable vertex arrays
	this->mArrayInfo [ ARRAY_COLOR ].Unbind ( GL_COLOR_ARRAY );
	this->mArrayInfo [ ARRAY_NORMAL ].Unbind ( GL_NORMAL_ARRAY );
	this->mArrayInfo [ ARRAY_TEX_COORD ].Unbind ( GL_TEXTURE_COORD_ARRAY );
	this->mArrayInfo [ ARRAY_VERTEX ].Unbind ( GL_VERTEX_ARRAY );

	// load identity matrix
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ();
	
	// ident the cpu transforms
	this->mUVTransform.Ident ();
	this->mVtxTransform.Ident ();
	
	// disable backface culling
	glDisable ( GL_CULL_FACE );
	
	// reset the pen width
	this->mPenWidth = 1.0f;
	glLineWidth (( GLfloat )this->mPenWidth );
	
	// reset the point size
	this->mPointSize = 1.0f;
	glPointSize (( GLfloat )this->mPointSize );
	
	// reset the scissor rect
	USGfxDevice& device = USGfxDevice::Get ();
	USRect scissorRect = device.GetRect ();
	glScissor (( int )scissorRect.mXMin, ( int )scissorRect.mYMin, ( int )scissorRect.Width (), ( int )scissorRect.Height ());
	this->mScissorRect = scissorRect;
}

//----------------------------------------------------------------//
void USDrawBuffer::SetBlendMode () {

	if ( this->mBlendEnabled ) {
		glDisable ( GL_BLEND );
		this->mBlendEnabled = false;
	}
}

//----------------------------------------------------------------//
void USDrawBuffer::SetBlendMode ( const USBlendMode& blendMode ) {

	if ( !( this->mBlendEnabled && this->mBlendMode.IsSame ( blendMode ))) {
		glBlendFunc ( blendMode.mSourceFactor, blendMode.mDestFactor );
		glEnable ( GL_BLEND );
		this->mBlendEnabled = true;
		this->mBlendMode = blendMode;
	}
}

//----------------------------------------------------------------//
void USDrawBuffer::SetBlendMode ( int srcFactor, int dstFactor ) {

	USBlendMode blendMode;
	blendMode.SetBlend ( srcFactor, dstFactor );
	
	this->SetBlendMode ( blendMode );
}

//----------------------------------------------------------------//
void USDrawBuffer::SetPenColor ( u32 color ) {

	this->mPenColor.SetRGBA ( color );
	this->mPackedColor = color;
}

//----------------------------------------------------------------//
void USDrawBuffer::SetPenColor ( const USColorVec& colorVec ) {

	this->mPenColor = colorVec;
	this->mPackedColor = this->mPenColor.PackRGBA ();
}

//----------------------------------------------------------------//
void USDrawBuffer::SetPenColor ( float r, float g, float b, float a ) {

	this->mPenColor.Set ( r, g, b, a );
	this->mPackedColor = this->mPenColor.PackRGBA ();
}

//----------------------------------------------------------------//
void USDrawBuffer::SetPenWidth ( float penWidth ) {

	if ( this->mPenWidth != penWidth ) {
		this->mPenWidth = penWidth;
		glLineWidth (( GLfloat )penWidth );
	}
}

//----------------------------------------------------------------//
void USDrawBuffer::SetPointSize ( float pointSize ) {

	if ( this->mPointSize != pointSize ) {
		this->mPointSize = pointSize;
		glPointSize (( GLfloat )pointSize );
	}
}

//----------------------------------------------------------------//
void USDrawBuffer::SetPrimType ( u32 primType ) {

	if ( this->mPrimType != primType ) {

		this->Flush ();
		this->mPrimType = primType;

		switch ( primType ) {
		
			case GL_POINTS:
				this->mPrimSize = 1;
				break;
			
			case GL_LINES:
				this->mPrimSize = 2;
				break;
			
			case GL_TRIANGLES:
				this->mPrimSize = 3;
				break;
			
			case GL_LINE_LOOP:
			case GL_LINE_STRIP:
			case GL_TRIANGLE_FAN:
			case GL_TRIANGLE_STRIP:
			default:
				this->mPrimSize = 0;
				break;
		}
	}
}

//----------------------------------------------------------------//
void USDrawBuffer::SetScissorRect () {

	USGfxDevice& device = USGfxDevice::Get ();
	this->SetScissorRect ( device.GetRect ());
}

//----------------------------------------------------------------//
void USDrawBuffer::SetScissorRect ( const USRect& rect ) {
	
	USRect& current = this->mScissorRect;
	
	if (	( current.mXMin != rect.mXMin ) ||
			( current.mYMin != rect.mYMin ) ||
			( current.mXMax != rect.mXMax ) ||
			( current.mYMax != rect.mYMax )) {
	
		this->Flush ();
		glScissor (( int )rect.mXMin, ( int )rect.mYMin, ( int )rect.Width (), ( int )rect.Height ());
		this->mScissorRect = rect;
	}
}

//----------------------------------------------------------------//
void USDrawBuffer::SetUVTransform () {

	this->mUVTransform.Ident ();	
}

//----------------------------------------------------------------//
void USDrawBuffer::SetUVTransform ( const USAffine2D& uvTransform ) {

	this->mUVTransform.Init ( uvTransform );	
}

//----------------------------------------------------------------//
void USDrawBuffer::SetVtxTransform () {

	this->mVtxTransform.Ident ();	
}

//----------------------------------------------------------------//
void USDrawBuffer::SetVtxTransform ( const USAffine2D& vtxTransform ) {

	this->mVtxTransform.Init ( vtxTransform );
}

//----------------------------------------------------------------//
USDrawBuffer::USDrawBuffer () :
	mBuffer ( 0 ),
	mSize ( 0 ),
	mTop ( 0 ),
	mPrimTop ( 0 ),
	mPrimType ( GL_POINTS ),
	mPrimSize ( 0 ),
	mPrimCount ( 0 ),
	mMaxPrims ( 0 ),
	mVertexSize ( 0 ),
	mTexture ( 0 ),
	mPackedColor ( 0xffffffff ),
	mPenWidth ( 1.0f ),
	mPointSize ( 1.0f ),
	mBlendEnabled ( 0 ) {
	
	RTTI_SINGLE ( USDrawBuffer )
	
	this->Reserve ( DEFAULT_BUFFER_SIZE );
	
	this->mVtxTransform.Ident ();
	this->mUVTransform.Ident ();
	this->mPenColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
}

//----------------------------------------------------------------//
USDrawBuffer::~USDrawBuffer () {

	this->Clear ();
}