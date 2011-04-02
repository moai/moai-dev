// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXBUFFER_H
#define	MOAIVERTEXBUFFER_H

class MOAIVertexFormat;

//================================================================//
// MOAIVertexBuffer
//================================================================//
/**	@brief	Vertex buffer class.

	@const	GL_BYTE
	@const	GL_FIXED
	@const	GL_FLOAT
	@const	GL_SHORT
	@const	GL_UNSIGNED_BYTE
	@const	GL_UNSIGNED_SHORT
*/
class MOAIVertexBuffer :
	public USLuaObject {
private:

	USLeanArray < u8 > mBuffer;
	USByteStream mStream;

	USRef < MOAIVertexFormat > mFormat;
	u32 mPrimType;
	
	USRect mBounds;
	u32 mPrimSize;
	
	//----------------------------------------------------------------//
	static int		_bless					( lua_State* L );
	static int		_release				( lua_State* L );
	static int		_reserve				( lua_State* L );
	static int		_reserveVerts			( lua_State* L );
	static int		_reset					( lua_State* L );
	static int		_seek					( lua_State* L );
	static int		_setFormat				( lua_State* L );
	static int		_setPrimType			( lua_State* L );
	static int		_writeColor				( lua_State* L );
	static int		_writeFloat				( lua_State* L );
	static int		_writeInt8				( lua_State* L );
	static int		_writeInt16				( lua_State* L );
	static int		_writeInt32				( lua_State* L );

	//----------------------------------------------------------------//
	void			ClearBuffer				();
	
public:
	
	DECL_LUA_FACTORY ( MOAIVertexBuffer )
	
	GET ( const USRect&, Bounds, mBounds )
	GET ( MOAIVertexFormat*, Format, mFormat )
	
	//----------------------------------------------------------------//
	void			Draw					();
	bool			IsValid					();
					MOAIVertexBuffer		();
					~MOAIVertexBuffer		();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			Release					();
	void			Reserve					( u32 size );
	void			SetPrimType				( u32 primType );
	STLString		ToString				();
};

#endif
