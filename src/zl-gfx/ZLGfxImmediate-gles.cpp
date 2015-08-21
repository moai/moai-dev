// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-gfx/headers.h>
#include <zl-gfx/zl_gfx.h>
#include <zl-gfx/ZLGfx-gles.h>
#include <zl-gfx/ZLGfxEnum.h>
#include <zl-gfx/ZLGfxImmediate.h>

//================================================================//
// ZLGfxImmediate
//================================================================//

//----------------------------------------------------------------//
void ZLGfxImmediate::ActiveTexture ( u32 textureUnit ) {

	glActiveTexture ( GL_TEXTURE0 + textureUnit );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::AttachShader ( ZLGfxHandle* program, ZLGfxHandle* shader ) {

	glAttachShader ( program ? program->mGLID : 0, shader ? shader->mGLID : 0 );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BindAttribLocation ( ZLGfxHandle* program, u32 index, cc8* name ) {

	glBindAttribLocation ( program ? program->mGLID : 0, index, name );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BindBuffer ( u32 target, ZLGfxHandle* handle ) {

	glBindBuffer ( target, handle ? handle->mGLID : 0 );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BindFramebuffer ( u32 target, ZLGfxHandle* handle ) {

	glBindFramebuffer ( target, handle ? handle->mGLID : 0 );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BindRenderbuffer ( ZLGfxHandle* handle ) {

	glBindRenderbuffer ( GL_RENDERBUFFER, handle ? handle->mGLID : 0 );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BindTexture ( ZLGfxHandle* handle ) {

	glBindTexture ( GL_TEXTURE_2D, handle ? handle->mGLID : 0 );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BindVertexArray ( ZLGfxHandle* handle ) {

	glBindVertexArray ( handle ? handle->mGLID : 0 );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BlendFunc ( u32 sourceFactor, u32 destFactor ) {

	glBlendFunc ( ZLGfxEnum::MapZLToNative ( sourceFactor ), ZLGfxEnum::MapZLToNative ( destFactor ));
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BlendMode ( u32 mode ) {

	glBlendEquation ( ZLGfxEnum::MapZLToNative ( mode ));
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BufferData ( u32 target, u32 size, ZLRevBufferEdition* buffer, size_t offset, u32 usage ) {

	const GLvoid* data = ( const GLvoid* )(( size_t )( buffer ? buffer->GetData () : 0 ) + offset );
	glBufferData ( ZLGfxEnum::MapZLToNative ( target ), ( GLsizeiptr )size, data, ZLGfxEnum::MapZLToNative ( usage ));
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BufferSubData ( u32 target, u32 offset, u32 size, ZLRevBufferEdition* buffer, size_t srcOffset ) {

	const GLvoid* data = ( const GLvoid* )(( size_t )( buffer ? buffer->GetData () : 0 ) + offset );
	glBufferSubData ( ZLGfxEnum::MapZLToNative ( target ), ( GLintptr )offset, ( GLsizeiptr )size, data );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::CheckFramebufferStatus ( u32 target ) {
	
	GLenum status = glCheckFramebufferStatus ( ZLGfxEnum::MapZLToNative ( target ));
	
	if ( status == ZGL_FRAMEBUFFER_STATUS_COMPLETE ) {
		this->mError = false;
	}
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Clear ( u32 mask ) {

	GLbitfield glMask = 0;

	if ( mask & ZGL_CLEAR_COLOR_BUFFER_BIT ) {
		glMask |= GL_COLOR_BUFFER_BIT;
	}

	if ( mask & ZGL_CLEAR_DEPTH_BUFFER_BIT ) {
		glMask |= GL_DEPTH_BUFFER_BIT;
	}

	if ( mask & ZGL_CLEAR_STENCIL_BUFFER_BIT ) {
		glMask |= GL_STENCIL_BUFFER_BIT;
	}

	glClear ( glMask );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::ClearColor ( float r, float g, float b, float a ) {

	glClearColor ( r, g, b, a );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Color ( float r, float g, float b, float a ) {

	glColor4f ( r, g, b, a );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::CompileShader ( ZLGfxHandle* shader, bool verbose ) {

	GLuint shaderID = ( GLuint )( shader ? shader->mGLID : 0 );

	glCompileShader ( shaderID );
	
	s32 status;
	glGetShaderiv ( shaderID, GL_COMPILE_STATUS, &status );
	
	if ( status == 0 ) {
	
		this->mError = true;
	
		if ( verbose ) {
	
			s32 logLength;
			glGetShaderiv ( shaderID, GL_INFO_LOG_LENGTH, &logLength );

			if ( logLength > 1 ) {
				char* log = ( char* )malloc ( logLength );
				glGetShaderInfoLog ( shaderID, ( GLsizei )logLength, ( GLsizei* )&logLength, ( GLchar* )log );
				printf ( "%s\n", log );
				free ( log );
			}
		}
	}
}

//----------------------------------------------------------------//
void ZLGfxImmediate::CompressedTexImage2D ( u32 level, u32 internalFormat, u32 width, u32 height, u32 imageSize, ZLGfxBufferRef bufferRef ) {
	
	glCompressedTexImage2D (
		GL_TEXTURE_2D,
		( GLint )level,
		( GLint )ZLGfxEnum::MapZLToNative ( internalFormat ),
		( GLsizei )width,
		( GLsizei )height,
		0,
		( GLsizei )imageSize,
		( const GLvoid* )bufferRef.mBuffer
	);
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxImmediate::Create ( ZLGfxHandle* handle, u32 param ) {

	if ( handle ) {
	
		switch ( handle->mType ) {
		
			case ZLGfxHandle::BUFFER:
				glGenBuffers ( 1, &handle->mGLID );
				break;
				
			case ZLGfxHandle::FRAMEBUFFER:
				glGenFramebuffers ( 1, &handle->mGLID );
				break;
				
			case ZLGfxHandle::PROGRAM:
				handle->mGLID = ( u32 )glCreateProgram ();
				break;
				
			case ZLGfxHandle::SHADER:
				handle->mGLID = ( u32 )glCreateShader ( ZLGfxEnum::MapZLToNative ( param ));
				break;
				
			case ZLGfxHandle::TEXTURE:
				glGenTextures ( 1, ( GLuint* )&handle->mGLID );
				break;
				
			case ZLGfxHandle::RENDERBUFFER:
				glGenRenderbuffers ( 1, &handle->mGLID );
				break;
				
			case ZLGfxHandle::VERTEXARRAY:
				#ifdef MOAI_OS_ANDROID
					handle->mGLID = 0;
				#else
					glGenVertexArrays ( 1, &handle->mGLID );
				#endif
				break;
		}
	}
	
	return handle;
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxImmediate::CreateBuffer () {

	return this->Create ( new ZLGfxHandle ( ZLGfxHandle::BUFFER, 0, true ), 0 );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxImmediate::CreateFramebuffer () {

	return this->Create ( new ZLGfxHandle ( ZLGfxHandle::FRAMEBUFFER, 0, true ), 0 );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxImmediate::CreateProgram () {

	return this->Create ( new ZLGfxHandle ( ZLGfxHandle::PROGRAM, 0, true ), 0 );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxImmediate::CreateRenderbuffer () {

	return this->Create ( new ZLGfxHandle ( ZLGfxHandle::RENDERBUFFER, 0, true ), 0 );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxImmediate::CreateShader ( u32 shaderType ) {

	return this->Create ( new ZLGfxHandle ( ZLGfxHandle::SHADER, 0, true ), shaderType );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxImmediate::CreateTexture () {

	return this->Create ( new ZLGfxHandle ( ZLGfxHandle::TEXTURE, 0, true ), 0 );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxImmediate::CreateVertexArray () {

	return this->Create ( new ZLGfxHandle ( ZLGfxHandle::VERTEXARRAY, 0, true ), 0 );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::CullFace ( u32 mode ) {

	glCullFace ( ZLGfxEnum::MapZLToNative ( mode ));
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DeleteHandle ( ZLGfxHandle* handle ) {

	if ( !handle ) return;
	
	switch ( handle->mOwns && handle->mType ) {
	
		case ZLGfxHandle::BUFFER:
			glDeleteBuffers ( 1, &handle->mGLID );
			break;
		
		case ZLGfxHandle::FRAMEBUFFER:
			glDeleteFramebuffers ( 1, &handle->mGLID );
			break;
		
		case ZLGfxHandle::PROGRAM:
			glDeleteProgram ( handle->mGLID );
			break;
		
		case ZLGfxHandle::SHADER:
			glDeleteShader ( handle->mGLID );
			break;
		
		case ZLGfxHandle::TEXTURE:
			glDeleteTextures ( 1, &handle->mGLID );
			break;
		
		case ZLGfxHandle::RENDERBUFFER:
			glDeleteRenderbuffers ( 1, &handle->mGLID );
			break;
		
		case ZLGfxHandle::VERTEXARRAY:
			#ifndef MOAI_OS_ANDROID
				glDeleteVertexArrays ( 1, &handle->mGLID );
			#endif
			break;
	}
	delete handle;
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DepthFunc ( u32 depthFunc ) {

	glDepthFunc ( ZLGfxEnum::MapZLToNative ( depthFunc ));
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DepthMask ( bool flag ) {

	glDepthMask ( flag ? GL_TRUE : GL_FALSE );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Disable ( u32 cap ) {

	glDisable ( ZLGfxEnum::MapZLToNative ( cap ));
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DisableClientState ( u32 cap ) {

	#if !MOAI_OS_NACL
		glDisableClientState ( ZLGfxEnum::MapZLToNative ( cap ));
	#endif
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DisableVertexAttribArray ( u32 index ) {

	glDisableVertexAttribArray (( GLuint )index );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DrawArrays ( u32 primType, u32 first, u32 count ) {

	glDrawArrays ( ZLGfxEnum::MapZLToNative ( primType ), ( GLint )first, ( GLsizei )count );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DrawElements ( u32 primType, u32 count, u32 indexType, ZLRevBufferEdition* buffer, size_t offset ) {

	glDrawElements (
		ZLGfxEnum::MapZLToNative ( primType ),
		( GLsizei )count,
		ZLGfxEnum::MapZLToNative ( indexType ),
		( const GLvoid* )(( size_t )(  buffer ? buffer->GetData () : 0 ) + offset )
	);
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Enable ( u32 cap ) {

	glEnable ( ZLGfxEnum::MapZLToNative ( cap ));
}

//----------------------------------------------------------------//
void ZLGfxImmediate::EnableClientState ( u32 cap ) {

	#if !MOAI_OS_NACL
		glEnableClientState ( ZLGfxEnum::MapZLToNative ( cap ));
	#endif
}

//----------------------------------------------------------------//
void ZLGfxImmediate::EnableVertexAttribArray ( u32 index ) {

	glEnableVertexAttribArray (( GLuint )index );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Flush () {

	glFlush ();
}

//----------------------------------------------------------------//
void ZLGfxImmediate::FramebufferRenderbuffer ( u32 target, u32 attachment, ZLGfxHandle* renderbuffer ) {
	
	glFramebufferRenderbuffer (
		ZLGfxEnum::MapZLToNative ( target ),
		ZLGfxEnum::MapZLToNative ( attachment ),
		GL_RENDERBUFFER,
		( GLuint )ZLGfxHandle::GLID ( renderbuffer )
	);
}

//----------------------------------------------------------------//
void ZLGfxImmediate::FramebufferTexture2D ( u32 target, u32 attachment, ZLGfxHandle* texture, s32 level ) {

	glFramebufferTexture2D (
		ZLGfxEnum::MapZLToNative ( target ),
		ZLGfxEnum::MapZLToNative ( attachment ),
		GL_TEXTURE_2D,
		( GLuint )ZLGfxHandle::GLID ( texture ),
		( GLint )level
	);
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxImmediate::GetCurrentFramebuffer () {

	int buffer;
	glGetIntegerv ( GL_FRAMEBUFFER_BINDING, &buffer );

	return new ZLGfxHandle ( ZLGfxHandle::FRAMEBUFFER, buffer, false );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::GetUniformLocation ( ZLGfxHandle* program, cc8* uniformName, ZLGfxListener* listener, void* userdata ) {

	if ( listener) {
		u32 addr = ( u32 )glGetUniformLocation (( GLuint )ZLGfxHandle::GLID ( program ), ( const GLchar* )uniformName );
		listener->OnUniformLocation ( addr, userdata );
	}
}

//----------------------------------------------------------------//
void ZLGfxImmediate::LineWidth ( float width ) {

	glLineWidth (( GLfloat )width );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::LinkProgram ( ZLGfxHandle* program, bool verbose ) {

	GLuint programID = ( GLuint )ZLGfxHandle::GLID ( program );

	glLinkProgram ( programID );
	
	s32 status;
	glGetShaderiv ( programID, GL_LINK_STATUS, &status );
	
	if ( status == 0 ) {
	
		this->mError = true;
	
		if ( verbose ) {
	
			s32 logLength;
			glGetShaderiv ( programID, GL_INFO_LOG_LENGTH, &logLength );

			if ( logLength > 1 ) {
				char* log = ( char* )malloc ( logLength );
				glGetShaderInfoLog ( programID, ( GLsizei )logLength, ( GLsizei* )&logLength, ( GLchar* )log );
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

	glRenderbufferStorage ( GL_RENDERBUFFER, ZLGfxEnum::MapZLToNative ( internalFormat ), ( GLsizei )width, ( GLsizei )height );
}

//----------------------------------------------------------------//
const ZLGfxBufferRef ZLGfxImmediate::RetainBuffer ( const void* buffer, size_t size ) {

	ZLGfxBufferRef bufferRef;
	bufferRef.mBufferStore = 0;
	bufferRef.mBuffer = buffer;
	bufferRef.mSize = size;
	return bufferRef;
}

//----------------------------------------------------------------//
const ZLGfxBufferRef ZLGfxImmediate::RetainBuffer ( const ZLCowBuffer& buffer ) {

	return this->RetainBuffer ( buffer.GetBuffer (), buffer.GetSize ());
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Scissor ( s32 x, s32 y, u32 w, u32 h ) {

	glScissor (( GLint )x, ( GLint )y, ( GLsizei )w, ( GLsizei )h );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::ShaderSource ( ZLGfxHandle* shader, cc8* source, size_t length ) {

	const GLchar* stringArray [] = {( GLchar* )source };
	const GLint lengthArray [] = {( GLint )length };

	glShaderSource (( GLuint )( shader ? shader->mGLID : 0 ), 1, stringArray, lengthArray );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::TexEnvi ( u32 pname, s32 param ) {

	#if !MOAI_OS_NACL
		glTexEnvi ( GL_TEXTURE_ENV, ZLGfxEnum::MapZLToNative ( pname ), ( GLint )param );
	#endif
}

//----------------------------------------------------------------//
void ZLGfxImmediate::TexImage2D ( u32 level, u32 internalFormat, u32 width, u32 height, u32 format, u32 type, ZLGfxBufferRef bufferRef ) {

	glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

	glTexImage2D (
		GL_TEXTURE_2D,
		( GLint )level,
		( GLint )ZLGfxEnum::MapZLToNative ( internalFormat ),
		( GLsizei )width,
		( GLsizei )height,
		0,
		ZLGfxEnum::MapZLToNative ( format ),
		ZLGfxEnum::MapZLToNative ( type ),
		( const GLvoid* )bufferRef.mBuffer
	);
}

//----------------------------------------------------------------//
void ZLGfxImmediate::TexParameteri ( u32 pname, s32 param ) {

	glTexParameteri ( GL_TEXTURE_2D, ZLGfxEnum::MapZLToNative ( pname ), ( GLint )ZLGfxEnum::MapZLToNative ( param ));
}

//----------------------------------------------------------------//
void ZLGfxImmediate::TexSubImage2D ( u32 level, s32 xOffset, s32 yOffset, u32 width, u32 height, u32 format, u32 type, ZLGfxBufferRef bufferRef ) {

	glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

	glTexSubImage2D (
		GL_TEXTURE_2D,
		( GLint )level,
		( GLint )xOffset,
		( GLint )yOffset,
		( GLsizei )width,
		( GLsizei )height,
		ZLGfxEnum::MapZLToNative ( format ),
		ZLGfxEnum::MapZLToNative ( type ),
		( const GLvoid* )bufferRef.mBuffer
	);
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Uniform1f ( u32 location, float v0 ) {

	glUniform1f (( GLint )location, ( GLfloat )v0 );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Uniform1i ( u32 location, s32 v0 ) {

	glUniform1i (( GLint )location, ( GLint )v0 );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Uniform4fv ( u32 location, u32 count, const float* value ) {

	glUniform4fv (( GLint )location, ( GLsizei )count, ( const GLfloat* )value );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::UniformMatrix3fv ( u32 location, u32 count, bool transpose, const float* mtx ) {

	glUniformMatrix3fv (( GLint )location, ( GLsizei )count, transpose ? GL_TRUE : GL_FALSE, ( const GLfloat* )mtx );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::UniformMatrix4fv ( u32 location, u32 count, bool transpose, const float* mtx ) {

	glUniformMatrix4fv (( GLint )location, ( GLsizei )count, transpose ? GL_TRUE : GL_FALSE, ( const GLfloat* )mtx );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::UseProgram ( ZLGfxHandle* program ) {

	glUseProgram (( GLuint )( program ? program->mGLID : 0 ));
}

//----------------------------------------------------------------//
void ZLGfxImmediate::VertexAttribPointer ( u32 index, u32 size, u32 type, bool normalized, u32 stride, ZLRevBufferEdition* buffer, size_t offset ) {

	glVertexAttribPointer (
		( GLuint )index,
		( GLint )size,
		ZLGfxEnum::MapZLToNative ( type ),
		normalized ? GL_TRUE : GL_FALSE,
		( GLsizei )stride,
		( const GLvoid* )(( size_t )( buffer ? buffer->GetData () : 0 ) + offset )
	);
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Viewport ( s32 x, s32 y, u32 w, u32 h ) {

	glViewport (( GLint )x, ( GLint )y, ( GLsizei )w, ( GLsizei )h );
}

//----------------------------------------------------------------//
ZLGfxImmediate::ZLGfxImmediate () :
	mError ( false ) {
}

//----------------------------------------------------------------//
ZLGfxImmediate::~ZLGfxImmediate () {
}
