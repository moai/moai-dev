// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifdef DOXYGEN

//================================================================//
// MOAICamera2D
//================================================================//
/**	@name	MOAICamera2D
	@text	2D camera.
*/
class MOAICamera2D :
	public MOAITransform2D {
protected:

	//----------------------------------------------------------------//
	static int		_getFarPlane		( lua_State* L );
	static int		_getNearPlane		( lua_State* L );
	static int		_setFarPlane		( lua_State* L );
	static int		_setNearPlane		( lua_State* L );
};

#endif