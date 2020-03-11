// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTURE2D_H
#define	MOAITEXTURE2D_H

#include <moai-gfx/MOAITexture.h>

class ZLTextureFormat;

//================================================================//
// MOAITexture2D
//================================================================//
class MOAITexture2D :
	public virtual MOAITexture {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND
	ZL_FINALIZATION_VISITOR_FRIEND

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
	void				_Finalize								();
	void				_RegisterLuaClass						( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs						( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_SerializeIn							( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void				_SerializeOut							( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );
	void				MOAIGfxResource_ClearReloadable			();
	bool				MOAIGfxResource_FinishLoading			();
	
public:

	//----------------------------------------------------------------//
	void				Init						( const ZLImage& image, cc8* debugname );
	void				Init						( const ZLImage& image, int srcX, int srcY, int width, int height, cc8* debugname );
	void				Init						( cc8* filename, u32 transform, cc8* debugname = NULL );
	void				Init						( ZLStream& stream, u32 transform, cc8* debugname );
	void				Init						( const void* data, size_t size, u32 transform, cc8* debugname );
	bool				Init						( MOAILuaState& state, int idx );
	void				Init						( MOAIDataBuffer& data, u32 transform, cc8* debugname );
						MOAITexture2D				();
						~MOAITexture2D				();
};

#endif
