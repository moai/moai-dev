// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifdef DOXYGEN

//================================================================//
// MOAITransform2D
//================================================================//
/**	@name	MOAITransform2D
	@text	2D transformation hierarchy node.

	@attr	ATTR_X_PIV
	@attr	ATTR_Y_PIV
	
	@attr	ATTR_X_LOC
	@attr	ATTR_Y_LOC
	
	@attr	ATTR_Z_ROT
	
	@attr	ATTR_X_SCL
	@attr	ATTR_Y_SCL
	
	@attr	INHERIT_LOC
	@attr	INHERIT_TRANSFORM
*/
class MOAITransform2D :
	public MOAITransformBase {
protected:

	//----------------------------------------------------------------//
	static int	_addLoc			( lua_State* L );
	static int	_addPiv			( lua_State* L );
	static int	_addRot			( lua_State* L );
	static int	_addScl			( lua_State* L );
	static int	_getLoc			( lua_State* L );
	static int	_getPiv			( lua_State* L );
	static int	_getRot			( lua_State* L );
	static int	_getScl			( lua_State* L );
	static int	_modelToWorld	( lua_State* L );
	static int	_move			( lua_State* L );
	static int	_moveLoc		( lua_State* L );
	static int	_movePiv		( lua_State* L );
	static int	_moveRot		( lua_State* L );
	static int	_moveScl		( lua_State* L );
	static int	_seek			( lua_State* L );
	static int	_seekLoc		( lua_State* L );
	static int	_seekPiv		( lua_State* L );
	static int	_seekRot		( lua_State* L );
	static int	_seekScl		( lua_State* L );
	static int	_setLoc			( lua_State* L );
	static int	_setParent		( lua_State* L );
	static int	_setPiv			( lua_State* L );
	static int	_setRot			( lua_State* L );
	static int	_setScl			( lua_State* L );
	static int	_worldToModel	( lua_State* L );
};

#endif