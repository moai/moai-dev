// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-gfx/headers.h>
#include <zl-gfx/ZLGfx-gles.h>
#include <zl-gfx/ZLGfxEnum.h>
#include <zl-gfx/ZLGfxImmediate.h>

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
void ZLGfxImmediate::AllocateResource ( ZLGfxResource& resource, ZLGfxEnum::Type param ) {

	resource.mGLID = 0;
	resource.mStatus = ZLGfxResource::GPU_ALLOCATED;

	switch ( resource.mType ) {
	
		case ZLGfxResource::BUFFER:
			glGenBuffers ( 1, &resource.mGLID );
			GL_LOG_ERRORS ( "glGenBuffers" )
			break;
			
		case ZLGfxResource::FRAMEBUFFER:
			glGenFramebuffers ( 1, &resource.mGLID );
			GL_LOG_ERRORS ( "glGenFramebuffers" )
			break;
			
		case ZLGfxResource::PROGRAM:
			resource.mGLID = ( u32 )glCreateProgram ();
			GL_LOG_ERRORS ( "glCreateProgram" )
			break;
			
		case ZLGfxResource::SHADER:
			resource.mGLID = ( u32 )glCreateShader ( ZLGfxEnum::MapZLToNative ( param ));
			GL_LOG_ERRORS ( "glCreateShader" )
			break;
			
		case ZLGfxResource::TEXTURE:
			glGenTextures ( 1, ( GLuint* )&resource.mGLID );
			GL_LOG_ERRORS ( "glGenTextures" )
			break;
			
		case ZLGfxResource::RENDERBUFFER:
			glGenRenderbuffers ( 1, &resource.mGLID );
			GL_LOG_ERRORS ( "glGenRenderbuffers" )
			break;
			
		case ZLGfxResource::VERTEXARRAY:
			#ifndef MOAI_OS_ANDROID
				glGenVertexArrays ( 1, &resource.mGLID );
				GL_LOG_ERRORS ( "glGenVertexArrays" )
			#endif
			break;
		
		default:
			resource.mType = ZLGfxResource::NONE;
			resource.mStatus = ZLGfxResource::NOT_ALLOCATED;
	}
}

//----------------------------------------------------------------//
void ZLGfxImmediate::AttachShader ( ZLGfxResource& program, ZLGfxResource& shader ) {

	glAttachShader ( program.GLID (), shader.GLID ());
	GL_LOG_ERRORS ( "glAttachShader" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BindAttribLocation ( ZLGfxResource& program, u32 index, cc8* name ) {

	glBindAttribLocation ( program.GLID (), index, name );
	GL_LOG_ERRORS ( "glBindAttribLocation" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BindBuffer ( ZLGfxEnum::Type target, ZLGfxResource& handle ) {

	glBindBuffer ( ZLGfxEnum::MapZLToNative ( target ), handle.GLID ());
	GL_LOG_ERRORS ( "glBindBuffer" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BindFramebuffer ( ZLGfxEnum::Type target, ZLGfxResource& handle ) {

	glBindFramebuffer ( ZLGfxEnum::MapZLToNative ( target ), handle.GLID ());
	GL_LOG_ERRORS ( "glBindFramebuffer" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BindRenderbuffer ( ZLGfxResource& handle ) {

	glBindRenderbuffer ( GL_RENDERBUFFER, handle.GLID ());
	GL_LOG_ERRORS ( "glBindRenderbuffer" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BindTexture ( ZLGfxResource& handle ) {

	glBindTexture ( GL_TEXTURE_2D, handle.GLID ());
	GL_LOG_ERRORS ( "glBindTexture" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BindVertexArray ( ZLGfxResource& handle ) {

	#if defined( MOAI_OS_ANDROID ) || defined( MOAI_OS_HTML )
		return;
	#else
		glBindVertexArray ( handle.GLID ());
		GL_LOG_ERRORS ( "glBindVertexArray" )
	#endif
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BlendFunc ( ZLGfxEnum::Type sourceFactor, ZLGfxEnum::Type destFactor ) {

	glBlendFunc ( ZLGfxEnum::MapZLToNative ( sourceFactor ), ZLGfxEnum::MapZLToNative ( destFactor ));
	GL_LOG_ERRORS ( "glBlendFunc" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BlendMode ( ZLGfxEnum::Type mode ) {

	glBlendEquation ( ZLGfxEnum::MapZLToNative ( mode ));
	GL_LOG_ERRORS ( "glBlendEquation" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BufferData ( ZLGfxEnum::Type target, size_t size, ZLSharedConstBuffer* buffer, size_t offset, ZLGfxEnum::Type usage ) {

	const GLvoid* data = ( const GLvoid* )(( size_t )ZLSharedConstBuffer::GetConstData ( buffer ) + offset ); // TODO: cast
	glBufferData ( ZLGfxEnum::MapZLToNative ( target ), ( GLsizeiptr )size, data, ZLGfxEnum::MapZLToNative ( usage ));
	GL_LOG_ERRORS ( "glBufferData" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::BufferSubData ( ZLGfxEnum::Type target, size_t offset, size_t size, ZLSharedConstBuffer* buffer, size_t srcOffset ) {

	const GLvoid* data = ( const GLvoid* )(( size_t )ZLSharedConstBuffer::GetConstData ( buffer ) + srcOffset );
	glBufferSubData ( ZLGfxEnum::MapZLToNative ( target ), ( GLintptr )offset, ( GLsizeiptr )size, data );
	GL_LOG_ERRORS ( "glBufferSubData" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::CheckFramebufferStatus ( ZLGfxEnum::Type target ) {
	
	GLenum status = glCheckFramebufferStatus ( ZLGfxEnum::MapZLToNative ( target ));
	GL_LOG_ERRORS ( "glCheckFramebufferStatus" )
	
	if ( status == ZLGfxEnum::FRAMEBUFFER_STATUS_COMPLETE ) {
		this->mError = false;
	}
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Clear ( u32 mask ) {

	GLbitfield glMask = 0;

	if ( mask & ZGLClearColorFlags::CLEAR_COLOR_BUFFER_BIT ) {
		glMask |= GL_COLOR_BUFFER_BIT;
	}

	if ( mask & ZGLClearColorFlags::CLEAR_DEPTH_BUFFER_BIT ) {
		glMask |= GL_DEPTH_BUFFER_BIT;
	}

	if ( mask & ZGLClearColorFlags::CLEAR_STENCIL_BUFFER_BIT ) {
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
void ZLGfxImmediate::CompileShader ( ZLGfxResource& shader, bool log ) {

	GLuint shaderID = ( GLuint )( shader.GLID ());

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
void ZLGfxImmediate::CompressedTexImage2D ( u32 level, ZLGfxEnum::Type internalFormat, u32 width, u32 height, u32 imageSize, ZLSharedConstBuffer* buffer ) {
	
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
void ZLGfxImmediate::CullFace ( ZLGfxEnum::Type mode ) {

	glCullFace ( ZLGfxEnum::MapZLToNative ( mode ));
	GL_LOG_ERRORS ( "glCullFace" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DeleteResource ( ZLGfxResource& resource ) {
	
	u32 glid	= resource.mGLID;
	u32 type	= resource.mType;
	u32 status	= resource.mStatus;
	
	resource.Discard ();

	if ( status != ZLGfxResource::GPU_ALLOCATED ) return;

	switch ( type ) {
	
		case ZLGfxResource::BUFFER:
			glDeleteBuffers ( 1, &glid );
			GL_LOG_ERRORS ( "glDeleteBuffers" )
			break;
		
		case ZLGfxResource::FRAMEBUFFER:
			glDeleteFramebuffers ( 1, &glid );
			GL_LOG_ERRORS ( "glDeleteFramebuffers" )
			break;
		
		case ZLGfxResource::PROGRAM: {
		
			GLint status;
			glGetProgramiv ( glid, GL_DELETE_STATUS, &status );
			GL_LOG_ERRORS ( "glGetProgramiv" )

			if ( status == GL_FALSE ) {
				glDeleteProgram ( glid );
				GL_LOG_ERRORS ( "glDeleteProgram" )
			}
			break;
		}
		case ZLGfxResource::SHADER: {
		
			GLint status;
			glGetShaderiv ( glid, GL_DELETE_STATUS, &status );
			GL_LOG_ERRORS ( "glGetShaderiv" )
		
			if ( status == GL_FALSE ) {
				glDeleteShader ( glid );
				GL_LOG_ERRORS ( "glDeleteShader" )
			}
			break;
		}
		case ZLGfxResource::TEXTURE:
			glDeleteTextures ( 1, &glid );
			GL_LOG_ERRORS ( "glDeleteTextures" )
			break;
		
		case ZLGfxResource::RENDERBUFFER:
			glDeleteRenderbuffers ( 1, &glid );
			GL_LOG_ERRORS ( "glDeleteRenderbuffers" )
			break;
		
		case ZLGfxResource::VERTEXARRAY:
			#if !( defined( MOAI_OS_ANDROID ) || defined( MOAI_OS_HTML ))
				glDeleteVertexArrays ( 1, &glid );
				GL_LOG_ERRORS ( "glDeleteVertexArrays" )
			#endif
			break;
	}
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DepthFunc ( ZLGfxEnum::Type depthFunc ) {

	glDepthFunc ( ZLGfxEnum::MapZLToNative ( depthFunc ));
	GL_LOG_ERRORS ( "glDepthFunc" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DepthMask ( bool flag ) {

	glDepthMask ( flag ? GL_TRUE : GL_FALSE );
	GL_LOG_ERRORS ( "glDepthMask" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Disable ( ZLGfxEnum::Type cap ) {

	glDisable ( ZLGfxEnum::MapZLToNative ( cap ));
	GL_LOG_ERRORS ( "glDisable" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DisableClientState ( ZLGfxEnum::Type cap ) {

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
void ZLGfxImmediate::DrawArrays ( ZLGfxEnum::Type primType, u32 first, u32 count ) {

	glDrawArrays ( ZLGfxEnum::MapZLToNative ( primType ), ( GLint )first, ( GLsizei )count );
	GL_LOG_ERRORS ( "glDrawArrays" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::DrawElements ( ZLGfxEnum::Type primType, u32 count, ZLGfxEnum::Type indexType, ZLSharedConstBuffer* buffer, size_t offset ) {

	glDrawElements (
		ZLGfxEnum::MapZLToNative ( primType ),
		( GLsizei )count,
		ZLGfxEnum::MapZLToNative ( indexType ),
		( const GLvoid* )(( size_t )ZLSharedConstBuffer::GetConstData ( buffer ) + offset )
	);
	GL_LOG_ERRORS ( "glDrawElements" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Enable ( ZLGfxEnum::Type cap ) {

	glEnable ( ZLGfxEnum::MapZLToNative ( cap ));
	GL_LOG_ERRORS ( "glEnable" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::EnableClientState ( ZLGfxEnum::Type cap ) {

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
		listener->ZLGfxListener_OnGfxEvent ( event, userdata );
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
void ZLGfxImmediate::FramebufferRenderbuffer ( ZLGfxEnum::Type target, ZLGfxEnum::Type attachment, ZLGfxResource& renderbuffer ) {
	
	glFramebufferRenderbuffer (
		ZLGfxEnum::MapZLToNative ( target ),
		ZLGfxEnum::MapZLToNative ( attachment ),
		GL_RENDERBUFFER,
		( GLuint )renderbuffer.GLID ()
	);
	GL_LOG_ERRORS ( "glFramebufferRenderbuffer" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::FramebufferTexture2D ( ZLGfxEnum::Type target, ZLGfxEnum::Type attachment, ZLGfxResource& texture, s32 level ) {

	glFramebufferTexture2D (
		ZLGfxEnum::MapZLToNative ( target ),
		ZLGfxEnum::MapZLToNative ( attachment ),
		GL_TEXTURE_2D,
		( GLuint )texture.GLID (),
		( GLint )level
	);
	GL_LOG_ERRORS ( "glFramebufferTexture2D" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::GetCurrentFramebuffer ( ZLGfxResource& framebuffer ) {

	int buffer;
	glGetIntegerv ( GL_FRAMEBUFFER_BINDING, &buffer );
	GL_LOG_ERRORS ( "glGetIntegerv" )

	framebuffer.mType		= ZLGfxResource::FRAMEBUFFER;
	framebuffer.mGLID		= ( u32 )buffer;
	framebuffer.mStatus		= ZLGfxResource::SYSTEM_ALLOCATED;
}

//----------------------------------------------------------------//
void ZLGfxImmediate::GetUniformLocation ( ZLGfxResource& program, cc8* uniformName, ZLGfxListener* listener, void* userdata ) {

	if ( listener) {
		u32 addr = ( u32 )glGetUniformLocation (( GLuint )program.GLID (), ( const GLchar* )uniformName );
		GL_LOG_ERRORS ( "glGetUniformLocation" )
		listener->ZLGfxListener_OnUniformLocation ( addr, userdata );
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
void ZLGfxImmediate::LinkProgram ( ZLGfxResource& program, bool log ) {

	GLuint programID = ( GLuint )program.GLID ();

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

	ZLGfxEnum::Type error = ZLGfxEnum::ERROR_NONE;
	while (( error = ZLGfxDevice::GetError ()) != ZLGfxEnum::ERROR_NONE ) {
	
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
void ZLGfxImmediate::ReadPixels ( s32 x, s32 y, u32 width, u32 height, ZLGfxEnum::Type format, ZLGfxEnum::Type type, u32 pixelSize, ZLGfxListener* listener, void* userdata ) {

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
		
		listener->ZLGfxListener_OnReadPixels ( copyOnWrite, userdata );
	}
}

//----------------------------------------------------------------//
void ZLGfxImmediate::RenderbufferStorage ( ZLGfxEnum::Type internalFormat, u32 width, u32 height ) {

	glRenderbufferStorage ( GL_RENDERBUFFER, ZLGfxEnum::MapZLToNative ( internalFormat ), ( GLsizei )width, ( GLsizei )height );
	GL_LOG_ERRORS ( "glRenderbufferStorage" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::Scissor ( s32 x, s32 y, u32 w, u32 h ) {

	glScissor (( GLint )x, ( GLint )y, ( GLsizei )w, ( GLsizei )h );
	GL_LOG_ERRORS ( "glScissor" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::ShaderSource ( ZLGfxResource& shader, cc8* source, size_t length ) {

	const GLchar* stringArray [] = {( GLchar* )source };
	const GLint lengthArray [] = {( GLint )length };

	glShaderSource (( GLuint )shader.GLID (), 1, stringArray, lengthArray );
	GL_LOG_ERRORS ( "glShaderSource" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::TexEnvi ( ZLGfxEnum::Type pname, ZLGfxEnum::Type param ) {

	#if !MOAI_OS_NACL
		glTexEnvi ( GL_TEXTURE_ENV, ZLGfxEnum::MapZLToNative ( pname ), ( GLint )ZLGfxEnum::MapZLToNative ( param ));
		GL_LOG_ERRORS ( "glTexEnvi" )
	#endif
}

//----------------------------------------------------------------//
void ZLGfxImmediate::TexImage2D ( u32 level, ZLGfxEnum::Type internalFormat, u32 width, u32 height, ZLGfxEnum::Type format, ZLGfxEnum::Type type, ZLSharedConstBuffer* buffer ) {

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
void ZLGfxImmediate::TexParameteri ( ZLGfxEnum::Type pname, ZLGfxEnum::Type param ) {

	glTexParameteri ( GL_TEXTURE_2D, ZLGfxEnum::MapZLToNative ( pname ), ( GLint )ZLGfxEnum::MapZLToNative ( param ));
	GL_LOG_ERRORS ( "glTexParameteri" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::TexSubImage2D ( u32 level, s32 xOffset, s32 yOffset, u32 width, u32 height, ZLGfxEnum::Type format, ZLGfxEnum::Type type, ZLSharedConstBuffer* buffer ) {

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
void ZLGfxImmediate::UniformFloat ( u32 location, u32 index, u32 width, u32 count, const float* value ) {

	switch ( width ) {
	
		case 1:
			glUniform1fv (( GLint )( location + index ), count, ( const GLfloat* )value );
			GL_LOG_ERRORS ( "glUniform1fv" )
			break;
			
		case 2:
			glUniform2fv (( GLint )( location + index ), count, ( const GLfloat* )value );
			GL_LOG_ERRORS ( "glUniform2fv" )
			break;
			
		case 3:
			glUniform3fv (( GLint )( location + index ), count, ( const GLfloat* )value );
			GL_LOG_ERRORS ( "glUniform3fv" )
			break;
			
		case 4:
			glUniform4fv (( GLint )( location + index ), count, ( const GLfloat* )value );
			GL_LOG_ERRORS ( "glUniform4fv" )
			break;
		
		case 9:
			glUniformMatrix3fv (( GLint )( location + ( index * 3 )), count, GL_FALSE, ( const GLfloat* )value );
			GL_LOG_ERRORS ( "glUniformMatrix3fv" )
			break;
		
		case 16:
			glUniformMatrix4fv (( GLint )( location + ( index * 4 )), count, GL_FALSE, ( const GLfloat* )value );
			GL_LOG_ERRORS ( "glUniformMatrix4fv" )
			break;
	}
}

//----------------------------------------------------------------//
void ZLGfxImmediate::UniformInt ( u32 location, u32 index, u32 width, u32 count, const s32* value ) {
	UNUSED(index);
	switch ( width ) {
	
		case 1:
			glUniform1iv (( GLint )location, count, ( const GLint* )value );
			GL_LOG_ERRORS ( "glUniform1iv" )
			break;
			
		case 2:
			glUniform2iv (( GLint )location, count, ( const GLint* )value );
			GL_LOG_ERRORS ( "glUniform2iv" )
			break;
			
		case 3:
			glUniform3iv (( GLint )location, count, ( const GLint* )value );
			GL_LOG_ERRORS ( "glUniform3iv" )
			break;
			
		case 4:
			glUniform4iv (( GLint )location, count, ( const GLint* )value );
			GL_LOG_ERRORS ( "glUniform4iv" )
			break;
	}
}

//----------------------------------------------------------------//
void ZLGfxImmediate::UseProgram ( ZLGfxResource& program ) {

	glUseProgram (( GLuint )program.GLID ());
	GL_LOG_ERRORS ( "glUseProgram" )
}

//----------------------------------------------------------------//
void ZLGfxImmediate::VertexAttribPointer ( u32 index, u32 size, ZLGfxEnum::Type type, bool normalized, u32 stride, ZLSharedConstBuffer* buffer, size_t offset ) {

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
