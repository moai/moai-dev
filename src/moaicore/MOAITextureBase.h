// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTUREBASE_H
#define	MOAITEXTUREBASE_H

#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAIGfxResource.h>
#include <moaicore/MOAIImage.h>
#include <moaicore/MOAILua.h>

class MOAIDataBuffer;
class MOAIFrameBufferTexture;
class MOAIImage;

//================================================================//
// MOAITextureBase
//================================================================//
/**	@name	MOAITextureBase
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
	GLuint				mGLTexID;
	
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
	static int		_getSize				( lua_State* L );
	static int		_release				( lua_State* L );
	static int		_setFilter				( lua_State* L );
	static int		_setWrap				( lua_State* L );

	//----------------------------------------------------------------//
	void			CreateTextureFromImage	( MOAIImage& image );
	void			CreateTextureFromPVR	( void* data, size_t size );
	bool			IsRenewable				();
	void			OnBind					();
	void			OnClear					();
	void			OnDestroy				();
	void			OnInvalidate			();
	void			ReleaseLoader			();
	void			UpdateTextureFromImage	( MOAIImage& image, USIntRect rect );

public:
	
	friend class MOAIGfxDevice;
	
	GET_SET ( cc8*, DebugName, mDebugName );

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
