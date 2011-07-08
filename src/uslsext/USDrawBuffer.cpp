// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USCanvas.h>
#include <uslsext/USDrawBuffer.h>
#include <uslsext/USGfxDevice.h>
#include <uslsext/USTexture.h>

//================================================================//
// USDrawBuffer
//================================================================//

//----------------------------------------------------------------//
void USDrawBuffer::BeginPrim () {

	if ( this->mPrimSize ) {

		u32 primBytes = this->mPrimSize * this->mVertexFormat.GetVertexSize ();

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

	u32 vertexSize = this->mVertexFormat.GetVertexSize ();

	if ( this->mPrimCount && vertexSize ) {
	
		u32 count = this->mPrimSize ? this->mPrimCount * this->mPrimSize : ( u32 )( this->mTop / vertexSize );
		glDrawArrays ( this->mPrimType, 0, count );
	}
}

//----------------------------------------------------------------//
void USDrawBuffer::DrawPrims ( const USVertexFormat& format, GLenum primType, void* buffer, u32 size ) {

	if ( !( buffer && size )) return;
	
	this->Flush ();
	
	// load the software render state
	glColor4f ( this->mPenColor.mR, this->mPenColor.mG, this->mPenColor.mB, this->mPenColor.mA );
	
	glMatrixMode ( GL_MODELVIEW );
	USCanvas::LoadMatrix ( this->mVtxTransform );
	
	glMatrixMode ( GL_TEXTURE );
	USCanvas::LoadMatrix ( this->mUVTransform );
	
	// draw the prims
	u32 nVerts = ( u32 )( size / format.GetVertexSize ());
	if ( nVerts ) {
		
		format.Bind ( buffer );
		glDrawArrays ( primType, 0, nVerts );
	}
	
	// reset
	this->mVertexFormat.Clear ();
	this->mVertexPreset = USVertexFormatMgr::CUSTOM_FORMAT;
	
	glColor4f ( 1.0f, 1.0f, 1.0f, 1.0f );
	
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ();
	
	glMatrixMode ( GL_TEXTURE );
	glLoadIdentity ();
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
	
	// clear the vertex format
	this->mVertexFormat.Clear ();
	this->mVertexPreset = USVertexFormatMgr::CUSTOM_FORMAT;

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
	
	// reset the current vertex color
	glColor4f ( 1.0f, 1.0f, 1.0f, 1.0f );
	
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
		this->Flush ();
		glDisable ( GL_BLEND );
		this->mBlendEnabled = false;
	}
}

//----------------------------------------------------------------//
void USDrawBuffer::SetBlendMode ( const USBlendMode& blendMode ) {

	if ( !( this->mBlendEnabled && this->mBlendMode.IsSame ( blendMode ))) {
		this->Flush ();
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
		this->Flush ();
		this->mPenWidth = penWidth;
		glLineWidth (( GLfloat )penWidth );
	}
}

//----------------------------------------------------------------//
void USDrawBuffer::SetPointSize ( float pointSize ) {

	if ( this->mPointSize != pointSize ) {
		this->Flush ();
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
bool USDrawBuffer::SetTexture ( USTexture* texture ) {
	
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
void USDrawBuffer::SetUVTransform () {

	this->mUVTransform.Ident ();	
}

//----------------------------------------------------------------//
void USDrawBuffer::SetUVTransform ( const USAffine2D& uvTransform ) {

	this->mUVTransform.Init ( uvTransform );	
}

//----------------------------------------------------------------//
void USDrawBuffer::SetVertexFormat () {

	this->Flush ();
	this->mVertexFormat.Clear ();
}

//----------------------------------------------------------------//
void USDrawBuffer::SetVertexFormat ( const USVertexFormat& format ) {

	if ( !this->mVertexFormat.IsMatch ( format )) {

		this->Flush ();
		this->mVertexPreset = USVertexFormatMgr::CUSTOM_FORMAT;

		this->mVertexFormat = format;

		this->mVertexFormat.Bind ( this->mBuffer );
		this->mVertexColorType = format.GetColorType ();
	}
	
}

//----------------------------------------------------------------//
void USDrawBuffer::SetVertexPreset ( u32 presetID ) {

	if ( this->mVertexPreset != presetID ) {

		this->Flush ();
		this->mVertexPreset = presetID;
		
		const USVertexFormat& preset = USVertexFormatMgr::Get ().GetPreset ( presetID );
		this->mVertexFormat = preset;

		this->mVertexFormat.Bind ( this->mBuffer );
		this->mVertexColorType = preset.GetColorType ();
	}
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
	mVertexPreset ( USVertexFormatMgr::CUSTOM_FORMAT ),
	mVertexColorType ( 0 ),
	mBuffer ( 0 ),
	mSize ( 0 ),
	mTop ( 0 ),
	mPrimTop ( 0 ),
	mPrimType ( GL_POINTS ),
	mPrimSize ( 0 ),
	mPrimCount ( 0 ),
	mMaxPrims ( 0 ),
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

//----------------------------------------------------------------//
void USDrawBuffer::WriteQuad ( USVec2D* vtx, USVec2D* uv ) {

	this->mVtxTransform.TransformQuad ( vtx );
	this->mUVTransform.Transform ( uv [ 0 ] );
	this->mUVTransform.Transform ( uv [ 1 ] );	
	this->mUVTransform.Transform ( uv [ 2 ] );	
	this->mUVTransform.Transform ( uv [ 3 ] );	
	
	this->BeginPrim();
		this->Write ( vtx[ 3 ] );
		this->Write ( uv [ 3 ] );
		this->WritePenColor();	
	
		this->Write ( vtx[ 1 ] );
		this->Write ( uv [ 1 ] );
		this->WritePenColor();	
	
		this->Write ( vtx[ 0 ] );
		this->Write ( uv [ 0 ] );
		this->WritePenColor();
	this->EndPrim();
	
	this->BeginPrim();
		this->Write ( vtx[ 3 ] );
		this->Write ( uv [ 3 ] );
		this->WritePenColor();	
	
		this->Write ( vtx[ 2 ] );
		this->Write ( uv [ 2 ] );
		this->WritePenColor();	
	
		this->Write ( vtx[ 1 ] );
		this->Write ( uv [ 1 ] );
		this->WritePenColor();
	this->EndPrim();
	
}
