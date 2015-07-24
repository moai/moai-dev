// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-gfx/headers.h>
#include <zl-gfx/zl_gfx.h>
#include <zl-gfx/ZLGfxImmediate.h>

//================================================================//
// ZLGfxImmediate
//================================================================//

//----------------------------------------------------------------//
void ZLGfxImmediate::ActiveTexture ( u32 textureUnit ) {

	zglActiveTexture ( textureUnit );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::AttachShader ( ZLGfxHandle* program, ZLGfxHandle* shader ) {

	zglAttachShader ( program ? program->mGLID : 0, shader ? shader->mGLID : 0 );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BindAttribLocation ( ZLGfxHandle* program, u32 index, cc8* name ) {

	zglBindAttribLocation ( program ? program->mGLID : 0, index, name );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BindBuffer ( u32 target, ZLGfxHandle* handle ) {

	zglBindBuffer ( target, handle ? handle->mGLID : 0 );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BindFramebuffer ( u32 target, ZLGfxHandle* handle ) {

	zglBindFramebuffer ( target, handle ? handle->mGLID : 0 );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BindRenderbuffer ( ZLGfxHandle* handle ) {

	zglBindRenderbuffer ( handle ? handle->mGLID : 0 );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BindTexture ( ZLGfxHandle* handle ) {

	zglBindTexture ( handle ? handle->mGLID : 0 );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BindVertexArray ( ZLGfxHandle* handle ) {

	zglBindVertexArray ( handle ? handle->mGLID : 0 );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BlendFunc ( u32 sourceFactor, u32 destFactor ) {

	zglBlendFunc ( sourceFactor, destFactor );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BlendMode ( u32 mode ) {

	zglBlendMode ( mode );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BufferData ( u32 target, u32 size, ZLRevBufferEdition* buffer, size_t offset, u32 usage ) {

	const void* addr = buffer ? buffer->GetData () : 0;
	zglBufferData ( target, size, ( const void* )(( size_t )addr + offset ), usage );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BufferSubData ( u32 target, u32 offset, u32 size, ZLRevBufferEdition* buffer, size_t srcOffset ) {

	const void* addr = buffer ? buffer->GetData () : 0;
	zglBufferSubData ( target, offset, size, ( const void* )(( size_t )addr + offset ));
}

//----------------------------------------------------------------//
void ZLGfxImmediate::CheckFramebufferStatus ( u32 target ) {

	u32 status = zglCheckFramebufferStatus ( target );
	
	if ( status == ZGL_FRAMEBUFFER_STATUS_COMPLETE ) {
		this->mError = false;
	}
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Clear ( u32 mask ) {

	zglClear ( mask );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::ClearColor ( float r, float g, float b, float a ) {

	zglClearColor ( r, g, b, a );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Color ( float r, float g, float b, float a ) {

	zglColor ( r, g, b, a );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::CompileShader ( ZLGfxHandle* shader, bool verbose ) {

	zglCompileShader ( shader ? shader->mGLID : 0 );
	
	s32 status;
	zglGetShaderiv ( shader->mGLID, ZGL_SHADER_INFO_COMPILE_STATUS, &status );
	
	if ( status == 0 ) {
	
		this->mError = true;
	
		if ( verbose ) {
	
			s32 logLength;
			zglGetShaderiv ( shader->mGLID, ZGL_SHADER_INFO_LOG_LENGTH, &logLength );

			if ( logLength > 1 ) {
				char* log = ( char* )malloc ( logLength );
				zglGetShaderInfoLog ( shader->mGLID, logLength, ( u32* )&logLength, log );
				printf ( "%s\n", log );
				free ( log );
			}
		}
	}
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Create ( ZLGfxHandle* handle, u32 param ) {

	if ( handle ) {
	
		switch ( handle->mType ) {
		
			case ZLGfxHandle::BUFFER:
				handle->mGLID = zglCreateBuffer ();
				break;
				
			case ZLGfxHandle::FRAMEBUFFER:
				handle->mGLID = zglCreateFramebuffer ();
				break;
				
			case ZLGfxHandle::PROGRAM:
				handle->mGLID = zglCreateProgram ();
				break;
				
			case ZLGfxHandle::SHADER:
				handle->mGLID = zglCreateShader ( param );
				break;
				
			case ZLGfxHandle::TEXTURE:
				handle->mGLID = zglCreateTexture ();
				break;
				
			case ZLGfxHandle::RENDERBUFFER:
				handle->mGLID = zglCreateRenderbuffer ();
				break;
				
			case ZLGfxHandle::VERTEXARRAY:
				handle->mGLID = zglCreateVertexArray ();
				break;
		}
	}
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxImmediate::CreateBuffer () {

	return new ZLGfxHandle ( ZLGfxHandle::BUFFER, zglCreateBuffer (), true );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxImmediate::CreateFramebuffer () {

	return new ZLGfxHandle ( ZLGfxHandle::FRAMEBUFFER, zglCreateFramebuffer (), true );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxImmediate::CreateProgram () {

	return new ZLGfxHandle ( ZLGfxHandle::PROGRAM, zglCreateProgram (), true );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxImmediate::CreateRenderbuffer () {

	return new ZLGfxHandle ( ZLGfxHandle::RENDERBUFFER, zglCreateRenderbuffer (), true );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxImmediate::CreateShader ( u32 shaderType ) {

	return new ZLGfxHandle ( ZLGfxHandle::SHADER, zglCreateShader ( shaderType ), true );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxImmediate::CreateTexture () {

	return new ZLGfxHandle ( ZLGfxHandle::TEXTURE, zglCreateTexture (), true );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxImmediate::CreateVertexArray () {

	return new ZLGfxHandle ( ZLGfxHandle::VERTEXARRAY, zglCreateVertexArray (), true );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::CullFace ( u32 mode ) {

	zglCullFace ( mode );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DeleteHandle ( ZLGfxHandle* handle ) {

	if ( !handle ) return;
	
	switch ( handle->mOwns && handle->mType ) {
	
		case ZLGfxHandle::BUFFER:
			zglDeleteBuffer ( handle->mGLID );
			break;
		
		case ZLGfxHandle::FRAMEBUFFER:
			zglDeleteFramebuffer ( handle->mGLID );
			break;
		
		case ZLGfxHandle::PROGRAM:
			zglDeleteProgram ( handle->mGLID );
			break;
		
		case ZLGfxHandle::SHADER:
			zglDeleteShader ( handle->mGLID );
			break;
		
		case ZLGfxHandle::TEXTURE:
			zglDeleteTexture ( handle->mGLID );
			break;
		
		case ZLGfxHandle::RENDERBUFFER:
			zglDeleteRenderbuffer ( handle->mGLID );
			break;
		
		case ZLGfxHandle::VERTEXARRAY:
			zglDeleteVertexArray ( handle->mGLID );
			break;
	}
	delete handle;
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DepthFunc ( u32 depthFunc ) {

	zglDepthFunc ( depthFunc );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DepthMask ( bool flag ) {

	zglDepthMask ( flag );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Disable ( u32 cap ) {

	zglDisable ( cap );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DisableClientState ( u32 cap ) {

	zglDisableClientState ( cap );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DisableVertexAttribArray ( u32 index ) {

	zglDisableVertexAttribArray ( index );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DrawArrays ( u32 primType, u32 first, u32 count ) {

	zglDrawArrays ( primType, first, count );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DrawElements ( u32 primType, u32 count, u32 indexType, ZLRevBufferEdition* buffer, size_t offset ) {

	const void* addr = buffer ? buffer->GetData () : 0;
	zglDrawElements ( primType, count, indexType, ( const void* )(( size_t )addr + offset ));
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Enable ( u32 cap ) {

	zglEnable ( cap );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::EnableClientState ( u32 cap ) {

	zglEnableClientState ( cap );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::EnableVertexAttribArray ( u32 index ) {

	zglEnableVertexAttribArray ( index );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Flush () {

	zglFlush ();
}

//----------------------------------------------------------------//
void ZLGfxImmediate::FramebufferRenderbuffer ( u32 target, u32 attachment, ZLGfxHandle* renderbuffer ) {

	zglFramebufferRenderbuffer ( target, attachment, renderbuffer->mGLID );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::FramebufferTexture2D ( u32 target, u32 attachment, ZLGfxHandle* texture, s32 level ) {

	zglFramebufferTexture2D ( target, attachment, texture->mGLID, level );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxImmediate::GetCurrentFramebuffer () {

	return new ZLGfxHandle ( ZLGfxHandle::FRAMEBUFFER, zglGetCurrentFramebuffer (), false );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::GetUniformLocation ( ZLGfxHandle* program, cc8* uniformName, ZLGfxListener* listener, void* userdata ) {

	if ( listener) {
		u32 addr = zglGetUniformLocation ( ZLGfxHandle::GLID ( program ), uniformName );
		listener->OnUniformLocation ( addr, userdata );
	}
}

//----------------------------------------------------------------//
void ZLGfxImmediate::LineWidth ( float width ) {

	zglLineWidth ( width );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::LinkProgram ( ZLGfxHandle* program, bool verbose ) {

	zglLinkProgram ( program ? program->mGLID : 0 );
	
	s32 status;
	zglGetProgramiv ( program->mGLID, ZGL_PROGRAM_INFO_LINK_STATUS, &status );
	
	if ( status == 0 ) {
	
		this->mError = true;
	
		if ( verbose ) {
	
			s32 logLength;
			zglGetProgramiv ( program->mGLID, ZGL_SHADER_INFO_LOG_LENGTH, &logLength );

			if ( logLength > 1 ) {
				char* log = ( char* )malloc ( logLength );
				zglGetProgramInfoLog ( program->mGLID, logLength, ( u32* )&logLength, log );
				printf ( "%s\n", log );
				free ( log );
			}
		}
	}
}

//----------------------------------------------------------------//
void ZLGfxImmediate::PopSection () {
}

//----------------------------------------------------------------//
bool ZLGfxImmediate::PushErrorHandler () {

	return this->mError;
}

//----------------------------------------------------------------//
void ZLGfxImmediate::PushSection () {

	this->mError = false;
}

//----------------------------------------------------------------//
bool ZLGfxImmediate::PushSuccessHandler () {

	return !this->mError;
}

//----------------------------------------------------------------//
void ZLGfxImmediate::RenderbufferStorage ( u32 internalFormat, u32 width, u32 height ) {

	zglRenderbufferStorage ( internalFormat, width, height );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Scissor ( s32 x, s32 y, u32 w, u32 h ) {

	zglScissor ( x, y, w, h );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::ShaderSource ( ZLGfxHandle* shader, u32 count, const char** string, const s32* length ) {

	zglShaderSource ( shader ? shader->mGLID : 0, count, string, length );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::TexEnvi ( u32 pname, s32 param ) {

	zglTexEnvi ( pname, param );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::TexImage2D ( u32 level, u32 internalFormat, u32 width, u32 height, u32 format, u32 type, const void* data ) {

	zglTexImage2D ( level, internalFormat, width, height, format, type, data );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::TexParameteri ( u32 pname, s32 param ) {

	zglTexParameteri ( pname, param );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::TexSubImage2D ( u32 level, s32 xOffset, s32 yOffset, u32 width, u32 height, u32 format, u32 type, const void* data ) {

	zglTexSubImage2D ( level, xOffset, yOffset, width, height, format, type, data );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Uniform1f ( u32 location, float v0 ) {

	zglUniform1f ( location, v0 );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Uniform1i ( u32 location, s32 v0 ) {

	zglUniform1i ( location, v0 );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Uniform4fv ( u32 location, u32 count, const float* value ) {

	zglUniform4fv ( location, count, value );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::UniformMatrix3fv ( u32 location, u32 count, bool transpose, const float* mtx ) {

	zglUniformMatrix3fv ( location, count, transpose, mtx );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::UniformMatrix4fv ( u32 location, u32 count, bool transpose, const float* mtx ) {

	zglUniformMatrix4fv ( location, count, transpose, mtx );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::UseProgram ( ZLGfxHandle* program ) {

	zglUseProgram ( program ? program->mGLID : 0 );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::VertexAttribPointer ( u32 index, u32 size, u32 type, bool normalized, u32 stride, ZLRevBufferEdition* buffer, size_t offset ) {

	const void* addr = buffer ? buffer->GetData () : 0;
	zglVertexAttribPointer ( index, size, type, normalized, stride, ( const void* )(( size_t )addr + offset ));
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Viewport ( s32 x, s32 y, u32 w, u32 h ) {

	zglViewport ( x, y, w, h );
}

//----------------------------------------------------------------//
ZLGfxImmediate::ZLGfxImmediate () :
	mError ( false ) {
}

//----------------------------------------------------------------//
ZLGfxImmediate::~ZLGfxImmediate () {
}
