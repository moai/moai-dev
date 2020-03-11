// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTVIEWLAYER_H
#define	MOAIABSTRACTVIEWLAYER_H

#include <moai-sim/MOAIAbstractLayer.h>
#include <moai-sim/MOAIGraphicsProp.h>
#include <moai-sim/MOAIPartitionHolder.h>
#include <moai-sim/MOAIViewport.h>

class MOAICamera;

//================================================================//
// MOAIAbstractViewLayer
//================================================================//
// TODO: doxygen
class MOAIAbstractViewLayer :
	public virtual MOAIGraphicsProp,
	public virtual MOAIAbstractLayer {
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
	void					_RegisterLuaClass					( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_RegisterLuaFuncs					( RTTIVisitorHistory& history, MOAILuaState& state );
	ZLBounds				MOAIAbstractProp_GetModelBounds		();
	virtual void			MOAIAbstractViewLayer_Draw			() = 0;
	void					MOAIDrawable_Draw					( int subPrimID );
	void					MOAIDrawable_DrawDebug				( int subPrimID );

public:

	DECL_LUA_ABSTRACT ( MOAIAbstractLayer )

	//----------------------------------------------------------------//
	float					GetFitting					( ZLRect& worldRect, float hPad, float vPad );
	ZLMatrix4x4				GetWndToWorldMtx			() const;
	ZLMatrix4x4				GetWorldToWndMtx			() const;
							MOAIAbstractViewLayer		();
							~MOAIAbstractViewLayer		();
};

#endif
