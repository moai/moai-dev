// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXDEVICE_H
#define	MOAIGFXDEVICE_H

#include <moai-sim/MOAIBlendMode.h>
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIFrameBuffer.h>
#include <moai-sim/MOAIGfxDeviceVertexWriter.h>
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
// MOAIGfxDevice
//================================================================//
/**	@lua	MOAIGfxDevice
	@text	Interface to the graphics singleton.
	
	@const	EVENT_RESIZE
*/
class MOAIGfxDevice :
	public MOAIGfxDeviceVertexWriter,
	public MOAIGlobalClass < MOAIGfxDevice, MOAIGlobalEventSource > {
public:
	
	enum {
		EVENT_RESIZE,
		TOTAL_EVENTS,
	};
	
private:

	bool									mHasContext;

	bool									mIsFramebufferSupported;
	bool									mIsOpenGLES;

	u32										mMajorVersion;
	u32										mMinorVersion;
	
	size_t									mTextureMemoryUsage;
	u32										mMaxTextureSize;

	MOAILuaSharedPtr < MOAIFrameBuffer >	mDefaultFrameBuffer;
	MOAILuaSharedPtr < MOAITexture >		mDefaultTexture;

	//----------------------------------------------------------------//
	static int			_getFrameBuffer				( lua_State* L );
	static int			_getMaxTextureSize			( lua_State* L );
	static int			_getMaxTextureUnits			( lua_State* L );
	static int			_getViewSize				( lua_State* L );
	static int			_setDefaultTexture			( lua_State* L );
	static int			_setPenColor				( lua_State* L );
	static int			_setPenWidth				( lua_State* L );

	//----------------------------------------------------------------//
	void				TransformAndWriteQuad		( ZLVec4D* vtx, ZLVec2D* uv );
	
public:
	
	friend class MOAIGfxResource;
	friend class MOAIShaderProgram;
	friend class MOAISingleTexture;
	
	DECL_LUA_SINGLETON ( MOAIGfxDevice )
	
	GET ( size_t, TextureMemoryUsage, mTextureMemoryUsage )
	GET ( u32, MaxTextureSize, mMaxTextureSize )
	GET ( bool, HasContext, mHasContext )
	
	GET_BOOL ( IsOpenGLES, mIsOpenGLES )
	GET_BOOL ( IsFramebufferSupported, mIsFramebufferSupported )
	
	GET ( MOAIBlendMode, BlendMode, mBlendMode )
	
	GET ( MOAIFrameBuffer*, DefaultFrameBuffer, mDefaultFrameBuffer )
	GET ( MOAITexture*, DefaultTexture, mDefaultTexture )
	
	//----------------------------------------------------------------//
	
	void			ClearErrors				();
	void			ClearSurface			( u32 clearFlags ); // takes zgl clear flags
	void			DetectContext			();
	void			DetectFramebuffer		();
	
	float			GetDeviceScale			();
	
	u32				GetHeight				() const;
	u32				GetWidth				() const;
	
	bool			IsOpaque				() const;
	u32				LogErrors				();
	
					MOAIGfxDevice			();
					~MOAIGfxDevice			();
	
	void			OnGlobalsFinalize		();

	void			RegisterLuaClass		( MOAILuaState& state );
	
	void			ReportTextureAlloc		( cc8* name, size_t size );
	void			ReportTextureFree		( cc8* name, size_t size );
	
	void			ResetDrawCount			();
	void			ResetState				();
	
	void			SetBufferScale			( float scale );
	void			SetBufferSize			( u32 width, u32 height );
	
	void			SetScreenSpace			( MOAIViewport& viewport );

	void			SetViewRect				();
	void			SetViewRect				( ZLRect rect );
		
	void			UpdateShaderGlobals		();
};

#endif
