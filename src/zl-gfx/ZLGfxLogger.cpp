// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-gfx/headers.h>
#include <zl-gfx/ZLGfx-gles.h>
#include <zl-gfx/ZLGfxEnum.h>
#include <zl-gfx/ZLGfxLogger.h>
#include <zl-gfx/ZLGfxRetained.h>

//================================================================//
// ZLGfxLogger
//================================================================//

//----------------------------------------------------------------//
void ZLGfxLogger::ActiveTexture ( u32 textureUnit ) {

	this->PrintLine ( "glActiveTexture - texture unit: %d\n", textureUnit );
}

//----------------------------------------------------------------//
void ZLGfxLogger::AllocateResource ( ZLGfxResource& resource, ZLGfxEnum::_ param ) {
	
	switch ( resource.mType ) {
	
		case ZLGfxResource::BUFFER:
			this->PrintLine ( "glGenBuffers\n" );
			break;
			
		case ZLGfxResource::FRAMEBUFFER:
			this->PrintLine ( "glGenFramebuffers\n" );
			break;
			
		case ZLGfxResource::PROGRAM:
			this->PrintLine ( "glCreateProgram\n" );
			break;
			
		case ZLGfxResource::SHADER:
			this->PrintLine ( "glCreateShader - param: %d\n", param );
			break;
			
		case ZLGfxResource::TEXTURE:
			this->PrintLine ( "glGenTextures\n" );
			break;
			
		case ZLGfxResource::RENDERBUFFER:
			this->PrintLine ( "glGenRenderbuffers\n" );
			break;
			
		case ZLGfxResource::VERTEXARRAY:
			#ifndef MOAI_OS_ANDROID
				this->PrintLine ( "glGenVertexArrays\n" );
			#endif
			break;
	}
}

//----------------------------------------------------------------//
void ZLGfxLogger::AttachShader ( ZLGfxResource& program, ZLGfxResource& shader ) {

	this->PrintLine ( "glAttachShader - program: %d shader: %d\n", program.mGLID, shader.mGLID);
}

//----------------------------------------------------------------//
void ZLGfxLogger::BindAttribLocation ( ZLGfxResource& program, u32 index, cc8* name ) {

	this->PrintLine ( "glBindAttribLocation: program: %d index: %d name: %s\n", program.mGLID, index, name );
}

//----------------------------------------------------------------//
void ZLGfxLogger::BindBuffer ( ZLGfxEnum::_ target, ZLGfxResource& handle ) {

	this->PrintLine ( "glBindBuffer - target: %d handle: %d\n", target, handle.mGLID );
}

//----------------------------------------------------------------//
void ZLGfxLogger::BindFramebuffer ( ZLGfxEnum::_ target, ZLGfxResource& handle ) {

	this->PrintLine ( "glBindFramebuffer - target: %d handle: %d\n", target, handle.mGLID );
}

//----------------------------------------------------------------//
void ZLGfxLogger::BindRenderbuffer ( ZLGfxResource& handle ) {

	this->PrintLine ( "glBindRenderbuffer - handle: %d\n", handle.mGLID );
}

//----------------------------------------------------------------//
void ZLGfxLogger::BindTexture ( ZLGfxResource& handle ) {

	this->PrintLine ( "glBindTexture - handle: %d\n", handle.mGLID );
}

//----------------------------------------------------------------//
void ZLGfxLogger::BindVertexArray ( ZLGfxResource& handle ) {

	this->PrintLine ( "glBindVertexArray - handle: %d\n", handle.mGLID );
}

//----------------------------------------------------------------//
void ZLGfxLogger::BlendFunc ( ZLGfxEnum::_ sourceFactor, ZLGfxEnum::_ destFactor ) {

	this->PrintLine ( "glBlendFunc - sourceFactor: %d destFactor: %d\n", sourceFactor, destFactor );
}

//----------------------------------------------------------------//
void ZLGfxLogger::BlendMode ( ZLGfxEnum::_ mode ) {

	this->PrintLine ( "glBlendEquation - mode: %d\n", mode );
}

//----------------------------------------------------------------//
void ZLGfxLogger::BufferData ( ZLGfxEnum::_ target, size_t size, ZLSharedConstBuffer* buffer, size_t offset, ZLGfxEnum::_ usage ) {

	const void* data = ( const void* )ZLSharedConstBuffer::GetConstData ( buffer );
	this->PrintLine ( "glBufferData - target: %d size: %d buffer: %p offset: %d usage: %d\n", target, size, data, offset, usage );
}

//----------------------------------------------------------------//
void ZLGfxLogger::BufferSubData ( ZLGfxEnum::_ target, size_t offset, size_t size, ZLSharedConstBuffer* buffer, size_t srcOffset ) {

	const void* data = ( const void* )ZLSharedConstBuffer::GetConstData ( buffer );
	this->PrintLine ( "glBufferSubData - target: %d offset: %d size: %d buffer: %p srcOffset: %d\n", target, offset, size, data, srcOffset );
}

//----------------------------------------------------------------//
void ZLGfxLogger::CheckFramebufferStatus ( ZLGfxEnum::_ target ) {
	
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
void ZLGfxLogger::CompileShader ( ZLGfxResource& shader, bool log ) {
	UNUSED ( log );

	this->PrintLine ( "glCompileShader - shader: %d\n", shader.mGLID );
}

//----------------------------------------------------------------//
void ZLGfxLogger::CompressedTexImage2D ( u32 level, ZLGfxEnum::_ internalFormat, u32 width, u32 height, u32 imageSize, ZLSharedConstBuffer* buffer ) {
	
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
void ZLGfxLogger::CullFace ( ZLGfxEnum::_ mode ) {

	this->PrintLine ( "glCullFace - mode: %d\n", mode );
}

//----------------------------------------------------------------//
void ZLGfxLogger::DeleteResource ( ZLGfxResource& resource ) {

	switch ( resource.mType ) {
	
		case ZLGfxResource::BUFFER:
			this->PrintLine ( "glDeleteBuffers - handle: %d\n", resource.mGLID );
			break;
		
		case ZLGfxResource::FRAMEBUFFER:
			this->PrintLine ( "glDeleteFramebuffers - handle: %d\n", resource.mGLID );
			break;
		
		case ZLGfxResource::PROGRAM: {
			this->PrintLine ( "glDeleteProgram - handle: %d\n", resource.mGLID );
			break;
		}
		case ZLGfxResource::SHADER: {
			this->PrintLine ( "glDeleteShader - handle: %d\n", resource.mGLID );
			break;
		}
		case ZLGfxResource::TEXTURE:
			this->PrintLine ( "glDeleteTextures - handle: %d\n", resource.mGLID );
			break;
		
		case ZLGfxResource::RENDERBUFFER:
			this->PrintLine ( "glDeleteRenderbuffers - handle: %d\n", resource.mGLID );
			break;
		
		case ZLGfxResource::VERTEXARRAY:
			#ifndef MOAI_OS_ANDROID
				this->PrintLine ( "glDeleteVertexArrays - handle: %d\n", resource.mGLID );
			#endif
		break;
	}
}

//----------------------------------------------------------------//
void ZLGfxLogger::DepthFunc ( ZLGfxEnum::_ depthFunc ) {

	this->PrintLine ( "glDepthFunc - depthFunc: %d\n", depthFunc );
}

//----------------------------------------------------------------//
void ZLGfxLogger::DepthMask ( bool flag ) {

	this->PrintLine ( "glDepthMask - flag: %s\n", flag ? "true" : "false" );
}

//----------------------------------------------------------------//
void ZLGfxLogger::Disable ( ZLGfxEnum::_ cap ) {

	this->PrintLine ( "glDisable - cap: %d\n", cap );
}

//----------------------------------------------------------------//
void ZLGfxLogger::DisableClientState ( ZLGfxEnum::_ cap ) {

	#if !MOAI_OS_NACL
		this->PrintLine ( "glDisableClientState - cap: %d\n", cap );
	#endif
}

//----------------------------------------------------------------//
void ZLGfxLogger::DisableVertexAttribArray ( u32 index ) {

	this->PrintLine ( "glDisableVertexAttribArray - index: %d\n", index );
}

//----------------------------------------------------------------//
void ZLGfxLogger::DrawArrays ( ZLGfxEnum::_ primType, u32 first, u32 count ) {

	this->PrintLine ( "glDrawArrays - primType: %d first: %d count: %d\n", primType, first, count );
}

//----------------------------------------------------------------//
void ZLGfxLogger::DrawElements ( ZLGfxEnum::_ primType, u32 count, ZLGfxEnum::_ indexType, ZLSharedConstBuffer* buffer, size_t offset ) {

	const void* data = ( const void* )ZLSharedConstBuffer::GetConstData ( buffer );
	this->PrintLine ( "glDrawElements - primType: %d count: %d indexType: %p buffer: %d offset: %d\n", primType, count, indexType, data, offset );
}

//----------------------------------------------------------------//
void ZLGfxLogger::Enable ( ZLGfxEnum::_ cap ) {

	this->PrintLine ( "glEnable - cap: %d\n", cap );
}

//----------------------------------------------------------------//
void ZLGfxLogger::EnableClientState ( ZLGfxEnum::_ cap ) {

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
void ZLGfxLogger::FramebufferRenderbuffer ( ZLGfxEnum::_ target, ZLGfxEnum::_ attachment, ZLGfxResource& renderbuffer ) {
	
	this->PrintLine ( "glFramebufferRenderbuffer - target: %d attachment: %d renderbuffer: %d\n", target, attachment, renderbuffer.mGLID );
}

//----------------------------------------------------------------//
void ZLGfxLogger::FramebufferTexture2D ( ZLGfxEnum::_ target, ZLGfxEnum::_ attachment, ZLGfxResource& texture, s32 level ) {

	this->PrintLine ( "glFramebufferTexture2D - target: %d attachment: %d texture: %d level: %d\n", target, attachment, texture.mGLID, level );
}

//----------------------------------------------------------------//
void ZLGfxLogger::GetCurrentFramebuffer ( ZLGfxResource& framebuffer ) {
	UNUSED ( framebuffer );

	this->PrintLine ( "glGetIntegerv - GET CURRENT FRAMEBUFFER\n" );
}

//----------------------------------------------------------------//
void ZLGfxLogger::GetUniformLocation ( ZLGfxResource& program, cc8* uniformName, ZLGfxListener* listener, void* userdata ) {

	this->PrintLine ( "glGetUniformLocation - program: %d uniformName: %s listener: %p userdata: %p\n", program.mGLID, uniformName, listener, userdata );
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
void ZLGfxLogger::LinkProgram ( ZLGfxResource& program, bool log ) {
	UNUSED ( log );

	this->PrintLine ( "glLinkProgram - program: %d\n", program.mGLID );
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
void ZLGfxLogger::ReadPixels ( s32 x, s32 y, u32 width, u32 height, ZLGfxEnum::_ format, ZLGfxEnum::_ type, u32 pixelSize, ZLGfxListener* listener, void* userdata ) {

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
void ZLGfxLogger::RenderbufferStorage ( ZLGfxEnum::_ internalFormat, u32 width, u32 height ) {

	this->PrintLine ( "glReadPixels - internalFormat: %d width: %d height: %d\n", internalFormat, width, height );
}

//----------------------------------------------------------------//
void ZLGfxLogger::Scissor ( s32 x, s32 y, u32 w, u32 h ) {

	this->PrintLine ( "glScissor - x: %d y: %d w: %d h: %d\n", x, y, w, h );
}

//----------------------------------------------------------------//
void ZLGfxLogger::ShaderSource ( ZLGfxResource& shader, cc8* source, size_t length ) {
	UNUSED ( source );
	UNUSED ( shader );

	this->PrintLine ( "glShaderSource - length: %d\n", length );
}

//----------------------------------------------------------------//
void ZLGfxLogger::TexEnvi ( ZLGfxEnum::_ pname, ZLGfxEnum::_ param ) {

	#if !MOAI_OS_NACL
		this->PrintLine ( "glTexEnvi - pname: %d param: %d\n", pname, param );
	#endif
}

//----------------------------------------------------------------//
void ZLGfxLogger::TexImage2D ( u32 level, ZLGfxEnum::_ internalFormat, u32 width, u32 height, ZLGfxEnum::_ format, ZLGfxEnum::_ type, ZLSharedConstBuffer* buffer ) {

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
void ZLGfxLogger::TexParameteri ( ZLGfxEnum::_ pname, ZLGfxEnum::_ param ) {

	this->PrintLine ( "glTexParameteri - pname: %d param: %d\n", pname, param );
}

//----------------------------------------------------------------//
void ZLGfxLogger::TexSubImage2D ( u32 level, s32 xOffset, s32 yOffset, u32 width, u32 height, ZLGfxEnum::_ format, ZLGfxEnum::_ type, ZLSharedConstBuffer* buffer ) {
	
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
void ZLGfxLogger::UniformFloat ( u32 location, u32 index, u32 width, u32 count, const float* value ) {

	if ( width <= 4 ) {

		this->PrintLine ( "glUniform*fv - location: %d index: %d width: %d count: %d\n", location, index, width, count );
	}
	else if ( width == 9 ) {
	
		this->PrintLine ( "glUniformMatrix3fv - location: %d index: %d count: %d\n", location, index, count );
	}
	else if ( width == 16 ) {
	
		this->PrintLine ( "glUniformMatrix4fv - location: %d index: %d count: %d\n", location, index, count );
	}
	
	this->PrintLine ( " value: " );
	for ( u32 j = 0; j < width; ++j ) {
		this->PrintLine ( " %f", value [ j ]);
	}
	this->PrintLine ( "\n" );
}

//----------------------------------------------------------------//
void ZLGfxLogger::UniformInt ( u32 location, u32 index, u32 width, u32 count, const s32* value ) {

	this->PrintLine ( "glUniform*iv - location: %d index: %d width: %d count: %d\n", location, index, width, count );

	for ( u32 i = 0; i < 1; ++i ) {
		const s32* v = &value [ i * width ];
		
		this->PrintLine ( " value[%d]: ", i );
		
		for ( u32 j = 0; j < width; ++j ) {
			this->PrintLine ( " %d", v [ j ]);
		}
	}
	this->PrintLine ( "\n" );
}

//----------------------------------------------------------------//
void ZLGfxLogger::UseProgram ( ZLGfxResource& program ) {

	this->PrintLine ( "glUseProgram - program: %d\n", program.mGLID );
}

//----------------------------------------------------------------//
void ZLGfxLogger::VertexAttribPointer ( u32 index, u32 size, ZLGfxEnum::_ type, bool normalized, u32 stride, ZLSharedConstBuffer* buffer, size_t offset ) {

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
