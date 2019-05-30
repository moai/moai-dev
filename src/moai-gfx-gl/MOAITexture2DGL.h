// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTURE2DGL_H
#define	MOAITEXTURE2DGL_H

#include <moai-gfx-gl/MOAITextureGL.h>

class ZLTextureFormat;

//================================================================//
// MOAITexture2DGL
//================================================================//
/**	@lua	MOAITexture2DGL
	@text	Texture class.
*/
class MOAITexture2DGL :
	public virtual MOAITextureGL {
protected:

	// for loading from file
	STLString			mFilename;
	u32					mTransform;
	
	// for loading from image
	ZLImage* 			mImage;
	
	// for loading compressed data
	void*				mTextureData;
	size_t				mTextureDataSize;
	ZLTextureFormat*	mTextureDataFormat;

	//----------------------------------------------------------------//
	static int			_load						( lua_State* L );

	//----------------------------------------------------------------//
	void				ClearImage					();
	void				CopyImage					( const ZLImage& image );
	bool				LoadFromStream				( ZLStream& stream, u32 transform );
	
	//----------------------------------------------------------------//
	bool				ZLAbstractGfxResource_OnCPUCreate				();
	void				ZLAbstractGfxResource_OnCPUDestroy				();
	bool				ZLAbstractGfxResource_OnGPUCreate				();

public:
		
	static const u32 DEFAULT_TRANSFORM = ZLImageTransform::TRUECOLOR | ZLImageTransform::PREMULTIPLY_ALPHA;
	
	DECL_LUA_FACTORY ( MOAITexture2DGL )
	
	//----------------------------------------------------------------//
	void				Clear					();
	void				Init					( const ZLImage& image, cc8* debugname );
	void				Init					( const ZLImage& image, int srcX, int srcY, int width, int height, cc8* debugname );
	void				Init					( cc8* filename, u32 transform, cc8* debugname = NULL );
	void				Init					( ZLStream& stream, u32 transform, cc8* debugname );
	void				Init					( const void* data, size_t size, u32 transform, cc8* debugname );
	bool				Init					( MOAILuaState& state, int idx );
	void				Init					( MOAIDataBuffer& data, u32 transform, cc8* debugname );
						MOAITexture2DGL			();
						~MOAITexture2DGL		();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
