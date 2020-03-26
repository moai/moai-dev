// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAWINGAPIOBJECT_H
#define	MOAIDRAWINGAPIOBJECT_H

#include <moai-gfx/MOAIAbstractDrawingAPI.h>
#include <moai-gfx/MOAIGfxConsts.h>

//================================================================//
// MOAIAbstractDrawingAPIObject
//================================================================//
// TODO: doxygen
class MOAIAbstractDrawingAPIObject :
	public virtual MOAIAbstractDrawingAPI,
	public virtual MOAILuaObject {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	//----------------------------------------------------------------//
	static int			_bindVectorDrawing			( lua_State* L );
	static int			_call						( lua_State* L );
	static int			_callFromShader				( lua_State* L );
	static int			_clearSurface				( lua_State* L );
	static int			_drawBezierCurve			( lua_State* L );
	static int			_drawBoxOutline				( lua_State* L );
	static int			_drawCircle					( lua_State* L );
	static int			_drawCircleSpokes			( lua_State* L );
	static int			_drawElements				( lua_State* L );
	static int			_drawEllipse				( lua_State* L );
	static int			_drawEllipseSpokes			( lua_State* L );
	static int			_drawGrid					( lua_State* L );
	static int			_drawLine					( lua_State* L );
	static int			_drawPoints					( lua_State* L );
	static int			_drawRay					( lua_State* L );
	static int			_drawRect					( lua_State* L );
	static int			_fillCircle					( lua_State* L );
	static int			_fillEllipse				( lua_State* L );
	static int			_fillFan					( lua_State* L );
	static int			_fillRect					( lua_State* L );
	static int			_fillRoundedRect			( lua_State* L );
	static int			_drawAxis2D					( lua_State* L );
	static int			_popGfxState				( lua_State* L );
	static int			_pushGfxState				( lua_State* L );
	static int			_setBlendMode				( lua_State* L );
	static int			_setCullFunc				( lua_State* L );
	static int			_setDepthFunc				( lua_State* L );
	static int			_setDepthMask				( lua_State* L );
	static int			_setFrameBuffer				( lua_State* L );
	static int			_setIndexBuffer				( lua_State* L );
	static int			_setMatrix					( lua_State* L );
	static int			_setPenColor				( lua_State* L );
	static int			_setPenWidth				( lua_State* L );
	static int			_setScissorRect				( lua_State* L );
	static int			_setShader					( lua_State* L );
	static int			_setTexture					( lua_State* L );
 	static int			_setUniform					( lua_State* L );
	static int			_setVertexArray				( lua_State* L );
	static int			_setVertexBuffer			( lua_State* L );
	static int			_setVertexFormat			( lua_State* L );
	static int			_setViewRect				( lua_State* L );
	static int			_strokeRoundedRect			( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	//----------------------------------------------------------------//
						MOAIAbstractDrawingAPIObject		();
	virtual				~MOAIAbstractDrawingAPIObject		();
};

#endif
