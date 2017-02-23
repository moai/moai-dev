// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVIEWLAYER_H
#define	MOAIVIEWLAYER_H

#include <moai-sim/MOAIFrameBuffer.h>
#include <moai-sim/MOAIGraphicsProp.h>
#include <moai-sim/MOAIPartitionHolder.h>
#include <moai-sim/MOAILayer.h>
#include <moai-sim/MOAIViewport.h>

class MOAICamera;

//================================================================//
// MOAIViewLayer
//================================================================//
// TODO: doxygen
class MOAIViewLayer :
	public virtual MOAIGraphicsProp,
	public virtual MOAILayer {
protected:

	MOAILuaSharedPtr < MOAICamera >			mCamera;
	MOAILuaSharedPtr < MOAICamera >			mDebugCamera;
	MOAILuaSharedPtr < MOAIViewport >		mViewport;

	ZLVec3D			mParallax;
	bool			mShowDebugLines;

	//----------------------------------------------------------------//
	static int		_getCamera				( lua_State* L );
	static int		_getFitting				( lua_State* L );
	static int		_getFitting3D			( lua_State* L );
	static int		_getViewport			( lua_State* L );
	static int		_setDebugCamera			( lua_State* L );
	static int		_setCamera				( lua_State* L );
	static int		_setParallax			( lua_State* L );
	static int		_setViewport			( lua_State* L );
	static int		_showDebugLines			( lua_State* L );
	static int		_wndToWorld				( lua_State* L );
	static int		_wndToWorldRay			( lua_State* L );
	static int		_worldToWnd				( lua_State* L );

	//----------------------------------------------------------------//
	void					MOAIDrawable_Draw					( int subPrimID );
	ZLBounds				MOAIPartitionHull_GetModelBounds	();
	virtual void			MOAIViewLayer_Draw					() = 0;

public:

	DECL_LUA_ABSTRACT ( MOAILayer )

	//----------------------------------------------------------------//
	float					GetFitting				( ZLRect& worldRect, float hPad, float vPad );
	ZLMatrix4x4				GetWndToWorldMtx		() const;
	ZLMatrix4x4				GetWorldToWndMtx		() const;
							MOAIViewLayer			();
							~MOAIViewLayer			();
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
