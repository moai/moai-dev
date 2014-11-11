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

	ZLLeanArray < u32 > mIndices;
	
	u32		mGLBufferID;
	u32		mHint;
	
	ZLByteStream mStream;
	
	//----------------------------------------------------------------//
	static int	_release				( lua_State* L );
	static int	_reserve				( lua_State* L );
	static int	_setIndex				( lua_State* L );

	//----------------------------------------------------------------//
	u32			GetLoadingPolicy		();
	bool		OnCPUCreate				();
	void		OnCPUDestroy			();
	void		OnGPUBind				();
	bool		OnGPUCreate				();
	void		OnGPUDestroy			();
	void		OnGPULost				();
	void		OnGPUUnbind				();
	
public:
	
	DECL_LUA_FACTORY ( MOAIIndexBuffer )
	
	GET ( u32, IndexCount, ( u32 )mIndices.Size ())
	GET ( ZLStream&, Stream, mStream )
	
	//----------------------------------------------------------------//
	void		Clear					();
				MOAIIndexBuffer			();
				~MOAIIndexBuffer		();
	void		RegisterLuaClass		( MOAILuaState& state );
	void		RegisterLuaFuncs		( MOAILuaState& state );
	void		ReserveIndices			( u32 indexCount );
	void		SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void		SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void		SetIndex				( u32 idx, u32 value );
};

#endif
