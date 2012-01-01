// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICAMERA3D_H
#define	MOAICAMERA3D_H

#include <moaicore/MOAICamera.h>

class MOAIViewport;

//================================================================//
// MOAICamera3D
//================================================================//
/**	@name	MOAICamera3D
	@text	Abstract base class for camera objects. Plugs into
			MOAILayer to project geometry being rendered.
*/
class MOAICamera3D :
	public virtual MOAICamera {
private:

	float		mFieldOfView;
	float		mNearPlane;
	float		mFarPlane;

	//----------------------------------------------------------------//
	static int		_getFarPlane		( lua_State* L );
	static int		_getFieldOfView		( lua_State* L );
	static int		_getFocalLength		( lua_State* L );
	static int		_getNearPlane		( lua_State* L );
	static int		_setFarPlane		( lua_State* L );
	static int		_setFieldOfView		( lua_State* L );
	static int		_setNearPlane		( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAICamera3D )
	
	//----------------------------------------------------------------//
	USMatrix4x4		GetProjMtx			( const MOAIViewport& viewport ) const;
	USMatrix4x4		GetProjMtxInv		( const MOAIViewport& viewport ) const;
					MOAICamera3D		();
					~MOAICamera3D		();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif
