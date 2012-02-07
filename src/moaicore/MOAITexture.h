// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTURE_H
#define	MOAITEXTURE_H

#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAIGfxResource.h>
#include <moaicore/MOAIImage.h>
#include <moaicore/MOAILua.h>

class MOAIDataBuffer;
class MOAIFrameBuffer;
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
	public virtual MOAILuaObject,
	public MOAIGfxResource {
private:

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
	
	STLString			mFilename;
	MOAITextureLoader*	mLoader;
	MOAIFrameBuffer*	mFrameBuffer;
	
	size_t				mDataSize;

	bool				mIsRenewable;
	u32					mTransform;
	
	bool				mIsDirty;

	//----------------------------------------------------------------//
	static int		_bind					( lua_State* L );
	static int		_getSize				( lua_State* L );
	static int		_initFrameBuffer		( lua_State* L );
	static int		_load					( lua_State* L );
	static int		_release				( lua_State* L );
	static int		_setFilter				( lua_State* L );
	static int		_setWrap				( lua_State* L );

	//----------------------------------------------------------------//
	void			CreateTextureFromImage	( MOAIImage& image );
	void			CreateTextureFromPVR	( void* data, size_t size );
	bool			IsRenewable				();
	void			OnBind					();
	void			OnClear					();
	void			OnLoad					();
	void			OnRenew					();
	void			OnUnload				();
	void			ReleaseLoader			();

public:
	
	static const u32 DEFAULT_TRANSFORM = MOAIImageTransform::TRUECOLOR | MOAIImageTransform::PREMULTIPLY_ALPHA;
	
	DECL_LUA_FACTORY ( MOAITexture )
	
	friend class MOAIGfxDevice;
	
	GET ( MOAIFrameBuffer*, FrameBuffer, mFrameBuffer )
	
	//----------------------------------------------------------------//
	static MOAIGfxState*	AffirmTexture			( MOAILuaState& state, int idx );
	void					CreateTexture			();
	void					DeleteTexture			();
	u32						GetHeight				();
	u32						GetWidth				();
	void					Init					( MOAIImage& image, cc8* debugname = 0 );
	void					Init					( cc8* filename, u32 transform = DEFAULT_TRANSFORM );
	void					Init					( MOAIDataBuffer& data, u32 transform = DEFAULT_TRANSFORM, cc8* debugname = 0 );
	void					Init					( const void* data, u32 size, u32 transform = DEFAULT_TRANSFORM, cc8* debugname = 0 );
	void					InitFrameBuffer			( u32 width, u32 height, GLenum colorFormat, GLenum depthFormat, GLenum stencilFormat );
	bool					IsFrameBuffer			();
	bool					IsValid					();
	bool					LoadGfxState			();
							MOAITexture				();
							~MOAITexture			();
	void					SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void					SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void					SetFilter				( int filter );
	void					SetFilter				( int min, int mag );
	void					SetWrap					( int wrap );
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
