// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXBUFFER_H
#define	MOAIGFXBUFFER_H

#include <moai-sim/MOAIGfxResource.h>

class MOAIVertexFormat;

//================================================================//
// MOAIGfxBufferLoader
//================================================================//
class MOAIGfxBufferLoader {
public:

	STLString			mFilename;
	size_t				mOffset;
	size_t				mElemSize;
	size_t				mStride;
	size_t				mTotalElements;
};

//================================================================//
// MOAIGfxBuffer
//================================================================//
// TODO: doxygen
class MOAIGfxBuffer :
	public MOAIGfxResource,
	public MOAIStream,
	public ZLByteStream {
private:
	
	ZLLeanArray < u32 >					mVBOs;
	u32									mCurrentVBO;
	u32									mTarget;
	bool								mIsDirty;

	MOAIGfxBufferLoader*				mLoader;
	void*								mData;

	//----------------------------------------------------------------//
	static int				_computeBounds			( lua_State* L );
	static int				_countElements			( lua_State* L );
	static int				_load					( lua_State* L );
	static int				_makeDirty				( lua_State* L );
	static int				_release				( lua_State* L );
	static int				_reserve				( lua_State* L );
	static int				_reserveVBOs			( lua_State* L );
	static int				_reset					( lua_State* L );
	static int				_setTarget				( lua_State* L );
	static int				_writeColor32			( lua_State* L );
	
	//----------------------------------------------------------------//
	u32						GetLoadingPolicy		();
	bool					OnCPUCreate				();
	void					OnCPUDestroy			();
	void					OnGPUBind				();
	bool					OnGPUCreate				();
	void					OnGPUDestroy			();
	void					OnGPULost				();
	void					OnGPUUnbind				();
	
public:
	
	DECL_LUA_FACTORY ( MOAIGfxBuffer )
	
	GET ( size_t, BufferCount, mVBOs.Size ())
	IS ( Dirty, mIsDirty, true )
	
	GET_SET ( u32, Target, mTarget )
	
	//----------------------------------------------------------------//
	void					Clear					();
	size_t					GetSize					();
	void					MakeDirty				();
							MOAIGfxBuffer			();
							~MOAIGfxBuffer			();
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
	void					Reserve					( u32 size );
	void					ReserveVBOs				( u32 gpuBuffers );
	void					SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void					SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
