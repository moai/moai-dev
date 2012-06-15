// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICAMERA_H
#define	MOAICAMERA_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAITransform.h>

class MOAIViewport;

//================================================================//
// MOAICamera
//================================================================//
/**	@name	MOAICamera
	@text	Perspective or orthographic camera.
*/
class MOAICamera :
	public virtual MOAITransform {
private:

	float		mFieldOfView;
	float		mNearPlane;
	float		mFarPlane;

	bool		mOrtho;

	//----------------------------------------------------------------//
	static int		_getFarPlane		( lua_State* L );
	static int		_getFieldOfView		( lua_State* L );
	static int		_getFocalLength		( lua_State* L );
	static int		_getNearPlane		( lua_State* L );
	static int		_setFarPlane		( lua_State* L );
	static int		_setFieldOfView		( lua_State* L );
	static int		_setNearPlane		( lua_State* L );
	static int		_setOrtho			( lua_State* L );
	
public:
	
	DECL_LUA_FACTORY ( MOAICamera )
	
	//----------------------------------------------------------------//
	USMatrix4x4		GetBillboardMtx		() const;
	USMatrix4x4		GetProjMtx			( const MOAIViewport& viewport ) const;
	USMatrix4x4		GetProjMtxInv		( const MOAIViewport& viewport ) const;
	USMatrix4x4		GetViewMtx			() const;
					MOAICamera			();
					~MOAICamera			();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif
