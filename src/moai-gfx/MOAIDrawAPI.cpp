// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractChildTransform.h>
#include <moai-gfx/MOAIDrawAPI.h>
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
// MOAIAbstractDrawShapeVertexLineStripWriter2D
//================================================================//
class MOAIAbstractDrawShapeVertexLineStripWriter2D :
	public ZLAbstractVertexWriter2D {
private:

	u32				mVertexCount;
	ZLVec2D			mLastVertex;
	MOAIGfxMgr&		mGfxMgr;

public:

	//----------------------------------------------------------------//
	void Begin () {
	
		this->mVertexCount = 0;
		this->mGfxMgr.BeginPrim ( MOAIGfxTopologyEnum::LINE_STRIP, 1 );
	}
	
	//----------------------------------------------------------------//
	MOAIAbstractDrawShapeVertexLineStripWriter2D () :
		mGfxMgr ( MOAIGfxMgr::Get ()) {
	}

	//----------------------------------------------------------------//
	void WriteVertex ( const ZLVec2D& v ) {
		
		if (( this->mVertexCount > 0 ) && ( this->mGfxMgr.ContinuePrim ( 1 ) == MOAIGfxMgr::CONTINUE_ROLLOVER )) {

			// if we roll over, repeat the last vertex to start a new line strip
			this->mGfxMgr.WriteVtx ( this->mLastVertex.mX, this->mLastVertex.mY );
			this->mGfxMgr.WritePenColor4b ();
		}
		
		this->mGfxMgr.WriteVtx ( v.mX, v.mY );
		this->mGfxMgr.WritePenColor4b ();
		
		this->mLastVertex = v;
	}
	
	//----------------------------------------------------------------//
	void End () {
	
		this->mGfxMgr.EndPrim ();
	}
};

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDrawAPI::_bindVectorPresets ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
	self->BindVectorPresets ();
	return 0;
}

//----------------------------------------------------------------//
int MOAIDrawAPI::_clearSurface ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
	self->ClearSurface ();
	
	lua_pushvalue ( state, 1 );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDrawAPI::_drawAnimCurve ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )

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
int MOAIDrawAPI::_drawAxis2D ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
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
int MOAIDrawAPI::_drawBezierCurve ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
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
int MOAIDrawAPI::_drawBoxOutline ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )

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
int MOAIDrawAPI::_drawCircle ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
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
int MOAIDrawAPI::_drawCircleSpokes ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
	float x0	= state.GetValue < float >( 2, 0.0f );
	float y0	= state.GetValue < float >( 3, 0.0f );
	float r		= state.GetValue < float >( 4, 0.0f );
	u32 steps	= state.GetValue < u32 >( 5, DEFAULT_ELLIPSE_STEPS );

	self->DrawEllipseSpokes ( x0, y0, r, r, steps );
	return 0;
}

//----------------------------------------------------------------//
// TODO: implement
int MOAIDrawAPI::_drawElements ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
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
int MOAIDrawAPI::_drawEllipse ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
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
int MOAIDrawAPI::_drawEllipseSpokes ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
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
int MOAIDrawAPI::_drawGrid ( lua_State* L ) {
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
int MOAIDrawAPI::_drawLine ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )

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
int MOAIDrawAPI::_drawPoints ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
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
int MOAIDrawAPI::_drawRay ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
	float x		= state.GetValue < float >( 2, 0.0f );
	float y		= state.GetValue < float >( 3, 0.0f );
	float dx	= state.GetValue < float >( 4, 0.0f );
	float dy	= state.GetValue < float >( 5, 0.0f );

	self->DrawRay ( x, y, dx, dy );
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
int MOAIDrawAPI::_drawRect ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
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
int MOAIDrawAPI::_fillCircle ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
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
int MOAIDrawAPI::_fillEllipse ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
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
int MOAIDrawAPI::_fillFan ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )

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
int MOAIDrawAPI::_fillRect ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
	float x0 = state.GetValue < float >( 2, 0.0f );
	float y0 = state.GetValue < float >( 3, 0.0f );
	float x1 = state.GetValue < float >( 4, 0.0f );
	float y1 = state.GetValue < float >( 5, 0.0f );

	self->DrawRectFill ( x0, y0, x1, y1 );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDrawAPI::_fillRoundedRect ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
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
int MOAIDrawAPI::_popGfxState ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
	self->PopGfxState ();
	return 0;
}

//----------------------------------------------------------------//
int MOAIDrawAPI::_pushGfxState ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
	self->PushGfxState ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDrawAPI::_setBlendMode ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
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
int MOAIDrawAPI::_setCullFunc ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )

	self->SetCullFunc (( MOAICullFuncEnum::_ )state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDrawAPI::_setDepthFunc ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )

	self->SetDepthFunc (( MOAIDepthFuncEnum::_ )state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDrawAPI::_setDepthMask ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
	self->SetDepthMask ( state.GetValue < bool >( 2, false ));
	return 0;
}

//----------------------------------------------------------------//
int MOAIDrawAPI::_setFrameBuffer ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )

	self->SetFrameBuffer ( state.GetLuaObject < MOAIFrameBuffer >( 2, false ));
	lua_pushvalue ( state, 1 );
	return 1;
}

//----------------------------------------------------------------//
int MOAIDrawAPI::_setIndexBuffer ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )

	self->SetIndexBuffer ( state.GetLuaObject < MOAIIndexBuffer >( 2, false ));
	return 0;
}

//----------------------------------------------------------------//
int MOAIDrawAPI::_setMatrix ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )

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
int MOAIDrawAPI::_setPenColor ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )

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
int MOAIDrawAPI::_setPenWidth ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )

	float width = state.GetValue < float >( 2, 1.0f );
	self->SetPenWidth ( width );
	return 0;
}


//----------------------------------------------------------------//
int MOAIDrawAPI::_setScissorRect ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )

	// TODO: DRY with setViewRect
	
	if ( state.IsType ( 2, LUA_TUSERDATA )) {
		MOAIViewport* viewport = state.GetLuaObject < MOAIViewport >( 2, true );
		self->SetScissorRect ( viewport );
	}
	else if ( state.GetTop () > 1 ) {
		ZLRect rect ( 0.0, 0.0, 0.0, 0.0 );
		rect = state.GetValue < ZLRect >( 3, rect );
		self->SetScissorRect ( rect );
	}
	else {
		self->SetScissorRect ();
	}
	
	lua_pushvalue ( state, 1 );
	return 1;
}

//----------------------------------------------------------------//
int MOAIDrawAPI::_setShader ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
	MOAIShader* shader 		= MOAIGfxMgr::Get ().AffirmShader ( state, 2 );
	self->SetShader ( shader );
	
	lua_pushvalue ( state, 1 );
	return 1;
}

//----------------------------------------------------------------//
int MOAIDrawAPI::_setTexture ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
	MOAITexture* texture	= MOAIGfxMgr::Get ().AffirmTexture ( state, 2 );
	ZLIndex textureUnit		= state.GetValue < u32 >( 3, 0 );

	self->SetTexture ( texture, textureUnit );
	
	lua_pushvalue ( state, 1 );
	return 1;
}

//----------------------------------------------------------------//
int MOAIDrawAPI::_setVertexArray ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
	self->SetVertexArray ( state.GetLuaObject < MOAIVertexArray >( 2, false ));
	return 0;
}

//----------------------------------------------------------------//
int MOAIDrawAPI::_setVertexBuffer ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )

	self->SetVertexBuffer ( state.GetLuaObject < MOAIVertexBuffer >( 2, false ));
	return 0;
}

//----------------------------------------------------------------//
int MOAIDrawAPI::_setVertexFormat ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )

	MOAIVertexFormat* vertexFormat = MOAIGfxMgr::Get ().AffirmVertexFormat ( state, 2 );
	self->SetVertexFormat ( vertexFormat );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDrawAPI::_setViewProj ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
	MOAIViewport* viewport = state.GetLuaObject < MOAIViewport >( 2, false );
	MOAICamera* camera = state.GetLuaObject < MOAICamera >( 3, false );
	
	self->SetViewProj ( viewport, camera );
	return 0;
}

//----------------------------------------------------------------//
int MOAIDrawAPI::_setViewRect ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
	if ( state.IsType ( 2, LUA_TUSERDATA )) {
		MOAIViewport* viewport = state.GetLuaObject < MOAIViewport >( 2, true );
		self->SetViewRect ( viewport );
	}
	else if ( state.GetTop () > 1 ) {
		ZLRect rect ( 0.0, 0.0, 0.0, 0.0 );
		rect = state.GetValue < ZLRect >( 3, rect );
		self->SetViewRect ( rect );
	}
	else {
		self->SetViewRect ();
	}
	
	lua_pushvalue ( state, 1 );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDrawAPI::_strokeRoundedRect ( lua_State* L ) {
	MOAI_LUA_CMD_SETUP ( MOAIDrawAPI )
	
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

//================================================================//
// MOAIDrawAPI
//================================================================//

//----------------------------------------------------------------//
void MOAIDrawAPI::BindVectorPresets () {

	this->SubmitCommand ( BIND_VECTOR_PRESETS );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::ClearSurface () {

	this->SubmitCommand ( CLEAR_SURFACE );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::Draw () {

	this->SubmitCommand ( DRAW );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawAnimCurve ( MOAIAnimCurve& curve, u32 resolution ) {

	MOAIDrawAPIParam::DrawAnimCurve param;
	param.mAnimCurve = &curve;
	param.mResolution = resolution;
	this->SubmitCommand < MOAIDrawAPIParam::DrawAnimCurve >( DRAW_ANIM_CURVE, param );
	this->RetainObject ( &curve );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawAxis2D ( float x, float y, float dx, float dy ) {

	MOAIDrawAPIParam::DrawAxis2D param;
	param.mV0 .Init ( x, y );
	param.mD.Init ( dx, dy );
	this->SubmitCommand < MOAIDrawAPIParam::DrawAxis2D >( DRAW_AXIS_2D, param );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawArcFill ( float x, float y, float xRad, float yRad, float d0, float d1, u32 steps ) {

	// TODO: this is in essence drawing a scaled circle. so d0 and d1 may not give expected results if xRad != yRad. the
	// todo is to draw a proper arc as though the ellipse was overlaid on a circle and the circle's degrees are used.
	// in other words, if 2 o'clock and 4 o'clock are given, any elliptical arc overlaid on a circle arc should start and
	// stop in the same place. the (lazy) way this is written now, they won't. but... since we are only writing this
	// for the rounded rect, we won't worry about it just yet.

	float angle = ( float )( d0 * D2R );
	float angleStep = ( float )(( d1 - d0 ) * D2R ) / ( float )steps;
	
	ZLVec3D v0 ( x, y, 0.0f );
	ZLVec3D v1 ( x + ( Cos ( angle ) * xRad ), y + ( Sin ( angle ) * yRad ), 0.0f );
	
	for ( u32 i = 0; i < steps; ++i ) {
		
		angle += angleStep;
		ZLVec3D v2 ( x + ( Cos ( angle ) * xRad ), y + ( Sin ( angle ) * yRad ), 0.0f );
		
		this->DrawTriangleFill ( v0, v1, v2 );
		
		v1 = v2;
	}
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawArcStroke ( float x, float y, float xRad, float yRad, float d0, float d1, u32 steps, float stroke, float offset ) {

	// TODO: see concerns in DrawArcFill; same here. but moreso.

	stroke = stroke * 0.5;

	float angle = ( float )( d0 * D2R );
	float angleStep = ( float )(( d1 - d0 ) * D2R ) / ( float )steps;
	
	float innerXRad = xRad + offset - stroke;
	float outerXRad = xRad + offset + stroke;
	
	float innerYRad = yRad + offset - stroke;
	float outerYRad = yRad + offset + stroke;
	
	ZLVec3D inner0 ( x + ( Cos ( angle ) * innerXRad ), y + ( Sin ( angle ) * innerYRad ), 0.0f );
	ZLVec3D outer0 ( x + ( Cos ( angle ) * outerXRad ), y + ( Sin ( angle ) * outerYRad ), 0.0f );
	
	for ( u32 i = 0; i < steps; ++i ) {
		
		angle += angleStep;
		ZLVec3D inner1 ( x + ( Cos ( angle ) * innerXRad ), y + ( Sin ( angle ) * innerYRad ), 0.0f );
		ZLVec3D outer1 ( x + ( Cos ( angle ) * outerXRad ), y + ( Sin ( angle ) * outerYRad ), 0.0f );
		
		this->DrawTriangleFill ( inner0, outer0, outer1 );
		this->DrawTriangleFill ( inner0, outer1, inner1 );
		
		inner0 = inner1;
		outer0 = outer1;
	}
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawBezierCurve ( const ZLCubicBezier2D& bezier ) {

	MOAIAbstractDrawShapeVertexLineStripWriter2D writer;
	
	writer.Begin ();
		bezier.Flatten ( writer );
	writer.End ();
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawBoxAxis ( const ZLBox& box ) {

	ZLVec3D center;
	box.GetCenter ( center );
	
	this->DrawLine ( box.mMin.mX, center.mY, center.mZ, box.mMax.mX, center.mY, center.mZ );
	this->DrawLine ( center.mX, box.mMin.mY, center.mZ, center.mX, box.mMax.mY, center.mZ );
	this->DrawLine ( center.mX, center.mY, box.mMin.mZ, center.mX, center.mY, box.mMax.mZ );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawBoxDiagonals ( const ZLBox& box ) {

	this->DrawLine ( box.mMin.mX, box.mMin.mY, box.mMin.mZ, box.mMax.mX, box.mMax.mY, box.mMax.mZ );
	this->DrawLine ( box.mMin.mX, box.mMax.mY, box.mMin.mZ, box.mMax.mX, box.mMin.mY, box.mMax.mZ );
	this->DrawLine ( box.mMax.mX, box.mMax.mY, box.mMin.mZ, box.mMin.mX, box.mMin.mY, box.mMax.mZ );
	this->DrawLine ( box.mMax.mX, box.mMin.mY, box.mMin.mZ, box.mMin.mX, box.mMax.mY, box.mMax.mZ );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawBoxOutline ( const ZLBox& box ) {

	this->DrawLine ( box.mMin.mX, box.mMin.mY, box.mMin.mZ, box.mMax.mX, box.mMin.mY, box.mMin.mZ );
	this->DrawLine ( box.mMin.mX, box.mMin.mY, box.mMin.mZ, box.mMin.mX, box.mMax.mY, box.mMin.mZ );
	this->DrawLine ( box.mMin.mX, box.mMin.mY, box.mMin.mZ, box.mMin.mX, box.mMin.mY, box.mMax.mZ );
	
	this->DrawLine ( box.mMax.mX, box.mMax.mY, box.mMax.mZ, box.mMin.mX, box.mMax.mY, box.mMax.mZ );
	this->DrawLine ( box.mMax.mX, box.mMax.mY, box.mMax.mZ, box.mMax.mX, box.mMin.mY, box.mMax.mZ );
	this->DrawLine ( box.mMax.mX, box.mMax.mY, box.mMax.mZ, box.mMax.mX, box.mMax.mY, box.mMin.mZ );
	
	this->DrawLine ( box.mMax.mX, box.mMin.mY, box.mMin.mZ, box.mMax.mX, box.mMax.mY, box.mMin.mZ );
	this->DrawLine ( box.mMax.mX, box.mMin.mY, box.mMin.mZ, box.mMax.mX, box.mMin.mY, box.mMax.mZ );

	this->DrawLine ( box.mMin.mX, box.mMax.mY, box.mMin.mZ, box.mMax.mX, box.mMax.mY, box.mMin.mZ );
	this->DrawLine ( box.mMin.mX, box.mMax.mY, box.mMin.mZ, box.mMin.mX, box.mMax.mY, box.mMax.mZ );
	
	this->DrawLine ( box.mMin.mX, box.mMin.mY, box.mMax.mZ, box.mMax.mX, box.mMin.mY, box.mMax.mZ );
	this->DrawLine ( box.mMin.mX, box.mMin.mY, box.mMax.mZ, box.mMin.mX, box.mMax.mY, box.mMax.mZ );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawCircleFill ( float x, float y, float radius, u32 steps ) {

	this->DrawEllipseFill ( x, y, radius, radius, steps );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawCircleOutline ( float x, float y, float radius, u32 steps ) {

	this->DrawEllipseOutline ( x, y, radius, radius, steps );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawEllipseFill ( const ZLRect& rect, u32 steps ) {

	float xRad = ( rect.mXMax - rect.mXMin ) * 0.5f;
	float yRad = ( rect.mYMax - rect.mYMin ) * 0.5f;

	this->DrawEllipseFill ( rect.mXMin + xRad, rect.mYMin + yRad, xRad, yRad, steps );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawEllipseFill ( float x, float y, float xRad, float yRad, u32 steps ) {
	
	float angle = ( float )PI;
	float angleStep = ( float )TWOPI / ( float )steps;
	
	ZLVec3D v0 ( x, y, 0.0f );
	ZLVec3D v1 ( x + ( Cos ( angle ) * xRad ), y + ( Sin ( angle ) * yRad ), 0.0f );
	
	for ( u32 i = 0; i < steps; ++i ) {
		
		angle += angleStep;
		ZLVec3D v2 ( x + ( Cos ( angle ) * xRad ), y + ( Sin ( angle ) * yRad ), 0.0f );
		
		this->DrawTriangleFill ( v0, v1, v2 );
		
		v1 = v2;
	}
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawEllipseOutline ( const ZLRect& rect, u32 steps ) {
	
	float xRad = ( rect.mXMax - rect.mXMin ) * 0.5f;
	float yRad = ( rect.mYMax - rect.mYMin ) * 0.5f;

	this->DrawEllipseOutline ( rect.mXMin + xRad, rect.mYMin + yRad, xRad, yRad, steps );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawEllipseOutline ( float x, float y, float xRad, float yRad, u32 steps ) {

	float angle = ( float )PI;
	float angleStep = ( float )TWOPI / ( float )steps;
	
	ZLVec2D v0 ( x + ( Cos ( angle ) * xRad ), y + ( Sin ( angle ) * yRad ));
	
	for ( u32 i = 0; i < steps; ++i ) {
		
		angle += angleStep;
		ZLVec2D v1 ( x + ( Cos ( angle ) * xRad ), y + ( Sin ( angle ) * yRad ));
		
		this->DrawLine ( v0, v1 );
		
		v0 = v1;
	}
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawEllipseSpokes ( float x, float y, float xRad, float yRad, u32 steps ) {

	float step = ( float )TWOPI / ( float )steps;
	float angle = ( float )PI;
	
	for ( u32 i = 0; i < steps; ++i, angle += step ) {
	
		this->DrawLine (
			x,
			y,
			x + ( Cos ( angle ) * xRad ),
			y + ( Sin ( angle ) * yRad )
		);
	}
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawGrid ( const ZLRect& rect, u32 xCells, u32 yCells ) {

	if ( xCells > 1 ) {
		float xStep = rect.Width () / ( float )xCells;
		for ( u32 i = 1; i < xCells; ++i ) {
			float x = rect.mXMin + (( float )i * xStep );
			ZLVec2D v0 ( x, rect.mYMin );
			ZLVec2D v1 ( x, rect.mYMax );
			
			this->DrawLine ( v0, v1 );
		}
	}

	if ( yCells > 1 ) {
		float yStep = rect.Height () / ( float )yCells;
		for ( u32 i = 1; i < yCells; ++i ) {
			float y = rect.mYMin + (( float )i * yStep );
			ZLVec2D v0 ( rect.mXMin, y );
			ZLVec2D v1 ( rect.mXMax, y );
			
			this->DrawLine ( v0, v1 );
		}
	}

	this->DrawRectOutline ( rect );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawLine ( const ZLVec2D& v0, const ZLVec2D& v1 ) {

	this->DrawLine ( v0.mX, v0.mY, v1.mX, v1.mY );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawLine ( const ZLVec3D& v0, const ZLVec3D& v1 ) {

	this->DrawLine ( v0.mX, v0.mY, v0.mZ, v1.mX, v1.mY, v1.mZ );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawLine ( float x0, float y0, float x1, float y1 ) {
	
	this->DrawLine ( x0, y0, 0.0f, x1, y1, 0.0f );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawLine ( float x0, float y0, float z0, float x1, float y1, float z1 ) {

	MOAIDrawAPIParam::DrawLine param;
	param.mV0.Init ( x0, y0, z0 );
	param.mV1.Init ( x1, y1, z1 );
	this->SubmitCommand < MOAIDrawAPIParam::DrawLine >( DRAW_LINE, param );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawLuaArray ( lua_State* L, MOAIGfxTopologyEnum::_ primType ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	MOAILuaState state ( L );

	float x = 0.0f;
	float y = 0.0f;
	
	size_t size = state.GetTableSize ( -1 ) >> 2;
	
	gfxMgr.BeginPrim ( primType, size );

	u32 counter = 0;
	lua_pushnil ( L );
    while ( lua_next ( L, 1 ) != 0 ) {
		// Assuming odd-numbered array entries to be x-coordinate (abscissa),
		// even-numbered array entries to be y-coordinate (oordinate).
		if ( counter % 2 == 0 ) {
			x = state.GetValue < float >( -1, 0.0f );
		} else {
			y = state.GetValue < float >( -1, 0.0f );
			gfxMgr.WriteVtx ( x, y );
			gfxMgr.WritePenColor4b ();
		}
		++counter;
		lua_pop ( L, 1 );
	}

	gfxMgr.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawLuaParams ( lua_State* L, MOAIGfxTopologyEnum::_ primType ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	MOAILuaState state ( L );

	u32 total = ( state.GetTop () - 1 ) >> 1;
	
	gfxMgr.BeginPrim ( primType, total );
	
	for ( u32 i = 0; i < total; ++i ) {
		
		u32 idx = ( i << 1 ) + 2;
		
		float x = state.GetValue < float >( idx, 0.0f );
		float y = state.GetValue < float >( idx + 1, 0.0f );
		
		gfxMgr.WriteVtx ( x, y, 0.0f );
		gfxMgr.WritePenColor4b ();
	}
	
	gfxMgr.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawPoint ( const ZLVec2D& loc ) {

	this->DrawPoint ( loc.mX, loc.mY );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawPoint ( float x, float y, float z ) {

	this->SubmitCommand < ZLVec3D >( DRAW_POINT, ZLVec3D ( x, y, z ));
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawPolyOutline ( const ZLPolygon2D& poly ) {

	ZLSize size = poly.GetSize ();

	for ( ZLIndex i = 0; i < size; ++i ) {
		const ZLVec2D& v0 = poly.GetVertex ( i );
		const ZLVec2D& v1 = poly.GetVertex (  ZLIndexOp::AddAndWrap ( i, 1, size ));
		this->DrawLine ( v0, v1 );
	}
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawQuadOutline ( const ZLQuad& quad ) {

	this->DrawLine ( quad.mV [ 0 ].mX, quad.mV [ 0 ].mY, quad.mV [ 1 ].mX, quad.mV [ 1 ].mY );
	this->DrawLine ( quad.mV [ 1 ].mX, quad.mV [ 1 ].mY, quad.mV [ 2 ].mX, quad.mV [ 2 ].mY );
	this->DrawLine ( quad.mV [ 2 ].mX, quad.mV [ 2 ].mY, quad.mV [ 3 ].mX, quad.mV [ 3 ].mY );
	this->DrawLine ( quad.mV [ 3 ].mX, quad.mV [ 3 ].mY, quad.mV [ 0 ].mX, quad.mV [ 0 ].mY );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawRay ( float x, float y, float dx, float dy ) {

	MOAIDrawAPIParam::DrawRay param;
	param.mLoc = ZLVec2D ( x, y );
	param.mVec = ZLVec2D ( dx, dy );
	this->SubmitCommand < MOAIDrawAPIParam::DrawRay >( DRAW_RAY, param );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawRectEdges ( ZLRect rect, u32 edges ) {

	rect.Bless ();
	
	// right
	if ( edges & ZLRect::kRight ) {
		this->DrawLine ( rect.mXMax, rect.mYMin, rect.mXMax, rect.mYMax );
	}

	// top
	if ( edges & ZLRect::kTop ) {			
		this->DrawLine ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMin );
	}

	// left
	if ( edges & ZLRect::kLeft ) {			
		this->DrawLine ( rect.mXMin, rect.mYMin, rect.mXMin, rect.mYMax );
	}

	// bottom
	if ( edges & ZLRect::kBottom ) {			
		this->DrawLine ( rect.mXMin, rect.mYMax, rect.mXMax, rect.mYMax );
	}	
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawRectFill ( ZLRect rect ) {

	rect.Bless ();
	this->DrawRectFill ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMax );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawRectFill ( float left, float top, float right, float bottom ) {
	
	ZLVec3D lt ( left, top, 0.0f );
	ZLVec3D rt ( right, top, 0.0f );
	ZLVec3D rb ( right, bottom, 0.0f );
	ZLVec3D lb ( left, bottom, 0.0f );
	
	this->DrawTriangleFill ( lb, rt, lt );
	this->DrawTriangleFill ( lb, rb, rt );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawRectOutline ( const ZLRect& rect ) {
	
	this->DrawRectOutline ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMax );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawRectOutline ( float left, float top, float right, float bottom ) {
	
	this->DrawLine ( left, top, right, top );
	this->DrawLine ( right, top, right, bottom );
	this->DrawLine ( right, bottom, left, bottom );
	this->DrawLine ( left, bottom, left, top );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawRoundedRectFill ( float left, float top, float right, float bottom, float xRad, float yRad, u32 steps ) {

	ZLRect rect ( left, top, right, bottom );
	rect.Bless ();

	float hWidth	= rect.Width () * 0.5;
	float hHeight	= rect.Height () * 0.5;

	if (( hWidth > 0.0 ) && ( hHeight > 0.0 )) {

		// quarter of the steps for each corner
		steps = steps >> 2;

		// clamp radii
		xRad = xRad > hWidth ? hWidth : xRad;
		yRad = yRad > hHeight ? hHeight : yRad;

		// draw sections
		this->DrawRectFill ( rect.mXMin + xRad, rect.mYMax - yRad, rect.mXMax - xRad, rect.mYMax ); // top section
		this->DrawRectFill ( rect.mXMin, rect.mYMin + yRad, rect.mXMax, rect.mYMax - yRad ); // middle section
		this->DrawRectFill ( rect.mXMin + xRad, rect.mYMin, rect.mXMax - xRad, rect.mYMin + yRad ); // bottom section

		// draw corners
		DrawArcFill ( rect.mXMax - xRad, rect.mYMax - yRad, xRad, yRad, 90.0, 0.0, steps );
		DrawArcFill ( rect.mXMin + xRad, rect.mYMax - yRad, xRad, yRad, 180.0, 90.0, steps );
		DrawArcFill ( rect.mXMin + xRad, rect.mYMin + yRad, xRad, yRad, 270.0, 180.0, steps );
		DrawArcFill ( rect.mXMax - xRad, rect.mYMin + yRad, xRad, yRad, 360.0, 270.0, steps );
	}
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawRoundedRectStroke ( float left, float top, float right, float bottom, float xRad, float yRad, u32 steps, float stroke, float offset ) {

	if ( stroke <= 0.0 ) return;

	ZLRect rect ( left, top, right, bottom );
	rect.Bless ();

	float hWidth	= rect.Width () * 0.5;
	float hHeight	= rect.Height () * 0.5;

	if (( hWidth > 0.0 ) && ( hHeight > 0.0 )) {

		// half stroke
		float hStroke = stroke * 0.5;

		// quarter of the steps for each corner
		steps = steps >> 2;

		// clamp radii
		xRad = xRad > hWidth ? hWidth : xRad;
		yRad = yRad > hHeight ? hHeight : yRad;

		// stroke edges

		this->DrawRectFill ( rect.mXMin - offset - hStroke, rect.mYMax - yRad, rect.mXMin - offset + hStroke, rect.mYMin + yRad ); // left
		this->DrawRectFill ( rect.mXMax + offset - hStroke, rect.mYMax - yRad, rect.mXMax + offset + hStroke, rect.mYMin + yRad ); // right

		this->DrawRectFill ( rect.mXMin + xRad, rect.mYMax + offset + hStroke, rect.mXMax - xRad, rect.mYMax + offset - hStroke ); // top
		this->DrawRectFill ( rect.mXMin + xRad, rect.mYMin - offset + hStroke, rect.mXMax - xRad, rect.mYMin - offset - hStroke ); // bottom

		// stroke corners		
		DrawArcStroke ( rect.mXMax - xRad, rect.mYMax - yRad, xRad, yRad, 90.0, 0.0, steps, stroke, offset );
		DrawArcStroke ( rect.mXMin + xRad, rect.mYMax - yRad, xRad, yRad, 180.0, 90.0, steps, stroke, offset );
		DrawArcStroke ( rect.mXMin + xRad, rect.mYMin + yRad, xRad, yRad, 270.0, 180.0, steps, stroke, offset );
		DrawArcStroke ( rect.mXMax - xRad, rect.mYMin + yRad, xRad, yRad, 360.0, 270.0, steps, stroke, offset );
	}
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawTriangleFill ( const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2 ) {

	MOAIDrawAPIParam::DrawTriangle param;
	param.mV0 = v0;
	param.mV1 = v1;
	param.mV2 = v2;
	this->SubmitCommand < MOAIDrawAPIParam::DrawTriangle >( DRAW_TRIANGLE, param );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawTriangleOutline ( const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2 ) {

	this->DrawLine ( v0, v1 );
	this->DrawLine ( v1, v2 );
	this->DrawLine ( v2, v0 );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawVec ( float x, float y, float dx, float dy, float t ) {

	this->DrawVec ( x, y, 0, dx, dy, 0, t );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::DrawVec ( float x, float y, float z, float dx, float dy, float dz, float t ) {

	this->DrawLine ( x, y, z, x + ( dx * t ), y + ( dy * t ), z + ( dz * t ));
}

//----------------------------------------------------------------//
MOAIDrawAPI::MOAIDrawAPI () {
}

//----------------------------------------------------------------//
MOAIDrawAPI::~MOAIDrawAPI () {
}

//----------------------------------------------------------------//
void MOAIDrawAPI::PopGfxState () {

	this->SubmitCommand ( POP_GFX_STATE );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::PushGfxState () {

	this->SubmitCommand ( PUSH_GFX_STATE );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::SetBlendMode ( const MOAIBlendMode& blendMode ) {

	this->SubmitCommand < MOAIBlendMode >( SET_BLEND_MODE, blendMode );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::SetCullFunc ( MOAICullFuncEnum::_ cullFunc ) {

	this->SubmitCommand < MOAICullFuncEnum::_ >( SET_CULL_FUNC, cullFunc );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::SetDepthFunc ( MOAIDepthFuncEnum::_ depthFunc ) {

	this->SubmitCommand < MOAIDepthFuncEnum::_  >( SET_DEPTH_FUNC, depthFunc );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::SetDepthMask ( bool depthMask ) {

	this->SubmitCommand < bool >( SET_DEPTH_MASK, depthMask );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::SetFrameBuffer ( MOAIFrameBuffer* frameBuffer ) {

	this->SubmitCommand < MOAIFrameBuffer* >( SET_FRAME_BUFFER, frameBuffer );
	this->RetainObject ( frameBuffer );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::SetIndexBuffer ( MOAIIndexBuffer* indexBuffer ) {

	this->SubmitCommand < MOAIIndexBuffer* >( SET_INDEX_BUFFER, indexBuffer );
	this->RetainObject ( indexBuffer );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::SetMatrix ( u32 matrixID, const ZLMatrix4x4& mtx ) {
	
	MOAIDrawAPIParam::SetMatrix param;
	param.mMatrixID = matrixID;
	param.mMatrix = mtx;
	this->SubmitCommand < MOAIDrawAPIParam::SetMatrix >( SET_MATRIX, param );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::SetMatrixFromTransform ( u32 matrixID, MOAIAbstractChildTransform& transform ) {

	MOAIDrawAPIParam::SetMatrixFromTransform param;
	param.mMatrixID = matrixID;
	param.mTransform = &transform;
	this->SubmitCommand < MOAIDrawAPIParam::SetMatrixFromTransform >( SET_MATRIX_FROM_TRANSFORM, param );
	this->RetainObject ( &transform );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::SetPenColor ( u32 color ) {

	this->SubmitCommand < u32 >( SET_PEN_COLOR, color );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::SetPenWidth ( float width ) {

	this->SubmitCommand < float >( SET_PEN_WIDTH, width );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::SetScissorRect () {

	this->SubmitCommand ( DISABLE_SCISSOR_RECT );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::SetScissorRect ( const ZLRect& rect ) {

	this->SubmitCommand < ZLRect >( SET_SCISSOR_RECT, rect );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::SetScissorRect ( MOAIViewport* viewport ) {
	
	this->SubmitCommand < MOAIViewport* >( SET_SCISSOR_RECT_FROM_VIEWPORT, viewport );
	this->RetainObject ( viewport );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::SetShader ( MOAIShader* shader ) {

	this->SubmitCommand < MOAIShader* >( SET_SHADER, shader );
	this->RetainObject ( shader );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::SetTexture ( MOAITexture* texture, ZLIndex textureUnit ) {

	MOAIDrawAPIParam::SetTexture param;
	param.mTexture = texture;
	param.mTextureUnit = textureUnit;
	this->SubmitCommand < MOAIDrawAPIParam::SetTexture >( SET_TEXTURE, param );
	this->RetainObject ( texture );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::SetVertexArray ( MOAIVertexArray* vertexArray ) {

	this->SubmitCommand < MOAIVertexArray* >( SET_VERTEX_ARRAY, vertexArray );
	this->RetainObject ( vertexArray );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::SetVertexBuffer ( MOAIVertexBuffer* vertexBuffer ) {

	this->SubmitCommand < MOAIVertexBuffer* >( SET_VERTEX_BUFFER, vertexBuffer );
	this->RetainObject ( vertexBuffer );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::SetVertexFormat ( MOAIVertexFormat* vertexFormat ) {

	this->SubmitCommand < MOAIVertexFormat* >( SET_VERTEX_FORMAT, vertexFormat );
	this->RetainObject ( vertexFormat );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::SetViewProj ( MOAIViewport* viewport, MOAICamera* camera ) {

	MOAIDrawAPIParam::SetViewProj param;
	param.mViewport = viewport;
	param.mCamera = camera;
	this->SubmitCommand < MOAIDrawAPIParam::SetViewProj >( SET_VIEW_PROJ, param );
	this->RetainObject ( viewport );
	this->RetainObject ( camera );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::SetViewRect () {

	this->SubmitCommand ( DISABLE_VIEW_RECT );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::SetViewRect ( const ZLRect& rect ) {

	this->SubmitCommand < ZLRect >( SET_VIEW_RECT, rect );
}

//----------------------------------------------------------------//
void MOAIDrawAPI::SetViewRect ( MOAIViewport* viewport ) {
	
	this->SubmitCommand < MOAIViewport* >( SET_VIEW_RECT_FROM_VIEWPORT, viewport );
	this->RetainObject ( viewport );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDrawAPI::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
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
void MOAIDrawAPI::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIDrawAPI::MOAIAbstractCmdAPI_RegisterLuaAPI ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "bindVectorPresets",			_bindVectorPresets },
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
