// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTURE2D_H
#define	MOAITEXTURE2D_H

#include <moaicore/MOAIDeck.h>

class MOAIDataBuffer;
class MOAIImage;
class MOAITextureLoader;

//================================================================//
// MOAITexture
//================================================================//
/**	@name	MOAITexture
	@text	Texture class.

	@const	GL_LINEAR
	@const	GL_LINEAR_MIPMAP_LINEAR
	@const	GL_LINEAR_MIPMAP_NEAREST
	@const	GL_NEAREST
	@const	GL_NEAREST_MIPMAP_LINEAR
	@const	GL_NEAREST_MIPMAP_NEAREST
*/
class MOAITexture :
	public virtual USLuaObject {
private:

	static const u32 DEFAULT_TRANSFORM = USImageTransform::TRUECOLOR | USImageTransform::PREMULTIPLY_ALPHA;

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
	
	STLString			mTexturePath;
	STLString			mFilename;
	MOAITextureLoader*	mLoader;

	//----------------------------------------------------------------//
	static int		_bind				( lua_State* L );
	static int		_getSize			( lua_State* L );
	static int		_load				( lua_State* L );
	static int		_release			( lua_State* L );
	static int		_setFilter			( lua_State* L );
	static int		_setWrap			( lua_State* L );

	//----------------------------------------------------------------//
	void					Affirm					();
	bool					Bind					();
	void					CreateTextureFromImage	( USImage& image );
	void					CreateTextureFromPVR	( void* data, size_t size );
	void					ReleaseLoader			();

public:
	
	DECL_LUA_FACTORY ( MOAITexture )
	
	friend class MOAIGfxDevice;
	
	//----------------------------------------------------------------//
	static MOAITexture*		AffirmTexture			( USLuaState& state, int idx );
	u32						GetHeight				();
	u32						GetWidth				();
	bool					IsOK					();
	void					Load					( MOAIImage& image );
	void					Load					( cc8* filename, u32 transform = DEFAULT_TRANSFORM );
	void					Load					( MOAIDataBuffer& data, u32 transform = DEFAULT_TRANSFORM);
	void					Load					( const void* data, u32 size, u32 transform = DEFAULT_TRANSFORM ) ;
							MOAITexture				();
							~MOAITexture			();
	void					SerializeIn				( USLuaState& state, USLuaSerializer& serializer );
	void					SerializeOut			( USLuaState& state, USLuaSerializer& serializer );
	void					SetFilter				( int filter );
	void					SetFilter				( int min, int mag );
	void					SetWrap					( int wrap );
	void					Release					();
	void					RegisterLuaClass		( USLuaState& state );
	void					RegisterLuaFuncs		( USLuaState& state );
	STLString				ToString				();
};

#endif
