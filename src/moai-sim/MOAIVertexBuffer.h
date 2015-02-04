// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXBUFFER_H
#define	MOAIVERTEXBUFFER_H

#include <moai-sim/MOAIGfxBuffer.h>
#include <moai-sim/MOAIGfxResource.h>
#include <moai-util/MOAIStream.h>
class MOAIVertexFormat;

//================================================================//
// MOAIVbo
//================================================================//
class MOAIVbo {
public:

	u32		mVAO; // vertex array
	u32		mVBO; // vertex buffer
};

//================================================================//
// MOAIVertexBuffer
//================================================================//
/**	@lua	MOAIVertexBuffer
	@text	Vertex buffer class.
*/
class MOAIVertexBuffer :
	public MOAIGfxBuffer {
private:

	static const u32 NULL_FORMAT = 0xffffffff;

	u32										mDefaultFormat;
	MOAILuaSharedPtr < MOAIVertexFormat >	mFormat;
	
	//----------------------------------------------------------------//
	static int		_bless					( lua_State* L );
	static int		_reserveVerts			( lua_State* L );
	static int		_setFormat				( lua_State* L );
	static int		_writeColor32			( lua_State* L );

	//----------------------------------------------------------------//
//	bool			OnCPUCreate				();
//	void			OnCPUDestroy			();
//	void			OnGPUBind				();
//	bool			OnGPUCreate				();
//	void			OnGPUDestroy			();
//	void			OnGPULost				();
//	void			OnGPUUnbind				();
	
public:
	
	DECL_LUA_FACTORY ( MOAIVertexBuffer )
	
	GET ( u32, VertexCount, mTotalElements )
	GET ( u32, TotalElements, mTotalElements )
	
	HAS ( Bounds, mHasBounds, true )
	GET ( const ZLBox&, Bounds, mBounds )
	
	//----------------------------------------------------------------//
	void						Bless					();
	
	size_t						GetSize					();
	const MOAIVertexFormat*		GetVertexFormat			() const;
								MOAIVertexBuffer		();
								~MOAIVertexBuffer		();
	void						RegisterLuaClass		( MOAILuaState& state );
	void						RegisterLuaFuncs		( MOAILuaState& state );
	void						SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void						SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
