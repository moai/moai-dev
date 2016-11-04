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
/**	@lua	MOAIGfxBuffer
	@text	Base class for MOAIVertexBuffer and MOAIIndexBuffer.
*/
class MOAIGfxBuffer :
	public MOAIGfxResource,
	public virtual MOAIStream,
	public ZLCopyOnWrite {
protected:
	
	friend class MOAIGfxPipelineClerk;
	friend class MOAIGfxStateCache;
	
	enum {
		UPDATE_MODE_MAPBUFFER,
		UPDATE_MODE_ORPHAN,
		UPDATE_MODE_SUBDATA,
	};
	
	ZLLeanArray < ZLGfxHandle* >	mVBOs;
	u32								mCurrentVBO;
	u32								mTarget;

	MOAIGfxBufferLoader*			mLoader;

	bool							mUseVBOs;
	bool							mCopyOnUpdate;

	//----------------------------------------------------------------//
	static int				_copyFromStream			( lua_State* L );
	static int				_release				( lua_State* L );
	static int				_reserve				( lua_State* L );
	static int				_reserveVBOs			( lua_State* L );
	static int				_scheduleFlush			( lua_State* L );
	
	//----------------------------------------------------------------//
	//void					BindVertexFormat		( MOAIVertexFormat* format );
	bool					OnCPUCreate				();
	void					OnCPUDestroy			();
	void					OnGPUBind				();
	bool					OnGPUCreate				();
	void					OnGPUDeleteOrDiscard	( bool shouldDelete );
	void					OnGPUUnbind				();

public:
	
	GET ( size_t, BufferCount, mVBOs.Size ())
	GET ( u32, Target, mTarget )
	GET_SET ( bool, CopyOnUpdate, mCopyOnUpdate )
	
	IS ( UsingVBOs, mUseVBOs, true )
	
	//----------------------------------------------------------------//
	void						Clear					();
	void						CopyFromStream			( ZLStream& stream, size_t size );
	ZLSharedConstBuffer*		GetBuffer				();
								MOAIGfxBuffer			();
								~MOAIGfxBuffer			();
	bool						OnGPUUpdate				();
	void						RegisterLuaClass		( MOAILuaState& state );
	void						RegisterLuaFuncs		( MOAILuaState& state );
	void						Reserve					( u32 size );
	void						ReserveVBOs				( u32 gpuBuffers );
	void						SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void						SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
