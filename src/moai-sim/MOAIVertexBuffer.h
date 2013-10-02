// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXBUFFER_H
#define	MOAIVERTEXBUFFER_H

class MOAIVertexFormat;

//================================================================//
// MOAIVertexBuffer
//================================================================//
/**	@name	MOAIVertexBuffer
	@text	Vertex buffer class.
*/
class MOAIVertexBuffer :
	public MOAILuaObject {
private:

	static const u32 NULL_FORMAT = 0xffffffff;

	u32										mDefaultFormat;
	MOAILuaSharedPtr < MOAIVertexFormat >	mFormat;
	u32										mVertexCount;

	ZLLeanArray < u8 >	mBuffer;
	ZLByteStream		mStream;
	ZLBox				mBounds;
	
	//----------------------------------------------------------------//
	static int		_bless					( lua_State* L );
	static int		_release				( lua_State* L );
	static int		_reserve				( lua_State* L );
	static int		_reserveVerts			( lua_State* L );
	static int		_reset					( lua_State* L );
	static int		_setFormat				( lua_State* L );
	static int		_writeColor32			( lua_State* L );
	static int		_writeFloat				( lua_State* L );
	static int		_writeInt8				( lua_State* L );
	static int		_writeInt16				( lua_State* L );
	static int		_writeInt32				( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIVertexBuffer )
	
	GET_SET ( u32, DefaultFormat, mDefaultFormat )
	
	GET ( const ZLBox&, Bounds, mBounds )
	GET ( ZLStream&, Stream, mStream )
	GET ( u32, VertexCount, mVertexCount )
	
	//----------------------------------------------------------------//
	bool						Bind					();
	void						Bless					();
	void						Clear					();
	const MOAIVertexFormat*		GetFormat				();
	bool						IsValid					();
								MOAIVertexBuffer		();
								~MOAIVertexBuffer		();
	void						RegisterLuaClass		( MOAILuaState& state );
	void						RegisterLuaFuncs		( MOAILuaState& state );
	void						Reserve					( u32 size );
	void						Unbind					();
};

#endif
