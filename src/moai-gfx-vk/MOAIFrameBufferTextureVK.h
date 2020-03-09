// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFRAMEBUFFERTEXTUREVK_H
#define	MOAIFRAMEBUFFERTEXTUREVK_H

#include <moai-gfx-vk/MOAIFrameBufferVK.h>
#include <moai-gfx-vk/MOAITextureVK.h>

//================================================================//
// MOAIFrameBufferTextureVK
//================================================================//
class MOAIFrameBufferTextureVK :
	public virtual MOAIFrameBufferVK,
	public virtual MOAITextureVK {
private:
	
	//----------------------------------------------------------------//
	void				MOAIFrameBufferVK_AffirmBuffers				();
	void				MOAILuaObject_RegisterLuaClass				( RTTIVisitorHistory& history, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs				( RTTIVisitorHistory& history, MOAILuaState& state );
	void				MOAILuaObject_SerializeIn					( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void				MOAILuaObject_SerializeOut					( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );

public:
	
	DECL_LUA_FACTORY ( MOAIFrameBufferTextureVK )
	
	//----------------------------------------------------------------//
						MOAIFrameBufferTextureVK		();
						~MOAIFrameBufferTextureVK		();
};

#endif
