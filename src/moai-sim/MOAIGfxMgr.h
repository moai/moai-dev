// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGR_H
#define	MOAIGFXMGR_H

#include <moai-sim/MOAIBlendMode.h>
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIFrameBuffer.h>
#include <moai-sim/MOAIGfxGlobalsCache.h>
#include <moai-sim/MOAIGfxPipelineClerk.h>
#include <moai-sim/MOAIGfxResourceClerk.h>
#include <moai-sim/MOAIGfxVertexCache.h>
#include <moai-sim/MOAIImage.h>

class MOAICamera;
class MOAIFrameBuffer;
class MOAIGfxResource;
class MOAIMultiTexture;
class MOAIShader;
class MOAIShaderProgram;
class MOAITexture;
class MOAISingleTexture;
class MOAIVertexFormat;
class MOAIViewport;

//================================================================//
// MOAIGfxMgr
//================================================================//
/**	@lua	MOAIGfxMgr
	@text	Interface to the graphics singleton.
	
	@const	EVENT_RESIZE
*/
class MOAIGfxMgr :
	public ZLContextClass < MOAIGfxMgr, MOAIGlobalEventSource > {
public:
	
	enum {
		EVENT_RESIZE,
		TOTAL_EVENTS,
	};
	
private:

	bool				mHasContext;

	bool				mIsFramebufferSupported;
	bool				mIsOpenGLES;

	u32					mMajorVersion;
	u32					mMinorVersion;
	
	size_t				mTextureMemoryUsage;
	u32					mMaxTextureSize;

	//----------------------------------------------------------------//
	static int			_enablePipelineLogging		( lua_State* L );
	static int			_getFrameBuffer				( lua_State* L );
	static int			_getMaxTextureSize			( lua_State* L );
	static int			_getMaxTextureUnits			( lua_State* L );
	static int			_getViewSize				( lua_State* L );
	static int			_purgeResources				( lua_State* L );
	static int			_renewResources				( lua_State* L );
	static int			_setDefaultTexture			( lua_State* L );
	static int			_setPenColor				( lua_State* L );
	static int			_setPenWidth				( lua_State* L );
	
	//----------------------------------------------------------------//
	void				OnGlobalsFinalize			();
	void				OnGlobalsInitialize			();
	
public:
	
	friend class MOAIGfxResource;
	friend class MOAIShaderProgram;
	friend class MOAISingleTexture;
	
	DECL_LUA_SINGLETON ( MOAIGfxMgr )
	
	GET ( size_t, TextureMemoryUsage, mTextureMemoryUsage )
	GET ( u32, MaxTextureSize, mMaxTextureSize )
	GET ( bool, HasContext, mHasContext )
	
	GET_BOOL ( IsOpenGLES, mIsOpenGLES )
	GET_BOOL ( IsFramebufferSupported, mIsFramebufferSupported )
	
	MOAIGfxResourceClerk		mResourceMgr;
	MOAIGfxGlobalsCache			mGfxState;
	MOAIGfxVertexCache			mVertexCache;
	MOAIGfxPipelineClerk		mPipelineMgr;
	
	//----------------------------------------------------------------//
	
	void			ClearErrors				();
	void			ClearSurface			( u32 clearFlags ); // takes zgl clear flags
	void			DetectContext			();
	void			DetectFramebuffer		();
	
	bool			IsOpaque				();
	u32				LogErrors				();
	
					MOAIGfxMgr				();
					~MOAIGfxMgr				();

	void			RegisterLuaClass		( MOAILuaState& state );
	
	void			ReportTextureAlloc		( cc8* name, size_t size );
	void			ReportTextureFree		( cc8* name, size_t size );
	
	void			ResetDrawCount			();
	
	void			SetBufferScale			( float scale );
	void			SetBufferSize			( u32 width, u32 height );
	
	//----------------------------------------------------------------//
	static ZLGfx& GetDrawingAPI () {
	
		ZLGfx* gfx = MOAIGfxMgr::Get ().mPipelineMgr.GetDrawingAPI ();
		assert ( gfx );
		
		return *gfx;
	}
};

#endif
