// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXDEVICE_H
#define	MOAIGFXDEVICE_H

#include <moaicore/MOAIBlendMode.h>
#include <moaicore/MOAIFrameBuffer.h>
#include <moaicore/MOAIColor.h>
#include <moaicore/MOAIImage.h>
#include <moaicore/MOAIEventSource.h>
#include <moaicore/MOAILua.h>

class MOAIFrameBuffer;
class MOAIGfxResource;
class MOAIGfxState;
class MOAIMultiTexture;
class MOAIShader;
class MOAITexture;
class MOAITextureBase;
class MOAIVertexFormat;
class MOAIViewport;

//================================================================//
// MOAIGfxDeleter
//================================================================//
class MOAIGfxDeleter {
public:

	enum {
		DELETE_BUFFER,
		DELETE_FRAMEBUFFER,
		DELETE_PROGRAM,
		DELETE_SHADER,
		DELETE_TEXTURE,
		DELETE_RENDERBUFFER,
	};

	GLuint	mResourceID;
	u32		mType;

	//----------------------------------------------------------------//
	void		Delete			();
};

//================================================================//
// MOAIGfxDevice
//================================================================//
/**	@name	MOAIGfxDevice
	@text	Interface to the graphics singleton.
	
	@const	EVENT_RESIZE
*/
class MOAIGfxDevice :
	public MOAIGlobalClass < MOAIGfxDevice, MOAIGlobalEventSource > {
public:
	
	enum {
		VTX_STAGE_MODEL,
		VTX_STAGE_WORLD,
		VTX_STAGE_VIEW,
		VTX_STAGE_PROJ,
	};
	
	enum {
		UV_STAGE_MODEL,
		UV_STAGE_TEXTURE,
	};
	
	enum {
		VTX_WORLD_TRANSFORM,
		VTX_VIEW_TRANSFORM,
		VTX_PROJ_TRANSFORM,
		TOTAL_VTX_TRANSFORMS,
	};
	
	enum {
		EVENT_RESIZE,
		TOTAL_EVENTS,
	};
	
private:
	
	static const u32 DEFAULT_BUFFER_SIZE	= 0x8000;
	
	int				mCullFunc;
	int				mDepthFunc;
	bool			mDepthMask;

	MOAIBlendMode	mBlendMode;
	bool			mBlendEnabled;

	void*			mBuffer;

	bool			mCpuVertexTransform;
	USMatrix4x4		mCpuVertexTransformMtx; // composition of matrices to be applied via CPU
	bool			mCpuVertexTransformCache [ TOTAL_VTX_TRANSFORMS ];
	USMatrix4x4		mCpuVertexTransformCacheMtx [ TOTAL_VTX_TRANSFORMS ]; // composition of VIEW and PROJ matrices via CPU

	bool			mCpuUVTransform;

	u32				mDrawCount;
	bool			mHasContext;

	bool			mIsFramebufferSupported;
	bool			mIsOpenGLES;
	bool			mIsProgrammable;	

	u32				mMajorVersion;
	u32				mMaxPrims;
	u32				mMinorVersion;
	
	USColorVec		mAmbientColor;
	USColorVec		mPenColor;
	
	USColorVec		mFinalColor;
	u32				mFinalColor32;
	
	float			mPenWidth;
	float			mPointSize;
	
	u32				mPrimCount;
	u32				mPrimSize;
	u32				mPrimTop;
	GLenum			mPrimType;
	
	typedef USLeanList < MOAIGfxResource* >::Iterator ResourceIt;
	USLeanList < MOAIGfxResource* > mResources;

	USRect			mScissorRect;
	MOAIShader*		mShader;
	u32				mSize;
	
	USLeanArray < MOAITextureBase* > mTextureUnits;
	u32				mActiveTextures;
	size_t			mTextureMemoryUsage;
	u32				mMaxTextureSize;
	u32				mTop;
	
	u32				mUVMtxInput;
	u32				mUVMtxOutput;
	USMatrix4x4		mUVTransform;

	const MOAIVertexFormat*	mVertexFormat;
	void* mVertexFormatBuffer;

	u32				mVertexMtxInput;
	u32				mVertexMtxOutput;
	USMatrix4x4		mVertexTransforms [ TOTAL_VTX_TRANSFORMS ];
	USMatrix4x4		mBillboardMtx;

	USRect			mViewRect;
	USFrustum		mViewVolume;
	
	USLeanStack < MOAIGfxDeleter, 32 > mDeleterStack;

	MOAILuaSharedPtr < MOAITexture > mDefaultTexture;

	MOAILuaSharedPtr < MOAIFrameBuffer > mDefaultBuffer;
	MOAIFrameBuffer* mFrameBuffer;

	//----------------------------------------------------------------//
	static int				_getFrameBuffer			( lua_State* L );
	static int				_getMaxTextureUnits		( lua_State* L );
	static int				_getViewSize			( lua_State* L );
	static int				_isProgrammable			( lua_State* L );
	static int				_setDefaultTexture		( lua_State* L );
	static int				_setPenColor			( lua_State* L );
	static int				_setPenWidth			( lua_State* L );
	static int				_setPointSize			( lua_State* L );

	//----------------------------------------------------------------//
	void					Clear					();
	void					DisableTextureUnits		( u32 activeTextures );
	void					DrawPrims				();
	void					GpuLoadMatrix			( const USMatrix4x4& mtx ) const;
	void					GpuMultMatrix			( const USMatrix4x4& mtx ) const;
	void					InsertGfxResource		( MOAIGfxResource& resource );
	void					RemoveGfxResource		( MOAIGfxResource& resource );
	bool					SetTexture				( u32 textureUnit, MOAITextureBase* texture );
	void					TransformAndWriteQuad	( USVec4D* vtx, USVec2D* uv );
	void					UpdateFinalColor		();
	void					UpdateCpuVertexMtx		();
	void					UpdateGpuVertexMtx		();
	void					UpdateUVMtx				();
	
public:
	
	friend class MOAIGfxResource;
	friend class MOAITextureBase;
	
	DECL_LUA_SINGLETON ( MOAIGfxDevice )
	
	GET ( size_t, TextureMemoryUsage, mTextureMemoryUsage )
	GET ( u32, MaxTextureSize, mMaxTextureSize )
	GET ( bool, HasContext, mHasContext )
	
	GET_BOOL ( IsOpenGLES, mIsOpenGLES )
	GET_BOOL ( IsProgrammable, mIsProgrammable )
	GET_BOOL ( IsFramebufferSupported, mIsFramebufferSupported )
	
	GET ( const USFrustum&, ViewVolume, mViewVolume )
	
	GET ( USColorVec, AmbientColor, mAmbientColor )
	GET ( USColorVec, FinalColor, mFinalColor )
	GET ( USColorVec, PenColor, mPenColor )
	
	GET ( MOAIFrameBuffer*, DefaultBuffer, mDefaultBuffer )
	
	//----------------------------------------------------------------//
	void					BeginLayer				();
	void					BeginPrim				();
	void					BeginPrim				( u32 primType );
	void					ClearColorBuffer		( u32 color );
	
	void					ClearErrors				();
	void					DetectContext			();
	void					EndPrim					();
	void					Flush					();
	
	const USMatrix4x4&		GetBillboardMtx			() const;
	
	float					GetDeviceScale			();
	u32						GetDrawCount			() const { return mDrawCount; }
	cc8*					GetErrorString			( int error ) const;
	
	u32						GetHeight				() const;
	
	USMatrix4x4				GetNormToWndMtx			() const;
	
	const USMatrix4x4&		GetUVTransform			() const;
	const USMatrix4x4&		GetVertexTransform		( u32 id ) const;
	USMatrix4x4				GetViewProjMtx			() const;

	u32						GetWidth				() const;
	
	USMatrix4x4				GetWorldToWndMtx		() const;
	USMatrix4x4				GetWndToNormMtx			() const;
	USMatrix4x4				GetWndToWorldMtx		() const;
	
	u32						LogErrors				();
	
							MOAIGfxDevice			();
							~MOAIGfxDevice			();
	
	void					ProcessDeleters			();
	void					PushDeleter				( u32 type, GLuint id );

	void					RegisterLuaClass		( MOAILuaState& state );
	void					ReleaseResources		();
	void					RenewResources			();
	
	void					ReportTextureAlloc		( cc8* name, size_t size );
	void					ReportTextureFree		( cc8* name, size_t size );
	
	void					Reserve					( u32 size );
	void					ResetDrawCount			();
	void					ResetResources			();
	void					ResetState				();
	
	void					SetAmbientColor			( u32 color );
	void					SetAmbientColor			( const USColorVec& colorVec );
	void					SetAmbientColor			( float r, float g, float b, float a );
	
	void					SetBillboardMtx			();
	void					SetBillboardMtx			( const USMatrix4x4& mtx );
	
	void					SetBlendMode			();
	void					SetBlendMode			( const MOAIBlendMode& blendMode );
	void					SetBlendMode			( int srcFactor, int dstFactor );
	
	void					SetBufferScale			( float scale );
	void					SetBufferSize			( u32 width, u32 height );
	
	void					SetCullFunc				();
	void					SetCullFunc				( int cullFunc );

	void					SetDepthFunc			();
	void					SetDepthFunc			( int depthFunc );
	void					SetDepthMask			( bool depthMask );
	void					SetFrameBuffer			( MOAIFrameBuffer* frameBuffer );
	bool					SetGfxState				( MOAIGfxState* gfxState );
	void					SetPenColor				( u32 color );
	void					SetPenColor				( const USColorVec& colorVec );
	void					SetPenColor				( float r, float g, float b, float a );
	void					SetPenWidth				( float penWidth );
	void					SetPointSize			( float pointSize );
	void					SetPrimType				( u32 primType );
	void					SetScissorRect			();
	void					SetScissorRect			( USRect rect );
	void					SetScreenSpace			( MOAIViewport& viewport );
	void					SetShader				( MOAIShader* shader = 0 );
	void					SetShaderPreset			( u32 preset );
	bool					SetTexture				();
	bool					SetTexture				( MOAITextureBase* texture );
	bool					SetTexture				( MOAIMultiTexture* multi );
	
	void					SetUVMtxMode			( u32 input, u32 output );
	void					SetUVTransform			();
	void					SetUVTransform			( const USAffine3D& transform );
	void					SetUVTransform			( const USMatrix4x4& transform );
	
	void					SetVertexFormat			();
	void					SetVertexFormat			( const MOAIVertexFormat& format );
	void					SetVertexFormat			( const MOAIVertexFormat& format, void* buffer );
	void					SetVertexMtxMode		( u32 input, u32 output );
	void					SetVertexPreset			( u32 preset );
	void					SetVertexTransform		( u32 id );
	void					SetVertexTransform		( u32 id, const USAffine3D& transform );
	void					SetVertexTransform		( u32 id, const USMatrix4x4& transform );
	
	void					SetViewRect				();
	void					SetViewRect				( USRect rect );
	
	void					SoftReleaseResources	( u32 age );
	
	void					UpdateViewVolume		();
	
	void					WriteQuad				( const USVec2D* vtx, const USVec2D* uv );
	void					WriteQuad				( const USVec2D* vtx, const USVec2D* uv, float xOff, float yOff, float zOff );
	void					WriteQuad				( const USVec2D* vtx, const USVec2D* uv, float xOff, float yOff, float zOff, float xScale, float yScale );
	void					WriteQuad				( const USVec2D* vtx, const USVec2D* uv, float xOff, float yOff, float zOff, float xScale, float yScale, float uOff, float vOff, float uScale, float vScale );
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	inline void Write ( const TYPE& type ) {
		
		size_t top = this->mTop + sizeof ( TYPE );
		assert ( top < this->mSize );
		
		*( TYPE* )(( size_t )this->mBuffer + this->mTop ) = type;
		this->mTop = top;
	}
	
	//----------------------------------------------------------------//
	inline void WriteColor ( float r, float g, float b, float a ) {
		UNUSED ( r );
		UNUSED ( g );
		UNUSED ( b );
		UNUSED ( a );
	}
	
	//----------------------------------------------------------------//
	inline void WriteFinalColor4b () {
		
		this->Write < u32 >( this->mFinalColor32 );
	}
	
	//----------------------------------------------------------------//
	inline void WriteFinalColor4f () {
		
		this->Write < USColorVec >( this->mFinalColor );
	}
	
	//----------------------------------------------------------------//
	inline void WriteUV ( float u, float v ) {
	
		USVec2D uv;
		uv.mX = u;
		uv.mY = v;
	
		if ( this->mCpuUVTransform ) {
			this->mUVTransform.Transform ( uv );
		}
		this->Write ( uv );
	}
	
	//----------------------------------------------------------------//
	inline void WriteUV ( USVec2D uv ) {
	
		if ( this->mCpuUVTransform ) {
			this->mUVTransform.Transform ( uv );
		}
		this->Write ( uv );
	}
	
	//----------------------------------------------------------------//
	inline void WriteVtx ( float x, float y ) {
		
		this->WriteVtx ( x, y, 0.0f );
	}
	
	//----------------------------------------------------------------//
	inline void WriteVtx ( float x, float y, float z ) {
		
		USVec4D vtx;
		vtx.mX = x;
		vtx.mY = y;
		vtx.mZ = z;
		vtx.mW = 1.0f;
		
		if ( this->mCpuVertexTransform ) {
			this->mCpuVertexTransformMtx.Transform ( vtx );	
		}
		this->Write ( vtx );
	}
	
	//----------------------------------------------------------------//
	inline void WriteVtx ( USVec2D vtx ) {
		
		this->WriteVtx ( vtx.mX, vtx.mY, 0.0f );
	}
	
	//----------------------------------------------------------------//
	inline void WriteVtx ( USVec3D vtx ) {
		
		this->WriteVtx ( vtx.mX, vtx.mY, vtx.mZ );
	}
};

#endif
