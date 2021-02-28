// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXBUFFER_H
#define	MOAIGFXBUFFER_H

#include <moai-gfx/MOAIGfxResource.h>

//================================================================//
// MOAIGfxBuffer
//================================================================//
class MOAIGfxBuffer :
	public virtual MOAIGfxResource,
	public virtual MOAIStream,
	public virtual ZLCopyOnWrite {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	//----------------------------------------------------------------//
	static int			_copyFromStream				( lua_State* L );
	static int			_reserve					( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_SerializeIn				( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void				_SerializeOut				( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );

public:

	//----------------------------------------------------------------//
	void				Initialize					( ZLSize size );
	void				Initialize					( ZLStream& stream, size_t size );
						MOAIGfxBuffer				( ZLContext& context );
						~MOAIGfxBuffer				();
};

#endif
