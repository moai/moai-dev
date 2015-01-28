// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXBUFFER_H
#define	MOAIGFXBUFFER_H

#include <moai-sim/MOAIGfxResource.h>

class MOAIVertexFormat;

//================================================================//
// MOAIGfxBufferBase
//================================================================//
class MOAIGfxBufferBase :
	public MOAIGfxResource,
	public virtual MOAIStream {
protected:
	
	u32			mTotalElements;
	bool		mHasBounds;
	ZLBox		mBounds;
	
public:

	GET ( u32, TotalElements, mTotalElements )
	HAS ( Bounds, mHasBounds, true )

	//----------------------------------------------------------------//
	bool								GetBounds						( ZLBox& bounds );
	virtual size_t						GetSize							() = 0;
	virtual const MOAIVertexFormat*		GetVertexFormat					() const = 0; // back compat
										MOAIGfxBufferBase				();
	virtual								~MOAIGfxBufferBase				();
	void								RegisterLuaClass				( MOAILuaState& state );
	void								RegisterLuaFuncs				( MOAILuaState& state );
};

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
	public MOAIGfxBufferBase,
	public ZLStreamProxy {
private:
	
	ZLLeanArray < u32 >					mVBOs;
	u32									mCurrentVBO;
	u32									mTarget;
	bool								mIsDirty;
	bool								mIsDynamic;

	MOAIGfxBufferLoader*				mLoader;

	MOAILuaSharedPtr < MOAIStream >		mStream;

	//----------------------------------------------------------------//
	static int				_bless					( lua_State* L );
	static int				_load					( lua_State* L );
	static int				_release				( lua_State* L );
	static int				_reserve				( lua_State* L );
	static int				_reserveVBOs			( lua_State* L );
	static int				_reset					( lua_State* L );
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
	void					SetStream				( MOAIStream* stream );
	
public:
	
	DECL_LUA_FACTORY ( MOAIGfxBuffer )
	
	//----------------------------------------------------------------//
	void						Clear					();
	size_t						GetSize					();
	const MOAIVertexFormat*		GetVertexFormat			() const;
								MOAIGfxBuffer			();
								~MOAIGfxBuffer			();
	void						RegisterLuaClass		( MOAILuaState& state );
	void						RegisterLuaFuncs		( MOAILuaState& state );
	void						Reserve					( u32 size );
	void						ReserveVBOs				( u32 gpuBuffers );
	void						SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void						SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
