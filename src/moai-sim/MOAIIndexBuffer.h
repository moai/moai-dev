// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIINDEXBUFFER_H
#define	MOAIINDEXBUFFER_H

#include <moai-sim/MOAIGfxResource.h>

//================================================================//
// MOAIIndexBuffer
//================================================================//
/**	@lua	MOAIIndexBuffer
	@text	Index buffer class. Unused at this time.
*/
class MOAIIndexBuffer :
	public virtual MOAILuaObject,
	public MOAIGfxResource {
private:

	static const u32 LEGACY_INDEX_SIZE_IN_BYTES = 4;
	static const u32 DEFAULT_INDEX_SIZE_IN_BYTES = 2;

	u32		mIndexSizeInBytes;

	void*	mIndexBuffer;
	u32		mCapacity;
	
	u32		mGLBufferID;
	u32		mHint;
	
	//----------------------------------------------------------------//
	static int	_release				( lua_State* L );
	static int	_reserve				( lua_State* L );
	static int	_setIndex				( lua_State* L );
	static int	_setIndexSizeInBytes	( lua_State* L );

	//----------------------------------------------------------------//
	u32			GetLoadingPolicy		();
	bool		IsValidIndexSize		( u32 indexSizeInBytes );
	bool		OnCPUCreate				();
	void		OnCPUDestroy			();
	void		OnGPUBind				();
	bool		OnGPUCreate				();
	void		OnGPUDestroy			();
	void		OnGPULost				();
	void		OnGPUUnbind				();
	
public:
	
	DECL_LUA_FACTORY ( MOAIIndexBuffer )
	
	GET ( size_t, BufferSize, mCapacity * mIndexSizeInBytes )
	GET ( u32, IndexCount, mCapacity)
	GET ( u32, IndexSizeInBytes, mIndexSizeInBytes )
	
	//----------------------------------------------------------------//
	void		Clear					();
	void		CopyFromStream			( ZLStream& stream, u32 streamIndexSizeInBytes );
	void		Draw					( u32 zglPrimType );
				MOAIIndexBuffer			();
				~MOAIIndexBuffer		();
	void		RegisterLuaClass		( MOAILuaState& state );
	void		RegisterLuaFuncs		( MOAILuaState& state );
	void		ReserveIndices			( u32 capacity );
	void		ReserveIndices			( u32 capacity, u32 indexSizeInBytes );
	void		SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void		SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void		SetIndex				( u32 idx, u32 value );
};

#endif
