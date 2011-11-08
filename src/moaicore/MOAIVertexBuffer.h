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

	@const	GL_POINTS
	@const	GL_LINES
	@const	GL_TRIANGLES
	@const	GL_LINE_LOOP
	@const	GL_LINE_STRIP
	@const	GL_TRIANGLE_FAN
	@const	GL_TRIANGLE_STRIP
*/
class MOAIVertexBuffer :
	public MOAIObject {
private:

	USLeanArray < u8 > mBuffer;
	USByteStream mStream;

	MOAILuaSharedPtr < MOAIVertexFormat > mFormat;
	u32 mPrimType;
	
	float	mPenWidth;
	float	mPointSize;
	
	USRect	mBounds;
	u32		mPrimSize;
	
	//----------------------------------------------------------------//
	static int		_bless					( lua_State* L );
	static int		_release				( lua_State* L );
	static int		_reserve				( lua_State* L );
	static int		_reserveVerts			( lua_State* L );
	static int		_reset					( lua_State* L );
	static int		_setFormat				( lua_State* L );
	static int		_setPenWidth			( lua_State* L );
	static int		_setPointSize			( lua_State* L );
	static int		_setPrimType			( lua_State* L );
	static int		_writeColor32			( lua_State* L );
	static int		_writeFloat				( lua_State* L );
	static int		_writeInt8				( lua_State* L );
	static int		_writeInt16				( lua_State* L );
	static int		_writeInt32				( lua_State* L );

	//----------------------------------------------------------------//
	void			Clear					();
	
public:
	
	DECL_LUA_FACTORY ( MOAIVertexBuffer )
	
	GET ( const USRect&, Bounds, mBounds )
	GET ( MOAIVertexFormat*, Format, mFormat )
	
	//----------------------------------------------------------------//
	void			Draw					();
	bool			IsValid					();
					MOAIVertexBuffer		();
					~MOAIVertexBuffer		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			Reserve					( u32 size );
	void			SetPrimType				( u32 primType );
};

#endif
