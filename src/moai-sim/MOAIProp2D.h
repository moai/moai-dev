// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifdef DOXYGEN

//================================================================//
// MOAIProp2D
//================================================================//
/**	@name	MOAIProp2D
	@text	2D prop.
*/
class MOAIProp2D :
	public MOAITransform2D,
	public MOAIColor,
	public MOAIRenderable {
protected:

	//----------------------------------------------------------------//
	static int		_getRect			( lua_State* L );
	static int		_getGrid			( lua_State* L );
	static int		_getIndex			( lua_State* L );
	static int		_getPriority		( lua_State* L );
	static int		_inside				( lua_State* L );
	static int		_setBlendMode		( lua_State* L );
	static int		_setCullMode		( lua_State* L );
	static int		_setDeck			( lua_State* L );
	static int		_setDepthMask		( lua_State* L );
	static int		_setDepthTest		( lua_State* L );
	static int		_setExpandForSort	( lua_State* L );
	static int		_setFrame			( lua_State* L );
	static int		_setGrid			( lua_State* L );
	static int		_setGridScale		( lua_State* L );
	static int		_setIndex			( lua_State* L );
	static int		_setParent			( lua_State* L );
	static int		_setPriority		( lua_State* L );
	static int		_setRemapper		( lua_State* L );
	static int		_setShader			( lua_State* L );
	static int		_setTexture			( lua_State* L );
	static int		_setUVTransform		( lua_State* L );
	static int		_setVisible			( lua_State* L );
};

#endif