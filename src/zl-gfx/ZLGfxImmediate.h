// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLGFXIMMEDIATE_H
#define ZLGFXIMMEDIATE_H

#include <zl-gfx/ZLGfx.h>

//================================================================//
// ZLGfxImmediate
//================================================================//
class ZLGfxImmediate :
	public ZLGfx {
public:

	//----------------------------------------------------------------//
	void					ActiveTexture				( u32 textureUnit );
	void					AttachShader				( ZLGfxHandle* program, ZLGfxHandle* shader );
	
	void					BindAttribLocation			( ZLGfxHandle* program, u32 index, cc8* name );
	
	void					BindBuffer					( u32 target, ZLGfxHandle* handle );
	void					BindFramebuffer				( u32 target, ZLGfxHandle* handle );
	void					BindRenderbuffer			( ZLGfxHandle* handle );
	void					BindTexture					( ZLGfxHandle* handle );
	void					BindVertexArray				( ZLGfxHandle* handle );
	
	void					BlendFunc					( u32 sourceFactor, u32 destFactor );
	void					BlendMode					( u32 mode );
	void					BufferData					( u32 target, u32 size, const void* data, u32 usage );
	void					BufferSubData				( u32 target, u32 offset, u32 size, const void* data );
	void					Clear						( u32 mask );
	void					ClearColor					( float r, float g, float b, float a );
	void					Color						( float r, float g, float b, float a );
	
	void					CompileShader				( ZLGfxHandle* shader );
	
	ZLGfxHandle*			CreateBuffer				();
	ZLGfxHandle*			CreateFramebuffer			();
	ZLGfxHandle*			CreateProgram				();
	ZLGfxHandle*			CreateRenderbuffer			();
	ZLGfxHandle*			CreateShader				( u32 shaderType );
	ZLGfxHandle*			CreateTexture				();
	ZLGfxHandle*			CreateVertexArray			();
	
	void					CullFace					( u32 mode );
	
	void					DeleteHandle				( ZLGfxHandle* handle );
	
	void					DepthFunc					( u32 depthFunc );
	void					DepthMask					( bool flag );
	void					Disable						( u32 cap );
	void					DisableClientState			( u32 cap );
	void					DisableVertexAttribArray	( u32 index );
	void					DrawArrays					( u32 primType, u32 first, u32 count );
	void					DrawElements				( u32 primType, u32 count, u32 indexType, const void* indices );
	void					Enable						( u32 cap );
	void					EnableClientState			( u32 cap );
	void					EnableVertexAttribArray		( u32 index );
	void					Flush						();
	void					FramebufferRenderbuffer		( u32 target, u32 attachment, ZLGfxHandle* renderbuffer );
	void					FramebufferTexture2D		( u32 target, u32 attachment, ZLGfxHandle* texture, s32 level );
	void					LineWidth					( float width );
	
	void					LinkProgram					( ZLGfxHandle* program );
	
	void					RenderbufferStorage			( u32 internalFormat, u32 width, u32 height );
	void					Scissor						( s32 x, s32 y, u32 w, u32 h );
	
	void					ShaderSource				( ZLGfxHandle* shader, u32 count, const char** string, const s32* length );
	
	void					TexEnvi						( u32 pname, s32 param );
	void					TexImage2D					( u32 level, u32 internalFormat, u32 width, u32 height, u32 format, u32 type, const void* data );
	void					TexParameteri				( u32 pname, s32 param );
	void					TexSubImage2D				( u32 level, s32 xOffset, s32 yOffset, u32 width, u32 height, u32 format, u32 type, const void* data );
	void					Uniform1f					( u32 location, float v0 );
	void					Uniform1i					( u32 location, s32 v0 );
	void					Uniform4fv					( u32 location, u32 count, const float* value );
	void					UniformMatrix3fv			( u32 location, u32 count, bool transpose, const float* mtx );
	void					UniformMatrix4fv			( u32 location, u32 count, bool transpose, const float* mtx );
	void					UseProgram					( ZLGfxHandle* program );
	void					VertexAttribPointer			( u32 index, u32 size, u32 type, bool normalized, u32 stride, const void* pointer );
	void					Viewport					( s32 x, s32 y, u32 w, u32 h );
};

#endif
