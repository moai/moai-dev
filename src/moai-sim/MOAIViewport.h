// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVIEWPORT_H
#define	MOAIVIEWPORT_H

//================================================================//
// MOAIViewport
//================================================================//
/**	@lua	MOAIViewport
	@text	Viewport object.
*/
class MOAIViewport :
	public virtual MOAILuaObject,
	public virtual ZLViewport {
private:

	//----------------------------------------------------------------//
	static int		_getFrame		( lua_State* L );
	static int		_getSize		( lua_State* L );
	static int		_setOffset		( lua_State* L );
	static int		_setRotation	( lua_State* L );
	static int		_setScale		( lua_State* L );
	static int		_setSize		( lua_State* L );

	//----------------------------------------------------------------//
	void			MOAILuaObject_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAIViewport )
	
	friend class MOAICamera;
	
	//----------------------------------------------------------------//
					MOAIViewport			();
					~MOAIViewport			();
};

#endif
