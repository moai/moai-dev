// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTURE2DVK_H
#define	MOAITEXTURE2DVK_H

#include <moai-gfx-vk/MOAITextureVK.h>

class ZLTextureFormat;

//================================================================//
// MOAITexture2DVK
//================================================================//
/**	@lua	MOAITexture2DVK
	@text	Texture class.
*/
class MOAITexture2DVK :
	public virtual MOAITexture2D,
	public virtual MOAITextureVK {
protected:

	//----------------------------------------------------------------//
	void				MOAILuaObject_RegisterLuaClass			( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs			( MOAIComposer& composer, MOAILuaState& state );

public:
		
//	static const u32 DEFAULT_TRANSFORM = ZLImageTransform::TRUECOLOR | ZLImageTransform::PREMULTIPLY_ALPHA;
	
	DECL_LUA_FACTORY ( MOAITexture2DVK )
	
	//----------------------------------------------------------------//
						MOAITexture2DVK			();
						~MOAITexture2DVK		();
};

#endif
