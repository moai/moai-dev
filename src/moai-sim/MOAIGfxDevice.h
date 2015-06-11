// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXDEVICE_H
#define	MOAIGFXDEVICE_H

#include <moai-sim/MOAIBlendMode.h>
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIFrameBuffer.h>
#include <moai-sim/MOAIGfxDeviceBase.h>
#include <moai-sim/MOAIGfxDeviceMtxCache.h>
#include <moai-sim/MOAIGfxDeviceVertexWriter.h>
#include <moai-sim/MOAIImage.h>

class MOAICamera;
class MOAIFrameBuffer;
class MOAIGfxResource;
class MOAIGfxState;
class MOAIMultiTexture;
class MOAIShader;
class MOAIShaderProgram;
class MOAITexture;
class MOAITextureBase;
class MOAIVertexFormat;
class MOAIViewport;

//================================================================//
// MOAIGfxDevice
//================================================================//
/**	@lua	MOAIGfxDevice
	@text	Interface to the graphics singleton.
	
	@const	EVENT_RESIZE
*/
class MOAIGfxDevice :
	public virtual MOAIGfxDeviceBase,
	public virtual MOAIGfxDeviceMtxCache,
	public virtual MOAIGfxDeviceVertexWriter,
	public MOAIGlobalClass < MOAIGfxDevice, MOAIGlobalEventSource > {
public:
	
	enum {
		EVENT_RESIZE,
		TOTAL_EVENTS,
	};
	
private:
	
	int				mCullFunc;
	int				mDepthFunc;
	bool			mDepthMask;

	MOAIBlendMode	mBlendMode;
	bool			mBlendEnabled;

	bool			mHasContext;

	bool			mIsFramebufferSupported;
	bool			mIsOpenGLES;

	u32				mMajorVersion;
	u32				mMinorVersion;
	
	float			mPenWidth;
	float			mPointSize;

	ZLRect					mScissorRect;
	MOAIShaderProgram*		mShaderProgram;
	
	ZLLeanArray < MOAITextureBase* >	mTextureUnits;
	u32									mActiveTextures;
	size_t								mTextureMemoryUsage;
	u32									mMaxTextureSize;

	const MOAIVertexFormat*				mVertexFormat;
	void*								mVertexFormatBuffer;

	MOAILuaSharedPtr < MOAITexture >	mDefaultTexture;

	MOAILuaSharedPtr < MOAIFrameBuffer >	mDefaultBuffer;
	MOAIFrameBuffer*						mFrameBuffer;

	//----------------------------------------------------------------//
	static int				_getFrameBuffer				( lua_State* L );
	static int				_getMaxTextureUnits			( lua_State* L );
	static int				_getViewSize				( lua_State* L );
	static int				_setDefaultTexture			( lua_State* L );
	static int				_setPenColor				( lua_State* L );
	static int				_setPenWidth				( lua_State* L );

	//----------------------------------------------------------------//
	void					Clear						();
	void					DisableTextureUnits			( u32 activeTextures );
	bool					SetTexture					( u32 textureUnit, MOAITextureBase* texture );
	void					TransformAndWriteQuad		( ZLVec4D* vtx, ZLVec2D* uv );
	
public:
	
	friend class MOAIGfxResource;
	friend class MOAIShaderProgram;
	friend class MOAITextureBase;
	
	DECL_LUA_SINGLETON ( MOAIGfxDevice )
	
	GET ( size_t, TextureMemoryUsage, mTextureMemoryUsage )
	GET ( u32, MaxTextureSize, mMaxTextureSize )
	GET ( bool, HasContext, mHasContext )
	
	GET_BOOL ( IsOpenGLES, mIsOpenGLES )
	GET_BOOL ( IsFramebufferSupported, mIsFramebufferSupported )
	
	GET ( MOAIBlendMode, BlendMode, mBlendMode )
	
	GET ( MOAIFrameBuffer*, DefaultBuffer, mDefaultBuffer )
	
	//----------------------------------------------------------------//
	
	void					ClearErrors				();
	void					ClearSurface			( u32 clearFlags ); // takes zgl clear flags
	void					DetectContext			();
	void					DetectFramebuffer		();
	void					Flush					();
	
	float					GetDeviceScale			();
	
	u32						GetHeight				() const;
	u32						GetWidth				() const;
	
	bool					IsOpaque				() const;
	u32						LogErrors				();
	
							MOAIGfxDevice			();
							~MOAIGfxDevice			();
	
	void					OnGlobalsFinalize		();

	void					RegisterLuaClass		( MOAILuaState& state );
	
	void					ReportTextureAlloc		( cc8* name, size_t size );
	void					ReportTextureFree		( cc8* name, size_t size );
	
	void					ResetDrawCount			();
	void					ResetState				();

	void					SetBlendMode			();
	void					SetBlendMode			( const MOAIBlendMode& blendMode );
	void					SetBlendMode			( int srcFactor, int dstFactor, int equation = 0 );
	
	void					SetBufferScale			( float scale );
	void					SetBufferSize			( u32 width, u32 height );
	
	void					SetCullFunc				();
	void					SetCullFunc				( int cullFunc );

	void					SetDepthFunc			();
	void					SetDepthFunc			( int depthFunc );
	void					SetDepthMask			( bool depthMask );
	void					SetFrameBuffer			( MOAIFrameBuffer* frameBuffer );
	bool					SetGfxState				( MOAIGfxState* gfxState );
	void					SetPenWidth				( float penWidth );
	void					SetScissorRect			();
	void					SetScissorRect			( ZLRect rect );
	void					SetScreenSpace			( MOAIViewport& viewport );
	void					SetShader				( MOAIShader* shader = 0 );
	void					SetShaderPreset			( u32 preset );
	void					SetShaderProgram		( MOAIShaderProgram* program = 0 );
	
	bool					SetTexture				();
	bool					SetTexture				( MOAITextureBase* texture );
	bool					SetTexture				( MOAIMultiTexture* multi );
	
	void					SetVertexFormat			();
	void					SetVertexFormat			( const MOAIVertexFormat& format );
	void					SetVertexFormat			( const MOAIVertexFormat& format, void* buffer );
	void					SetVertexPreset			( u32 preset );

	void					SetViewRect				();
	void					SetViewRect				( ZLRect rect );
		
	void					UpdateShaderGlobals		();
};

#endif
