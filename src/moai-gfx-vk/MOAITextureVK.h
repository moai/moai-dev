// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTUREVK_H
#define	MOAITEXTUREVK_H

#include <moai-gfx-vk/MOAIGfxResourceVK.h>

class ZLImage;

//================================================================//
// MOAITextureVK
//================================================================//
/**	@lua	MOAITextureVK
	@text	Base class for texture resources.

	@const	VK_LINEAR
	@const	VK_LINEAR_MIPMAP_LINEAR
	@const	VK_LINEAR_MIPMAP_NEAREST
	@const	VK_NEAREST
	@const	VK_NEAREST_MIPMAP_LINEAR
	@const	VK_NEAREST_MIPMAP_NEAREST
*/
class MOAITextureVK :
	public virtual MOAITexture,
	public virtual MOAIGfxResourceVK {
protected:

//	friend class MOAIGfxMgrVK_GPUCacheVK;
//	friend class ZLTextureFormat;
//
//	// VK texture
//	ZLGfxHandle			mGLTexture;
//
//	int					mGLInternalFormat;
//	int					mGLPixelType;

	//----------------------------------------------------------------//
//	static int			_getSize					( lua_State* L );
//	static int			_release					( lua_State* L );
//	static int			_setDebugName				( lua_State* L );
//	static int			_setFilter					( lua_State* L );
//	static int			_setWrap					( lua_State* L );

	//----------------------------------------------------------------//
//	void				CleanupOnError				();
//	bool				CreateTextureFromImage		( ZLImage& srcImage );
//	void				SetGLTexture				( const ZLGfxHandle& glTexture, int internalFormat, int pixelType, size_t textureSize );
//	bool				ShouldGenerateMipmaps		();
//	bool				UpdateTextureFromImage		( ZLImage& image, ZLIntRect rect );
	
	//----------------------------------------------------------------//
	bool				MOAIGfxResource_OnCPUCreate				();
	void				MOAIGfxResource_OnCPUPurgeRecoverable	();
	void				MOAIGfxResourceVK_OnGPUBind				();
	void				MOAIGfxResourceVK_OnGPUDeleteOrDiscard	( bool shouldDelete );
	void				MOAIGfxResourceVK_OnGPUUnbind			();
	bool				MOAIGfxResourceVK_OnGPUUpdate			();
	void				MOAILuaObject_RegisterLuaClass			( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs			( MOAIComposer& composer, MOAILuaState& state );
//	void				ZLGfxListener_OnGfxEvent				( u32 event, void* userdata );
	
public:
	
//	GET_CONST ( ZLGfxHandle&, GLTexture, mGLTexture );

	//----------------------------------------------------------------//
//	static void			CheckFilterModes			( int min, int mag );
						MOAITextureVK				();
						~MOAITextureVK				();
	
};

#endif
