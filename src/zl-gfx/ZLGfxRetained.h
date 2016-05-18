// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLGFXRETAINED_H
#define ZLGFXRETAINED_H

#include <zl-gfx/ZLGfx.h>
#include <zl-util/STLList.h>
#include <zl-util/STLMap.h>
#include <zl-util/ZLCopyOnWrite.h>
#include <zl-util/ZLMemStream.h>
#include <zl-util/ZLRefCountedObject.h>

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
	
	ZLGfxListenerHandle*	mListenerHandle;
	void*					mUserdata;
	
	u32						mCallbackID;
	u32						mEvent;
	u32						mUniformAddr;
	
	ZLCopyOnWrite			mCopyOnWrite;

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
		ZLGFX_COLOR,
		ZLGFX_COMMENT,
		ZLGFX_COMPILE_SHADER,
		ZLGFX_COMPRESSED_TEX_IMAGE_2D,
		ZLGFX_CREATE,
		ZLGFX_CULL_FACE,
		
		ZLGFX_DELETE,
		ZLGFX_DEPTH_FUNC,
		ZLGFX_DEPTH_MASK,
		ZLGFX_DISABLE,
		ZLGFX_DISABLE_CLIENT_STATE,
		
		ZLGFX_DISABLE_VERTEX_ATTRIB_ARRAY,
		ZLGFX_DRAW_ARRAYS,
		ZLGFX_DRAW_ELEMENTS,
		ZLGFX_ENABLE,
		ZLGFX_ENABLE_CLIENT_STATE,
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
		ZLGFX_TEX_ENVI,
		ZLGFX_TEX_IMAGE_2D,
		ZLGFX_TEX_PARAMETERI,
		ZLGFX_TEX_SUB_IMAGE_2D,
		
		ZLGFX_UNIFORM_1F,
		ZLGFX_UNIFORM_1I,
		ZLGFX_UNIFORM_4FV,
		ZLGFX_UNIFORM_MATRIX_3FV,
		ZLGFX_UNIFORM_MATRIX_4FV,
		
		ZLGFX_USE_PROGRAM,
		ZLGFX_VERTEX_ATTRIB_POINTER,
		ZLGFX_VIEWPORT,
	};

	ZLMemStream		mMemStream;
	ZLStream*		mStream;

	void*			mBuffer;
	size_t			mBufferSize;
	size_t			mBufferTop;
	
	size_t			mAllocated;
	size_t			mMaxAllocated;

	ZLLeanStack < ZLRefCountedObject*, 32 >				mReleaseStack;
	ZLLeanStack < ZLGfxListenerRecord, 32 >				mListenerRecords;

	//----------------------------------------------------------------//
	ZLGfxHandle*			Create						( ZLGfxHandle* handle, u32 param );
	void					Delete						( u32 type, u32 glid );
	void					OnGfxEvent					( u32 event, void* userdata );
	void					OnReadPixels				( const ZLCopyOnWrite& copyOnWrite, void* userdata );
	void					OnUniformLocation			( u32 addr, void* userdata );
	void					Retain						( ZLRefCountedObject* object );
	ZLGfxListenerRecord&	WriteListenerRecord			( ZLGfxListener* listener, void* userdata );

public:

	GET ( size_t, Length, this->mStream->GetCursor ())

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
	void					BufferData					( u32 target, size_t size, ZLSharedConstBuffer* buffer, size_t offset, u32 usage );
	void					BufferSubData				( u32 target, size_t offset, size_t size, ZLSharedConstBuffer* buffer, size_t srcOffset );
	
	void					CheckFramebufferStatus		( u32 target );
	
	void					Clear						( u32 mask );
	void					ClearColor					( float r, float g, float b, float a );
	void					Color						( float r, float g, float b, float a );
	
	void					Comment						( cc8* comment );
	
	void					CompileShader				( ZLGfxHandle* shader, bool log );
	void					CompressedTexImage2D		( u32 level, u32 internalFormat, u32 width, u32 height, u32 imageSize, ZLSharedConstBuffer* buffer );
	
	ZLSharedConstBuffer*	CopyBuffer					( ZLSharedConstBuffer* buffer );
	
	ZLGfxHandle*			CreateBuffer				();
	ZLGfxHandle*			CreateFramebuffer			();
	ZLGfxHandle*			CreateProgram				();
	ZLGfxHandle*			CreateRenderbuffer			();
	ZLGfxHandle*			CreateShader				( u32 shaderType );
	ZLGfxHandle*			CreateTexture				();
	ZLGfxHandle*			CreateVertexArray			();
	
	void					CullFace					( u32 mode );
	
	void					DepthFunc					( u32 depthFunc );
	void					DepthMask					( bool flag );
	void					Disable						( u32 cap );
	void					DisableClientState			( u32 cap );
	void					DisableVertexAttribArray	( u32 index );
	void					Draw						( ZLGfx& draw );
	void					DrawArrays					( u32 primType, u32 first, u32 count );
	void					DrawElements				( u32 primType, u32 count, u32 indexType, ZLSharedConstBuffer* buffer, size_t offset );
	void					Enable						( u32 cap );
	void					EnableClientState			( u32 cap );
	void					EnableVertexAttribArray		( u32 index );
	
	void					Event						( ZLGfxListener* listener, u32 event, void* userdata );
	
	void					Flush						( bool finish );
	void					FramebufferRenderbuffer		( u32 target, u32 attachment, ZLGfxHandle* renderbuffer );
	void					FramebufferTexture2D		( u32 target, u32 attachment, ZLGfxHandle* texture, s32 level );
	
	ZLGfxHandle*			GetCurrentFramebuffer		();
	void					GetUniformLocation			( ZLGfxHandle* program, cc8* uniformName, ZLGfxListener* listener, void* userdata );
	
	bool					HasContent					();
	
	bool					IsImmediate					();
	
	void					LineWidth					( float width );
	
	void					LinkProgram					( ZLGfxHandle* program, bool log );
	
	void					PopSection					();
	
	void					PublishEvents				();
	void					PublishEventsAndReset		();
	
	bool					PushErrorHandler			();
	void					PushSection					();
	bool					PushSuccessHandler			();
	
	void					ReadPixels					( s32 x, s32 y, u32 width, u32 height, u32 format, u32 type, u32 pixelSize, ZLGfxListener* listener, void* userdata );
	void					RenderbufferStorage			( u32 internalFormat, u32 width, u32 height );
	void					Reset						();
	
	void					Scissor						( s32 x, s32 y, u32 w, u32 h );
	
	void					ShaderSource				( ZLGfxHandle* shader, cc8* source, size_t length );
	
	void					TexEnvi						( u32 pname, s32 param );
	void					TexImage2D					( u32 level, u32 internalFormat, u32 width, u32 height, u32 format, u32 type, ZLSharedConstBuffer* buffer );
	void					TexParameteri				( u32 pname, s32 param );
	void					TexSubImage2D				( u32 level, s32 xOffset, s32 yOffset, u32 width, u32 height, u32 format, u32 type, ZLSharedConstBuffer* buffer );
	void					Uniform1f					( u32 location, float v0 );
	void					Uniform1i					( u32 location, s32 v0 );
	void					Uniform4fv					( u32 location, u32 count, const float* value );
	void					UniformMatrix3fv			( u32 location, u32 count, bool transpose, const float* mtx );
	void					UniformMatrix4fv			( u32 location, u32 count, bool transpose, const float* mtx );
	void					UseProgram					( ZLGfxHandle* program );
	void					VertexAttribPointer			( u32 index, u32 size, u32 type, bool normalized, u32 stride, ZLSharedConstBuffer* buffer, size_t offset );
	void					Viewport					( s32 x, s32 y, u32 w, u32 h );
	
							ZLGfxRetained				();
							~ZLGfxRetained				();
};

#endif
