// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIINDEXBUFFER_H
#define	MOAIINDEXBUFFER_H

#include <moai-sim/MOAIGfxBuffer.h>

//================================================================//
// MOAIIndexBuffer
//================================================================//
/**	@lua	MOAIIndexBuffer
	@text	Buffer for vertex indices.
*/
class MOAIIndexBuffer :
	public MOAIGfxBuffer {
private:

	friend class MOAIGfxDeviceBase;

	u32				mIndexSize;

	//----------------------------------------------------------------//
	static int		_copyFromStream			( lua_State* L );
	static int		_countElements			( lua_State* L );
	static int		_printIndices			( lua_State* L );
	static int		_setIndexSize			( lua_State* L );
	
public:
	
	GET ( u32, IndexSize, mIndexSize )
	
	DECL_LUA_FACTORY ( MOAIIndexBuffer )
	
	//----------------------------------------------------------------//
	void			CopyFromStream			( ZLStream& stream, u32 srcInputSizeInBytes );
					MOAIIndexBuffer			();
					~MOAIIndexBuffer		();
	void			PrintIndices			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void			SetIndexSize			( u32 idxSize );
	void			WriteIndex				( u32 index );
};

#endif
