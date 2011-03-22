// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICAMERA2D_H
#define	MOAICAMERA2D_H

#include <moaicore/MOAITransform2D.h>

//================================================================//
// MOAICamera2D
//================================================================//
/**	@brief 2D camera manipulation class.
*/
class MOAICamera2D :
	public MOAITransform2D {
private:


	//----------------------------------------------------------------//
	//static int	_addZoom			( lua_State* L );
	//static int	_getZoom			( lua_State* L );
	//static int	_moveZoom			( lua_State* L );
	//static int	_seekZoom			( lua_State* L );
	//static int	_setZoom			( lua_State* L );

public:

	DECL_LUA_DATA ( MOAICamera2D )
	
	//----------------------------------------------------------------//
					MOAICamera2D		();
					~MOAICamera2D		();
	void			RegisterLuaClass	( USLuaState& state );
	void			RegisterLuaFuncs	( USLuaState& state );
};

#endif
