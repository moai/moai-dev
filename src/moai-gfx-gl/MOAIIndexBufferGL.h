// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIINDEXBUFFERGL_H
#define	MOAIINDEXBUFFERGL_H

#include <moai-gfx-gl/MOAIAbstractGfxBufferGL.h>

//================================================================//
// MOAIIndexBufferGL
//================================================================//
/**	@lua	MOAIIndexBufferGL
	@text	Buffer for vertex indices.
*/
class MOAIIndexBufferGL :
	public virtual MOAIAbstractIndexBuffer,
	public virtual MOAIAbstractGfxBufferGL {
private:

	friend class MOAIGfxMgrGL_PipelineClerkGL;

	u32				mIndexSize;

	//----------------------------------------------------------------//
	static int		_copyFromStream			( lua_State* L );
	static int		_countElements			( lua_State* L );
	static int		_printIndices			( lua_State* L );
	static int		_setIndexSize			( lua_State* L );
	
public:
	
	GET ( u32, IndexSize, mIndexSize )
	
	DECL_LUA_FACTORY ( MOAIIndexBufferGL )
	
	//----------------------------------------------------------------//
	u32				CountIndices			();
	void			CopyFromStream			( ZLStream& stream, size_t size, u32 srcInputSizeInBytes );
	u32				GetIndex				( u32 element );
					MOAIIndexBufferGL		();
					~MOAIIndexBufferGL		();
	void			PrintIndices			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void			SetIndexSize			( u32 idxSize );
	void			WriteIndex				( u32 index );
};

#endif
