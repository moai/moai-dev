// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGfxPrim_H
#define	MOAIGfxPrim_H

#include <moai-sim/MOAIGfxResource.h>

class MOAIGfxBuffer;
class MOAIVertexFormat;

//================================================================//
// MOAIVbo
//================================================================//
//class MOAIVbo {
//public:
//
//	MOAILuaSharedPtr < MOAIGfxBuffer >		mBuffer;
//	MOAILuaSharedPtr < MOAIVertexFormat >	mFormat;
//};

//================================================================//
// MOAIGfxPrim
//================================================================//
// TODO: doxygen
class MOAIGfxPrim :
	public MOAIGfxResource {
private:

	ZLLeanArray < u32 >						mVAOs; // vertex array object to bind all the vbo state
	u32										mCurrentVAO;

	//ZLLeanArray < MOAIVbo >					mVBOs;

	MOAILuaSharedPtr < MOAIGfxBuffer >		mIndexBuffer;

	u32										mVertexCount;
	ZLBox									mBounds;
	
	//----------------------------------------------------------------//
	static int		_getBounds				( lua_State* L );
	static int		_reserveVAOs			( lua_State* L );
	static int		_reserveVBOs			( lua_State* L );
	static int		_setFormat				( lua_State* L );
	static int		_setIndexBuffer			( lua_State* L );
	static int		_setVertexBuffer		( lua_State* L );
	static int		_updateBounds			( lua_State* L );

	//----------------------------------------------------------------//
	u32				GetLoadingPolicy		();
	bool			OnCPUCreate				();
	void			OnCPUDestroy			();
	void			OnGPUBind				();
	bool			OnGPUCreate				();
	void			OnGPUDestroy			();
	void			OnGPULost				();
	void			OnGPUUnbind				();
	
public:
	
	DECL_LUA_FACTORY ( MOAIGfxPrim )
	
	GET ( const ZLBox&, Bounds, mBounds )
	GET ( u32, VertexCount, mVertexCount )
	
	//----------------------------------------------------------------//
	void			Clear					();
					MOAIGfxPrim				();
					~MOAIGfxPrim			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			ReserveVAOs				( u32 size );
	void			ReserveVBOs				( u32 size );
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void			SetFormat				( u32 idx, MOAIVertexFormat* format );
	void			SetIndexBuffer			( MOAIGfxBuffer* buffer );
	void			SetVertexFormat			( u32 idx, MOAIGfxBuffer* buffer );
	void			UpdateBounds			();
};

#endif
