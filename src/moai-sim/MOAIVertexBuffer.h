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

	u32		mVAO;
	u32		mVBO;
};

//================================================================//
// MOAIVertexBuffer
//================================================================//
/**	@name	MOAIVertexBuffer
	@text	Vertex buffer class.
*/
class MOAIVertexBuffer :
	public virtual MOAILuaObject,
	public MOAIGfxResource {
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
	static int		_writeFloat				( lua_State* L );
	static int		_writeInt8				( lua_State* L );
	static int		_writeInt16				( lua_State* L );
	static int		_writeInt32				( lua_State* L );

	//----------------------------------------------------------------//
	bool			IsRenewable				();
	bool			IsValid					();
	void			OnBind					();
	void			OnClear					();
	void			OnCreate				();
	void			OnDestroy				();
	void			OnInvalidate			();
	void			OnLoad					();

public:
	
	DECL_LUA_FACTORY ( MOAIVertexBuffer )
	
	GET_SET ( u32, DefaultFormat, mDefaultFormat )
	
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
	void						Reserve					( u32 size, u32 gpuBuffers );
	void						Unbind					();
};

#endif
