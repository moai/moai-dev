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
	public virtual MOAIGfxBufferVK,
	public virtual MOAIGfxBufferWithUsageVK < VK_BUFFER_USAGE_VERTEX_BUFFER_BIT > {
private:
	
	//----------------------------------------------------------------//
	void			MOAILuaObject_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAILuaObject_SerializeIn			( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void			MOAILuaObject_SerializeOut			( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );
	
public:
	
	DECL_LUA_FACTORY ( MOAIVertexBufferVK )
	
	//----------------------------------------------------------------//
					MOAIVertexBufferVK		();
					~MOAIVertexBufferVK		();
};

#endif
