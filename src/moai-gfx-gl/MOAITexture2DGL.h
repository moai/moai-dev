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
	public virtual MOAITexture2D,
	public virtual MOAITextureGL {
protected:

	//----------------------------------------------------------------//
	static int			_init					( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass						( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs						( RTTIVisitorHistory& history, MOAILuaState& state );
	bool				MOAIGfxResourceGL_OnGPUCreate			();

public:
		
	static const u32 DEFAULT_TRANSFORM = ZLImageTransform::TRUECOLOR | ZLImageTransform::PREMULTIPLY_ALPHA;
	
	DECL_LUA_FACTORY ( MOAITexture2DGL )
	
	//----------------------------------------------------------------//
						MOAITexture2DGL			( ZLContext& context );
						~MOAITexture2DGL		();
};

#endif
