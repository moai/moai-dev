// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTUREBASE_H
#define	MOAITEXTUREBASE_H

#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAIImage.h>

class MOAIDataBuffer;
class MOAIFrameBufferTexture;
class MOAIImage;

//================================================================//
// MOAITextureBase
//================================================================//
/**	@lua	MOAITextureBase
	@text	Base class for texture resources.

	@const	GL_LINEAR
	@const	GL_LINEAR_MIPMAP_LINEAR
	@const	GL_LINEAR_MIPMAP_NEAREST
	@const	GL_NEAREST
	@const	GL_NEAREST_MIPMAP_LINEAR
	@const	GL_NEAREST_MIPMAP_NEAREST
*/
class MOAITextureBase :
	public virtual MOAILuaObject,
	public MOAIGfxResource {
protected:

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
	
	friend class MOAIGfxDevice;
	friend class MOAIImageFormat;
	
	GET_SET ( cc8*, DebugName, mDebugName );
	GET ( u32, TextureID, mGLTexID );

	//----------------------------------------------------------------//
	void			DeleteTexture			();
	u32				GetHeight				();
	u32				GetWidth				();
	bool			IsValid					();
	bool			LoadGfxState			();
					MOAITextureBase			();
					~MOAITextureBase		();
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void			SetFilter				( int filter );
	void			SetFilter				( int min, int mag );
	void			SetWrap					( int wrap );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
