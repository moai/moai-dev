// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXBUFFER_H
#define	MOAIGFXBUFFER_H

#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/ZLGfxBuffer.h>

class MOAIVertexFormat;

//================================================================//
// MOAIGfxBuffer
//================================================================//
/**	@lua	MOAIGfxBuffer
	@text	Base class for MOAIVertexBuffer and MOAIIndexBuffer.
*/
class MOAIGfxBuffer :
	public virtual MOAIStream {
protected:
	
	friend class MOAIGfxPipelineClerk;
	friend class ZLGfxStateGPUCache;
	friend class MOAIVertexArrayItem;

	//----------------------------------------------------------------//
	static int			_copyFromStream			( lua_State* L );
	static int			_release				( lua_State* L );
	static int			_reserve				( lua_State* L );
	static int			_reserveVBOs			( lua_State* L );
	static int			_scheduleFlush			( lua_State* L );

	//----------------------------------------------------------------//
	virtual ZLGfxBuffer&		MOAIGfxBuffer_ZLGfxBuffer		() = 0;

public:
	
	//----------------------------------------------------------------//
						MOAIGfxBuffer			();
						~MOAIGfxBuffer			();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
