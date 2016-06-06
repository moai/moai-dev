// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-gfx/headers.h>
#include <zl-gfx/zl_gfx.h>
#include <zl-gfx/ZLGfx-gles.h>
#include <zl-gfx/ZLGfxEnum.h>
#include <zl-gfx/ZLGfxLogger.h>
#include <zl-gfx/ZLGfxRetained.h>
#include <zl-util/ZLLog.h>

#define DEREF_HANDLE(handle) ( handle ? handle->mGLID : 0 )

//================================================================//
// ZLGfxLogger
//================================================================//

//----------------------------------------------------------------//
void ZLGfxLogger::ActiveTexture ( u32 textureUnit ) {

	this->PrintLine ( "glActiveTexture - texture unit: %d\n", textureUnit );
}

//----------------------------------------------------------------//
void ZLGfxLogger::AttachShader ( ZLGfxHandle* program, ZLGfxHandle* shader ) {

	this->PrintLine ( "glAttachShader - program: %d shader: %d\n", DEREF_HANDLE ( program ), DEREF_HANDLE ( shader ));
}

//----------------------------------------------------------------//
void ZLGfxLogger::BindAttribLocation ( ZLGfxHandle* program, u32 index, cc8* name ) {

	this->PrintLine ( "glBindAttribLocation: program: %d index: %d name: %s\n", DEREF_HANDLE ( program ), index, name );
}

//----------------------------------------------------------------//
void ZLGfxLogger::BindBuffer ( u32 target, ZLGfxHandle* handle ) {

	this->PrintLine ( "glBindBuffer - target: %d handle: %d\n", target, DEREF_HANDLE ( handle ));
}

//----------------------------------------------------------------//
void ZLGfxLogger::BindFramebuffer ( u32 target, ZLGfxHandle* handle ) {

	this->PrintLine ( "glBindFramebuffer - target: %d handle: %d\n", target, DEREF_HANDLE ( handle ));
}

//----------------------------------------------------------------//
void ZLGfxLogger::BindRenderbuffer ( ZLGfxHandle* handle ) {

	this->PrintLine ( "glBindRenderbuffer - handle: %d\n", DEREF_HANDLE ( handle ));
}

//----------------------------------------------------------------//
void ZLGfxLogger::BindTexture ( ZLGfxHandle* handle ) {

	this->PrintLine ( "glBindTexture - handle: %d\n", DEREF_HANDLE ( handle ));
}

//----------------------------------------------------------------//
void ZLGfxLogger::BindVertexArray ( ZLGfxHandle* handle ) {

	this->PrintLine ( "glBindVertexArray - handle: %d\n", DEREF_HANDLE ( handle ));
}

//----------------------------------------------------------------//
void ZLGfxLogger::BlendFunc ( u32 sourceFactor, u32 destFactor ) {

	this->PrintLine ( "glBlendFunc - sourceFactor: %d destFactor: %d\n", sourceFactor, destFactor );
}

//----------------------------------------------------------------//
void ZLGfxLogger::BlendMode ( u32 mode ) {

	this->PrintLine ( "glBlendEquation - mode: %d\n", mode );
}

//----------------------------------------------------------------//
void ZLGfxLogger::BufferData ( u32 target, size_t size, ZLSharedConstBuffer* buffer, size_t offset, u32 usage ) {

	const void* data = ( const void* )ZLSharedConstBuffer::GetConstData ( buffer );
	this->PrintLine ( "glBufferData - target: %d size: %d buffer: %p offset: %d usage: %d\n", target, size, data, offset, usage );
}

//----------------------------------------------------------------//
void ZLGfxLogger::BufferSubData ( u32 target, size_t offset, size_t size, ZLSharedConstBuffer* buffer, size_t srcOffset ) {

	const void* data = ( const void* )ZLSharedConstBuffer::GetConstData ( buffer );
	this->PrintLine ( "glBufferSubData - target: %d offset: %d size: %d buffer: %p srcOffset: %d\n", target, offset, size, data, srcOffset );
}

//----------------------------------------------------------------//
void ZLGfxLogger::CheckFramebufferStatus ( u32 target ) {
	
	this->PrintLine ( "glCheckFramebufferStatus - target: %d\n", target );
}

//----------------------------------------------------------------//
void ZLGfxLogger::Clear ( u32 mask ) {
	
	this->PrintLine ( "glClear - mask: 0x%08x\n", mask );
}

//----------------------------------------------------------------//
void ZLGfxLogger::ClearColor ( float r, float g, float b, float a ) {

	this->PrintLine ( "glClearColor - r: %f g: %f b: %f a: %f\n", r, g, b, a );
}

//----------------------------------------------------------------//
void ZLGfxLogger::Color ( float r, float g, float b, float a ) {

	this->PrintLine ( "glColor4f - r: %f g: %f b: %f a: %f\n", r, g, b, a );
}

//----------------------------------------------------------------//
void ZLGfxLogger::Comment ( cc8* comment ) {

	this->PrintLine ( comment ? "COMMENT - %s\n" : "\n", comment );
	this->mCommentNo++;
}

//----------------------------------------------------------------//
void ZLGfxLogger::CompileShader ( ZLGfxHandle* shader, bool log ) {
	UNUSED ( log );

	this->PrintLine ( "glCompileShader - shader: %d\n", DEREF_HANDLE ( shader ));
}

//----------------------------------------------------------------//
void ZLGfxLogger::CompressedTexImage2D ( u32 level, u32 internalFormat, u32 width, u32 height, u32 imageSize, ZLSharedConstBuffer* buffer ) {
	
	//const void* data = ( const void* )ZLSharedConstBuffer::GetConstData ( buffer );
	
	this->PrintLine ( "glCompressedTexImage2D - level: %d internalFormat: %d width: %d height: %d imageSize: %d buffer: %p\n",
		level,
		internalFormat,
		width,
		height,
		imageSize,
		buffer
	);
}

//----------------------------------------------------------------//
ZLSharedConstBuffer* ZLGfxLogger::CopyBuffer ( ZLSharedConstBuffer* buffer ) {

	return buffer;
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxLogger::Create ( ZLGfxHandle* handle, u32 param ) {

	if ( handle ) {
	
		switch ( handle->mType ) {
		
			case ZLGfxHandle::BUFFER:
				this->PrintLine ( "glGenBuffers\n" );
				break;
				
			case ZLGfxHandle::FRAMEBUFFER:
				this->PrintLine ( "glGenFramebuffers\n" );
				break;
				
			case ZLGfxHandle::PROGRAM:
				this->PrintLine ( "glCreateProgram\n" );
				break;
				
			case ZLGfxHandle::SHADER:
				this->PrintLine ( "glCreateShader - param: %d\n", param );
				break;
				
			case ZLGfxHandle::TEXTURE:
				this->PrintLine ( "glGenTextures\n" );
				break;
				
			case ZLGfxHandle::RENDERBUFFER:
				this->PrintLine ( "glGenRenderbuffers\n" );
				break;
				
			case ZLGfxHandle::VERTEXARRAY:
				#ifndef MOAI_OS_ANDROID
					this->PrintLine ( "glGenVertexArrays\n" );
				#endif
				break;
		}
	}
	return 0;
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxLogger::CreateBuffer () {

	return this->Create ( new ZLGfxHandle ( ZLGfxHandle::BUFFER, 0, true ), 0 );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxLogger::CreateFramebuffer () {

	return this->Create ( new ZLGfxHandle ( ZLGfxHandle::FRAMEBUFFER, 0, true ), 0 );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxLogger::CreateProgram () {

	return this->Create ( new ZLGfxHandle ( ZLGfxHandle::PROGRAM, 0, true ), 0 );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxLogger::CreateRenderbuffer () {

	return this->Create ( new ZLGfxHandle ( ZLGfxHandle::RENDERBUFFER, 0, true ), 0 );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxLogger::CreateShader ( u32 shaderType ) {

	return this->Create ( new ZLGfxHandle ( ZLGfxHandle::SHADER, 0, true ), shaderType );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxLogger::CreateTexture () {

	return this->Create ( new ZLGfxHandle ( ZLGfxHandle::TEXTURE, 0, true ), 0 );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxLogger::CreateVertexArray () {

	return this->Create ( new ZLGfxHandle ( ZLGfxHandle::VERTEXARRAY, 0, true ), 0 );
}

//----------------------------------------------------------------//
void ZLGfxLogger::CullFace ( u32 mode ) {

	this->PrintLine ( "glCullFace - mode: %d\n", mode );
}

//----------------------------------------------------------------//
void ZLGfxLogger::Delete ( u32 type, u32 glid ) {

	switch ( type ) {
	
		case ZLGfxHandle::BUFFER:
			this->PrintLine ( "glDeleteBuffers - handle: %d\n", glid );
			break;
		
		case ZLGfxHandle::FRAMEBUFFER:
			this->PrintLine ( "glDeleteFramebuffers - handle: %d\n", glid );
			break;
		
		case ZLGfxHandle::PROGRAM: {
			this->PrintLine ( "glDeleteProgram - handle: %d\n", glid );
			break;
		}
		case ZLGfxHandle::SHADER: {
			this->PrintLine ( "glDeleteShader - handle: %d\n", glid );
			break;
		}
		case ZLGfxHandle::TEXTURE:
			this->PrintLine ( "glDeleteTextures - handle: %d\n", glid );
			break;
		
		case ZLGfxHandle::RENDERBUFFER:
			this->PrintLine ( "glDeleteRenderbuffers - handle: %d\n", glid );
			break;
		
		case ZLGfxHandle::VERTEXARRAY:
			#ifndef MOAI_OS_ANDROID
				this->PrintLine ( "glDeleteVertexArrays - handle: %d\n", glid );
			#endif
		break;
	}
}

//----------------------------------------------------------------//
void ZLGfxLogger::DepthFunc ( u32 depthFunc ) {

	this->PrintLine ( "glDepthFunc - depthFunc: %d\n", depthFunc );
}

//----------------------------------------------------------------//
void ZLGfxLogger::DepthMask ( bool flag ) {

	this->PrintLine ( "glDepthMask - flag: %s\n", flag ? "true" : "false" );
}

//----------------------------------------------------------------//
void ZLGfxLogger::Disable ( u32 cap ) {

	this->PrintLine ( "glDisable - cap: %d\n", cap );
}

//----------------------------------------------------------------//
void ZLGfxLogger::DisableClientState ( u32 cap ) {

	#if !MOAI_OS_NACL
		this->PrintLine ( "glDisableClientState - cap: %d\n", cap );
	#endif
}

//----------------------------------------------------------------//
void ZLGfxLogger::DisableVertexAttribArray ( u32 index ) {

	this->PrintLine ( "glDisableVertexAttribArray - index: %d\n", index );
}

//----------------------------------------------------------------//
void ZLGfxLogger::DrawArrays ( u32 primType, u32 first, u32 count ) {

	this->PrintLine ( "glDrawArrays - primType: %d first: %d count: %d\n", primType, first, count );
}

//----------------------------------------------------------------//
void ZLGfxLogger::DrawElements ( u32 primType, u32 count, u32 indexType, ZLSharedConstBuffer* buffer, size_t offset ) {

	const void* data = ( const void* )ZLSharedConstBuffer::GetConstData ( buffer );
	this->PrintLine ( "glDrawElements - primType: %d count: %d indexType: %p buffer: %d offset: %d\n", primType, count, indexType, data, offset );
}

//----------------------------------------------------------------//
void ZLGfxLogger::Enable ( u32 cap ) {

	this->PrintLine ( "glEnable - cap: %d\n", cap );
}

//----------------------------------------------------------------//
void ZLGfxLogger::EnableClientState ( u32 cap ) {

	#if !MOAI_OS_NACL
		this->PrintLine ( "glEnableClientState - cap: %d\n", cap );
	#endif
}

//----------------------------------------------------------------//
void ZLGfxLogger::EnableVertexAttribArray ( u32 index ) {

	this->PrintLine ( "glEnableVertexAttribArray - index: %d\n", index );
}

//----------------------------------------------------------------//
void ZLGfxLogger::Event ( ZLGfxListener* listener, u32 event, void* userdata ) {

	this->PrintLine ( "EVENT - listener: %p event: %d userdata: %p\n", listener, event, userdata );
}

//----------------------------------------------------------------//
void ZLGfxLogger::Flush ( bool finish ) {

	this->PrintLine ( "glFlush\n" );

	if ( finish ) {
		this->PrintLine ( "glFinish\n" );
	}
}

//----------------------------------------------------------------//
void ZLGfxLogger::FramebufferRenderbuffer ( u32 target, u32 attachment, ZLGfxHandle* renderbuffer ) {
	
	this->PrintLine ( "glFramebufferRenderbuffer - target: %d attachment: %d renderbuffer: %d\n", target, attachment, DEREF_HANDLE ( renderbuffer ));
}

//----------------------------------------------------------------//
void ZLGfxLogger::FramebufferTexture2D ( u32 target, u32 attachment, ZLGfxHandle* texture, s32 level ) {

	this->PrintLine ( "glFramebufferTexture2D - target: %d attachment: %d texture: %d level: %d\n", target, attachment, DEREF_HANDLE ( texture ), level );
}

//----------------------------------------------------------------//
ZLGfxHandle* ZLGfxLogger::GetCurrentFramebuffer () {

	this->PrintLine ( "GET CURRENT FRAMEBUFFER\n" );
	return 0;
}

//----------------------------------------------------------------//
void ZLGfxLogger::GetUniformLocation ( ZLGfxHandle* program, cc8* uniformName, ZLGfxListener* listener, void* userdata ) {

	this->PrintLine ( "glGetUniformLocation - program: %d uniformName: %s listener: %p userdata: %p\n", DEREF_HANDLE ( program ), uniformName, listener, userdata );
}

//----------------------------------------------------------------//
bool ZLGfxLogger::IsImmediate () {

	return true;
}

//----------------------------------------------------------------//
void ZLGfxLogger::LineWidth ( float width ) {

	this->PrintLine ( "glLineWidth - width: %f\n", width );
}

//----------------------------------------------------------------//
void ZLGfxLogger::LinkProgram ( ZLGfxHandle* program, bool log ) {
	UNUSED ( log );

	this->PrintLine ( "glLinkProgram - program: %d\n", DEREF_HANDLE ( program ));
}

//----------------------------------------------------------------//
void ZLGfxLogger::PopSection () {

	this->PrintLine ( "POP SECTION\n" );
}

//----------------------------------------------------------------//
void ZLGfxLogger::Print ( cc8* filename, ZLGfxRetained& gfx ) {

	this->mPrefix = ( const void* )&gfx;

	this->mFile = 0;
	if ( filename ) {
		this->mFile = fopen ( filename, "w" );
		assert ( this->mFile );
	}

	this->mLineNo = 0;
	this->mCommentNo = 0;
	
	gfx.Draw ( *this );
	
	if ( this->mFile ) {
		fclose ( this->mFile );
		this->mFile = 0;
	}
}

//----------------------------------------------------------------//
void ZLGfxLogger::PrintLine ( cc8* format, ... ) {

	ZLLogF ( this->mFile, "%p %d: ", this->mPrefix, this->mLineNo++ );

	va_list args;
	va_start ( args, format );
	
	ZLLogV ( this->mFile, format, args );
	
	va_end ( args );
}

//----------------------------------------------------------------//
bool ZLGfxLogger::PushErrorHandler () {

	this->PrintLine ( "PUSH ERROR HANDLER\n" );
	return true;
}

//----------------------------------------------------------------//
void ZLGfxLogger::PushSection () {

	this->PrintLine ( "PUSH SECTION\n" );
}

//----------------------------------------------------------------//
bool ZLGfxLogger::PushSuccessHandler () {

	this->PrintLine ( "PUSH SUCCESS HANDLER\n" );
	return true;
}

//----------------------------------------------------------------//
void ZLGfxLogger::ReadPixels ( s32 x, s32 y, u32 width, u32 height, u32 format, u32 type, u32 pixelSize, ZLGfxListener* listener, void* userdata ) {

	this->PrintLine ( "glReadPixels - x: %d y: %d width: %d height: %d format: %d type: %d pixelSize: %d listener: %p userdata: %p\n",
		x,
		y,
		width,
		height,
		format,
		type,
		pixelSize,
		listener,
		userdata
	);
}

//----------------------------------------------------------------//
void ZLGfxLogger::RenderbufferStorage ( u32 internalFormat, u32 width, u32 height ) {

	this->PrintLine ( "glReadPixels - internalFormat: %d width: %d height: %d\n", internalFormat, width, height );
}

//----------------------------------------------------------------//
void ZLGfxLogger::Scissor ( s32 x, s32 y, u32 w, u32 h ) {

	this->PrintLine ( "glScissor - x: %d y: %d w: %d h: %d\n", x, y, w, h );
}

//----------------------------------------------------------------//
void ZLGfxLogger::ShaderSource ( ZLGfxHandle* shader, cc8* source, size_t length ) {
	UNUSED ( source );
	UNUSED ( shader );

	this->PrintLine ( "glShaderSource - length: %d\n", length );
}

//----------------------------------------------------------------//
void ZLGfxLogger::TexEnvi ( u32 pname, s32 param ) {

	#if !MOAI_OS_NACL
		this->PrintLine ( "glTexEnvi - pname: %d param: %d\n", pname, param );
	#endif
}

//----------------------------------------------------------------//
void ZLGfxLogger::TexImage2D ( u32 level, u32 internalFormat, u32 width, u32 height, u32 format, u32 type, ZLSharedConstBuffer* buffer ) {

	this->PrintLine ( "glPixelStorei - pname: %d param: %d\n", GL_UNPACK_ALIGNMENT, 1 );

	const void* data = ( const void* )ZLSharedConstBuffer::GetConstData ( buffer );

	this->PrintLine ( "glTexImage2D - level: %d internalFormat: %d width: %d height: %d format: %d type: %d buffer: %p\n",
		level,
		internalFormat,
		width,
		height,
		format,
		type,
		data
	);
}

//----------------------------------------------------------------//
void ZLGfxLogger::TexParameteri ( u32 pname, s32 param ) {

	this->PrintLine ( "glTexParameteri - pname: %d param: %d\n", pname, param );
}

//----------------------------------------------------------------//
void ZLGfxLogger::TexSubImage2D ( u32 level, s32 xOffset, s32 yOffset, u32 width, u32 height, u32 format, u32 type, ZLSharedConstBuffer* buffer ) {
	
	this->PrintLine ( "glPixelStorei - pname: %d param: %d\n", GL_UNPACK_ALIGNMENT, 1 );

	const void* data = ( const void* )ZLSharedConstBuffer::GetConstData ( buffer );

	this->PrintLine ( "glTexSubImage2D - level: %d xOffset: %d yOffset: %d width: %d height: %d format: %d type: %d buffer: %p\n",
		level,
		xOffset,
		yOffset,
		width,
		height,
		format,
		type,
		data
	);
}

//----------------------------------------------------------------//
void ZLGfxLogger::Uniform1f ( u32 location, float v0 ) {

	this->PrintLine ( "glUniform1f - location: %d v0: %f\n", location, v0 );
}

//----------------------------------------------------------------//
void ZLGfxLogger::Uniform1i ( u32 location, s32 v0 ) {

	this->PrintLine ( "glUniform1i - location: %d v0: %d\n", location, v0 );
}

//----------------------------------------------------------------//
void ZLGfxLogger::Uniform4fv ( u32 location, u32 count, const float* value ) {

	this->PrintLine ( "glUniform4fv - location: %d count: %d\n", location, count );

	for ( u32 i = 0; i < count; ++i ) {
		const float* v = &value [ i * 4 ];
		
		this->PrintLine ( " value[%d]: ", i );
		
		for ( u32 j = 0; j < 4; ++j ) {
			this->PrintLine ( " %f", v [ j ]);
		}
	}
	this->PrintLine ( "\n" );
}

//----------------------------------------------------------------//
void ZLGfxLogger::UniformMatrix3fv ( u32 location, u32 count, bool transpose, const float* mtx ) {

	this->PrintLine ( "glUniformMatrix3fv - location: %d count: %d transpose: %s\n", location, count, transpose ? "true" : "false" );

	for ( u32 i = 0; i < count; ++i ) {
		const float* v = &mtx [ i * 4 ];
		
		this->PrintLine ( " value[%d]: ", i );
		
		for ( u32 j = 0; j < 9; ++j ) {
			this->PrintLine ( " %f", v [ j ]);
		}
	}
	this->PrintLine ( "\n" );
}

//----------------------------------------------------------------//
void ZLGfxLogger::UniformMatrix4fv ( u32 location, u32 count, bool transpose, const float* mtx ) {

	this->PrintLine ( "glUniformMatrix4fv - location: %d count: %d transpose: %s\n", location, count, transpose ? "true" : "false" );

	for ( u32 i = 0; i < count; ++i ) {
		const float* v = &mtx [ i * 4 ];
		
		this->PrintLine ( " value[%d]: ", i );
		
		for ( u32 j = 0; j < 16; ++j ) {
			this->PrintLine ( " %f", v [ j ]);
		}
	}
	this->PrintLine ( "\n" );
}

//----------------------------------------------------------------//
void ZLGfxLogger::UseProgram ( ZLGfxHandle* program ) {

	this->PrintLine ( "glUseProgram - program: %d\n", DEREF_HANDLE ( program ));
}

//----------------------------------------------------------------//
void ZLGfxLogger::VertexAttribPointer ( u32 index, u32 size, u32 type, bool normalized, u32 stride, ZLSharedConstBuffer* buffer, size_t offset ) {

	const void* data = ( const void* )ZLSharedConstBuffer::GetConstData ( buffer );
	
	this->PrintLine ( "glVertexAttribPointer - index: %d size: %d type: %d normalized: %s stride: %d buffer: %p offset: %d\n",
		index,
		size,
		type,
		normalized ? "true" : "false",
		stride,
		data,
		offset
	);
}

//----------------------------------------------------------------//
void ZLGfxLogger::Viewport ( s32 x, s32 y, u32 w, u32 h ) {
	
	this->PrintLine ( "glViewport - x: %d y: %d w: %d h: %d\n", x, y, w, h );
}

//----------------------------------------------------------------//
ZLGfxLogger::ZLGfxLogger () :
	mPrefix ( 0 ),
	mLineNo ( 0 ),
	mCommentNo ( 0 ),
	mFile ( 0 ) {
}

//----------------------------------------------------------------//
ZLGfxLogger::~ZLGfxLogger () {
}
