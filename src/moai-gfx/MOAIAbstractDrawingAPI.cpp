// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractChildTransform.h>
#include <moai-gfx/MOAIAbstractDrawingAPI.h>
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
// MOAIAbstractDrawingAPI
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::BindVectorDrawing () {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	gfxMgr.SetTexture ();
	gfxMgr.SetShader ( MOAIShaderPresetEnum::LINE_SHADER );
	gfxMgr.SetVertexFormat ( MOAIVertexFormatPresetEnum::XYZWC );
	
	gfxMgr.SetVertexTransform ( MOAIGfxMgr::MODEL_TO_CLIP_MTX );
	gfxMgr.SetUVTransform ( MOAIGfxMgr::UV_TO_MODEL_MTX );

	gfxMgr.SetCullFunc (); // no culling in immediate mode drawing
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::Call () {

//	this->MOAIAbstractDrawingAPI_Call ();
	this->SubmitCommand ( MOAIDrawingCmdEnum::CALL );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::CallFromShader() {

//	this->MOAIAbstractDrawingAPI_FromShader ();
	this->SubmitCommand ( MOAIDrawingCmdEnum::CALL_FROM_SHADER );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::ClearSurface () {

	this->SubmitCommand ( MOAIDrawingCmdEnum::CLEAR_SURFACE );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawAnimCurve ( MOAIAnimCurve& curve, u32 resolution ) {

	MOAIDrawingParam::DrawAnimCurve param;
	param.mAnimCurve = &curve;
	param.mResolution = resolution;
	this->SubmitCommand < MOAIDrawingParam::DrawAnimCurve >( MOAIDrawingCmdEnum::DRAW_ANIM_CURVE, param );
	this->RetainObject ( &curve );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawAxis2D ( float x, float y, float dx, float dy ) {

	MOAIDrawingParam::DrawAxis2D param;
	param.mV0 .Init ( x, y );
	param.mD.Init ( dx, dy );
	this->SubmitCommand < MOAIDrawingParam::DrawAxis2D >( MOAIDrawingCmdEnum::DRAW_AXIS_2D, param );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawArcFill ( float x, float y, float xRad, float yRad, float d0, float d1, u32 steps ) {

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
void MOAIAbstractDrawingAPI::DrawArcStroke ( float x, float y, float xRad, float yRad, float d0, float d1, u32 steps, float stroke, float offset ) {

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
void MOAIAbstractDrawingAPI::DrawBezierCurve ( const ZLCubicBezier2D& bezier ) {

	MOAIAbstractDrawShapeVertexLineStripWriter2D writer;
	
	writer.Begin ();
		bezier.Flatten ( writer );
	writer.End ();
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawBoxAxis ( const ZLBox& box ) {

	ZLVec3D center;
	box.GetCenter ( center );
	
	this->DrawLine ( box.mMin.mX, center.mY, center.mZ, box.mMax.mX, center.mY, center.mZ );
	this->DrawLine ( center.mX, box.mMin.mY, center.mZ, center.mX, box.mMax.mY, center.mZ );
	this->DrawLine ( center.mX, center.mY, box.mMin.mZ, center.mX, center.mY, box.mMax.mZ );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawBoxDiagonals ( const ZLBox& box ) {

	this->DrawLine ( box.mMin.mX, box.mMin.mY, box.mMin.mZ, box.mMax.mX, box.mMax.mY, box.mMax.mZ );
	this->DrawLine ( box.mMin.mX, box.mMax.mY, box.mMin.mZ, box.mMax.mX, box.mMin.mY, box.mMax.mZ );
	this->DrawLine ( box.mMax.mX, box.mMax.mY, box.mMin.mZ, box.mMin.mX, box.mMin.mY, box.mMax.mZ );
	this->DrawLine ( box.mMax.mX, box.mMin.mY, box.mMin.mZ, box.mMin.mX, box.mMax.mY, box.mMax.mZ );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawBoxOutline ( const ZLBox& box ) {

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
void MOAIAbstractDrawingAPI::DrawCircleFill ( float x, float y, float radius, u32 steps ) {

	this->DrawEllipseFill ( x, y, radius, radius, steps );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawCircleOutline ( float x, float y, float radius, u32 steps ) {

	this->DrawEllipseOutline ( x, y, radius, radius, steps );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawEllipseFill ( const ZLRect& rect, u32 steps ) {

	float xRad = ( rect.mXMax - rect.mXMin ) * 0.5f;
	float yRad = ( rect.mYMax - rect.mYMin ) * 0.5f;

	this->DrawEllipseFill ( rect.mXMin + xRad, rect.mYMin + yRad, xRad, yRad, steps );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawEllipseFill ( float x, float y, float xRad, float yRad, u32 steps ) {
	
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
void MOAIAbstractDrawingAPI::DrawEllipseOutline ( const ZLRect& rect, u32 steps ) {
	
	float xRad = ( rect.mXMax - rect.mXMin ) * 0.5f;
	float yRad = ( rect.mYMax - rect.mYMin ) * 0.5f;

	this->DrawEllipseOutline ( rect.mXMin + xRad, rect.mYMin + yRad, xRad, yRad, steps );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawEllipseOutline ( float x, float y, float xRad, float yRad, u32 steps ) {

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
void MOAIAbstractDrawingAPI::DrawEllipseSpokes ( float x, float y, float xRad, float yRad, u32 steps ) {

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
void MOAIAbstractDrawingAPI::DrawGrid ( const ZLRect& rect, u32 xCells, u32 yCells ) {

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
void MOAIAbstractDrawingAPI::DrawLine ( const ZLVec2D& v0, const ZLVec2D& v1 ) {

	this->DrawLine ( v0.mX, v0.mY, v1.mX, v1.mY );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawLine ( const ZLVec3D& v0, const ZLVec3D& v1 ) {

	this->DrawLine ( v0.mX, v0.mY, v0.mZ, v1.mX, v1.mY, v1.mZ );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawLine ( float x0, float y0, float x1, float y1 ) {
	
	this->DrawLine ( x0, y0, 0.0f, x1, y1, 0.0f );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawLine ( float x0, float y0, float z0, float x1, float y1, float z1 ) {

	MOAIDrawingParam::DrawLine param;
	param.mV0.Init ( x0, y0, z0 );
	param.mV1.Init ( x1, y1, z1 );
	this->SubmitCommand < MOAIDrawingParam::DrawLine >( MOAIDrawingCmdEnum::DRAW_LINE, param );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawLuaArray ( lua_State* L, MOAIGfxTopologyEnum::_ primType ) {

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
void MOAIAbstractDrawingAPI::DrawLuaParams ( lua_State* L, MOAIGfxTopologyEnum::_ primType ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	MOAILuaState state ( L );

	u32 total = state.GetTop () >> 1;
	
	gfxMgr.BeginPrim ( primType, total );
	
	for ( u32 i = 0; i < total; ++i ) {
		
		u32 idx = ( i << 1 ) + 1;
		
		float x = state.GetValue < float >( idx, 0.0f );
		float y = state.GetValue < float >( idx + 1, 0.0f );
		
		gfxMgr.WriteVtx ( x, y, 0.0f );
		gfxMgr.WritePenColor4b ();
	}
	
	gfxMgr.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawPoint ( const ZLVec2D& loc ) {

	this->DrawPoint ( loc.mX, loc.mY );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawPoint ( float x, float y, float z ) {

	this->SubmitCommand < ZLVec3D >( MOAIDrawingCmdEnum::DRAW_POINT, ZLVec3D ( x, y, z ));
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawPolyOutline ( const ZLPolygon2D& poly ) {

	ZLSize size = poly.GetSize ();

	for ( ZLIndex i = 0; i < size; ++i ) {
		const ZLVec2D& v0 = poly.GetVertex ( i );
		const ZLVec2D& v1 = poly.GetVertex (  ZLIndexOp::AddAndWrap ( i, 1, size ));
		this->DrawLine ( v0, v1 );
	}
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawQuadOutline ( const ZLQuad& quad ) {

	this->DrawLine ( quad.mV [ 0 ].mX, quad.mV [ 0 ].mY, quad.mV [ 1 ].mX, quad.mV [ 1 ].mY );
	this->DrawLine ( quad.mV [ 1 ].mX, quad.mV [ 1 ].mY, quad.mV [ 2 ].mX, quad.mV [ 2 ].mY );
	this->DrawLine ( quad.mV [ 2 ].mX, quad.mV [ 2 ].mY, quad.mV [ 3 ].mX, quad.mV [ 3 ].mY );
	this->DrawLine ( quad.mV [ 3 ].mX, quad.mV [ 3 ].mY, quad.mV [ 0 ].mX, quad.mV [ 0 ].mY );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawRay ( float x, float y, float dx, float dy, float length ) {

	this->DrawLine ( x, y, x + ( dx * length ), y + ( dy * length ));
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawRectEdges ( ZLRect rect, u32 edges ) {

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
void MOAIAbstractDrawingAPI::DrawRectFill ( ZLRect rect ) {

	rect.Bless ();
	this->DrawRectFill ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMax );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawRectFill ( float left, float top, float right, float bottom ) {
	
	ZLVec3D lt ( left, top, 0.0f );
	ZLVec3D rt ( right, top, 0.0f );
	ZLVec3D rb ( right, bottom, 0.0f );
	ZLVec3D lb ( left, bottom, 0.0f );
	
	this->DrawTriangleFill ( lb, rt, lt );
	this->DrawTriangleFill ( lb, rb, rt );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawRectOutline ( const ZLRect& rect ) {
	
	this->DrawRectOutline ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMax );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawRectOutline ( float left, float top, float right, float bottom ) {
	
	this->DrawLine ( left, top, right, top );
	this->DrawLine ( right, top, right, bottom );
	this->DrawLine ( right, bottom, left, bottom );
	this->DrawLine ( left, bottom, left, top );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawRoundedRectFill ( float left, float top, float right, float bottom, float xRad, float yRad, u32 steps ) {

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
void MOAIAbstractDrawingAPI::DrawRoundedRectStroke ( float left, float top, float right, float bottom, float xRad, float yRad, u32 steps, float stroke, float offset ) {

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
void MOAIAbstractDrawingAPI::DrawTriangleFill ( const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2 ) {

	MOAIDrawingParam::DrawTriangle param;
	param.mV0 = v0;
	param.mV1 = v1;
	param.mV2 = v2;
	this->SubmitCommand < MOAIDrawingParam::DrawTriangle >( MOAIDrawingCmdEnum::DRAW_TRIANGLE, param );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::DrawTriangleOutline ( const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2 ) {

	this->DrawLine ( v0, v1 );
	this->DrawLine ( v1, v2 );
	this->DrawLine ( v2, v0 );
}

//----------------------------------------------------------------//
MOAIAbstractDrawingAPI::MOAIAbstractDrawingAPI () {
}

//----------------------------------------------------------------//
MOAIAbstractDrawingAPI::~MOAIAbstractDrawingAPI () {
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::PopGfxState () {

	this->SubmitCommand ( MOAIDrawingCmdEnum::POP_GFX_STATE );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::PushGfxState () {

	this->SubmitCommand ( MOAIDrawingCmdEnum::PUSH_GFX_STATE );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::RetainObject ( ZLRefCountedObject* object ) {

	this->MOAIAbstractDrawingAPI_RetainObject ( object );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::SetBlendMode ( const MOAIBlendMode& blendMode ) {

	this->SubmitCommand < MOAIBlendMode >( MOAIDrawingCmdEnum::SET_BLEND_MODE, blendMode );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::SetCullFunc ( MOAICullFuncEnum::_ cullFunc ) {

	this->SubmitCommand < MOAICullFuncEnum::_ >( MOAIDrawingCmdEnum::SET_CULL_FUNC, cullFunc );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::SetDepthFunc ( MOAIDepthFuncEnum::_ depthFunc ) {

	this->SubmitCommand < MOAIDepthFuncEnum::_  >( MOAIDrawingCmdEnum::SET_DEPTH_FUNC, depthFunc );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::SetDepthMask ( bool depthMask ) {

	this->SubmitCommand < bool >( MOAIDrawingCmdEnum::SET_DEPTH_MASK, depthMask );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::SetFrameBuffer ( MOAIFrameBuffer* frameBuffer ) {

	this->SubmitCommand < MOAIFrameBuffer* >( MOAIDrawingCmdEnum::SET_FRAME_BUFFER, frameBuffer );
	this->RetainObject ( frameBuffer );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::SetIndexBuffer ( MOAIIndexBuffer* indexBuffer ) {

	this->SubmitCommand < MOAIIndexBuffer* >( MOAIDrawingCmdEnum::SET_INDEX_BUFFER, indexBuffer );
	this->RetainObject ( indexBuffer );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::SetMatrix ( u32 matrixID, const ZLMatrix4x4& mtx ) {
	
	MOAIDrawingParam::SetMatrix param;
	param.mMatrixID = matrixID;
	param.mMatrix = mtx;
	this->SubmitCommand < MOAIDrawingParam::SetMatrix >( MOAIDrawingCmdEnum::SET_MATRIX, param );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::SetMatrixFromTransform ( u32 matrixID, MOAIAbstractChildTransform& transform ) {

	MOAIDrawingParam::SetMatrixFromTransform param;
	param.mMatrixID = matrixID;
	param.mTransform = &transform;
	this->SubmitCommand < MOAIDrawingParam::SetMatrixFromTransform >( MOAIDrawingCmdEnum::SET_MATRIX_FROM_TRANSFORM, param );
	this->RetainObject ( &transform );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::SetPenColor ( u32 color ) {

	this->SubmitCommand < u32 >( MOAIDrawingCmdEnum::SET_PEN_COLOR, color );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::SetPenWidth ( float width ) {

	this->SubmitCommand < float >( MOAIDrawingCmdEnum::SET_PEN_WIDTH, width );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::SetScissorRect ( const ZLRect& rect ) {

	this->SubmitCommand < ZLRect >( MOAIDrawingCmdEnum::SET_SCISSOR_RECT, rect );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::SetShader ( MOAIShader* shader ) {

	this->SubmitCommand < MOAIShader* >( MOAIDrawingCmdEnum::SET_SHADER, shader );
	this->RetainObject ( shader );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::SetTexture ( MOAITexture* texture, ZLIndex textureUnit ) {

	MOAIDrawingParam::SetTexture param;
	param.mTexture = texture;
	param.mTextureUnit = textureUnit;
	this->SubmitCommand < MOAIDrawingParam::SetTexture >( MOAIDrawingCmdEnum::SET_TEXTURE, param );
	this->RetainObject ( texture );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::SetVertexArray ( MOAIVertexArray* vertexArray ) {

	this->SubmitCommand < MOAIVertexArray* >( MOAIDrawingCmdEnum::SET_VERTEX_ARRAY, vertexArray );
	this->RetainObject ( vertexArray );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::SetVertexBuffer ( MOAIVertexBuffer* vertexBuffer ) {

	this->SubmitCommand < MOAIVertexBuffer* >( MOAIDrawingCmdEnum::SET_VERTEX_BUFFER, vertexBuffer );
	this->RetainObject ( vertexBuffer );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::SetVertexFormat ( MOAIVertexFormat* vertexFormat ) {

	this->SubmitCommand < MOAIVertexFormat* >( MOAIDrawingCmdEnum::SET_VERTEX_FORMAT, vertexFormat );
	this->RetainObject ( vertexFormat );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::SetViewProj ( MOAIViewport* viewport, MOAICamera* camera ) {

	MOAIDrawingParam::SetViewProj param;
	param.mViewport = viewport;
	param.mCamera = camera;
	this->SubmitCommand < MOAIDrawingParam::SetViewProj >( MOAIDrawingCmdEnum::SET_VIEW_PROJ, param );
	this->RetainObject ( viewport );
	this->RetainObject ( camera );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::SetViewRect ( const ZLRect& rect ) {

	this->SubmitCommand < ZLRect >( MOAIDrawingCmdEnum::SET_VIEW_RECT, rect );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::SetViewRect ( MOAIViewport* viewport ) {
	
	this->SubmitCommand < MOAIViewport* >( MOAIDrawingCmdEnum::SET_VIEW_RECT_FROM_VIEWPORT, viewport );
	this->RetainObject ( viewport );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawingAPI::SubmitCommand ( MOAIDrawingCmdEnum::_ cmd ) {
	this->MOAIAbstractDrawingAPI_SubmitCommand ( cmd, NULL, 0 );
}
