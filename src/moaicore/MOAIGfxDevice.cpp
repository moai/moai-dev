// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIGfxUtil.h>
#include <moaicore/MOAITexture.h>
#include <moaicore/MOAIVertexFormat.h>
#include <moaicore/MOAIVertexFormatMgr.h>

//================================================================//
// MOAIGfxDevice
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxDevice::BeginPrim () {

	if ( this->mPrimSize ) {

		u32 primBytes = this->mPrimSize * this->mVertexFormat->GetVertexSize ();

		this->mMaxPrims = ( u32 )( this->mSize / primBytes );
		this->mPrimTop = this->mTop + primBytes;
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::BeginPrim ( u32 primType ) {

	this->SetPrimType ( primType );
	this->BeginPrim ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::Clear () {

	if ( this->mBuffer ) {
		free ( this->mBuffer );
		this->mBuffer = 0;
		this->mSize = 0;
		this->mTop = 0;
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::ClearErrors () {

	while ( glGetError () != GL_NO_ERROR );
}

//----------------------------------------------------------------//
u32 MOAIGfxDevice::CountErrors () {

	u32 count = 0;
	while ( glGetError () != GL_NO_ERROR ) count++;
	return count;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::DrawPrims () {

	if ( this->mVertexFormat ) {

		u32 vertexSize = this->mVertexFormat->GetVertexSize ();

		if ( this->mPrimCount && vertexSize ) {
		
			u32 count = this->mPrimSize ? this->mPrimCount * this->mPrimSize : ( u32 )( this->mTop / vertexSize );
			glDrawArrays ( this->mPrimType, 0, count );
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::DrawPrims ( const MOAIVertexFormat& format, GLenum primType, void* buffer, u32 size ) {

	if ( !( buffer && size )) return;
	
	this->Flush ();
	this->SetVertexFormat ();
	
	// load the software render state
	glColor4f ( this->mPenColor.mR, this->mPenColor.mG, this->mPenColor.mB, this->mPenColor.mA );
	
	glMatrixMode ( GL_MODELVIEW );
	MOAIGfxUtil::LoadMatrix ( this->mVtxTransform );
	
	glMatrixMode ( GL_TEXTURE );
	MOAIGfxUtil::LoadMatrix ( this->mUVTransform );
	
	// draw the prims
	u32 nVerts = ( u32 )( size / format.GetVertexSize ());
	if ( nVerts ) {
		
		format.Bind ( buffer );
		glDrawArrays ( primType, 0, nVerts );
		format.Unbind ();
	}
	
	// reset
	glColor4f ( 1.0f, 1.0f, 1.0f, 1.0f );
	
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ();
	
	glMatrixMode ( GL_TEXTURE );
	glLoadIdentity ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::EndPrim () {

	if ( this->mPrimSize ) {
		this->mTop = this->mPrimTop;
	}
	++this->mPrimCount;
	
	if (( this->mPrimSize == 0 ) || ( this->mPrimCount >= this->mMaxPrims )) {
		this->Flush ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::Flush () {

	this->DrawPrims ();

	this->mTop = 0;
	this->mPrimTop = 0;
	this->mPrimCount = 0;
}

//----------------------------------------------------------------//
const USAffine2D& MOAIGfxDevice::GetCameraTransform () {

	return this->mCameraTransform;
}

//----------------------------------------------------------------//
cc8* MOAIGfxDevice::GetErrorString ( int error ) {

	switch ( error ) {
		case GL_INVALID_ENUM:		return "GL_INVALID_ENUM";
		case GL_INVALID_VALUE:		return "GL_INVALID_VALUE";
		case GL_INVALID_OPERATION:	return "GL_INVALID_OPERATION";
		case GL_STACK_OVERFLOW:		return "GL_STACK_OVERFLOW";
		case GL_STACK_UNDERFLOW:	return "GL_STACK_UNDERFLOW";
		case GL_OUT_OF_MEMORY:		return "GL_OUT_OF_MEMORY";
	}
	return "";
}

//----------------------------------------------------------------//
u32 MOAIGfxDevice::GetHeight () {

	return this->mHeight;
}

//----------------------------------------------------------------//
const USColorVec& MOAIGfxDevice::GetPenColor () {

	return this->mPenColor;
}

//----------------------------------------------------------------//
u32 MOAIGfxDevice::GetPipelineMode () {

	return GL_PIPELINE_PROGRAMMABLE;
}

//----------------------------------------------------------------//
USRect MOAIGfxDevice::GetRect () {

	USRect rect;
	rect.mXMin = 0;
	rect.mYMin = 0;
	rect.mXMax = ( float )this->mWidth;
	rect.mYMax = ( float )this->mHeight;
	
	return rect;
}

//----------------------------------------------------------------//
const USAffine2D& MOAIGfxDevice::GetUVTransform () {

	return this->mUVTransform;
}

//----------------------------------------------------------------//
const USAffine2D& MOAIGfxDevice::GetVtxTransform () {

	return this->mVtxTransform;
}

//----------------------------------------------------------------//
u32 MOAIGfxDevice::GetWidth () {

	return this->mWidth;
}

//----------------------------------------------------------------//
u32 MOAIGfxDevice::PrintErrors () {

	u32 count = 0;
	for ( int error = glGetError (); error != GL_NO_ERROR; error = glGetError (), ++count ) {
		printf ( "OPENGL ERROR: %s\n", this->GetErrorString ( error ));
	}
	return count;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::Rebind () {

	this->Flush ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::Reserve ( u32 size ) {

	this->mSize = size;
	this->mTop = 0;
	this->mBuffer = malloc ( size );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::Reset () {

	this->mTop = 0;
	this->mPrimCount = 0;

	// turn off texture
	glDisable ( GL_TEXTURE_2D );
	this->mTexture = 0;
	
	// turn off blending
	glDisable ( GL_BLEND );
	this->mBlendEnabled = false;
	
	// clear the vertex format
	this->SetVertexFormat ();
	
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
	
	// reset the scissor rect
	MOAIGfxDevice& device = MOAIGfxDevice::Get ();
	USRect scissorRect = device.GetRect ();
	glScissor (( int )scissorRect.mXMin, ( int )scissorRect.mYMin, ( int )scissorRect.Width (), ( int )scissorRect.Height ());
	this->mScissorRect = scissorRect;
	
	// fixed function reset
	if ( MOAIGfxDevice::Get ().GetPipelineMode () == MOAIGfxDevice::GL_PIPELINE_FIXED ) {
		
		// load identity matrix
		glMatrixMode ( GL_MODELVIEW );
		glLoadIdentity ();
		
		// reset the current vertex color
		glColor4f ( 1.0f, 1.0f, 1.0f, 1.0f );
		
		// reset the point size
		glPointSize (( GLfloat )this->mPointSize );
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetBlendMode () {

	if ( this->mBlendEnabled ) {
		this->Flush ();
		glDisable ( GL_BLEND );
		this->mBlendEnabled = false;
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetBlendMode ( const USBlendMode& blendMode ) {

	if ( !( this->mBlendEnabled && this->mBlendMode.IsSame ( blendMode ))) {
		this->Flush ();
		glBlendFunc ( blendMode.mSourceFactor, blendMode.mDestFactor );
		glEnable ( GL_BLEND );
		this->mBlendEnabled = true;
		this->mBlendMode = blendMode;
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetBlendMode ( int srcFactor, int dstFactor ) {

	USBlendMode blendMode;
	blendMode.SetBlend ( srcFactor, dstFactor );
	
	this->SetBlendMode ( blendMode );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetCameraTransform () {

	this->mCameraTransform.Ident ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetCameraTransform ( const USAffine2D& cameraTransform ) {

	this->mCameraTransform = cameraTransform;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetPenColor ( u32 color ) {

	this->mPenColor.SetRGBA ( color );
	this->mPackedColor = color;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetPenColor ( const USColorVec& colorVec ) {

	this->mPenColor = colorVec;
	this->mPackedColor = this->mPenColor.PackRGBA ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetPenColor ( float r, float g, float b, float a ) {

	this->mPenColor.Set ( r, g, b, a );
	this->mPackedColor = this->mPenColor.PackRGBA ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetPenWidth ( float penWidth ) {

	if ( this->mPenWidth != penWidth ) {
		this->Flush ();
		this->mPenWidth = penWidth;
		glLineWidth (( GLfloat )penWidth );
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetPointSize ( float pointSize ) {

	if ( this->mPointSize != pointSize ) {
		this->Flush ();
		this->mPointSize = pointSize;
		glPointSize (( GLfloat )pointSize );
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetPrimType ( u32 primType ) {

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
void MOAIGfxDevice::SetScissorRect () {

	MOAIGfxDevice& device = MOAIGfxDevice::Get ();
	this->SetScissorRect ( device.GetRect ());
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetScissorRect ( const USRect& rect ) {
	
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
void MOAIGfxDevice::SetSize ( u32 width, u32 height ) {

	this->mWidth = width;
	this->mHeight = height;
}

//----------------------------------------------------------------//
bool MOAIGfxDevice::SetTexture ( MOAITexture* texture ) {
	
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
void MOAIGfxDevice::SetUVTransform () {

	this->mUVTransform.Ident ();	
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetUVTransform ( const USAffine2D& uvTransform ) {

	this->mUVTransform.Init ( uvTransform );
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetVertexFormat () {

	this->Flush ();
	
	if ( this->mVertexFormat ) {
		this->mVertexFormat->Unbind ();
	}
	this->mVertexFormat = 0;
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetVertexFormat ( const MOAIVertexFormat& format ) {

	if ( this->mVertexFormat != &format ) {

		this->SetVertexFormat ();

		this->mVertexFormat = &format;

		this->mVertexFormat->Bind ( this->mBuffer );
		this->mVertexColorType = format.GetColorType ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetVertexPreset ( u32 preset ) {

	this->SetVertexFormat ( MOAIVertexFormatMgr::Get ().GetPreset ( preset ));
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetVtxTransform () {

	this->mVtxTransform.Ident ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::SetVtxTransform ( const USAffine2D& vtxTransform ) {

	this->mVtxTransform = vtxTransform;
}

//----------------------------------------------------------------//
MOAIGfxDevice::MOAIGfxDevice () :
	mVertexFormat ( 0 ),
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
	mBlendEnabled ( 0 ),
	mWidth ( 0 ),
	mHeight ( 0 ) {
	
	RTTI_SINGLE ( MOAIGfxDevice )
	
	this->Reserve ( DEFAULT_BUFFER_SIZE );
	
	this->mVtxTransform.Ident ();
	this->mUVTransform.Ident ();
	this->mPenColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
}

//----------------------------------------------------------------//
MOAIGfxDevice::~MOAIGfxDevice () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIGfxDevice::WriteQuad ( USVec2D* vtx, USVec2D* uv ) {

	this->mVtxTransform.TransformQuad ( vtx );
	this->mUVTransform.Transform ( uv [ 0 ]);
	this->mUVTransform.Transform ( uv [ 1 ]);	
	this->mUVTransform.Transform ( uv [ 2 ]);	
	this->mUVTransform.Transform ( uv [ 3 ]);	
	
	this->BeginPrim ();
		this->Write ( vtx[ 3 ]);
		this->Write ( uv [ 3 ]);
		this->WritePenColor ();
	
		this->Write ( vtx[ 1 ]);
		this->Write ( uv [ 1 ]);
		this->WritePenColor ();	
	
		this->Write ( vtx[ 0 ]);
		this->Write ( uv [ 0 ]);
		this->WritePenColor ();
	this->EndPrim ();
	
	this->BeginPrim ();
		this->Write ( vtx[ 3 ]);
		this->Write ( uv [ 3 ]);
		this->WritePenColor ();	
	
		this->Write ( vtx[ 2 ]);
		this->Write ( uv [ 2 ]);
		this->WritePenColor ();	
	
		this->Write ( vtx[ 1 ]);
		this->Write ( uv [ 1 ]);
		this->WritePenColor ();
	this->EndPrim ();
}
