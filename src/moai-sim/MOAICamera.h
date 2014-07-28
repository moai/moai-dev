// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
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
	public virtual MOAITransform {
private:

	float			mFieldOfView;
	float			mNearPlane;
	float			mFarPlane;

	u32				mType;

	//----------------------------------------------------------------//
	static int		_getFarPlane			( lua_State* L );
	static int		_getFieldOfView			( lua_State* L );
	static int		_getFloorMove			( lua_State* L );
	static int		_getFocalLength			( lua_State* L );
	static int		_getNearPlane			( lua_State* L );
	static int		_moveFieldOfView		( lua_State* L );
	static int		_seekFieldOfView		( lua_State* L );
	static int		_setFarPlane			( lua_State* L );
	static int		_setFieldOfView			( lua_State* L );
	static int		_setNearPlane			( lua_State* L );
	static int		_setOrtho				( lua_State* L );
	static int		_setType				( lua_State* L );
	
public:
	
	enum {
		CAMERA_TYPE_3D,
		CAMERA_TYPE_ORTHO,
		CAMERA_TYPE_WINDOW,
	};
	
	DECL_LUA_FACTORY ( MOAICamera )
	DECL_ATTR_HELPER ( MOAICamera )

	enum {
		ATTR_FOV,		
		TOTAL_ATTR,
	};
	
	GET_SET ( float, NearPlane, mNearPlane )
	GET_SET ( float, FarPlane, mFarPlane )
	
	GET_SET ( u32, Type, mType )
	
	//----------------------------------------------------------------//
	bool			ApplyAttrOp				( u32 attrID, MOAIAttrOp& attrOp, u32 op );
	ZLMatrix4x4		GetBillboardMtx			() const;
	float			GetFocalLength			( float width ) const;
	ZLMatrix4x4		GetProjMtx				( const MOAIViewport& viewport ) const;
	ZLMatrix4x4		GetProjMtxInv			( const MOAIViewport& viewport ) const;
	ZLMatrix4x4		GetViewMtx				() const;
	ZLMatrix4x4		GetWndToWorldMtx		( const MOAIViewport& viewport ) const;
	ZLMatrix4x4		GetWorldToWndMtx		( const MOAIViewport& viewport ) const;
					MOAICamera				();
					~MOAICamera				();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
