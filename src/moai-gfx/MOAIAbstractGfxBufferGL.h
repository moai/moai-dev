// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTGFXBUFFERGL_H
#define	MOAIABSTRACTGFXBUFFERGL_H

//================================================================//
// MOAIAbstractGfxBufferGL
//================================================================//
/**	@lua	MOAIAbstractGfxBufferGL
	@text	Base class for MOAIVertexBufferGL and MOAIIndexBufferGL.
*/
class MOAIAbstractGfxBufferGL :
	public virtual MOAIStream {
protected:
	
	friend class ZLGfxPipelineClerkGL;
	friend class ZLGfxStateGPUCacheGL;
	friend class MOAIVertexArrayItem;

	//----------------------------------------------------------------//
	static int			_copyFromStream			( lua_State* L );
	static int			_release				( lua_State* L );
	static int			_reserve				( lua_State* L );
	static int			_reserveVBOs			( lua_State* L );
	static int			_scheduleFlush			( lua_State* L );

	//----------------------------------------------------------------//
	virtual ZLGfxBufferGL&		MOAIGfxBuffer_ZLGfxBuffer		() = 0;

public:
	
	//----------------------------------------------------------------//
						MOAIAbstractGfxBufferGL		();
						~MOAIAbstractGfxBufferGL	();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
