// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXBUFFER_H
#define	MOAIVERTEXBUFFER_H

#include <moaicore/MOAILua.h>

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

	MOAILuaSharedPtr < MOAIVertexFormat > mFormat;

	USLeanArray < u8 >	mBuffer;
	USByteStream		mStream;
	USBox				mBounds;
	
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

	//----------------------------------------------------------------//
	void			Clear					();
	
public:
	
	DECL_LUA_FACTORY ( MOAIVertexBuffer )
	
	GET ( const USBox&, Bounds, mBounds )
	GET ( MOAIVertexFormat*, Format, mFormat )
	
	//----------------------------------------------------------------//
	bool			Bind					();
	u32				GetVertexCount			();
	bool			IsValid					();
					MOAIVertexBuffer		();
					~MOAIVertexBuffer		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			Reserve					( u32 size );
	void			Unbind					();
};

#endif
