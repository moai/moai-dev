// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLGFXRETAINED_H
#define ZLGFXRETAINED_H

#include <zl-gfx/ZLGfx.h>

//================================================================//
// ZLGfxListenerRecord
//================================================================//
class ZLGfxListenerRecord {
private:

	friend class ZLGfxRetained;

	enum {
		ON_EVENT,
		ON_READ_PIXELS,
		ON_UNIFORM_LOCATION,
		UNKNOWN,
	};
	
	ZLWeakPtr < ZLGfxListener >		mListener;
	void*							mUserdata;
	
	u32								mCallbackID;
	u32								mEvent;
	u32								mUniformAddr;
	
	ZLCopyOnWrite					mCopyOnWrite;

public:

	//----------------------------------------------------------------//
				ZLGfxListenerRecord			();
				~ZLGfxListenerRecord		();
};

//================================================================//
// ZLGfxRetained
//================================================================//
class ZLGfxRetained :
	public ZLGfx,
	public ZLGfxListener {
private:

	enum {
		ZLGFX_UNKNOWN,
		
		ZLGFX_ACTIVE_TEXTURE,
		ZLGFX_ALLOCATE_RESOURCE,
		ZLGFX_ATTACH_SHADER,
		ZLGFX_BIND_ATTRIB_LOCATION,
		ZLGFX_BIND_BUFFER,
		
		ZLGFX_BIND_FRAMEBUFFER,
		ZLGFX_BIND_RENDERBUFFER,
		ZLGFX_BIND_TEXTURE,
		ZLGFX_BIND_VERTEX_ARRAY,
		ZLGFX_BLEND_FUNC,
		
		ZLGFX_BLEND_MODE,
		ZLGFX_BUFFER_DATA,
		ZLGFX_BUFFER_SUB_DATA,
		ZLGFX_CHECK_FRAMEBUFFER_STATUS,
		ZLGFX_CLEAR,
		
		ZLGFX_CLEAR_COLOR,
		ZLGFX_COMMENT,
		ZLGFX_COMPILE_SHADER,
		ZLGFX_COMPRESSED_TEX_IMAGE_2D,
		ZLGFX_CULL_FACE,
		
		ZLGFX_DELETE_RESOURCE,
		ZLGFX_DEPTH_FUNC,
		ZLGFX_DEPTH_MASK,
		ZLGFX_DISABLE,
		
		ZLGFX_DISABLE_VERTEX_ATTRIB_ARRAY,
		ZLGFX_DRAW_ARRAYS,
		ZLGFX_DRAW_ELEMENTS,
		ZLGFX_ENABLE,
		ZLGFX_ENABLE_VERTEX_ATTRIB_ARRAY,
		
		ZLGFX_EVENT,
		
		ZLGFX_FLUSH,
		ZLGFX_FRAMEBUFFER_RENDERBUFFER,
		ZLGFX_FRAMEBUFFER_TEXTURE_2D,
		ZLGFX_GET_CURRENT_FRAMEBUFFER,
		
		ZLGFX_GET_UNIFORM_LOCATION,
		ZLGFX_LINE_WIDTH,
		ZLGFX_LINK_PROGRAM,
		ZLGFX_READ_PIXELS,
		ZLGFX_RENDER_BUFFER_STORAGE,
		ZLGFX_SCISSOR,
		
		ZLGFX_SHADER_SOURCE,
		ZLGFX_TEX_IMAGE_2D,
		ZLGFX_TEX_PARAMETERI,
		ZLGFX_TEX_SUB_IMAGE_2D,
		
		ZLGFX_UNIFORM_FLOAT,
		ZLGFX_UNIFORM_INT,
		
		ZLGFX_USE_PROGRAM,
		ZLGFX_VERTEX_ATTRIB_POINTER,
		ZLGFX_VIEWPORT,
	};

	ZLMemStream		mMemStream;
	ZLStream*		mStream;

	ZLLeanStack < ZLRefCountedObject*, 32 >			mReleaseStack;
	ZLLeanStack < ZLGfxListenerRecord, 32 >			mListenerRecords;

	//----------------------------------------------------------------//
	void					AllocateResource			( ZLGfxResource& resource, ZLGfxEnum::_ param );
	void					Retain						( ZLRefCountedObject* object );
	ZLGfxListenerRecord&	WriteListenerRecord			( ZLGfxListener* listener, void* userdata );

	//----------------------------------------------------------------//
	void					ZLGfxListener_OnGfxEvent			( u32 event, void* userdata );
	void					ZLGfxListener_OnReadPixels			( const ZLCopyOnWrite& copyOnWrite, void* userdata );
	void					ZLGfxListener_OnUniformLocation		( u32 addr, void* userdata );
	
public:

	GET ( size_t, Length, this->mStream->GetCursor ())

	//----------------------------------------------------------------//
	void					ActiveTexture				( u32 textureUnit );
	void					AttachShader				( ZLGfxResource& program, ZLGfxResource& shader );
	
	void					BindAttribLocation			( ZLGfxResource& program, u32 index, cc8* name );
	
	void					BindBuffer					( ZLGfxEnum::_ target, ZLGfxResource& handle );
	void					BindFramebuffer				( ZLGfxEnum::_ target, ZLGfxResource& handle );
	void					BindRenderbuffer			( ZLGfxResource& handle );
	void					BindTexture					( ZLGfxResource& handle );
	void					BindVertexArray				( ZLGfxResource& handle );
	
	void					BlendFunc					( ZLGfxEnum::_ sourceFactor, ZLGfxEnum::_ destFactor );
	void					BlendMode					( ZLGfxEnum::_ mode );
	void					BufferData					( ZLGfxEnum::_ target, size_t size, ZLSharedConstBuffer* buffer, size_t offset, ZLGfxEnum::_ usage );
	void					BufferSubData				( ZLGfxEnum::_ target, size_t offset, size_t size, ZLSharedConstBuffer* buffer, size_t srcOffset );
	
	void					CheckFramebufferStatus		( ZLGfxEnum::_ target );
	
	void					Clear						( u32 mask );
	void					ClearColor					( float r, float g, float b, float a );
	
	void					Comment						( cc8* comment );
	
	void					CompileShader				( ZLGfxResource& shader, bool log );
	void					CompressedTexImage2D		( u32 level, ZLGfxEnum::_ internalFormat, u32 width, u32 height, u32 imageSize, ZLSharedConstBuffer* buffer );
		
	void					CullFace					( ZLGfxEnum::_ mode );
	
	void					DeleteResource				( ZLGfxResource& resource );
	
	void					DepthFunc					( ZLGfxEnum::_ depthFunc );
	void					DepthMask					( bool flag );
	void					Disable						( ZLGfxEnum::_ cap );
	void					DisableVertexAttribArray	( u32 index );
	void					Draw						( ZLGfx& draw );
	void					DrawArrays					( ZLGfxEnum::_ primType, u32 first, u32 count );
	void					DrawElements				( ZLGfxEnum::_ primType, u32 count, ZLGfxEnum::_ indexType, ZLSharedConstBuffer* buffer, size_t offset );
	void					Enable						( ZLGfxEnum::_ cap );
	void					EnableVertexAttribArray		( u32 index );
	
	void					Event						( ZLGfxListener* listener, u32 event, void* userdata );
	
	void					Flush						( bool finish );
	void					FramebufferRenderbuffer		( ZLGfxEnum::_ target, ZLGfxEnum::_ attachment, ZLGfxResource& renderbuffer );
	void					FramebufferTexture2D		( ZLGfxEnum::_ target, ZLGfxEnum::_ attachment, ZLGfxResource& texture, s32 level );
	
	void					GetCurrentFramebuffer		( ZLGfxResource& framebuffer );
	void					GetUniformLocation			( ZLGfxResource& program, cc8* uniformName, ZLGfxListener* listener, void* userdata );
	
	bool					HasContent					();
	
	bool					IsImmediate					();
	
	void					LineWidth					( float width );
	
	void					LinkProgram					( ZLGfxResource& program, bool log );
	
	void					PopSection					();
	
	void					PublishEvents				();
	void					PublishEventsAndReset		();
	
	bool					PushErrorHandler			();
	void					PushSection					();
	bool					PushSuccessHandler			();
	
	void					ReadPixels					( s32 x, s32 y, u32 width, u32 height, ZLGfxEnum::_ format, ZLGfxEnum::_ type, u32 pixelSize, ZLGfxListener* listener, void* userdata );
	void					RenderbufferStorage			( ZLGfxEnum::_ internalFormat, u32 width, u32 height );
	void					Reset						();
	
	void					Scissor						( s32 x, s32 y, u32 w, u32 h );
	
	void					ShaderSource				( ZLGfxResource& shader, cc8* source, size_t length );
	
	void					TexImage2D					( u32 level, ZLGfxEnum::_ internalFormat, u32 width, u32 height, ZLGfxEnum::_ format, ZLGfxEnum::_ type, ZLSharedConstBuffer* buffer );
	void					TexParameteri				( ZLGfxEnum::_ pname, ZLGfxEnum::_ param );
	void					TexSubImage2D				( u32 level, s32 xOffset, s32 yOffset, u32 width, u32 height, ZLGfxEnum::_ format, ZLGfxEnum::_ type, ZLSharedConstBuffer* buffer );
	void					UniformFloat				( u32 location, u32 index, u32 width, u32 count, const float* value );
	void					UniformInt					( u32 location, u32 index, u32 width, u32 count, const s32* value );
	void					UseProgram					( ZLGfxResource& program );
	void					VertexAttribPointer			( u32 index, u32 size, ZLGfxEnum::_ type, bool normalized, u32 stride, ZLSharedConstBuffer* buffer, size_t offset );
	void					Viewport					( s32 x, s32 y, u32 w, u32 h );
	
							ZLGfxRetained				();
							~ZLGfxRetained				();
};

#endif
