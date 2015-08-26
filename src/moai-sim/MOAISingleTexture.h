// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISINGLETEXTURE_H
#define	MOAISINGLETEXTURE_H

#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAITextureBase.h>

class MOAIImage;

//================================================================//
// MOAISingleTexture
//================================================================//
/**	@lua	MOAISingleTexture
	@text	Base class for texture resources.

	@const	GL_LINEAR
	@const	GL_LINEAR_MIPMAP_LINEAR
	@const	GL_LINEAR_MIPMAP_NEAREST
	@const	GL_NEAREST
	@const	GL_NEAREST_MIPMAP_LINEAR
	@const	GL_NEAREST_MIPMAP_NEAREST
*/
class MOAISingleTexture :
	public MOAITextureBase, // MOAISingleTexture is a MOAITextureBase of *one* and contains only itself
	public MOAIGfxResource {
protected:

	friend class MOAIGfxDevice;
	friend class MOAIGfxDeviceStateCache;
	friend class MOAIImageFormat;

	// debug name for memory use reporting
	STLString			mDebugName;

	// GL texture
	u32					mGLTexID;
	
	// size of the original texture
	u32					mWidth;
	u32					mHeight;

	// GL_LINEAR
	// GL_LINEAR_MIPMAP_LINEAR
	// GL_LINEAR_MIPMAP_NEAREST
	// GL_NEAREST
	// GL_NEAREST_MIPMAP_LINEAR
	// GL_NEAREST_MIPMAP_NEAREST
	int					mMinFilter;
	int					mMagFilter;
	
	// GL_CLAMP_TO_EDGE
	// GL_REPEAT
	int					mWrap;
	
	int					mGLInternalFormat;
	int					mGLPixelType;
	
	size_t				mTextureSize;
	
	bool				mIsDirty;

	//----------------------------------------------------------------//
	static int			_getSize				( lua_State* L );
	static int			_release				( lua_State* L );
	static int			_setDebugName			( lua_State* L );
	static int			_setFilter				( lua_State* L );
	static int			_setWrap				( lua_State* L );

	//----------------------------------------------------------------//
	void				CleanupOnError				();
	bool				CreateTextureFromImage		( MOAIImage& srcImage );
	bool				OnCPUCreate					();
	void				OnCPUDestroy				();
	void				OnGPUBind					();
	void				OnGPUDestroy				();
	void				OnGPULost					();
	void				OnGPUUnbind					();
	void				SetTextureID				( u32 glTexID, int internalFormat, int pixelType, size_t textureSize );
	bool				ShouldGenerateMipmaps		();
	void				UpdateTextureFromImage		( MOAIImage& image, ZLIntRect rect );
	
public:
	
	GET_SET ( cc8*, DebugName, mDebugName );
	GET ( u32, TextureID, mGLTexID );

	//----------------------------------------------------------------//
	u32					CountActiveUnits			();
	void				DeleteTexture				();
	u32					GetHeight					();
	MOAISingleTexture*	GetTextureForUnit			( u32 unit );
	u32					GetWidth					();
	bool				IsValid						();
						MOAISingleTexture				();
						~MOAISingleTexture			();
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
	void				SetFilter					( int filter );
	void				SetFilter					( int min, int mag );
	void				SetWrap						( int wrap );
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
