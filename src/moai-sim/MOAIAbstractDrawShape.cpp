// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAbstractDrawShape.h>

#define DEFAULT_ELLIPSE_STEPS 64

//================================================================//
// MOAIAbstractDrawShape
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractDrawShape::DrawArcFill ( float x, float y, float xRad, float yRad, float d0, float d1, u32 steps ) {

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
void MOAIAbstractDrawShape::DrawArcStroke ( float x, float y, float xRad, float yRad, float d0, float d1, u32 steps, float stroke, float offset ) {

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
void MOAIAbstractDrawShape::DrawBoxAxis ( const ZLBox& box ) {

	ZLVec3D center;
	box.GetCenter ( center );
	
	this->DrawLine ( box.mMin.mX, center.mY, center.mZ, box.mMax.mX, center.mY, center.mZ );
	this->DrawLine ( center.mX, box.mMin.mY, center.mZ, center.mX, box.mMax.mY, center.mZ );
	this->DrawLine ( center.mX, center.mY, box.mMin.mZ, center.mX, center.mY, box.mMax.mZ );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawShape::DrawBoxDiagonals ( const ZLBox& box ) {

	this->DrawLine ( box.mMin.mX, box.mMin.mY, box.mMin.mZ, box.mMax.mX, box.mMax.mY, box.mMax.mZ );
	this->DrawLine ( box.mMin.mX, box.mMax.mY, box.mMin.mZ, box.mMax.mX, box.mMin.mY, box.mMax.mZ );
	this->DrawLine ( box.mMax.mX, box.mMax.mY, box.mMin.mZ, box.mMin.mX, box.mMin.mY, box.mMax.mZ );
	this->DrawLine ( box.mMax.mX, box.mMin.mY, box.mMin.mZ, box.mMin.mX, box.mMax.mY, box.mMax.mZ );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawShape::DrawBoxOutline ( const ZLBox& box ) {

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
void MOAIAbstractDrawShape::DrawCircleFill ( float x, float y, float radius, u32 steps ) {

	this->DrawEllipseFill ( x, y, radius, radius, steps );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawShape::DrawCircleOutline ( float x, float y, float radius, u32 steps ) {

	this->DrawEllipseOutline ( x, y, radius, radius, steps );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawShape::DrawEllipseFill ( const ZLRect& rect, u32 steps ) {

	float xRad = ( rect.mXMax - rect.mXMin ) * 0.5f;
	float yRad = ( rect.mYMax - rect.mYMin ) * 0.5f;

	this->DrawEllipseFill ( rect.mXMin + xRad, rect.mYMin + yRad, xRad, yRad, steps );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawShape::DrawEllipseFill ( float x, float y, float xRad, float yRad, u32 steps ) {
	
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
void MOAIAbstractDrawShape::DrawEllipseOutline ( const ZLRect& rect, u32 steps ) {
	
	float xRad = ( rect.mXMax - rect.mXMin ) * 0.5f;
	float yRad = ( rect.mYMax - rect.mYMin ) * 0.5f;

	this->DrawEllipseOutline ( rect.mXMin + xRad, rect.mYMin + yRad, xRad, yRad, steps );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawShape::DrawEllipseOutline ( float x, float y, float xRad, float yRad, u32 steps ) {

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
void MOAIAbstractDrawShape::DrawEllipseSpokes ( float x, float y, float xRad, float yRad, u32 steps ) {

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
void MOAIAbstractDrawShape::DrawLine ( const ZLVec2D& v0, const ZLVec2D& v1 ) {

	this->DrawLine ( v0.mX, v0.mY, v1.mX, v1.mY );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawShape::DrawLine ( const ZLVec3D& v0, const ZLVec3D& v1 ) {

	this->DrawLine ( v0.mX, v0.mY, v0.mZ, v1.mX, v1.mY, v1.mZ );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawShape::DrawLine ( float x0, float y0, float x1, float y1 ) {
	
	this->DrawLine ( x0, y0, 0.0f, x1, y1, 0.0f );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawShape::DrawLine ( float x0, float y0, float z0, float x1, float y1, float z1 ) {

	this->MOAIAbstractDrawShape_DrawLine ( x0, y0, z0, x1, y1, z1 );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawShape::DrawPoint ( const ZLVec2D& loc ) {

	this->MOAIAbstractDrawShape_DrawPoint ( loc.mX, loc.mY );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawShape::DrawPoint ( float x, float y, float z ) {

	this->MOAIAbstractDrawShape_DrawPoint ( x, y, z );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawShape::DrawPolyOutline ( const ZLPolygon2D& poly ) {

	ZLSize size = poly.GetSize ();

	for ( ZLIndex i = 0; i < size; ++i ) {
		const ZLVec2D& v0 = poly.GetVertex ( i );
		const ZLVec2D& v1 = poly.GetVertex (  ZLIndexOp::AddAndWrap ( i, 1, size ));
		this->DrawLine ( v0, v1 );
	}
}

//----------------------------------------------------------------//
void MOAIAbstractDrawShape::DrawQuadOutline ( const ZLQuad& quad ) {

	this->DrawLine ( quad.mV [ 0 ].mX, quad.mV [ 0 ].mY, quad.mV [ 1 ].mX, quad.mV [ 1 ].mY );
	this->DrawLine ( quad.mV [ 1 ].mX, quad.mV [ 1 ].mY, quad.mV [ 2 ].mX, quad.mV [ 2 ].mY );
	this->DrawLine ( quad.mV [ 2 ].mX, quad.mV [ 2 ].mY, quad.mV [ 3 ].mX, quad.mV [ 3 ].mY );
	this->DrawLine ( quad.mV [ 3 ].mX, quad.mV [ 3 ].mY, quad.mV [ 0 ].mX, quad.mV [ 0 ].mY );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawShape::DrawRay ( float x, float y, float dx, float dy, float length ) {

	this->DrawLine ( x, y, x + ( dx * length ), y + ( dy * length ));
}

//----------------------------------------------------------------//
void MOAIAbstractDrawShape::DrawRectEdges ( ZLRect rect, u32 edges ) {

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
void MOAIAbstractDrawShape::DrawRectFill ( ZLRect rect ) {

	rect.Bless ();
	this->DrawRectFill ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMax );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawShape::DrawRectFill ( float left, float top, float right, float bottom ) {
	
	ZLVec3D lt ( left, top, 0.0f );
	ZLVec3D rt ( right, top, 0.0f );
	ZLVec3D rb ( right, bottom, 0.0f );
	ZLVec3D lb ( left, bottom, 0.0f );
	
	this->DrawTriangleFill ( lb, rt, lt );
	this->DrawTriangleFill ( lb, rb, rt );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawShape::DrawRectOutline ( const ZLRect& rect ) {
	
	this->DrawRectOutline ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMax );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawShape::DrawRectOutline ( float left, float top, float right, float bottom ) {
	
	this->DrawLine ( left, top, right, top );
	this->DrawLine ( right, top, right, bottom );
	this->DrawLine ( right, bottom, left, bottom );
	this->DrawLine ( left, bottom, left, top );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawShape::DrawRoundedRectFill ( float left, float top, float right, float bottom, float xRad, float yRad, u32 steps ) {

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
void MOAIAbstractDrawShape::DrawRoundedRectStroke ( float left, float top, float right, float bottom, float xRad, float yRad, u32 steps, float stroke, float offset ) {

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
void MOAIAbstractDrawShape::DrawTriangleFill ( const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2 ) {

	this->MOAIAbstractDrawShape_DrawTriangleFill ( v0, v1, v2 );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawShape::DrawTriangleOutline ( const ZLVec3D& v0, const ZLVec3D& v1, const ZLVec3D& v2 ) {

	this->DrawLine ( v0, v1 );
	this->DrawLine ( v1, v2 );
	this->DrawLine ( v2, v0 );
}

//----------------------------------------------------------------//
MOAIAbstractDrawShape::MOAIAbstractDrawShape () {
}

//----------------------------------------------------------------//
MOAIAbstractDrawShape::~MOAIAbstractDrawShape () {
}

//----------------------------------------------------------------//
void MOAIAbstractDrawShape::SetPenColor ( u32 color ) {

	this->MOAIAbstractDrawShape_SetPenColor ( color );
}

//----------------------------------------------------------------//
void MOAIAbstractDrawShape::SetPenWidth ( float width ) {

	this->MOAIAbstractDrawShape_SetPenWidth ( width );
}
