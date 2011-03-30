// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXBUFFER_H
#define	MOAIVERTEXBUFFER_H

#include <moaicore/MOAIVertexFormat.h>

//================================================================//
// MOAIVertexBuffer
//================================================================//
/**	@brief Vertex buffer class.
*/
class MOAIVertexBuffer :
	public USLuaObject {
private:

	USRef < MOAIVertexFormat > mFormat;

	void*	mBuffer;
	u32		mBufferSize;
	
	GLuint	mGLBufferID;
	u32		mHint;
	
	//----------------------------------------------------------------//
	static int	_release				( lua_State* L );
	static int	_reserve				( lua_State* L );
	static int	_setFormat				( lua_State* L );

	//----------------------------------------------------------------//
	void		ClearBuffer				();
	
public:
	
	enum {
		NONE,
		VERTEX_BONE_IDX,
		VERTEX_BONE_WEIGHT,
		VERTEX_COLOR,
		VERTEX_COORD,
		VERTEX_NORMAL,
		VERTEX_TEXCOORD_0,
		VERTEX_TEXCOORD_1,
	};
	
	DECL_LUA_FACTORY ( MOAIVertexBuffer )
	
	//----------------------------------------------------------------//
	bool			Bind					();
					MOAIVertexBuffer		();
					~MOAIVertexBuffer		();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			Release					();
	void			Reserve					( u32 vertexCount );
	void			SetVertexElement		( u32 vtx, u32 elemIdx, float* params );
	STLString		ToString				();
};

#endif
