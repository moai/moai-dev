// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXFORMAT_H
#define	MOAIVERTEXFORMAT_H

//================================================================//
// MOAIVertexFormat
//================================================================//
/**	@brief	Vertex format class.
*/
class MOAIVertexFormat :
	public virtual USLuaObject,
	public USVertexFormat {
private:
	
	//----------------------------------------------------------------//
	static int	_clear					( lua_State* L );
	static int	_declareColor			( lua_State* L );
	static int	_declareCoord			( lua_State* L );
	static int	_declareNormal			( lua_State* L );
	static int	_declareUV				( lua_State* L );

	//----------------------------------------------------------------//
	
public:
	
	DECL_LUA_FACTORY ( MOAIVertexFormat )
	
	//----------------------------------------------------------------//
				MOAIVertexFormat		();
				~MOAIVertexFormat		();
	void		RegisterLuaClass		( USLuaState& state );
	void		RegisterLuaFuncs		( USLuaState& state );
	STLString	ToString				();
};

#endif
