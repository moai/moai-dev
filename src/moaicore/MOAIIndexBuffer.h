// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIINDEXBUFFER_H
#define	MOAIINDEXBUFFER_H

//================================================================//
// MOAIIndexBuffer
//================================================================//
/**	@brief	Index buffer class. Unused at this time.
*/
class MOAIIndexBuffer :
	public USLuaObject {
private:

	u16*	mBuffer;
	u32		mIndexCount;
	
	GLuint	mGLBufferID;
	u32		mHint;
	
	//----------------------------------------------------------------//
	static int	_release				( lua_State* L );
	static int	_reserveIndices			( lua_State* L );
	static int	_setIndex				( lua_State* L );

	//----------------------------------------------------------------//
	void		ClearBuffer				();
	
public:
	
	DECL_LUA_FACTORY ( MOAIIndexBuffer )
	
	//----------------------------------------------------------------//
	bool		Bind					();
	void		DrawElements			( GLenum mode, GLsizei count, u32 offset );
				MOAIIndexBuffer			();
				~MOAIIndexBuffer		();
	void		RegisterLuaClass		( USLuaState& state );
	void		RegisterLuaFuncs		( USLuaState& state );
	void		Release					();
	void		ReserveIndices			( u32 indexCount );
	void		SetIndex				( u32 idx, u16 value );
	STLString	ToString				();
};

#endif
