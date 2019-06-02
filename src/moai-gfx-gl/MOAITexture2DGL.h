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
	bool				MOAIGfxResource_OnCPUCreate 			();
	void				MOAIGfxResource_OnCPUPurgeRecoverable 	();
	bool				MOAIGfxResourceGL_OnGPUCreate			();
	void				MOAILuaObject_RegisterLuaClass			( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs			( MOAIComposer& composer, MOAILuaState& state );

public:
		
	static const u32 DEFAULT_TRANSFORM = ZLImageTransform::TRUECOLOR | ZLImageTransform::PREMULTIPLY_ALPHA;
	
	DECL_LUA_FACTORY ( MOAITexture2DGL )
	
	//----------------------------------------------------------------//
						MOAITexture2DGL			();
						~MOAITexture2DGL		();
};

#endif
