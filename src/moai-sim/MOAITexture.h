// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTURE_H
#define	MOAITEXTURE_H

#include <moai-sim/MOAIImage.h>
#include <moai-sim/MOAITextureBase.h>

class MOAIDataBuffer;
class MOAIImageFormat;

//================================================================//
// MOAITexture
//================================================================//
/**	@lua	MOAITexture
	@text	Texture class.
*/
class MOAITexture :
	public MOAITextureBase {
private:

	// for loading from file
	STLString			mFilename;
	u32					mTransform;
	
	// for loading from image
	MOAILuaSharedPtr < MOAIImage> mImage;
	bool				mAutoClearImage;
	
	// for loading compressed data
	void*				mTextureData;
	size_t				mTextureDataSize;
	MOAIImageFormat*	mTextureDataFormat;

	//----------------------------------------------------------------//
	static int			_load					( lua_State* L );

	//----------------------------------------------------------------//
	bool				LoadFromStream				( ZLStream& stream, u32 transform );
	bool				OnCPUCreate					();
	void				OnCPUDestroy				();
	bool				OnGPUCreate					();

public:
	
	DECL_LUA_FACTORY ( MOAITexture )
	
	static const u32 DEFAULT_TRANSFORM = MOAIImageTransform::TRUECOLOR | MOAIImageTransform::PREMULTIPLY_ALPHA;
	
	//----------------------------------------------------------------//
	static MOAIGfxState*	AffirmTexture			( MOAILuaState& state, int idx );
	void					Clear					();
	bool					Init					( MOAILuaState& state, int idx );
	void					Init					( MOAIImage& image, cc8* debugname, bool autoClear );
	void					Init					( MOAIImage& image, int srcX, int srcY, int width, int height, cc8* debugname );
	void					Init					( cc8* filename, u32 transform, cc8* debugname = 0 );
	void					Init					( ZLStream& stream, u32 transform, cc8* debugname );
	void					Init					( MOAIDataBuffer& data, u32 transform, cc8* debugname );
	void					Init					( const void* data, u32 size, u32 transform, cc8* debugname );
							MOAITexture				();
							~MOAITexture			();
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
	void					SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void					SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
