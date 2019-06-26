// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXBUFFERVK_H
#define	MOAIVERTEXBUFFERVK_H

#include <moai-gfx-vk/MOAIGfxBufferVK.h>

class MOAIVertexFormatVK;

//================================================================//
// MOAIVertexBufferVK
//================================================================//
/**	@lua	MOAIIndexBufferVK
	@text	Buffer for vertices.
*/
class MOAIVertexBufferVK :
	public virtual MOAIVertexBuffer,
	public virtual MOAIGfxBufferVK {
private:
	
//	friend class MOAIGfxMgrVK_PipelineClerkVK;
//	friend class MOAIVertexBufferWithFormatVK;
	
	//----------------------------------------------------------------//
	void			MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_SerializeIn			( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void			MOAILuaObject_SerializeOut			( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );
	
public:
	
	DECL_LUA_FACTORY ( MOAIVertexBufferVK )
	
	//----------------------------------------------------------------//
					MOAIVertexBufferVK		();
					~MOAIVertexBufferVK		();
};

#endif
