// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICAMERA_H
#define	MOAICAMERA_H

#include <moai-sim/MOAITransform.h>

class MOAIViewport;

//================================================================//
// MOAICamera
//================================================================//
/**	@lua	MOAICamera
	@text	Perspective or orthographic camera.
*/
class MOAICamera :
	public virtual ZLCamera,
	public virtual MOAITransform {
private:

	//----------------------------------------------------------------//
	static int		_getFarPlane			( lua_State* L );
	static int		_getFieldOfView			( lua_State* L );
	static int		_getFloorMove			( lua_State* L );
	static int		_getFocalLength			( lua_State* L );
	static int		_getNearPlane			( lua_State* L );
	static int		_getViewVector			( lua_State* L );
	static int		_lookAt					( lua_State* L );
	static int		_moveFieldOfView		( lua_State* L );
	static int		_seekFieldOfView		( lua_State* L );
	static int		_setFarPlane			( lua_State* L );
	static int		_setFieldOfView			( lua_State* L );
	static int		_setNearPlane			( lua_State* L );
	static int		_setOrtho				( lua_State* L );
	static int		_setType				( lua_State* L );
	
	//----------------------------------------------------------------//
	void			LookAt					( float x, float y, float z );
	
	//----------------------------------------------------------------//
	void			MOAILuaObject_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );
	bool			MOAINode_ApplyAttrOp				( ZLAttrID attrID, ZLAttribute& attr, u32 op );
	void			MOAINode_Update						();
	
public:
	
	enum {
		DEBUG_DRAW_FRAME,
		DEBUG_DRAW_RETICLE,
		TOTAL_DEBUG_LINE_STYLES,
	};
	
	DECL_LUA_FACTORY ( MOAICamera )
	DECL_ATTR_HELPER ( MOAICamera )

	enum {
		ATTR_FOV,		
		TOTAL_ATTR,
	};
	
	//----------------------------------------------------------------//
	void			DrawDebug 				();
					MOAICamera				();
					~MOAICamera				();
};

#endif
