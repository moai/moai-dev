// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTURE_H
#define	MOAITEXTURE_H

#include <moaicore/MOAIImage.h>
#include <moaicore/MOAITextureBase.h>

class MOAIDataBuffer;

//================================================================//
// MOAITexture
//================================================================//
/**	@name	MOAITexture
	@text	Texture class.
*/
class MOAITexture :
	public MOAITextureBase {
private:

	// for loading from file
	STLString			mFilename;
	u32					mTransform;
	
	// for loading from image
	MOAIImage			mImage;
	
	// for loading compressed data
	void*				mData;
	size_t				mDataSize;

	//----------------------------------------------------------------//
	static int			_load					( lua_State* L );

	//----------------------------------------------------------------//
	bool				IsRenewable				();
	void				OnClear					();
	void				OnCreate				();
	void				OnLoad					();

public:
	
	DECL_LUA_FACTORY ( MOAITexture )
	
	static const u32 DEFAULT_TRANSFORM = MOAIImageTransform::TRUECOLOR | MOAIImageTransform::PREMULTIPLY_ALPHA;
	
	//----------------------------------------------------------------//
	static MOAIGfxState*	AffirmTexture			( MOAILuaState& state, int idx );
	
	bool					Init					( MOAILuaState& state, int idx );
	void					Init					( MOAIImage& image, cc8* debugname );
	void					Init					( MOAIImage& image, int srcX, int srcY, int width, int height, cc8* debugname );
	void					Init					( cc8* filename, u32 transform, cc8* debugname = 0 );
	void					Init					( USStream& stream, u32 transform, cc8* debugname );
	void					Init					( MOAIDataBuffer& data, u32 transform, cc8* debugname );
	//void					Init					( const void* data, u32 size, u32 transform, cc8* debugname );
	
							MOAITexture				();
							~MOAITexture			();
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
	void					SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void					SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
