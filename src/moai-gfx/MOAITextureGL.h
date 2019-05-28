// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTUREGL_H
#define	MOAITEXTUREGL_H

#include <moai-gfx/MOAIGfxResourceGL.h>
#include <moai-gfx/MOAITextureBaseGL.h>

class MOAIDataBuffer;

//================================================================//
// MOAITextureGL
//================================================================//
/**	@lua	MOAITextureGL
	@text	Texture class.
*/
class MOAITextureGL :
	public virtual MOAIGfxResourceGL < ZLTextureGL >,
	public virtual MOAITextureBaseGL {
private:

	//----------------------------------------------------------------//
	static int			_load					( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAITextureGL )
	
	//----------------------------------------------------------------//
	bool				Init					( MOAILuaState& state, int idx );
	void				Init					( MOAIDataBuffer& data, u32 transform, cc8* debugname );
	void				Init					( const ZLImage& image, cc8* debugname );
	void				Init					( const ZLImage& image, int srcX, int srcY, int width, int height, cc8* debugname );
	void				Init					( cc8* filename, u32 transform, cc8* debugname = NULL );
	void				Init					( ZLStream& stream, u32 transform, cc8* debugname );
	void				Init					( const void* data, size_t size, u32 transform, cc8* debugname );
						MOAITextureGL			();
						~MOAITextureGL			();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
