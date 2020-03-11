// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIINDEXBUFFER_H
#define	MOAIINDEXBUFFER_H

#include <moai-gfx/MOAIGfxBuffer.h>

//================================================================//
// MOAIIndexBuffer
//================================================================//
class MOAIIndexBuffer :
	public virtual MOAIGfxBuffer {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	u32				mIndexSize;

	//----------------------------------------------------------------//
	static int		_copyFromStream			( lua_State* L );
	static int		_countElements			( lua_State* L );
	static int		_printIndices			( lua_State* L );
	static int		_setIndexSize			( lua_State* L );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_SerializeIn			( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void			_SerializeOut			( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );

public:

	GET ( u32, IndexSize, mIndexSize )

	//----------------------------------------------------------------//
	u32				CountIndices			();
	void			CopyFromStream			( ZLStream& stream, size_t size, u32 srcInputSizeInBytes );
	u32				GetIndex				( u32 element );
					MOAIIndexBuffer			();
					~MOAIIndexBuffer		();
	void			PrintIndices			();
	void			SetIndexSize			( u32 idxSize );
	void			WriteIndex				( u32 index );
};

#endif
