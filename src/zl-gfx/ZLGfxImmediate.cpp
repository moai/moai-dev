// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-gfx/headers.h>
#include <zl-gfx/zl_gfx.h>
#include <zl-gfx/ZLGfx-gles.h>
#include <zl-gfx/ZLGfxEnum.h>
#include <zl-gfx/ZLGfxImmediate.h>
#include <zl-util/ZLLog.h>

#ifdef DEBUG
	#define GL_LOG_ERRORS(name) this->LogErrors ( name );
#else
	#define GL_LOG_ERRORS(name) (( void )name );
#endif

//================================================================//
// ZLGfxImmediate
//================================================================//

//----------------------------------------------------------------//
void ZLGfxImmediate::ActiveTexture ( u32 textureUnit ) {

	glActiveTexture ( GL_TEXTURE0 + textureUnit );
	GL_LOG_ERRORS ( "glActiveTexture" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::AttachShader ( ZLGfxHandle* program, ZLGfxHandle* shader ) {

	glAttachShader ( program ? program->mGLID : 0, shader ? shader->mGLID : 0 );
	GL_LOG_ERRORS ( "glAttachShader" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BindAttribLocation ( ZLGfxHandle* program, u32 index, cc8* name ) {

	glBindAttribLocation ( program ? program->mGLID : 0, index, name );
	GL_LOG_ERRORS ( "glBindAttribLocation" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BindBuffer ( u32 target, ZLGfxHandle* handle ) {

	glBindBuffer ( ZLGfxEnum::MapZLToNative ( target ), handle ? handle->mGLID : 0 );
	GL_LOG_ERRORS ( "glBindBuffer" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BindFramebuffer ( u32 target, ZLGfxHandle* handle ) {

	glBindFramebuffer ( ZLGfxEnum::MapZLToNative ( target ), handle ? handle->mGLID : 0 );
	GL_LOG_ERRORS ( "glBindFramebuffer" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BindRenderbuffer ( ZLGfxHandle* handle ) {

	glBindRenderbuffer ( GL_RENDERBUFFER, handle ? handle->mGLID : 0 );
	GL_LOG_ERRORS ( "glBindRenderbuffer" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BindTexture ( ZLGfxHandle* handle ) {

	glBindTexture ( GL_TEXTURE_2D, handle ? handle->mGLID : 0 );
	GL_LOG_ERRORS ( "glBindTexture" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BindVertexArray ( ZLGfxHandle* handle ) {

	#if defined( MOAI_OS_ANDROID ) || defined( MOAI_OS_HTML )
		return;
	#else
		glBindVertexArray ( handle ? handle->mGLID : 0 );
		GL_LOG_ERRORS ( "glBindVertexArray" )
	#endif
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BlendFunc ( u32 sourceFactor, u32 destFactor ) {

	glBlendFunc ( ZLGfxEnum::MapZLToNative ( sourceFactor ), ZLGfxEnum::MapZLToNative ( destFactor ));
	GL_LOG_ERRORS ( "glBlendFunc" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BlendMode ( u32 mode ) {

	glBlendEquation ( ZLGfxEnum::MapZLToNative ( mode ));
	GL_LOG_ERRORS ( "glBlendEquation" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BufferData ( u32 target, size_t size, ZLSharedConstBuffer* buffer, size_t offset, u32 usage ) {

	const GLvoid* data = ( const GLvoid* )(( size_t )ZLSharedConstBuffer::GetConstData ( buffer ) + offset ); // TODO: cast
	glBufferData ( ZLGfxEnum::MapZLToNative ( target ), ( GLsizeiptr )size, data, ZLGfxEnum::MapZLToNative ( usage ));
	GL_LOG_ERRORS ( "glBufferData" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BufferSubData ( u32 target, size_t offset, size_t size, ZLSharedConstBuffer* buffer, size_t srcOffset ) {

	const GLvoid* data = ( const GLvoid* )(( size_t )ZLSharedConstBuffer::GetConstData ( buffer ) + srcOffset );
	glBufferSubData ( ZLGfxEnum::MapZLToNative ( target ), ( GLintptr )offset, ( GLsizeiptr )size, data );
	GL_LOG_ERRORS ( "glBufferSubData" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::CheckFramebufferStatus ( u32 target ) {
	
	GLenum status = glCheckFramebufferStatus ( ZLGfxEnum::MapZLToNative ( target ));
	GL_LOG_ERRORS ( "glCheckFramebufferStatus" )
	
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
	GL_LOG_ERRORS ( "glClear" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::ClearColor ( float r, float g, float b, float a ) {

	glClearColor ( r, g, b, a );
	GL_LOG_ERRORS ( "glClearColor" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Color ( float r, float g, float b, float a ) {

	glColor4f ( r, g, b, a );
	GL_LOG_ERRORS ( "glColor4f" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Comment ( cc8* comment ) {
	UNUSED ( comment );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::CompileShader ( ZLGfxHandle* shader, bool log ) {

	GLuint shaderID = ( GLuint )( shader ? shader->mGLID : 0 );

	glCompileShader ( shaderID );
	GL_LOG_ERRORS ( "glCompileShader" )
	
	s32 status;
	glGetShaderiv ( shaderID, GL_COMPILE_STATUS, &status );
	GL_LOG_ERRORS ( "glGetShaderiv" )
	
	if ( status == 0 ) {
	
		this->mError = true;
	
		if ( log ) {
	
			s32 logLength;
			glGetShaderiv ( shaderID, GL_INFO_LOG_LENGTH, &logLength );
			GL_LOG_ERRORS ( "glGetShaderiv" )

			if ( logLength > 1 ) {
				char* message = ( char* )malloc ( logLength );
				
				glGetShaderInfoLog ( shaderID, ( GLsizei )logLength, ( GLsizei* )&logLength, ( GLchar* )message );
				GL_LOG_ERRORS ( "glGetShaderInfoLog" )
				
				printf ( "%s\n", message );
				free ( message );
			}
		}
	}
}

//----------------------------------------------------------------//
void ZLGfxImmediate::CompressedTexImage2D ( u32 level, u32 internalFormat, u32 width, u32 height, u32 imageSize, ZLSharedConstBuffer* buffer ) {
	
	glCompressedTexImage2D (
		GL_TEXTURE_2D,
		( GLint )level,
		( GLint )ZLGfxEnum::MapZLToNative ( internalFormat ),
		( GLsizei )width,
		( GLsizei )height,
		0,
		( GLsizei )imageSize,
		( const GLvoid* )ZLSharedConstBuffer::GetConstData ( buffer )
	);
	
	GL_LOG_ERRORS ( "glCompressedTexImage2D" )
}

//----------------------------------------------------------------//
ZLSharedConstBuffer* ZLGfxImmediate::CopyBuffer ( ZLSharedConstBuffer* buffer ) {

	return buffer;
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxImmediate::Create ( ZLGfxHandle* handle, u32 param ) {

	if ( handle ) {
	
		switch ( handle->mType ) {
		
			case ZLGfxHandle::BUFFER:
				glGenBuffers ( 1, &handle->mGLID );
				GL_LOG_ERRORS ( "glGenBuffers" )
				break;
				
			case ZLGfxHandle::FRAMEBUFFER:
				glGenFramebuffers ( 1, &handle->mGLID );
				GL_LOG_ERRORS ( "glGenFramebuffers" )
				break;
				
			case ZLGfxHandle::PROGRAM:
				handle->mGLID = ( u32 )glCreateProgram ();
				GL_LOG_ERRORS ( "glCreateProgram" )
				break;
				
			case ZLGfxHandle::SHADER:
				handle->mGLID = ( u32 )glCreateShader ( ZLGfxEnum::MapZLToNative ( param ));
				GL_LOG_ERRORS ( "glCreateShader" )
				break;
				
			case ZLGfxHandle::TEXTURE:
				glGenTextures ( 1, ( GLuint* )&handle->mGLID );
				GL_LOG_ERRORS ( "glGenTextures" )
				break;
				
			case ZLGfxHandle::RENDERBUFFER:
				glGenRenderbuffers ( 1, &handle->mGLID );
				GL_LOG_ERRORS ( "glGenRenderbuffers" )
				break;
				
			case ZLGfxHandle::VERTEXARRAY:
				#ifdef MOAI_OS_ANDROID
					handle->mGLID = 0;
				#else
					glGenVertexArrays ( 1, &handle->mGLID );
					GL_LOG_ERRORS ( "glGenVertexArrays" )
				#endif
				break;
		}
	}
	
	return handle;
}

//----------------------------------------------------------------//
void ZLGfxImmediate::CullFace ( u32 mode ) {

	glCullFace ( ZLGfxEnum::MapZLToNative ( mode ));
	GL_LOG_ERRORS ( "glCullFace" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Delete ( u32 type, u32 glid ) {

	switch ( type ) {
	
		case ZLGfxHandle::BUFFER:
			glDeleteBuffers ( 1, &glid );
			GL_LOG_ERRORS ( "glDeleteBuffers" )
			break;
		
		case ZLGfxHandle::FRAMEBUFFER:
			glDeleteFramebuffers ( 1, &glid );
			GL_LOG_ERRORS ( "glDeleteFramebuffers" )
			break;
		
		case ZLGfxHandle::PROGRAM: {
		
			GLint status;
			glGetProgramiv ( glid, GL_DELETE_STATUS, &status );
			GL_LOG_ERRORS ( "glGetProgramiv" )

			if ( status == GL_FALSE ) {
				glDeleteProgram ( glid );
				GL_LOG_ERRORS ( "glDeleteProgram" )
			}
			break;
		}
		case ZLGfxHandle::SHADER: {
		
			GLint status;
			glGetShaderiv ( glid, GL_DELETE_STATUS, &status );
			GL_LOG_ERRORS ( "glGetShaderiv" )
		
			if ( status == GL_FALSE ) {
				glDeleteShader ( glid );
				GL_LOG_ERRORS ( "glDeleteShader" )
			}
			break;
		}
		case ZLGfxHandle::TEXTURE:
			glDeleteTextures ( 1, &glid );
			GL_LOG_ERRORS ( "glDeleteTextures" )
			break;
		
		case ZLGfxHandle::RENDERBUFFER:
			glDeleteRenderbuffers ( 1, &glid );
			GL_LOG_ERRORS ( "glDeleteRenderbuffers" )
			break;
		
		case ZLGfxHandle::VERTEXARRAY:
			#if !( defined( MOAI_OS_ANDROID ) || defined( MOAI_OS_HTML ))
				glDeleteVertexArrays ( 1, &glid );
				GL_LOG_ERRORS ( "glDeleteVertexArrays" )
			#endif
			break;
	}
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DepthFunc ( u32 depthFunc ) {

	glDepthFunc ( ZLGfxEnum::MapZLToNative ( depthFunc ));
	GL_LOG_ERRORS ( "glDepthFunc" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DepthMask ( bool flag ) {

	glDepthMask ( flag ? GL_TRUE : GL_FALSE );
	GL_LOG_ERRORS ( "glDepthMask" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Disable ( u32 cap ) {

	glDisable ( ZLGfxEnum::MapZLToNative ( cap ));
	GL_LOG_ERRORS ( "glDisable" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DisableClientState ( u32 cap ) {

	#if !MOAI_OS_NACL
		glDisableClientState ( ZLGfxEnum::MapZLToNative ( cap ));
		GL_LOG_ERRORS ( "glDisableClientState" )
	#endif
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DisableVertexAttribArray ( u32 index ) {

	glDisableVertexAttribArray (( GLuint )index );
	GL_LOG_ERRORS ( "glDisableVertexAttribArray" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DrawArrays ( u32 primType, u32 first, u32 count ) {

	glDrawArrays ( ZLGfxEnum::MapZLToNative ( primType ), ( GLint )first, ( GLsizei )count );
	GL_LOG_ERRORS ( "glDrawArrays" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DrawElements ( u32 primType, u32 count, u32 indexType, ZLSharedConstBuffer* buffer, size_t offset ) {

	glDrawElements (
		ZLGfxEnum::MapZLToNative ( primType ),
		( GLsizei )count,
		ZLGfxEnum::MapZLToNative ( indexType ),
		( const GLvoid* )(( size_t )ZLSharedConstBuffer::GetConstData ( buffer ) + offset )
	);
	GL_LOG_ERRORS ( "glDrawElements" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Enable ( u32 cap ) {

	glEnable ( ZLGfxEnum::MapZLToNative ( cap ));
	GL_LOG_ERRORS ( "glEnable" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::EnableClientState ( u32 cap ) {

	#if !MOAI_OS_NACL
		glEnableClientState ( ZLGfxEnum::MapZLToNative ( cap ));
		GL_LOG_ERRORS ( "glEnableClientState" )
	#endif
}

//----------------------------------------------------------------//
void ZLGfxImmediate::EnableVertexAttribArray ( u32 index ) {

	glEnableVertexAttribArray (( GLuint )index );
	GL_LOG_ERRORS ( "glEnableVertexAttribArray" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Event ( ZLGfxListener* listener, u32 event, void* userdata ) {

	if ( listener ) {
		listener->OnGfxEvent ( event, userdata );
	}
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Flush ( bool finish ) {

	glFlush ();
	if ( finish ) {
		glFinish ();
	}
	GL_LOG_ERRORS ( "glFlush" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::FramebufferRenderbuffer ( u32 target, u32 attachment, ZLGfxHandle* renderbuffer ) {
	
	glFramebufferRenderbuffer (
		ZLGfxEnum::MapZLToNative ( target ),
		ZLGfxEnum::MapZLToNative ( attachment ),
		GL_RENDERBUFFER,
		( GLuint )ZLGfxHandle::GLID ( renderbuffer )
	);
	GL_LOG_ERRORS ( "glFramebufferRenderbuffer" )
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
	GL_LOG_ERRORS ( "glFramebufferTexture2D" )
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxImmediate::GetCurrentFramebuffer () {

	int buffer;
	glGetIntegerv ( GL_FRAMEBUFFER_BINDING, &buffer );
	GL_LOG_ERRORS ( "glGetIntegerv" )

	return new ZLGfxHandle ( ZLGfxHandle::FRAMEBUFFER, buffer, false );
}

//----------------------------------------------------------------//
void ZLGfxImmediate::GetUniformLocation ( ZLGfxHandle* program, cc8* uniformName, ZLGfxListener* listener, void* userdata ) {

	if ( listener) {
		u32 addr = ( u32 )glGetUniformLocation (( GLuint )ZLGfxHandle::GLID ( program ), ( const GLchar* )uniformName );
		GL_LOG_ERRORS ( "glGetUniformLocation" )
		listener->OnUniformLocation ( addr, userdata );
	}
}

//----------------------------------------------------------------//
bool ZLGfxImmediate::IsImmediate () {

	return true;
}

//----------------------------------------------------------------//
void ZLGfxImmediate::LineWidth ( float width ) {

	glLineWidth (( GLfloat )width );
	GL_LOG_ERRORS ( "glLineWidth" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::LinkProgram ( ZLGfxHandle* program, bool log ) {

	GLuint programID = ( GLuint )ZLGfxHandle::GLID ( program );

	glLinkProgram ( programID );
	GL_LOG_ERRORS ( "glLinkProgram" )
	
	s32 status;
	glGetProgramiv ( programID, GL_LINK_STATUS, &status );
	GL_LOG_ERRORS ( "glGetProgramiv" )
	
	if ( status == 0 ) {
	
		this->mError = true;
	
		if ( log ) {
	
			s32 logLength;
			glGetProgramiv ( programID, GL_INFO_LOG_LENGTH, &logLength );
			GL_LOG_ERRORS ( "glGetProgramiv" )

			if ( logLength > 1 ) {
				char* message = ( char* )malloc ( logLength );
				
				glGetProgramInfoLog ( programID, ( GLsizei )logLength, ( GLsizei* )&logLength, ( GLchar* )message );
				GL_LOG_ERRORS ( "glGetProgramInfoLog" )
				
				printf ( "%s\n", message );
				free ( message );
			}
		}
	}
}

//----------------------------------------------------------------//
void ZLGfxImmediate::LogErrors ( cc8* origin ) {

	bool hasErrors = false;

	GLenum error = ZGL_ERROR_NONE;
	while (( error = ZLGfxDevice::GetError ()) != ZGL_ERROR_NONE ) {
	
		ZLLog_Error ( "GL ERROR: %s - %s\n", ZLGfxDevice::GetErrorString ( error ), origin ? origin : "<unknown>" );
		hasErrors = true;
	}
	assert ( !hasErrors );
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
void ZLGfxImmediate::ReadPixels ( s32 x, s32 y, u32 width, u32 height, u32 format, u32 type, u32 pixelSize, ZLGfxListener* listener, void* userdata ) {

	if ( listener ) {

		ZLCopyOnWrite copyOnWrite;
		unsigned char* buffer = ( unsigned char* )copyOnWrite.Alloc ( width * height * pixelSize );

		glReadPixels (
			( GLint )x,
			( GLint )y,
			( GLsizei )width,
			( GLsizei )height,
			ZLGfxEnum::MapZLToNative ( format ),
			ZLGfxEnum::MapZLToNative ( type ),
			( GLvoid* )buffer
		);
		
		//image is flipped vertically, flip it back
		int index,indexInvert;
		for ( u32 yFlip = 0; yFlip < ( height / 2 ); ++yFlip ) {
			for ( u32 xFlip = 0; xFlip < width; ++xFlip ) {
				for ( u32 i = 0; i < pixelSize; ++i ) {

					index = i + ( xFlip * pixelSize ) + ( yFlip * width * pixelSize );
					indexInvert = i + ( xFlip * pixelSize ) + (( height - 1 - yFlip ) * width * pixelSize );

					unsigned char temp = buffer [ indexInvert ];
					buffer [ indexInvert ] = buffer [ index ];
					buffer [ index ] = temp;
				}
			}
		}
		
		listener->OnReadPixels ( copyOnWrite, userdata );
	}
}

//----------------------------------------------------------------//
void ZLGfxImmediate::RenderbufferStorage ( u32 internalFormat, u32 width, u32 height ) {

	glRenderbufferStorage ( GL_RENDERBUFFER, ZLGfxEnum::MapZLToNative ( internalFormat ), ( GLsizei )width, ( GLsizei )height );
	GL_LOG_ERRORS ( "glRenderbufferStorage" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Scissor ( s32 x, s32 y, u32 w, u32 h ) {

	glScissor (( GLint )x, ( GLint )y, ( GLsizei )w, ( GLsizei )h );
	GL_LOG_ERRORS ( "glScissor" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::ShaderSource ( ZLGfxHandle* shader, cc8* source, size_t length ) {

	const GLchar* stringArray [] = {( GLchar* )source };
	const GLint lengthArray [] = {( GLint )length };

	glShaderSource (( GLuint )( shader ? shader->mGLID : 0 ), 1, stringArray, lengthArray );
	GL_LOG_ERRORS ( "glShaderSource" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::TexEnvi ( u32 pname, s32 param ) {

	#if !MOAI_OS_NACL
		glTexEnvi ( GL_TEXTURE_ENV, ZLGfxEnum::MapZLToNative ( pname ), ( GLint )param );
		GL_LOG_ERRORS ( "glTexEnvi" )
	#endif
}

//----------------------------------------------------------------//
void ZLGfxImmediate::TexImage2D ( u32 level, u32 internalFormat, u32 width, u32 height, u32 format, u32 type, ZLSharedConstBuffer* buffer ) {

	glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
	GL_LOG_ERRORS ( "glPixelStorei" )

	glTexImage2D (
		GL_TEXTURE_2D,
		( GLint )level,
		( GLint )ZLGfxEnum::MapZLToNative ( internalFormat ),
		( GLsizei )width,
		( GLsizei )height,
		0,
		ZLGfxEnum::MapZLToNative ( format ),
		ZLGfxEnum::MapZLToNative ( type ),
		( const GLvoid* )ZLSharedConstBuffer::GetConstData ( buffer )
	);
	GL_LOG_ERRORS ( "glTexImage2D" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::TexParameteri ( u32 pname, s32 param ) {

	glTexParameteri ( GL_TEXTURE_2D, ZLGfxEnum::MapZLToNative ( pname ), ( GLint )ZLGfxEnum::MapZLToNative ( param ));
	GL_LOG_ERRORS ( "glTexParameteri" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::TexSubImage2D ( u32 level, s32 xOffset, s32 yOffset, u32 width, u32 height, u32 format, u32 type, ZLSharedConstBuffer* buffer ) {

	glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
	GL_LOG_ERRORS ( "glPixelStorei" )

	glTexSubImage2D (
		GL_TEXTURE_2D,
		( GLint )level,
		( GLint )xOffset,
		( GLint )yOffset,
		( GLsizei )width,
		( GLsizei )height,
		ZLGfxEnum::MapZLToNative ( format ),
		ZLGfxEnum::MapZLToNative ( type ),
		( const GLvoid* )ZLSharedConstBuffer::GetConstData ( buffer )
	);
	GL_LOG_ERRORS ( "glTexSubImage2D" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Uniform1f ( u32 location, float v0 ) {

	glUniform1f (( GLint )location, ( GLfloat )v0 );
	GL_LOG_ERRORS ( "glUniform1f" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Uniform1i ( u32 location, s32 v0 ) {

	glUniform1i (( GLint )location, ( GLint )v0 );
	GL_LOG_ERRORS ( "glUniform1i" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Uniform4fv ( u32 location, u32 count, const float* value ) {

	glUniform4fv (( GLint )location, ( GLsizei )count, ( const GLfloat* )value );
	GL_LOG_ERRORS ( "glUniform4fv" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::UniformMatrix3fv ( u32 location, u32 count, bool transpose, const float* mtx ) {

	glUniformMatrix3fv (( GLint )location, ( GLsizei )count, transpose ? GL_TRUE : GL_FALSE, ( const GLfloat* )mtx );
	GL_LOG_ERRORS ( "glUniformMatrix3fv" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::UniformMatrix4fv ( u32 location, u32 count, bool transpose, const float* mtx ) {

	glUniformMatrix4fv (( GLint )location, ( GLsizei )count, transpose ? GL_TRUE : GL_FALSE, ( const GLfloat* )mtx );
	GL_LOG_ERRORS ( "glUniformMatrix4fv" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::UseProgram ( ZLGfxHandle* program ) {

	glUseProgram (( GLuint )( program ? program->mGLID : 0 ));
	GL_LOG_ERRORS ( "glUseProgram" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::VertexAttribPointer ( u32 index, u32 size, u32 type, bool normalized, u32 stride, ZLSharedConstBuffer* buffer, size_t offset ) {

	glVertexAttribPointer (
		( GLuint )index,
		( GLint )size,
		ZLGfxEnum::MapZLToNative ( type ),
		normalized ? GL_TRUE : GL_FALSE,
		( GLsizei )stride,
		( const GLvoid* )(( size_t )ZLSharedConstBuffer::GetConstData ( buffer ) + offset )
	);
	GL_LOG_ERRORS ( "glVertexAttribPointer" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Viewport ( s32 x, s32 y, u32 w, u32 h ) {

	glViewport (( GLint )x, ( GLint )y, ( GLsizei )w, ( GLsizei )h );
	GL_LOG_ERRORS ( "glViewport" )
}

//----------------------------------------------------------------//
ZLGfxImmediate::ZLGfxImmediate () :
	mError ( false ) {
}

//----------------------------------------------------------------//
ZLGfxImmediate::~ZLGfxImmediate () {
}
