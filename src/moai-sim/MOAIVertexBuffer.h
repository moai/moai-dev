// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXBUFFER_H
#define	MOAIVERTEXBUFFER_H

#include <moai-sim/MOAIGfxResource.h>

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
/**	@name	MOAIVertexBuffer
	@text	Vertex buffer class.
*/
class MOAIVertexBuffer :
	public MOAIGfxResource,
	public MOAIStream {
private:

	static const u32 NULL_FORMAT = 0xffffffff;

	u32										mDefaultFormat;
	MOAILuaSharedPtr < MOAIVertexFormat >	mFormat;
	u32										mVertexCount;

	ZLLeanArray < u8 >			mBuffer;
	ZLByteStream				mStream;
	ZLBox						mBounds;
	
	ZLLeanArray < MOAIVbo >		mVBOs;
	u32							mCurrentVBO;
	u32							mHint;
	
	bool						mIsDirty;
	bool						mIsValid;
	bool						mUseVBOs;
	
	//----------------------------------------------------------------//
	static int		_bless					( lua_State* L );
	static int		_release				( lua_State* L );
	static int		_reserve				( lua_State* L );
	static int		_reserveVBOs			( lua_State* L );
	static int		_reserveVerts			( lua_State* L );
	static int		_reset					( lua_State* L );
	static int		_setFormat				( lua_State* L );
	static int		_writeColor32			( lua_State* L );

	//----------------------------------------------------------------//
	bool			IsRenewable				();
	bool			IsValid					();
	void			OnBind					();
	void			OnClear					();
	void			OnCreate				();
	void			OnDestroy				();
	void			OnInvalidate			();
	void			OnLoad					();
	void			OnUnbind				();

public:
	
	DECL_LUA_FACTORY ( MOAIVertexBuffer )
	
	GET ( const ZLBox&, Bounds, mBounds )
	GET ( ZLStream&, Stream, mStream )
	GET ( u32, VertexCount, mVertexCount )
	
	//----------------------------------------------------------------//
	void						Bless					();
	void						Clear					();
	const MOAIVertexFormat*		GetFormat				();
								MOAIVertexBuffer		();
								~MOAIVertexBuffer		();
	void						RegisterLuaClass		( MOAILuaState& state );
	void						RegisterLuaFuncs		( MOAILuaState& state );
	void						Reserve					( u32 size );
	void						ReserveVBOs				( u32 gpuBuffers );
	void						SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void						SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
