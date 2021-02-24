// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLGFX_H
#define ZLGFX_H

#include <zl-gfx/ZLGfxEnum.h>
#include <zl-gfx/ZLGfxHandle.h>
#include <zl-gfx/ZLGfxListener.h>

class ZLCopyOnWrite;

// hardware PVR support is based on device
#ifdef MOAI_OS_IPHONE
	#define ZGL_DEVCAPS_PVR_TEXTURE 1
#else
	#define ZGL_DEVCAPS_PVR_TEXTURE 0
#endif

#define ZGL_COMMENT(gfx,comment) ( gfx ).Comment ( comment )

//================================================================//
// ZLGfx
//================================================================//
class ZLGfx {
private:

	friend class ZLGfxImmediate;
	friend class ZLGfxRetained;

	//----------------------------------------------------------------//
	virtual void					AllocateResource			( ZLGfxResource& resource, ZLGfxEnum::_ param ) = 0;
	virtual void					GetCurrentFramebuffer		( ZLGfxResource& resource ) = 0;

public:

	// api for drawing

	//----------------------------------------------------------------//
	virtual void					ActiveTexture				( u32 textureUnit ) = 0;
	virtual void					AttachShader				( ZLGfxResource& program, ZLGfxResource& shader ) = 0;
	
	virtual void					BindAttribLocation			( ZLGfxResource& program, u32 index, cc8* name ) = 0;
	
	virtual void					BindBuffer					( ZLGfxEnum::_ target, ZLGfxResource& buffer ) = 0;
	virtual void					BindFramebuffer				( ZLGfxEnum::_ target, ZLGfxResource& frameBuffer ) = 0;
	virtual void					BindRenderbuffer			( ZLGfxResource& renderbuffer ) = 0;
	virtual void					BindTexture					( ZLGfxResource& texture ) = 0;
	virtual void					BindVertexArray				( ZLGfxResource& vertexArray ) = 0;
	
	virtual void					BlendFunc					( ZLGfxEnum::_ sourceFactor, ZLGfxEnum::_ destFactor ) = 0;
	virtual void					BlendMode					( ZLGfxEnum::_ mode ) = 0;
	
	virtual void					BufferData					( ZLGfxEnum::_ target, size_t size, ZLSharedConstBuffer* buffer, size_t offset, ZLGfxEnum::_ usage ) = 0;
	virtual void					BufferSubData				( ZLGfxEnum::_ target, size_t offset, size_t size, ZLSharedConstBuffer* buffer, size_t srcOffset ) = 0;
	
	virtual void					CheckFramebufferStatus		( ZLGfxEnum::_ target ) = 0;
	
	virtual void					Clear						( u32 mask ) = 0;
	virtual void					ClearColor					( float r, float g, float b, float a ) = 0;
	
	virtual void					Comment						( cc8* comment = 0 ) = 0;
	
	virtual void					CompileShader				( ZLGfxResource& shader, bool log ) = 0;
	virtual void					CompressedTexImage2D		( u32 level, ZLGfxEnum::_ internalFormat, u32 width, u32 height, u32 imageSize, ZLSharedConstBuffer* buffer ) = 0;
		
	ZLGfxHandle						CreateBuffer				();
	ZLGfxHandle						CreateFramebuffer			();
	ZLGfxHandle						CreateProgram				();
	ZLGfxHandle						CreateRenderbuffer			();
	ZLGfxHandle						CreateShader				( ZLGfxEnum::_ shaderType );
	ZLGfxHandle						CreateTexture				();
	ZLGfxHandle						CreateVertexArray			();
	
	virtual void					CullFace					( ZLGfxEnum::_ mode ) = 0;
	
	virtual void					DeleteResource				( ZLGfxResource& resource ) = 0;
	
	virtual void					DepthFunc					( ZLGfxEnum::_ depthFunc ) = 0;
	virtual void					DepthMask					( bool flag ) = 0;
	virtual void					Disable						( ZLGfxEnum::_ cap ) = 0;
	virtual void					DisableVertexAttribArray	( u32 index ) = 0;
	
	static void						DiscardResource				( ZLGfxResource& resource );
	
	virtual void					DrawArrays					( ZLGfxEnum::_ primType, u32 first, u32 count ) = 0;
	virtual void					DrawElements				( ZLGfxEnum::_ primType, u32 count, ZLGfxEnum::_ indexType, ZLSharedConstBuffer* buffer, size_t offset ) = 0;
	virtual void					Enable						( ZLGfxEnum::_ cap ) = 0;
	virtual void					EnableVertexAttribArray		( u32 index ) = 0;
	
	virtual void					Event						( ZLGfxListener* listener, u32 event, void* userdata ) = 0;
	
	virtual void					Flush						( bool finish = false ) = 0;
	virtual void					FramebufferRenderbuffer		( ZLGfxEnum::_ target, ZLGfxEnum::_ attachment, ZLGfxResource& renderbuffer ) = 0;
	virtual void					FramebufferTexture2D		( ZLGfxEnum::_ target, ZLGfxEnum::_ attachment, ZLGfxResource& texture, s32 level ) = 0;
	
	virtual ZLGfxHandle				GetCurrentFramebuffer		();
	virtual void					GetUniformLocation			( ZLGfxResource& program, cc8* uniformName, ZLGfxListener* listener, void* userdata ) = 0;
	
	static bool						IsFlag						( u32 flag );
	
	virtual bool					IsImmediate					() = 0;
	
	virtual void					LineWidth					( float width ) = 0;
	
	virtual void					LinkProgram					( ZLGfxResource& program, bool log ) = 0;
	
	virtual void					PopSection					() = 0;
	virtual bool					PushErrorHandler			() = 0;
	virtual void					PushSection					() = 0;
	virtual bool					PushSuccessHandler			() = 0;
	
	virtual void					ReadPixels					( s32 x, s32 y, u32 width, u32 height, ZLGfxEnum::_ format, ZLGfxEnum::_ type, u32 pixelSize, ZLGfxListener* listener, void* userdata ) = 0;
	virtual void					RenderbufferStorage			( ZLGfxEnum::_ internalFormat, u32 width, u32 height ) = 0;
	
	virtual void					Scissor						( s32 x, s32 y, u32 w, u32 h ) = 0;
	
	virtual void					ShaderSource				( ZLGfxResource& shader, cc8* source, size_t length ) = 0;
	
	virtual void					TexImage2D					( u32 level, ZLGfxEnum::_ internalFormat, u32 width, u32 height, ZLGfxEnum::_ format, ZLGfxEnum::_ type, ZLSharedConstBuffer* buffer ) = 0;
	virtual void					TexParameteri				( ZLGfxEnum::_ pname, ZLGfxEnum::_ param ) = 0;
	virtual void					TexSubImage2D				( u32 level, s32 xOffset, s32 yOffset, u32 width, u32 height, ZLGfxEnum::_ format, ZLGfxEnum::_ type, ZLSharedConstBuffer* buffer ) = 0;
	virtual void					UniformFloat				( u32 location, u32 index, u32 width, u32 count, const float* value ) = 0;
	virtual void					UniformInt					( u32 location, u32 index, u32 width, u32 count, const s32* value ) = 0;
	virtual void					UseProgram					( ZLGfxResource& program ) = 0;
	virtual void					VertexAttribPointer			( u32 index, u32 size, ZLGfxEnum::_ type, bool normalized, u32 stride, ZLSharedConstBuffer* buffer, size_t offset ) = 0;
	virtual void					Viewport					( s32 x, s32 y, u32 w, u32 h ) = 0;
									ZLGfx						() {}
	virtual							~ZLGfx						() {}
};

#endif
