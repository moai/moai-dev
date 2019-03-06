// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTGFXBUFFER_H
#define	MOAIABSTRACTGFXBUFFER_H

//================================================================//
// MOAIAbstractGfxBuffer
//================================================================//
/**	@lua	MOAIAbstractGfxBuffer
	@text	Base class for MOAIVertexBuffer and MOAIIndexBuffer.
*/
class MOAIAbstractGfxBuffer :
	public virtual MOAIStream {
protected:
	
	friend class ZLGfxPipelineClerk;
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
						MOAIAbstractGfxBuffer		();
						~MOAIAbstractGfxBuffer		();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
