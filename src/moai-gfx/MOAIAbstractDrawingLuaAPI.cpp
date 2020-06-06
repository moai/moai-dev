// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractChildTransform.h>
#include <moai-gfx/MOAIAbstractDrawingLuaAPI.h>
#include <moai-gfx/MOAIBlendMode.h>
#include <moai-gfx/MOAICamera.h>
#include <moai-gfx/MOAIFrameBuffer.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIIndexBuffer.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>
#include <moai-gfx/MOAIVertexArray.h>
#include <moai-gfx/MOAIVertexBuffer.h>
#include <moai-gfx/MOAIVertexFormat.h>
#include <moai-gfx/MOAIViewport.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractDrawingLuaAPI::_bindVectorDrawing ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "" )
	
	self->BindVectorDrawing ();
	return 0;
}

//----------------------------------------------------------------//
int MOAIAbstractDrawingLuaAPI::_clearSurface ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "U" )
	
	self->ClearSurface ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractDrawingLuaAPI::_drawAnimCurve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "U" )

	MOAIAnimCurve* curve	= state.GetLuaObject < MOAIAnimCurve >( 2, true );
	u32 resolution			= state.GetValue < u32 >( 3, 1 );

	if ( curve ) {
		self->DrawAnimCurve ( *curve, resolution );
	}
	return 0;
}


//----------------------------------------------------------------//
/**	@lua	drawAxis2D
	@text	Draw a ray.
	
	@in		number x
	@in		number y
	@in		number dx
	@in		number dy
	@out	nil
*/
int MOAIAbstractDrawingLuaAPI::_drawAxis2D ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "" )
	
	float x		= state.GetValue < float >( 2, 0.0f );
	float y		= state.GetValue < float >( 3, 0.0f );
	float dx	= state.GetValue < float >( 4, 0.0f );
	float dy	= state.GetValue < float >( 5, 0.0f );

	self->DrawAxis2D ( x, y, dx, dy );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	drawBezierCurve
	@text	Draws a bezier curve.
	
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@in		number x2
	@in		number y2
	@in		number x3
	@in		number y3
	@out	nil
*/
int MOAIAbstractDrawingLuaAPI::_drawBezierCurve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "" )
	
	ZLCubicBezier2D bezier;
	
	bezier.mP0.mX = state.GetValue < float >( 2, 0.0f );
	bezier.mP0.mY = state.GetValue < float >( 3, 0.0f );
	
	bezier.mP1.mX = state.GetValue < float >( 4, 0.0f );
	bezier.mP1.mY = state.GetValue < float >( 5, 0.0f );
	
	bezier.mP2.mX = state.GetValue < float >( 6, 0.0f );
	bezier.mP2.mY = state.GetValue < float >( 7, 0.0f );
	
	bezier.mP3.mX = state.GetValue < float >( 8, 0.0f );
	bezier.mP3.mY = state.GetValue < float >( 9, 0.0f );
	
	self->DrawBezierCurve ( bezier );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	drawBoxOutline
	@text	Draw a box outline.
	
	@in		number x0
	@in		number y0
	@in		number z0
	@in		number x1
	@in		number y1
	@in		number z1
	@out	nil
*/
int MOAIAbstractDrawingLuaAPI::_drawBoxOutline ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "" )

	ZLBox box;
	box.mMin.mX = state.GetValue < float >( 2, 0.0f );
	box.mMin.mY = state.GetValue < float >( 3, 0.0f );
	box.mMin.mZ = state.GetValue < float >( 4, 0.0f );
	box.mMax.mX = state.GetValue < float >( 5, box.mMin.mX );
	box.mMax.mY = state.GetValue < float >( 6, box.mMin.mY );
	box.mMax.mZ = state.GetValue < float >( 7, box.mMin.mZ );
	
	self->DrawBoxOutline ( box );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	drawCircle
	@text	Draw a circle.
	
	@in		number x
	@in		number y
	@in		number r
	@in		number steps
	@out	nil
*/
int MOAIAbstractDrawingLuaAPI::_drawCircle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "" )
	
	float x0	= state.GetValue < float >( 2, 0.0f );
	float y0	= state.GetValue < float >( 3, 0.0f );
	float r		= state.GetValue < float >( 4, 0.0f );
	u32 steps	= state.GetValue < u32 >( 5, DEFAULT_ELLIPSE_STEPS );

	self->DrawEllipseOutline ( x0, y0, r, r, steps );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	drawCircleSpokes
	@text	Draw the spokes of a circle.
	
	@in		number x
	@in		number y
	@in		number r
	@in		number steps
	@out	nil
*/
int MOAIAbstractDrawingLuaAPI::_drawCircleSpokes ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "" )
	
	float x0	= state.GetValue < float >( 2, 0.0f );
	float y0	= state.GetValue < float >( 3, 0.0f );
	float r		= state.GetValue < float >( 4, 0.0f );
	u32 steps	= state.GetValue < u32 >( 5, DEFAULT_ELLIPSE_STEPS );

	self->DrawEllipseSpokes ( x0, y0, r, r, steps );
	return 0;
}

//----------------------------------------------------------------//
// TODO: implement
int MOAIAbstractDrawingLuaAPI::_drawElements ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "" )
	assert ( false );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	drawEllipse
	@text	Draw an ellipse.
	
	@in		number x
	@in		number y
	@in		number xRad
	@in		number yRad
	@in		number steps
	@out	nil
*/
int MOAIAbstractDrawingLuaAPI::_drawEllipse ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "" )
	
	float x		= state.GetValue < float >( 2, 0.0f );
	float y		= state.GetValue < float >( 3, 0.0f );
	float xRad	= state.GetValue < float >( 4, 0.0f );
	float yRad	= state.GetValue < float >( 5, 0.0f );
	
	u32 steps = state.GetValue < u32 >( 6, DEFAULT_ELLIPSE_STEPS );

	self->DrawEllipseOutline ( x, y, xRad, yRad, steps );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	drawCircleSpokes
	@text	Draw the spokes of an ellipse.
	
	@in		number x
	@in		number y
	@in		number xRad
	@in		number yRad
	@in		number steps
	@out	nil
*/
int MOAIAbstractDrawingLuaAPI::_drawEllipseSpokes ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "" )
	
	float x		= state.GetValue < float >( 2, 0.0f );
	float y		= state.GetValue < float >( 3, 0.0f );
	float xRad	= state.GetValue < float >( 4, 0.0f );
	float yRad	= state.GetValue < float >( 5, 0.0f );

	u32 steps = state.GetValue < u32 >( 6, DEFAULT_ELLIPSE_STEPS );

	self->DrawEllipseSpokes ( x, y, xRad, yRad, steps );
	return 0;
}

//----------------------------------------------------------------//
// TODO: implement
int MOAIAbstractDrawingLuaAPI::_drawGrid ( lua_State* L ) {
	UNUSED ( L );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	drawLine
	@text	Draw a line.
	
	@in		... vertices		List of vertices (x, y) or an array of vertices
								{ x0, y0, x1, y1, ... , xn, yn }
	@out	nil
*/
int MOAIAbstractDrawingLuaAPI::_drawLine ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "" )

	if ( lua_istable ( L, -1 ) ) {
		self->DrawLuaArray ( L, MOAIGfxTopologyEnum::LINE_STRIP );
	}
	else {
		self->DrawLuaParams ( L, MOAIGfxTopologyEnum::LINE_STRIP );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	drawPoints
	@text	Draw a list of points.
	
	@in		... vertices		List of vertices (x, y) or an array of vertices
								{ x0, y0, x1, y1, ... , xn, yn }
	@out	nil
*/
int MOAIAbstractDrawingLuaAPI::_drawPoints ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "" )
	
	if ( lua_istable ( L, -1 ) ) {
		self->DrawLuaArray ( L, MOAIGfxTopologyEnum::POINT_LIST );
	}
	else {
		self->DrawLuaParams ( L, MOAIGfxTopologyEnum::POINT_LIST );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: implement
int MOAIAbstractDrawingLuaAPI::_drawRay ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "" )
	assert ( false );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	drawRect
	@text	Draw a rectangle.
	
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@out	nil
*/
int MOAIAbstractDrawingLuaAPI::_drawRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "" )
	
	float x0 = state.GetValue < float >( 2, 0.0f );
	float y0 = state.GetValue < float >( 3, 0.0f );
	float x1 = state.GetValue < float >( 4, 0.0f );
	float y1 = state.GetValue < float >( 5, 0.0f );

	self->DrawRectOutline ( x0, y0, x1, y1 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	fillCircle
	@text	Draw a filled circle.
	
	@in		number x
	@in		number y
	@in		number r
	@in		number steps
	@out	nil
*/
int MOAIAbstractDrawingLuaAPI::_fillCircle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "" )
	
	float x0	= state.GetValue < float >( 2, 0.0f );
	float y0	= state.GetValue < float >( 3, 0.0f );
	float r		= state.GetValue < float >( 4, 0.0f );
	u32 steps	= state.GetValue < u32 >( 5, DEFAULT_ELLIPSE_STEPS );

	self->DrawEllipseFill ( x0, y0, r, r, steps );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	fillEllipse
	@text	Draw a filled ellipse.
	
	@in		number x
	@in		number y
	@in		number xRad
	@in		number yRad
	@in		number steps
	@out	nil
*/
int MOAIAbstractDrawingLuaAPI::_fillEllipse ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "" )
	
	float x		= state.GetValue < float >( 2, 0.0f );
	float y		= state.GetValue < float >( 3, 0.0f );
	float xRad	= state.GetValue < float >( 4, 0.0f );
	float yRad	= state.GetValue < float >( 5, 0.0f );
	
	u32 steps = state.GetValue < u32 >( 6, DEFAULT_ELLIPSE_STEPS );

	self->DrawEllipseFill ( x, y, xRad, yRad, steps );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	fillFan
	@text	Draw a filled fan.
	
	@in		... vertices		List of vertices (x, y) or an array of vertices
								{ x0, y0, x1, y1, ... , xn, yn }
	@out	nil
*/
int MOAIAbstractDrawingLuaAPI::_fillFan ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "" )

	if ( lua_istable ( L, -1 ) ) {
		self->DrawLuaArray( L, MOAIGfxTopologyEnum::TRIANGLE_FAN );
	}
	else {
		self->DrawLuaParams( L, MOAIGfxTopologyEnum::TRIANGLE_FAN );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	fillRect
	@text	Draw a filled rectangle.
	
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@out	nil
*/
int MOAIAbstractDrawingLuaAPI::_fillRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "" )
	
	float x0 = state.GetValue < float >( 2, 0.0f );
	float y0 = state.GetValue < float >( 3, 0.0f );
	float x1 = state.GetValue < float >( 4, 0.0f );
	float y1 = state.GetValue < float >( 5, 0.0f );

	self->DrawRectFill ( x0, y0, x1, y1 );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractDrawingLuaAPI::_fillRoundedRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "" )
	
	float x0 = state.GetValue < float >( 2, 0.0f );
	float y0 = state.GetValue < float >( 3, 0.0f );
	float x1 = state.GetValue < float >( 4, 0.0f );
	float y1 = state.GetValue < float >( 5, 0.0f );
	
	float xRad		= state.GetValue < float >( 6, 0.0f );
	float yRad		= state.GetValue < float >( 7, xRad );
	u32 steps		= state.GetValue < u32 >( 8, DEFAULT_ELLIPSE_STEPS );

	self->DrawRoundedRectFill ( x0, y0, x1, y1, xRad, yRad, steps );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractDrawingLuaAPI::_strokeRoundedRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "" )
	
	float x0 = state.GetValue < float >( 2, 0.0f );
	float y0 = state.GetValue < float >( 3, 0.0f );
	float x1 = state.GetValue < float >( 4, 0.0f );
	float y1 = state.GetValue < float >( 5, 0.0f );
	
	float stroke	= state.GetValue < float >( 6, 0.0f );
	float offset	= state.GetValue < float >( 7, 0.0f );
	
	float xRad		= state.GetValue < float >( 8, 0.0f );
	float yRad		= state.GetValue < float >( 9, xRad );
	u32 steps		= state.GetValue < u32 >( 10, DEFAULT_ELLIPSE_STEPS );

	self->DrawRoundedRectStroke ( x0, y0, x1, y1, xRad, yRad, steps, stroke, offset );
	return 0;
}

//----------------------------------------------------------------//
int MOAIAbstractDrawingLuaAPI::_popGfxState ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "U" )
	
	self->PopGfxState ();
	return 0;
}

//----------------------------------------------------------------//
int MOAIAbstractDrawingLuaAPI::_pushGfxState ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "U" )
	
	self->PushGfxState ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractDrawingLuaAPI::_setBlendMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "U" )
	
	MOAIBlendFuncEnum::_ equation			= ( MOAIBlendFuncEnum::_ )state.GetValue < u32 >( 2, MOAIBlendFuncEnum::ADD );
	MOAIBlendFactorEnum::_ srcFactor		= ( MOAIBlendFactorEnum::_ )state.GetValue < u32 >( 3, MOAIBlendFactorEnum::ONE );
	MOAIBlendFactorEnum::_ dstFactor		= ( MOAIBlendFactorEnum::_ )state.GetValue < u32 >( 4, MOAIBlendFactorEnum::ONE_MINUS_SRC_ALPHA );

	MOAIBlendMode blendMode;
	blendMode.SetBlend ( equation, srcFactor, dstFactor );
	self->SetBlendMode ( blendMode );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractDrawingLuaAPI::_setCullFunc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "U" )

	self->SetCullFunc (( MOAICullFuncEnum::_ )state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractDrawingLuaAPI::_setDepthFunc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "U" )

	self->SetDepthFunc (( MOAIDepthFuncEnum::_ )state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractDrawingLuaAPI::_setDepthMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "U" )
	
	self->SetDepthMask ( state.GetValue < bool >( 2, false ));
	return 0;
}

//----------------------------------------------------------------//
int MOAIAbstractDrawingLuaAPI::_setFrameBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "U" )

	self->SetFrameBuffer ( state.GetLuaObject < MOAIFrameBuffer >( 2, false ));
	return 0;
}

//----------------------------------------------------------------//
int MOAIAbstractDrawingLuaAPI::_setIndexBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "U" )

	self->SetIndexBuffer ( state.GetLuaObject < MOAIIndexBuffer >( 2, false ));
	return 0;
}

//----------------------------------------------------------------//
int MOAIAbstractDrawingLuaAPI::_setMatrix ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "U" )

	u32 matrixID = state.GetValue < u32 >( 2, MOAIGfxMgr::MODEL_TO_WORLD_MTX );

	if ( MOAIGfxMgr::IsInputMtx ( matrixID )) {
	
		if ( state.IsType ( 2, LUA_TUSERDATA )) {
		
			MOAIAbstractChildTransform* transform = state.GetLuaObject < MOAIAbstractChildTransform >( 2, true );
			if ( transform ) {
				self->SetMatrix ( matrixID, transform->GetLocalToWorldMtx ());
			}
		}
		else {
			int size = state.GetTop () - 3;
			ZLMatrix4x4 mtx = state.GetMatrix ( 3, size );
			self->SetMatrix ( matrixID, mtx );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPenColor

	@in		number r
	@in		number g
	@in		number b
	@opt	number a	Default value is 1.
	@out	nil
*/
int MOAIAbstractDrawingLuaAPI::_setPenColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "U" )

	float r = state.GetValue < float >( 2, 1.0f );
	float g = state.GetValue < float >( 3, 1.0f );
	float b = state.GetValue < float >( 4, 1.0f );
	float a = state.GetValue < float >( 5, 1.0f );

	self->SetPenColor ( ZLColor::PackRGBA ( r, g, b, a ));
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPenWidth

	@in		number width
	@out	nil
*/
int MOAIAbstractDrawingLuaAPI::_setPenWidth ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "U" )

	float width = state.GetValue < float >( 2, 1.0f );
	self->SetPenWidth ( width );
	return 0;
}


//----------------------------------------------------------------//
int MOAIAbstractDrawingLuaAPI::_setScissorRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "U" )

	ZLRect rect ( 0.0, 0.0, 0.0, 0.0 );
	rect = state.GetValue < ZLRect >( 2, rect );
	self->SetScissorRect ( rect );
	return 0;
}

//----------------------------------------------------------------//
int MOAIAbstractDrawingLuaAPI::_setShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "U" )
	
	MOAIShader* shader 		= MOAIGfxMgr::Get ().AffirmShader ( state, 2 );
	self->SetShader ( shader );
	return 0;
}

//----------------------------------------------------------------//
int MOAIAbstractDrawingLuaAPI::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "U" )
	
	MOAITexture* texture	= MOAIGfxMgr::Get ().AffirmTexture ( state, 2 );
	ZLIndex textureUnit		= state.GetValue < ZLIndex >( 3, 0 );

	self->SetTexture ( texture, textureUnit );
	return 0;
}

//----------------------------------------------------------------//
int MOAIAbstractDrawingLuaAPI::_setVertexArray ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "U" )
	
	self->SetVertexArray ( state.GetLuaObject < MOAIVertexArray >( 2, false ));
	return 0;
}

//----------------------------------------------------------------//
int MOAIAbstractDrawingLuaAPI::_setVertexBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "U" )

	self->SetVertexBuffer ( state.GetLuaObject < MOAIVertexBuffer >( 2, false ));
	return 0;
}

//----------------------------------------------------------------//
int MOAIAbstractDrawingLuaAPI::_setVertexFormat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "U" )

	MOAIVertexFormat* vertexFormat = MOAIGfxMgr::Get ().AffirmVertexFormat ( state, 2 );
	self->SetVertexFormat ( vertexFormat );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractDrawingLuaAPI::_setViewProj ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "" )
	
	MOAIViewport* viewport = state.GetLuaObject < MOAIViewport >( 2, false );
	MOAICamera* camera = state.GetLuaObject < MOAICamera >( 3, false );
	
	self->SetViewProj ( viewport, camera );
	return 0;
}

//----------------------------------------------------------------//
int MOAIAbstractDrawingLuaAPI::_setViewRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDrawingLuaAPI, "U" )
	
	if ( state.IsType ( 2, LUA_TUSERDATA )) {
		MOAIViewport* viewport = state.GetLuaObject < MOAIViewport >( 2, true );
		self->SetViewRect ( viewport );
	}
	else {
		ZLRect rect ( 0.0, 0.0, 0.0, 0.0 );
		rect = state.GetValue < ZLRect >( 3, rect );
		self->SetViewRect ( rect );
	}
	return 0;
}

//================================================================//
// MOAIAbstractDrawingLuaAPI
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractDrawingLuaAPI::MOAIAbstractDrawingLuaAPI () {

	RTTI_BEGIN ( MOAIAbstractDrawingLuaAPI )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractDrawingLuaAPI >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractDrawingLuaAPI::~MOAIAbstractDrawingLuaAPI () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractDrawingLuaAPI::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	state.SetField ( -1, "UNIFORM_WIDTH_VEC_2",						( u32 )MOAIUniformDescriptor::UNIFORM_WIDTH_VEC_2 );
	state.SetField ( -1, "UNIFORM_WIDTH_VEC_3",						( u32 )MOAIUniformDescriptor::UNIFORM_WIDTH_VEC_3 );
	state.SetField ( -1, "UNIFORM_WIDTH_VEC_4",						( u32 )MOAIUniformDescriptor::UNIFORM_WIDTH_VEC_4 );
	state.SetField ( -1, "UNIFORM_WIDTH_MATRIX_3X3",				( u32 )MOAIUniformDescriptor::UNIFORM_WIDTH_MATRIX_3X3 );
	state.SetField ( -1, "UNIFORM_WIDTH_MATRIX_4X4",				( u32 )MOAIUniformDescriptor::UNIFORM_WIDTH_MATRIX_4X4 );

	state.SetField ( -1, "GLOBAL_CLIP_TO_DISPLAY_MTX",				( u32 )MOAIGfxMgr::CLIP_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_CLIP_TO_MODEL_MTX",				( u32 )MOAIGfxMgr::CLIP_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_CLIP_TO_VIEW_MTX",					( u32 )MOAIGfxMgr::CLIP_TO_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_CLIP_TO_WINDOW_MTX",				( u32 )MOAIGfxMgr::CLIP_TO_WINDOW_MTX );
	state.SetField ( -1, "GLOBAL_CLIP_TO_WORLD_MTX",				( u32 )MOAIGfxMgr::CLIP_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_DISPLAY_TO_CLIP_MTX",				( u32 )MOAIGfxMgr::DISPLAY_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_DISPLAY_TO_MODEL_MTX",				( u32 )MOAIGfxMgr::DISPLAY_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_DISPLAY_TO_VIEW_MTX",				( u32 )MOAIGfxMgr::DISPLAY_TO_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_DISPLAY_TO_WORLD_MTX",				( u32 )MOAIGfxMgr::DISPLAY_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_MODEL_TO_CLIP_MTX",				( u32 )MOAIGfxMgr::MODEL_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_MODEL_TO_DISPLAY_MTX",				( u32 )MOAIGfxMgr::MODEL_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_MODEL_TO_UV_MTX",					( u32 )MOAIGfxMgr::MODEL_TO_UV_MTX );
	state.SetField ( -1, "GLOBAL_MODEL_TO_VIEW_MTX",				( u32 )MOAIGfxMgr::MODEL_TO_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_MODEL_TO_WORLD_MTX",				( u32 )MOAIGfxMgr::MODEL_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_DISPLAY_MTX",		( u32 )MOAIGfxMgr::NORMAL_CLIP_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_MODEL_MTX",			( u32 )MOAIGfxMgr::NORMAL_CLIP_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_VIEW_MTX",			( u32 )MOAIGfxMgr::NORMAL_CLIP_TO_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_WINDOW_MTX",		( u32 )MOAIGfxMgr::NORMAL_CLIP_TO_WINDOW_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_WORLD_MTX",			( u32 )MOAIGfxMgr::NORMAL_CLIP_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_DISPLAY_TO_CLIP_MTX",		( u32 )MOAIGfxMgr::NORMAL_DISPLAY_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_DISPLAY_TO_MODEL_MTX",		( u32 )MOAIGfxMgr::NORMAL_DISPLAY_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_DISPLAY_TO_VIEW_MTX",		( u32 )MOAIGfxMgr::NORMAL_DISPLAY_TO_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_DISPLAY_TO_WORLD_MTX",		( u32 )MOAIGfxMgr::NORMAL_DISPLAY_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_CLIP_MTX",			( u32 )MOAIGfxMgr::NORMAL_MODEL_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_DISPLAY_MTX",		( u32 )MOAIGfxMgr::NORMAL_MODEL_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_UV_MTX",			( u32 )MOAIGfxMgr::NORMAL_MODEL_TO_UV_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_VIEW_MTX",			( u32 )MOAIGfxMgr::NORMAL_MODEL_TO_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_WORLD_MTX",		( u32 )MOAIGfxMgr::NORMAL_MODEL_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_WORLD_TO_DISPLAY_MTX",		( u32 )MOAIGfxMgr::NORMAL_WORLD_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_WORLD_TO_VIEW_MTX",			( u32 )MOAIGfxMgr::NORMAL_WORLD_TO_VIEW_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_UV_TO_MODEL_MTX",			( u32 )MOAIGfxMgr::NORMAL_UV_TO_MODEL_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_VIEW_TO_CLIP_MTX",			( u32 )MOAIGfxMgr::NORMAL_VIEW_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_VIEW_TO_DISPLAY_MTX",		( u32 )MOAIGfxMgr::NORMAL_VIEW_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_VIEW_TO_MODEL_MTX",			( u32 )MOAIGfxMgr::NORMAL_VIEW_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_VIEW_TO_WORLD_MTX",			( u32 )MOAIGfxMgr::NORMAL_VIEW_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_WINDOW_TO_CLIP_MTX",		( u32 )MOAIGfxMgr::NORMAL_WINDOW_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_WORLD_TO_CLIP_MTX",			( u32 )MOAIGfxMgr::NORMAL_WORLD_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_WORLD_TO_MODEL_MTX",		( u32 )MOAIGfxMgr::NORMAL_WORLD_TO_MODEL_MTX );

	state.SetField ( -1, "GLOBAL_PEN_COLOR",						( u32 )MOAIGfxMgr::PEN_COLOR );

	state.SetField ( -1, "GLOBAL_UV_TO_MODEL_MTX",					( u32 )MOAIGfxMgr::UV_TO_MODEL_MTX );

	state.SetField ( -1, "GLOBAL_VIEW_TO_CLIP_MTX",					( u32 )MOAIGfxMgr::VIEW_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_VIEW_TO_DISPLAY_MTX",				( u32 )MOAIGfxMgr::VIEW_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_VIEW_TO_MODEL_MTX",				( u32 )MOAIGfxMgr::VIEW_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_VIEW_TO_WORLD_MTX",				( u32 )MOAIGfxMgr::VIEW_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_WINDOW_TO_CLIP_MTX",				( u32 )MOAIGfxMgr::WINDOW_TO_CLIP_MTX );

	state.SetField ( -1, "GLOBAL_WORLD_TO_CLIP_MTX",				( u32 )MOAIGfxMgr::WORLD_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_WORLD_TO_DISPLAY_MTX",				( u32 )MOAIGfxMgr::WORLD_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_WORLD_TO_MODEL_MTX",				( u32 )MOAIGfxMgr::WORLD_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_WORLD_TO_VIEW_MTX",				( u32 )MOAIGfxMgr::WORLD_TO_VIEW_MTX );

	state.SetField ( -1, "GLOBAL_VIEW_HEIGHT",						( u32 )MOAIGfxMgr::VIEW_HEIGHT );
	state.SetField ( -1, "GLOBAL_VIEW_WIDTH",						( u32 )MOAIGfxMgr::VIEW_WIDTH );

	state.SetField ( -1, "GLOBAL_VIEW_HALF_HEIGHT",					( u32 )MOAIGfxMgr::VIEW_HALF_HEIGHT );
	state.SetField ( -1, "GLOBAL_VIEW_HALF_WIDTH",					( u32 )MOAIGfxMgr::VIEW_HALF_WIDTH );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingLuaAPI::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "bindVectorDrawing",			_bindVectorDrawing },
		{ "clearSurface",				_clearSurface },
		{ "drawAnimCurve",				_drawAnimCurve },
		{ "drawBezierCurve",			_drawBezierCurve },
		{ "drawBoxOutline",				_drawBoxOutline },
		{ "drawCircle",					_drawCircle },
		{ "drawCircleSpokes",			_drawCircleSpokes },
		{ "drawElements",				_drawElements },
		{ "drawEllipse",				_drawEllipse },
		{ "drawEllipseSpokes",			_drawEllipseSpokes },
		{ "drawGrid",					_drawGrid },
		{ "drawLine",					_drawLine },
		{ "drawPoints",					_drawPoints },
		{ "drawRay",					_drawRay },
		{ "drawRect",					_drawRect },
		{ "fillCircle",					_fillCircle },
		{ "fillEllipse",				_fillEllipse },
		{ "fillFan",					_fillFan },
		{ "fillRect",					_fillRect },
		{ "fillRoundedRect",			_fillRoundedRect },
		{ "drawAxis2D",					_drawAxis2D },
		{ "popGfxState",				_popGfxState },
		{ "pushGfxState",				_pushGfxState },
		{ "setBlendMode",				_setBlendMode },
		{ "setCullFunc",				_setCullFunc },
		{ "setDepthFunc",				_setDepthFunc },
		{ "setDepthMask",				_setDepthMask },
		{ "setFrameBuffer",				_setFrameBuffer },
		{ "setIndexBuffer",				_setIndexBuffer },
		{ "setMatrix",					_setMatrix },
		{ "setPenColor",				_setPenColor },
		{ "setPenWidth",				_setPenWidth },
		{ "setScissorRect",				_setScissorRect },
		{ "setShader",					_setShader },
		{ "setTexture",					_setTexture },
		{ "setVertexArray",				_setVertexArray },
		{ "setVertexBuffer",			_setVertexBuffer },
		{ "setVertexFormat",			_setVertexFormat },
		{ "setViewProj",				_setViewProj },
		{ "setViewRect",				_setViewRect },
		{ "strokeRoundedRect",			_strokeRoundedRect },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
