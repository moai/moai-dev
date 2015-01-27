// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXBUFFER_H
#define	MOAIVERTEXBUFFER_H

#include <moai-sim/MOAIGfxResource.h>

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
	public ZLStreamProxy,
	public virtual MOAIStream {
private:
	
	ZLLeanArray < u32 >					mVBOs;
	u32									mCurrentVBO;
	u32									mTarget;
	u32									mHint;
	bool								mIsDirty;
	bool								mIsDynamic;

	MOAIGfxBufferLoader*				mLoader;

	ZLLeanArray < u8 >					mBuffer;
	MOAILuaSharedPtr < MOAIStream >		mStream;

	//----------------------------------------------------------------//
	static int			_load					( lua_State* state );
	static int			_release				( lua_State* state );
	static int			_reserve				( lua_State* state );
	static int			_reserveVBOs			( lua_State* state );
	static int			_reset					( lua_State* state );
	static int			_writeColor32			( lua_State* state );
	
	//----------------------------------------------------------------//
	u32					GetLoadingPolicy		();
	bool				OnCPUCreate				();
	void				OnCPUDestroy			();
	void				OnGPUBind				();
	bool				OnGPUCreate				();
	void				OnGPUDestroy			();
	void				OnGPULost				();
	void				OnGPUUnbind				();
	void				SetStream				( MOAIStream* stream );
	
public:
	
	//----------------------------------------------------------------//
	void				Clear					();
	bool				IsDirty					();
						MOAIGfxBuffer			();
						~MOAIGfxBuffer			();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				Reserve					( u32 size );
	void				ReserveVBOs				( u32 gpuBuffers );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
