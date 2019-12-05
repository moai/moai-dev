// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLGFXLOGGER_H
#define ZLGFXLOGGER_H

#include <zl-gfx/ZLGfx.h>

class ZLGfxRetained;

//================================================================//
// ZLGfxLogger
//================================================================//
class ZLGfxLogger :
	public ZLGfx {
private:

	const void*		mPrefix;
	size_t			mLineNo;
	size_t			mCommentNo;
	FILE*			mFile;
	
	//----------------------------------------------------------------//
	void					AllocateResource			( ZLGfxResource& resource, ZGLEnum param );
	void					PrintLine					( cc8* format, ... );

public:

	GET ( size_t, CommandCount, mLineNo );
	GET ( size_t, CommentCount, mCommentNo );

	//----------------------------------------------------------------//
	void					ActiveTexture				( u32 textureUnit );
	void					AttachShader				( ZLGfxResource& program, ZLGfxResource& shader );
	
	void					BindAttribLocation			( ZLGfxResource& program, u32 index, cc8* name );
	
	void					BindBuffer					( ZGLEnum target, ZLGfxResource& handle );
	void					BindFramebuffer				( ZGLEnum target, ZLGfxResource& handle );
	void					BindRenderbuffer			( ZLGfxResource& handle );
	void					BindTexture					( ZLGfxResource& handle );
	void					BindVertexArray				( ZLGfxResource& handle );
	
	void					BlendFunc					( ZGLEnum sourceFactor, ZGLEnum destFactor );
	void					BlendMode					( ZGLEnum mode );
	void					BufferData					( ZGLEnum target, size_t size, ZLSharedConstBuffer* buffer, size_t offset, ZGLEnum usage );
	void					BufferSubData				( ZGLEnum target, size_t offset, size_t size, ZLSharedConstBuffer* buffer, size_t srcOffset );

	void					CheckFramebufferStatus		( ZGLEnum target );

	void					Clear						( u32 mask );
	void					ClearColor					( float r, float g, float b, float a );
	void					Color						( float r, float g, float b, float a );
	
	void					Comment						( cc8* comment );
	
	void					CompileShader				( ZLGfxResource& shader, bool log );
	void					CompressedTexImage2D		( u32 level, ZGLEnum internalFormat, u32 width, u32 height, u32 imageSize, ZLSharedConstBuffer* buffer );
		
	void					CullFace					( ZGLEnum mode );
	
	void					DeleteResource				( ZLGfxResource& resource );
	
	void					DepthFunc					( ZGLEnum depthFunc );
	void					DepthMask					( bool flag );
	void					Disable						( ZGLEnum cap );
	void					DisableClientState			( ZGLEnum cap );
	void					DisableVertexAttribArray	( u32 index );
	void					DrawArrays					( ZGLEnum primType, u32 first, u32 count );
	void					DrawElements				( ZGLEnum primType, u32 count, ZGLEnum indexType, ZLSharedConstBuffer* buffer, size_t offset );
	void					Enable						( ZGLEnum cap );
	void					EnableClientState			( ZGLEnum cap );
	void					EnableVertexAttribArray		( u32 index );

	void					Event						( ZLGfxListener* listener, u32 event, void* userdata );

	void					Flush						( bool finish );
	void					FramebufferRenderbuffer		( ZGLEnum target, ZGLEnum attachment, ZLGfxResource& renderbuffer );
	void					FramebufferTexture2D		( ZGLEnum target, ZGLEnum attachment, ZLGfxResource& texture, s32 level );
	
	void					GetCurrentFramebuffer		( ZLGfxResource& framebuffer );
	void					GetUniformLocation			( ZLGfxResource& program, cc8* uniformName, ZLGfxListener* listener, void* userdata );
	
	bool					IsImmediate					();
	
	void					LineWidth					( float width );
	
	void					LinkProgram					( ZLGfxResource& program, bool log );
	
	void					PopSection					();
	
	void					Print						( cc8* filename, ZLGfxRetained& gfx );
	
	bool					PushErrorHandler			();
	void					PushSection					();
	bool					PushSuccessHandler			();
	
	void					ReadPixels					( s32 x, s32 y, u32 width, u32 height, ZGLEnum format, ZGLEnum type, u32 pixelSize, ZLGfxListener* listener, void* userdata );
	void					RenderbufferStorage			( ZGLEnum internalFormat, u32 width, u32 height );
	
	void					Scissor						( s32 x, s32 y, u32 w, u32 h );
	
	void					ShaderSource				( ZLGfxResource& shader, cc8* source, size_t length );
	
	void					TexEnvi						( ZGLEnum pname, ZGLEnum param );
	void					TexImage2D					( u32 level, ZGLEnum internalFormat, u32 width, u32 height, ZGLEnum format, ZGLEnum type, ZLSharedConstBuffer* buffer );
	void					TexParameteri				( ZGLEnum pname, ZGLEnum param );
	void					TexSubImage2D				( u32 level, s32 xOffset, s32 yOffset, u32 width, u32 height, ZGLEnum format, ZGLEnum type, ZLSharedConstBuffer* buffer );
	void					UniformFloat				( u32 location, u32 index, u32 width, u32 count, const float* value );
	void					UniformInt					( u32 location, u32 index, u32 width, u32 count, const s32* value );
	void					UseProgram					( ZLGfxResource& program );
	void					VertexAttribPointer			( u32 index, u32 size, ZGLEnum type, bool normalized, u32 stride, ZLSharedConstBuffer* buffer, size_t offset );
	void					Viewport					( s32 x, s32 y, u32 w, u32 h );
	
							ZLGfxLogger					();
							~ZLGfxLogger				();
};

#endif
