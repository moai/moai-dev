// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLGFX_H
#define ZLGFX_H

#include <zl-util/ZLLeanArray.h>
#include <zl-util/ZLLeanStack.h>
#include <zl-util/ZLMatrix3x3.h>
#include <zl-util/ZLMatrix4x4.h>
#include <zl-util/ZLRefCountedObject.h>
#include <zl-util/ZLSharedHandle.h>

class ZLCopyOnWrite;

// hardware PVR support is based on device
#ifdef MOAI_OS_IPHONE
	#define ZGL_DEVCAPS_PVR_TEXTURE 1
#else
	#define ZGL_DEVCAPS_PVR_TEXTURE 0
#endif

#define ZGL_COMMENT(gfx,comment) ( gfx ).Comment ( comment )

//================================================================//
// ZLGfxHandle
//================================================================//
class ZLGfxHandle {
public:

	friend class ZLGfx;
	friend class ZLGfxImmediate;
	friend class ZLGfxRetained;

	u32		mType;
	u32		mGLID;
	bool	mOwns;

	//----------------------------------------------------------------//
	ZLGfxHandle ( u32 type, u32 glid, bool owns ) :
		mType ( type ),
		mGLID ( glid ),
		mOwns ( owns ) {
	}

public:

	enum {
		BUFFER,
		FRAMEBUFFER,
		PROGRAM,
		SHADER,
		TEXTURE,
		RENDERBUFFER,
		VERTEXARRAY,
	};
	
	//----------------------------------------------------------------//
					ZLGfxHandle				() {}
					~ZLGfxHandle			() {}
	
	//----------------------------------------------------------------//
	static u32 GLID ( ZLGfxHandle* handle ) {
		return handle ? handle->mGLID : 0;
	}
};

//================================================================//
// ZLGfxListener
//================================================================//
class ZLGfxListener :
	public ZLSharedHandleTarget < ZLGfxListener > {
private:

	GET ( ZLGfxListener*, HandleTarget, this )

public:

	//----------------------------------------------------------------//
	virtual	void			OnGfxEvent				( u32 event, void* userdata );
	virtual void			OnReadPixels			( const ZLCopyOnWrite& copyOnWrite, void* userdata );
	virtual void			OnUniformLocation		( u32 addr, void* userdata );
							ZLGfxListener			();
	virtual					~ZLGfxListener			();
};

typedef ZLSharedHandle < ZLGfxListener > ZLGfxListenerHandle;

//================================================================//
// ZLGfx
//================================================================//
class ZLGfx {

	friend class ZLGfxImmediate;
	friend class ZLGfxRetained;

	//----------------------------------------------------------------//
	virtual ZLGfxHandle*			Create						( ZLGfxHandle* handle, u32 param ) = 0;
	virtual void					Delete						( u32 type, u32 glid ) = 0;

public:

	// api for drawing

	//----------------------------------------------------------------//
	virtual void					ActiveTexture				( u32 textureUnit ) = 0;
	virtual void					AttachShader				( ZLGfxHandle* program, ZLGfxHandle* shader ) = 0;
	
	virtual void					BindAttribLocation			( ZLGfxHandle* program, u32 index, cc8* name ) = 0;
	
	virtual void					BindBuffer					( u32 target, ZLGfxHandle* buffer ) = 0;
	virtual void					BindFramebuffer				( u32 target, ZLGfxHandle* frameBuffer ) = 0;
	virtual void					BindRenderbuffer			( ZLGfxHandle* renderbuffer ) = 0;
	virtual void					BindTexture					( ZLGfxHandle* texID ) = 0;
	virtual void					BindVertexArray				( ZLGfxHandle* vertexArrayID ) = 0;
	
	virtual void					BlendFunc					( u32 sourceFactor, u32 destFactor ) = 0;
	virtual void					BlendMode					( u32 mode ) = 0;
	
	virtual void					BufferData					( u32 target, size_t size, ZLSharedConstBuffer* buffer, size_t offset, u32 usage ) = 0;
	virtual void					BufferSubData				( u32 target, size_t offset, size_t size, ZLSharedConstBuffer* buffer, size_t srcOffset ) = 0;
	
	virtual void					CheckFramebufferStatus		( u32 target ) = 0;
	
	virtual void					Clear						( u32 mask ) = 0;
	virtual void					ClearColor					( float r, float g, float b, float a ) = 0;
	virtual void					Color						( float r, float g, float b, float a ) = 0;
	
	virtual void					Comment						( cc8* comment = 0 ) = 0;
	
	virtual void					CompileShader				( ZLGfxHandle* shader, bool log ) = 0;
	virtual void					CompressedTexImage2D		( u32 level, u32 internalFormat, u32 width, u32 height, u32 imageSize, ZLSharedConstBuffer* buffer ) = 0;
	
	virtual ZLSharedConstBuffer*	CopyBuffer					( ZLSharedConstBuffer* buffer ) = 0;
	
	ZLGfxHandle*					CreateBuffer				();
	ZLGfxHandle*					CreateFramebuffer			();
	ZLGfxHandle*					CreateProgram				();
	ZLGfxHandle*					CreateRenderbuffer			();
	ZLGfxHandle*					CreateShader				( u32 shaderType );
	ZLGfxHandle*					CreateTexture				();
	ZLGfxHandle*					CreateVertexArray			();
	
	virtual void					CullFace					( u32 mode ) = 0;
	
	void							Delete						( ZLGfxHandle*& handle );
	
	virtual void					DepthFunc					( u32 depthFunc ) = 0;
	virtual void					DepthMask					( bool flag ) = 0;
	virtual void					Disable						( u32 cap ) = 0;
	virtual void					DisableClientState			( u32 cap ) = 0;
	virtual void					DisableVertexAttribArray	( u32 index ) = 0;
	
	static void						Discard						( ZLGfxHandle*& handle );
	
	virtual void					DrawArrays					( u32 primType, u32 first, u32 count ) = 0;
	virtual void					DrawElements				( u32 primType, u32 count, u32 indexType, ZLSharedConstBuffer* buffer, size_t offset ) = 0;
	virtual void					Enable						( u32 cap ) = 0;
	virtual void					EnableClientState			( u32 cap ) = 0;
	virtual void					EnableVertexAttribArray		( u32 index ) = 0;
	
	virtual void					Event						( ZLGfxListener* listener, u32 event, void* userdata ) = 0;
	
	virtual void					Flush						( bool finish = false ) = 0;
	virtual void					FramebufferRenderbuffer		( u32 target, u32 attachment, ZLGfxHandle* renderbuffer ) = 0;
	virtual void					FramebufferTexture2D		( u32 target, u32 attachment, ZLGfxHandle* texture, s32 level ) = 0;
	
	virtual ZLGfxHandle*			GetCurrentFramebuffer		() = 0;
	virtual void					GetUniformLocation			( ZLGfxHandle* program, cc8* uniformName, ZLGfxListener* listener, void* userdata ) = 0;
	
	virtual bool					IsImmediate					() = 0;
	
	virtual void					LineWidth					( float width ) = 0;
	
	virtual void					LinkProgram					( ZLGfxHandle* program, bool log ) = 0;
	
	virtual void					PopSection					() = 0;
	virtual bool					PushErrorHandler			() = 0;
	virtual void					PushSection					() = 0;
	virtual bool					PushSuccessHandler			() = 0;
	
	virtual void					ReadPixels					( s32 x, s32 y, u32 width, u32 height, u32 format, u32 type, u32 pixelSize, ZLGfxListener* listener, void* userdata ) = 0;
	virtual void					RenderbufferStorage			( u32 internalFormat, u32 width, u32 height ) = 0;
	
	virtual void					Scissor						( s32 x, s32 y, u32 w, u32 h ) = 0;
	
	virtual void					ShaderSource				( ZLGfxHandle* shader, cc8* source, size_t length ) = 0;
	
	virtual void					TexEnvi						( u32 pname, s32 param ) = 0;
	virtual void					TexImage2D					( u32 level, u32 internalFormat, u32 width, u32 height, u32 format, u32 type, ZLSharedConstBuffer* buffer ) = 0;
	virtual void					TexParameteri				( u32 pname, s32 param ) = 0;
	virtual void					TexSubImage2D				( u32 level, s32 xOffset, s32 yOffset, u32 width, u32 height, u32 format, u32 type, ZLSharedConstBuffer* buffer ) = 0;
	virtual void					Uniform1f					( u32 location, float v0 ) = 0;
	virtual void					Uniform1i					( u32 location, s32 v0 ) = 0;
	virtual void					Uniform4fv					( u32 location, u32 count, const float* value ) = 0;
	virtual void					UniformMatrix3fv			( u32 location, u32 count, bool transpose, const float* mtx ) = 0;
	virtual void					UniformMatrix4fv			( u32 location, u32 count, bool transpose, const float* mtx ) = 0;
	virtual void					UseProgram					( ZLGfxHandle* program ) = 0;
	virtual void					VertexAttribPointer			( u32 index, u32 size, u32 type, bool normalized, u32 stride, ZLSharedConstBuffer* buffer, size_t offset ) = 0;
	virtual void					Viewport					( s32 x, s32 y, u32 w, u32 h ) = 0;
									ZLGfx						() {}
	virtual							~ZLGfx						() {}
};

#endif
