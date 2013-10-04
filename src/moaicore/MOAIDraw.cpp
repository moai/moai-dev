// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIAnimCurve.h>
#include <moaicore/MOAIDraw.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIShaderMgr.h>
#include <moaicore/MOAIVertexFormatMgr.h>

#define DEFAULT_ELLIPSE_STEPS 64

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIDraw::_drawAnimCurve ( lua_State* L ) {

	MOAILuaState state ( L );

	MOAIAnimCurve* curve	= state.GetLuaObject < MOAIAnimCurve >( 1, true );
	u32 resolution			= state.GetValue < u32 >( 2, 1 );

	if ( curve ) {
		MOAIDraw::DrawAnimCurve ( *curve, resolution );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_drawAxisGrid ( lua_State* L ) {
	UNUSED ( L );
	return 0;
}

//----------------------------------------------------------------//
/** @name	drawAntialiasedLineSegment
	@text	Draw an anti-aliased line segment between two points with a series of triangles.  Eight vertices total.  In order for the effect to render properly, set the prop's blend mode to (MOAIProp.GL_SRC_ALPHA, MOAIProp.GL_ONE_MINUS_SRC_ALPHA ), or to MOAIProp.BLEND_ADD against a black background.
 
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@opt	number solidWidth default 1.0
	@opt	number blurMargin default 1.0
	@out	nil
 */
int MOAIDraw::_drawAntialiasedLineSegment( lua_State *L ){
	MOAILuaState state(L);
	
	float x0 = state.GetValue < float > (1, 0.0f);
	float y0 = state.GetValue < float > (2, 0.0f);
	float x1 = state.GetValue < float > (3, 0.0f);
	float y1 = state.GetValue < float > (4, 0.0f);
	float solidWidth = state.GetValue < float > (5, 1.0f);
	float blurMargin = state.GetValue < float > (6, 1.0f);
	
	MOAIDraw::DrawAntiAliasedLineSegment(x0, y0, x1, y1, solidWidth, blurMargin);
	
	return 0;
}

//----------------------------------------------------------------//
/** @name	drawBeveledCorner
	@text   Takes the x and y coordinates of three points and an optional line width.  The corner will appear at the second of three points.  Uses a triangle fan, or a series of triangle strips if antialiased (blurMargin parameter has positive value).  May render incorrectly if the line segments come together at a very small angle.  Alpha will render correctly under the blend mode (MOAIProp.GL_SRC_ALPHA, MOAIProp.GL_ONE_MINUS_SRC_ALPHA).
 
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@in		number x2
	@in		number y2
	@opt	number lineWidth default 1.0
	@opt	number blurMargin default 0.0
	@out	nil
 
*/
int MOAIDraw::_drawBeveledCorner(lua_State *L){
	MOAILuaState state ( L );
	
	float x0 = state.GetValue < float > (1, 0.0f);
	float y0 = state.GetValue < float > (2, 0.0f);
	float x1 = state.GetValue < float > (3, 0.0f);
	float y1 = state.GetValue < float > (4, 0.0f);
	float x2 = state.GetValue < float > (5, 0.0f);
	float y2 = state.GetValue < float > (6, 0.0f);
	float lineWidth = state.GetValue < float > (7, 1.0f);
	float blurMargin = state.GetValue <float> (8, 0.0f);
	
	MOAIDraw::DrawBeveledCorner(x0, y0, x1, y1, x2, y2, lineWidth, blurMargin);
	
	return 0;
}

//----------------------------------------------------------------//
/** @name	drawBeveledLines
	@text	when provided with three or more points, creates line segments that meet at corners.
 
	@in		table vertices
	@in		number lineWidth
	@opt	number blurMargin	default to 1.0
	@out	nil
*/
int MOAIDraw::_drawBeveledLines(lua_State *L){
	
	MOAILuaState state( L );
	
	float lineWidth = state.GetValue < float > (2, 1.0f);
	float blurMargin = state.GetValue < float > (3, 1.0f);
	
	MOAIDraw::DrawBeveledLines(L, lineWidth, blurMargin);
	
	return 0;
}

//----------------------------------------------------------------//
/** @name	drawBezierCurve
	@text	Draw a Bezier curve. Endpoints at x0y0 and x1y1.  Control points at cx0cy0 and cx1cy1.
 
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@in		number cx0
	@in		number cy0
	@in		number cx1
	@in		number cy0
	@opt	number steps
	@out	nil
*/
int MOAIDraw::_drawBezierCurve(lua_State *L){
	MOAILuaState state ( L );
	
	float x0 = state.GetValue < float > (1, 0.0f);
	float y0 = state.GetValue < float > (2, 0.0f);
	float x1 = state.GetValue < float > (3, 0.0f);
	float y1 = state.GetValue < float > (4, 0.0f);
	float cx0 = state.GetValue < float > (5, x0);
	float cy0 = state.GetValue < float > (6, y0);
	float cx1 = state.GetValue < float > (7, x1);
	float cy1 = state.GetValue < float > (8, y1);
	u32	steps = state.GetValue < u32 > (9, DEFAULT_ELLIPSE_STEPS);
	
	MOAIDraw::DrawBezierCurve(x0, y0, x1, y1, cx0, cy0, cx1, cy1, steps);
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	drawBoxOutline
	@text	Draw a box outline.
	
	@in		number x0
	@in		number y0
	@in		number z0
	@in		number x1
	@in		number y1
	@in		number z1
	@out	nil
*/
int MOAIDraw::_drawBoxOutline ( lua_State* L ) {

	MOAILuaState state ( L );

	USBox box;
	box.mMin.mX = state.GetValue < float >( 1, 0.0f );
	box.mMin.mY = state.GetValue < float >( 2, 0.0f );
	box.mMin.mZ = state.GetValue < float >( 3, 0.0f );
	box.mMax.mX = state.GetValue < float >( 4, box.mMin.mX );
	box.mMax.mY = state.GetValue < float >( 5, box.mMin.mY );
	box.mMax.mZ = state.GetValue < float >( 6, box.mMin.mZ );
	MOAIDraw::DrawBoxOutline(box);
	return 0;

}

//----------------------------------------------------------------//
/** @name	drawCardinalSpline
	@text	Draw a cardinal spline curve. Endpoints at x0y0 and x1y1.  Control points at cx0cy0 and cx1cy1.  Tension can range from -1 to 1.  A tension of 0.0 gives a Catmull-Rom curve.
 
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@in		number cx0
	@in		number cy0
	@in		number cx1
	@in		number cy0
	@opt	number tension
	@opt	number steps
	@out	nil
 */
int MOAIDraw::_drawCardinalSpline( lua_State* L ){
	
	MOAILuaState state ( L );
	
	float x0 = state.GetValue < float > (1, 0.0f);
	float y0 = state.GetValue < float > (2, 0.0f);
	float x1 = state.GetValue < float > (3, 0.0f);
	float y1 = state.GetValue < float > (4, 0.0f);
	float cx0 = state.GetValue < float > (5, x0);
	float cy0 = state.GetValue < float > (6, y0);
	float cx1 = state.GetValue < float > (7, x1);
	float cy1 = state.GetValue < float > (8, y1);
	float tension = state.GetValue < float > (9, 0.0f);
	u32	steps = state.GetValue < u32 > (10, DEFAULT_ELLIPSE_STEPS);
	
	MOAIDraw::DrawCardinalSpline(x0, y0, x1, y1, cx0, cy0, cx1, cy1, tension, steps);
	return 0;
}


//----------------------------------------------------------------//
/** @name	drawCatmullRomCurve
	@text	Draw a Catmull-Rom curve. Endpoints at x0y0 and x1y1.  Control points at cx0cy0 and cx1cy1.  Operates differently from Bezier curve.
 
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@in		number cx0
	@in		number cy0
	@in		number cx1
	@in		number cy0
	@opt	number steps
	@out	nil
*/
int MOAIDraw::_drawCatmullRomCurve( lua_State* L ){
	
	MOAILuaState state ( L );
	
	float x0 = state.GetValue < float > (1, 0.0f);
	float y0 = state.GetValue < float > (2, 0.0f);
	float x1 = state.GetValue < float > (3, 0.0f);
	float y1 = state.GetValue < float > (4, 0.0f);
	float cx0 = state.GetValue < float > (5, x0);
	float cy0 = state.GetValue < float > (6, y0);
	float cx1 = state.GetValue < float > (7, x1);
	float cy1 = state.GetValue < float > (8, y1);
	u32	steps = state.GetValue < u32 > (9, DEFAULT_ELLIPSE_STEPS);
	
	MOAIDraw::DrawCatmullRomCurve(x0, y0, x1, y1, cx0, cy0, cx1, cy1, steps);
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	drawCircle
	@text	Draw a circle.
	
	@in		number x
	@in		number y
	@in		number r
	@in		number steps
	@out	nil
*/
int MOAIDraw::_drawCircle ( lua_State* L ) {

	MOAILuaState state ( L );
	
	float x0	= state.GetValue < float >( 1, 0.0f );
	float y0	= state.GetValue < float >( 2, 0.0f );
	float r		= state.GetValue < float >( 3, 0.0f );
	u32 steps	= state.GetValue < u32 >( 4, DEFAULT_ELLIPSE_STEPS );

	MOAIDraw::DrawEllipseOutline ( x0, y0, r, r, steps );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	drawEllipse
	@text	Draw an ellipse.
	
	@in		number x
	@in		number y
	@in		number xRad
	@in		number yRad
	@in		number steps
	@out	nil
*/
int MOAIDraw::_drawEllipse ( lua_State* L ) {

	MOAILuaState state ( L );
	
	float x		= state.GetValue < float >( 1, 0.0f );
	float y		= state.GetValue < float >( 2, 0.0f );
	float xRad	= state.GetValue < float >( 3, 0.0f );
	float yRad	= state.GetValue < float >( 4, 0.0f );
	
	u32 steps = state.GetValue < u32 >( 5, DEFAULT_ELLIPSE_STEPS );

	MOAIDraw::DrawEllipseOutline ( x, y, xRad, yRad, steps );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDraw::_drawGrid ( lua_State* L ) {
	UNUSED ( L );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	drawLine
	@text	Draw a line.
	
	@in		...		List of vertices (x, y) or an array of vertices
					{ x0, y0, x1, y1, ... , xn, yn }
	@out	nil
*/
int MOAIDraw::_drawLine ( lua_State* L ) {

	if ( lua_istable ( L, -1 ) ) {
		MOAIDraw::DrawLuaArray( L, GL_LINE_STRIP );
	} else {
		MOAIDraw::DrawLuaParams( L, GL_LINE_STRIP );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	drawPoints
	@text	Draw a list of points.
	
	@in		...		List of vertices (x, y) or an array of vertices
					{ x0, y0, x1, y1, ... , xn, yn }
	@out	nil
*/
int MOAIDraw::_drawPoints ( lua_State* L ) {
	
	if ( lua_istable ( L, -1 ) ) {
		MOAIDraw::DrawLuaArray( L, GL_POINTS );
	} else {
		MOAIDraw::DrawLuaParams( L, GL_POINTS );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	drawRay
	@text	Draw a ray.
	
	@in		number x
	@in		number y
	@in		number dx
	@in		number dy
	@out	nil
*/
int MOAIDraw::_drawRay ( lua_State* L ) {

	MOAILuaState state ( L );
	
	float x		= state.GetValue < float >( 1, 0.0f );
	float y		= state.GetValue < float >( 2, 0.0f );
	float dx	= state.GetValue < float >( 3, 0.0f );
	float dy	= state.GetValue < float >( 4, 0.0f );

	MOAIDraw::DrawRay ( x, y, dx, dy );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	drawRect
	@text	Draw a rectangle.
	
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@out	nil
*/
int MOAIDraw::_drawRect ( lua_State* L ) {

	MOAILuaState state ( L );
	
	float x0 = state.GetValue < float >( 1, 0.0f );
	float y0 = state.GetValue < float >( 2, 0.0f );
	float x1 = state.GetValue < float >( 3, 0.0f );
	float y1 = state.GetValue < float >( 4, 0.0f );

	MOAIDraw::DrawRectOutline ( x0, y0, x1, y1 );
	return 0;
}
//----------------------------------------------------------------//
/** @name	fillCenteredRectangularGradient
	@text	Draw a rectangle with the edges one color and a line
 
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@in		number centerR		red of central color
	@in		number centerG		green of central color
	@in		number centerB		blue of central color
	@in		number centerA		alpha of central color
	@in		number edgeR		red of outer color
	@in		number edgeG		green of outer color
	@in		number edgeB		blue of outer color
	@in		number edgeA		alpha of outer color
	@opt	number innerWidth	Width of rect in central color. Default 0.
	@opt	number innerHeight	Height of rect in central color. Default 0.
	@out	nil
 */

int MOAIDraw::_fillCenteredRectangularGradient ( lua_State *L ) {
	
	MOAILuaState state ( L );
	
	float x0 = state.GetValue < float >( 1, 0.0f );
	float y0 = state.GetValue < float >( 2, 0.0f );
	float x1 = state.GetValue < float >( 3, 0.0f );
	float y1 = state.GetValue < float >( 4, 0.0f );
	
	float centerR = state.GetValue < float > (5, 1.0f);
	float centerG = state.GetValue < float > (6, 1.0f);
	float centerB = state.GetValue < float > (7, 1.0f);
	float centerA = state.GetValue < float > (8, 1.0f);
	
	float edgeR = state.GetValue < float > (9,  1.0f);
	float edgeG = state.GetValue < float > (10, 1.0f);
	float edgeB = state.GetValue < float > (11, 1.0f);
	float edgeA = state.GetValue < float > (12, 1.0f);
	
	float innerWidth = state.GetValue < float > (13, 0.0f);
	float innerHeight = state.GetValue < float > (14, 0.0f);
	
	USColorVec centerColor(centerR, centerG, centerB, centerA);
	USColorVec edgeColor(edgeR, edgeG, edgeB, edgeA);
	
	MOAIDraw::DrawRectCenteredGradientFill(x0, y0, x1, y1, centerColor, edgeColor, innerWidth, innerHeight);
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	fillCircle
	@text	Draw a filled circle.
	
	@in		number x
	@in		number y
	@in		number r
	@in		number steps
	@out	nil
*/
int MOAIDraw::_fillCircle ( lua_State* L ) {

	MOAILuaState state ( L );
	
	float x0	= state.GetValue < float >( 1, 0.0f );
	float y0	= state.GetValue < float >( 2, 0.0f );
	float r		= state.GetValue < float >( 3, 0.0f );
	u32 steps	= state.GetValue < u32 >( 4, DEFAULT_ELLIPSE_STEPS );

	MOAIDraw::DrawEllipseFill ( x0, y0, r, r, steps );
	return 0;
}
//----------------------------------------------------------------//
/** @name	fillCircularGradient
	@text	Draw a filled circle with the outer color fading to the central color.
	
	@in		number x		x-coordinate of circle
	@in		number y		y-coordinate of circle
	@in		number r		radius of circle
	@in		number steps	
	@in		number centerR	red of central color
	@in		number centerG	green of central color
	@in		number centerB  blue of central color
	@in		number centerA	alpha of central color
	@in		number edgeR	red of outer color
	@in		number edgeG	green of outer color
	@in		number edgeB	blue of outer color
	@in		number edgeA	alpha of outer color
	@out	nil
 */

int MOAIDraw::_fillCircularGradient( lua_State *L ){
	MOAILuaState state ( L );
	
	float x0	= state.GetValue < float >( 1, 0.0f );
	float y0	= state.GetValue < float >( 2, 0.0f );
	float r		= state.GetValue < float >( 3, 0.0f );
	u32 steps	= state.GetValue < u32 >( 4, DEFAULT_ELLIPSE_STEPS );
	
	float centerR = state.GetValue < float > (5, 1.0f);
	float centerG = state.GetValue < float > (6, 1.0f);
	float centerB = state.GetValue < float > (7, 1.0f);
	float centerA = state.GetValue < float > (8, 1.0f);
	
	float edgeR = state.GetValue < float > (9, 1.0f);
	float edgeG = state.GetValue < float > (10, 1.0f);
	float edgeB = state.GetValue < float > (11, 1.0f);
	float edgeA = state.GetValue < float > (12, 1.0f);
	
	USColorVec centerColor(centerR, centerG, centerB, centerA);
	USColorVec edgeColor(edgeR, edgeG, edgeB, edgeA);
	
	MOAIDraw::DrawEllipticalGradientFill(x0, y0, r, r, steps, centerColor, edgeColor);
	
	return 0;
}
//----------------------------------------------------------------//
/**	@name	fillEllipse
	@text	Draw a filled ellipse.
	
	@in		number x
	@in		number y
	@in		number xRad
	@in		number yRad
	@in		number steps
	@out	nil
*/
int MOAIDraw::_fillEllipse ( lua_State* L ) {

	MOAILuaState state ( L );
	
	float x		= state.GetValue < float >( 1, 0.0f );
	float y		= state.GetValue < float >( 2, 0.0f );
	float xRad	= state.GetValue < float >( 3, 0.0f );
	float yRad	= state.GetValue < float >( 4, 0.0f );
	
	u32 steps = state.GetValue < u32 >( 5, DEFAULT_ELLIPSE_STEPS );

	MOAIDraw::DrawEllipseFill ( x, y, xRad, yRad, steps );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	fillEllipticalGradient
	@text	Draw a filled ellipse with the outer color fading to the central color.
 
	@in		number x		x-coordinate of ellipse
	@in		number y		y-coordinate of ellipse
	@in		number xRad		radius of ellipse on x-axis
	@in		number yRad r	adius of ellipse on y-axis
	@in		number steps
	@in		number centerR	red of central color
	@in		number centerG	green of central color
	@in		number centerB  blue of central color
	@in		number centerA	alpha of central color
	@in		number edgeR	red of outer color
	@in		number edgeG	green of outer color
	@in		number edgeB	blue of outer color
	@in		number edgeA	alpha of outer color
 */
int MOAIDraw::_fillEllipticalGradient( lua_State *L ){
	MOAILuaState state ( L );
	
	float x		= state.GetValue < float >( 1, 0.0f );
	float y		= state.GetValue < float >( 2, 0.0f );
	float xRad	= state.GetValue < float >( 3, 0.0f );
	float yRad	= state.GetValue < float >( 4, 0.0f );
	
	u32 steps = state.GetValue < u32 >( 5, DEFAULT_ELLIPSE_STEPS );
	
	float centerR = state.GetValue < float > (6, 1.0f);
	float centerG = state.GetValue < float > (7, 1.0f);
	float centerB = state.GetValue < float > (8, 1.0f);
	float centerA = state.GetValue < float > (9, 1.0f);
	
	float edgeR = state.GetValue < float > (10, 1.0f);
	float edgeG = state.GetValue < float > (11, 1.0f);
	float edgeB = state.GetValue < float > (12, 1.0f);
	float edgeA = state.GetValue < float > (13, 1.0f);
	
	USColorVec centerColor(centerR, centerG, centerB, centerA);
	USColorVec edgeColor(edgeR, edgeG, edgeB, edgeA);
	
	MOAIDraw::DrawEllipticalGradientFill(x, y, xRad, yRad, steps, centerColor, edgeColor);
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	fillFan
	@text	Draw a filled fan.
	
	@in		...		List of vertices (x, y) or an array of vertices
					{ x0, y0, x1, y1, ... , xn, yn }
	@out	nil
*/
int MOAIDraw::_fillFan ( lua_State* L ) {

	if ( lua_istable ( L, -1 ) ) {
		MOAIDraw::DrawLuaArray( L, GL_TRIANGLE_FAN );
	} else {
		MOAIDraw::DrawLuaParams( L, GL_TRIANGLE_FAN );
	}
	return 0;
}
//----------------------------------------------------------------//
/** @name	fillHorizontalRectangularGradient
	@text	Draw a filled rectangle with a gradient between two colors
			going from left to right.
 
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@in		number r1	Left color red
	@in		number g1	
	@in		number b1
	@in		number a1
	@in		number r2	Right color red
	@in		number g2
	@in		number b2
	@in		number a2
	@out	nil
*/
int MOAIDraw::_fillHorizontalRectangularGradient(lua_State *L){
	
	MOAILuaState state ( L );
	
	float x0 = state.GetValue < float >( 1, 0.0f );
	float y0 = state.GetValue < float >( 2, 0.0f );
	float x1 = state.GetValue < float >( 3, 0.0f );
	float y1 = state.GetValue < float >( 4, 0.0f );
	
	float r1, g1, b1, a1, r2, g2, b2, a2;
	if (state.GetTop() >= 11){
		r1 = state.GetValue <float> ( 5, 1.0f );
		g1 = state.GetValue <float> ( 6, 1.0f );
		b1 = state.GetValue <float> ( 7, 1.0f );
		a1 = state.GetValue <float> ( 8, 1.0f );
		
		r2 = state.GetValue <float> ( 9, 1.0f );
		g2 = state.GetValue <float> ( 10, 1.0f );
		b2 = state.GetValue <float> ( 11, 1.0f );
		a2 = state.GetValue <float> ( 12, 1.0f );
	}
	else{
		r1 = state.GetValue <float> ( 5, 1.0f );
		g1 = state.GetValue <float> ( 6, 1.0f );
		b1 = state.GetValue <float> ( 7, 1.0f );
		a1 = 1.0f;
		
		r2 = state.GetValue <float> ( 8, 1.0f );
		g2 = state.GetValue <float> ( 9, 1.0f );
		b2 = state.GetValue <float> ( 10, 1.0f );
		a2 = 1.0f;
	}
	USColorVec leftColor(r1, g1, b1, a1);
	USColorVec rightColor(r2, g2, b2, a2);
	
	MOAIDraw::DrawRectHorizontalGradientFill(x0, y0, x1, y1, leftColor, rightColor);
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	fillRect
	@text	Draw a filled rectangle.
	
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@out	nil
*/
int MOAIDraw::_fillRect ( lua_State* L ) {

	MOAILuaState state ( L );
	
	float x0 = state.GetValue < float >( 1, 0.0f );
	float y0 = state.GetValue < float >( 2, 0.0f );
	float x1 = state.GetValue < float >( 3, 0.0f );
	float y1 = state.GetValue < float >( 4, 0.0f );

	MOAIDraw::DrawRectFill ( x0, y0, x1, y1 );
	return 0;
}
//----------------------------------------------------------------//
/** @name	fillVerticalRectangularGradient
	@text	Draw a filled rectangle with a gradient between two colors
			going from top to bottom.
 
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@in		number r1	Top color red
	@in		number g1
	@in		number b1
	@in		number a1
	@in		number r2	Bottom color red
	@in		number g2
	@in		number b2
	@in		number a2
	@out	nil
 */

int MOAIDraw::_fillVerticalRectangularGradient ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	float x0 = state.GetValue < float >( 1, 0.0f );
	float y0 = state.GetValue < float >( 2, 0.0f );
	float x1 = state.GetValue < float >( 3, 0.0f );
	float y1 = state.GetValue < float >( 4, 0.0f );
	
	float r1, g1, b1, a1, r2, g2, b2, a2;
	if (state.GetTop() >= 11){
		r1 = state.GetValue <float> ( 5, 1.0f );
		g1 = state.GetValue <float> ( 6, 1.0f );
		b1 = state.GetValue <float> ( 7, 1.0f );
		a1 = state.GetValue <float> ( 8, 1.0f );
		
		r2 = state.GetValue <float> ( 9, 1.0f );
		g2 = state.GetValue <float> ( 10, 1.0f );
		b2 = state.GetValue <float> ( 11, 1.0f );
		a2 = state.GetValue <float> ( 12, 1.0f );
	}
	else{
		r1 = state.GetValue <float> ( 5, 1.0f );
		g1 = state.GetValue <float> ( 6, 1.0f );
		b1 = state.GetValue <float> ( 7, 1.0f );
		a1 = 1.0f;
		
		r2 = state.GetValue <float> ( 8, 1.0f );
		g2 = state.GetValue <float> ( 9, 1.0f );
		b2 = state.GetValue <float> ( 10, 1.0f );
		a2 = 1.0f;
	}
	
	USColorVec topColor(r1, g1, b1, a1);
	USColorVec bottomColor(r2, g2, b2, a2);
	
	MOAIDraw::DrawRectVerticalGradientFill ( x0, y0, x1, y1, topColor, bottomColor );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	drawTexture
	@text	Draw a filled rectangle.
	
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@in		MOAITexture texture
	@out	nil
*/
int MOAIDraw::_drawTexture ( lua_State* L ) {

	MOAILuaState state ( L );
	
	float x0 = state.GetValue < float >( 1, 0.0f );
	float y0 = state.GetValue < float >( 2, 0.0f );
	float x1 = state.GetValue < float >( 3, 0.0f );
	float y1 = state.GetValue < float >( 4, 0.0f );
	MOAITexture* texture = (MOAITexture*)MOAITexture::AffirmTexture ( state, 5 );

	MOAIDraw::DrawTexture ( x0, y0, x1, y1, texture );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	drawText
	@text	Draws a string.
	
	@in		MOAIFont font
	@in		number size of the font
	@in		string text
	@in		number x (top-left position)
	@in		number y (top-left position)
	@in		number scale
	@in		number shadow offset x
	@in		number shadow offset y
	@out	nil
*/
int MOAIDraw::_drawText ( lua_State* L ) {

	MOAILuaState state ( L );

	// TODO	
	//cc8* text = lua_tostring ( state, 3 );
	//if ( text ) {

	//	float x = state.GetValue < float >( 4, 0.0f );
	//	float y = state.GetValue < float >( 5, 0.0f );
	//	float scale = state.GetValue < float >( 6, 1.0f );

	//	float shadowOffsetX = state.GetValue < float >( 7, 0.0f );
	//	float shadowOffsetY = state.GetValue < float >( 8, 0.0f );

	//	MOAIFont* font = state.GetLuaObject < MOAIFont >( 1, true );
	//	if ( font ) {

	//		float fontSize = state.GetValue < float >( 2, font->GetDefaultSize () );

	//		MOAIDraw::DrawText ( text, x, y, scale, *font, fontSize, shadowOffsetX, shadowOffsetY, 0, 0 );
	//	}
	//}

	return 0;
}

//================================================================//
// MOAIDraw
//================================================================//

//----------------------------------------------------------------//
void MOAIDraw::Bind () {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	gfxDevice.SetTexture ();
	gfxDevice.SetShaderPreset ( MOAIShaderMgr::LINE_SHADER );
	gfxDevice.SetVertexPreset ( MOAIVertexFormatMgr::XYZWC );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawAnimCurve ( MOAIAnimCurve& curve, u32 resolution ) {

	curve.Draw ( resolution );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawAxisGrid ( USVec2D loc, USVec2D vec, float size ) {

	USMatrix4x4 mtx = MOAIGfxDevice::Get ().GetViewProjMtx ();
	
	USMatrix4x4 invMtx;
	invMtx.Inverse ( mtx );
	
	// Set the axis to the grid length so we can get the length back post-transform
	vec.SetLength ( size );
	
	mtx.Transform ( loc );
	mtx.TransformVec ( vec );

	// Get the axis unit vector
	USVec2D norm = vec;
	size = norm.NormSafe ();
	
	// Get the axis normal
	USVec2D perpNorm ( norm.mY, -norm.mX );
	
	// Project the corners of the viewport onto the axis to get the mix/max bounds
	float dot;
	float min;
	float max;
	
	USVec2D corner;
	
	// left, top
	corner.Init ( -1.0f, 1.0f );
	corner.Sub ( loc );
	dot = norm.Dot ( corner );
	
	min = dot;
	max = dot;
	
	// right, top
	corner.Init ( 1.0f, 1.0f );
	corner.Sub ( loc );
	dot = norm.Dot ( corner );
	
	min = ( dot < min ) ? dot : min;
	max = ( dot > max ) ? dot : max;
	
	// right, bottom
	corner.Init ( 1.0f, -1.0f );
	corner.Sub ( loc );
	dot = norm.Dot ( corner );
	
	min = ( dot < min ) ? dot : min;
	max = ( dot > max ) ? dot : max;
	
	// left, bottom
	corner.Init ( -1.0f, -1.0f );
	corner.Sub ( loc );
	dot = norm.Dot ( corner );
	
	min = ( dot < min ) ? dot : min;
	max = ( dot > max ) ? dot : max;
	
	// Get the start andstop grids
	s32 start = ( s32 )( min / size ) - 1;
	s32 stop = ( s32 )( max / size ) + 1;
	
	// Set the pen to the first...
	USVec2D pen = norm;
	pen.Scale (( float )start * size );
	pen.Add ( loc );
	
	// Step along the axis to draw perpendicular grid lines
	USRect viewRect;
	viewRect.Init ( -1.0f, -1.0f, 1.0f, 1.0f );
	
	for ( ; start < stop; ++start ) {
		
		USVec2D p0;
		USVec2D p1;
		
		if ( viewRect.GetIntersection ( pen, perpNorm, p0, p1 )) {
			
			invMtx.Transform ( p0 );
			invMtx.Transform ( p1 );
			
			MOAIDraw::DrawLine ( p0, p1 );
		}
		
		pen.Add ( vec );
	}
}

//----------------------------------------------------------------//
void MOAIDraw::DrawBeveledCorner(float x0, float y0, float x1, float y1, float x2, float y2, float lineWidth, float blurMargin){
	
	// get the line vectors
	USVec2D vecL1;
	vecL1.Init(x1 - x0, y1 - y0);
	if (vecL1.LengthSquared() == 0.0) {
		return;
	}
	
	vecL1.Norm();
	
	
	USVec2D vecL2;
	vecL2.Init(x2 - x1, y2 - y1);
	if (vecL2.LengthSquared() == 0.0) {
		return;
	}
	vecL2.Norm();
	
	// half line width to multiply with vec for determining point locations
	float lw = lineWidth / 2;
	
	// L1 == (x0, y0) to (x1, y1)
	// L2 == (x1, y1) to (x2, y2)
	// create two lines parallel to each L1 and L2 a constant distant away
	
	USVec2D p1; // "north" of x0, y0
	p1.Init(x0 + lw * vecL1.mY, y0 - lw * vecL1.mX);
	
	USVec2D p2; // "south" of x0, y0
	p2.Init(x0 - lw * vecL1.mY, y0 + lw * vecL1.mX);
	
	USVec2D p5; // "north" of x2, y2
	p5.Init(x2 + lw * vecL2.mY, y2 - lw * vecL2.mX);
	
	USVec2D p6; // "south" of x2, y2
	p6.Init(x2 - lw * vecL2.mY, y2 + lw * vecL2.mX);
	
	USVec2D p3a; // "north" of x1, y1 using vecL1
	p3a.Init(x1 + lw * vecL1.mY, y1 - lw * vecL1.mX);
	
	USVec2D p4a; // "south" of x1, y1 using vecL1
	p4a.Init( x1 - lw * vecL1.mY, y1 + lw * vecL1.mX );
	
	USVec2D p3b; // "north" of x1, y1 using vecL2
	p3b.Init(x1 + lw * vecL2.mY, y1 - lw * vecL2.mX);
	
	USVec2D p4b; // "south" of x1, y1 (intersection)
	p4b.Init( x1 - lw * vecL2.mY, y1 + lw * vecL2.mX );
	
	USVec2D p3; // intersection of p1-p3a and p3b-p5
	bool i1;
	USVec2D::GetLineIntersection(p1, p3a, p3b, p5, &p3, &i1);
	
	USVec2D p4; // intersection of p2-p4a and p4b-p6
	bool i2;
	USVec2D::GetLineIntersection(p2, p4a, p4b, p6, &p4, &i2);
	
	// assign p3 and p4 if GetLineIntersection() did not return a point in either case.
	if (!(i1 && i2)) {
		
		// co-linear segments
		p3.Init(p3a);
		p4.Init(p4a);
	}
	
	MOAIGfxDevice &gfxDevice = MOAIGfxDevice::Get();
	// anti-aliased total of twelve points and two prims
	if (blurMargin > 0.0f) {
		
		float bw = lw + blurMargin;
		
		USVec2D p1z; // "north" of x0, y0 (blur margin)
		p1z.Init(x0 + bw * vecL1.mY, y0 - bw * vecL1.mX);
		
		USVec2D p2z; // "south" of x0, y0 (blur margin)
		p2z.Init(x0 - bw * vecL1.mY, y0 + bw * vecL1.mX);
		
		
		USVec2D p5z; // "north" of x2, y2
		p5z.Init(x2 + bw * vecL2.mY, y2 - bw * vecL2.mX);
		
		USVec2D p6z; // "south" of x2, y2
		p6z.Init(x2 - bw * vecL2.mY, y2 + bw * vecL2.mX);
		
		USVec2D p3za; // "north" of x1, y1 using vecL1
		p3za.Init(x1 + bw * vecL1.mY, y1 - bw * vecL1.mX);
		
		USVec2D p4za; // "south" of x1, y1 using vecL1
		p4za.Init( x1 - bw * vecL1.mY, y1 + bw * vecL1.mX );
		
		USVec2D p3zb; // "north" of x1, y1 using vecL2
		p3zb.Init(x1 + bw * vecL2.mY, y1 - bw * vecL2.mX);
		
		USVec2D p4zb; // "south" of x1, y1 (intersection)
		p4zb.Init( x1 - bw * vecL2.mY, y1 + bw * vecL2.mX );
		
		
		USVec2D p3z; // intersection of p1z-p3za and p3zb-p5z
		USVec2D::GetLineIntersection(p1z, p3za, p3zb, p5z, &p3z, &i1);
		
		USVec2D p4z; // intersection of p2z-p4za and p4zb-p6z
		USVec2D::GetLineIntersection(p2z, p4za, p4zb, p6z, &p4z, &i2);
		
		if (!(i1 && i2)) {
			
			// co-linear segments
			p3z.Init(p3za);
			p4z.Init(p4za);
		}
		
		// get pen color
		USColorVec penColor = gfxDevice.GetPenColor();
		// make transparent color
		USColorVec transColor(penColor);
		transColor.mA = 0.0f;
		
		// render the L1 segment
		gfxDevice.BeginPrim(GL_TRIANGLE_STRIP);
		
		gfxDevice.SetPenColor(transColor);
		
		// write p1z
		gfxDevice.WriteVtx ( p1z.mX, p1z.mY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// write p3z
		gfxDevice.WriteVtx ( p3z.mX, p3z.mY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		gfxDevice.SetPenColor(penColor);
		
		// write p1
		gfxDevice.WriteVtx ( p1.mX, p1.mY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// write p3
		gfxDevice.WriteVtx ( p3.mX, p3.mY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// write p2
		gfxDevice.WriteVtx ( p2.mX, p2.mY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// write p4
		gfxDevice.WriteVtx ( p4.mX, p4.mY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		gfxDevice.SetPenColor(transColor);
		
		// write p2z
		gfxDevice.WriteVtx ( p2z.mX, p2z.mY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// write p4z
		gfxDevice.WriteVtx ( p4z.mX, p4z.mY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		
		gfxDevice.EndPrim();
		
		// render the L2 segment
		gfxDevice.BeginPrim(GL_TRIANGLE_STRIP);
		
		gfxDevice.SetPenColor(transColor);
		
		// write p3z
		gfxDevice.WriteVtx ( p3z.mX, p3z.mY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// write p5z
		gfxDevice.WriteVtx ( p5z.mX, p5z.mY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		gfxDevice.SetPenColor(penColor);
		// write p3
		gfxDevice.WriteVtx ( p3.mX, p3.mY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// write p5
		gfxDevice.WriteVtx ( p5.mX, p5.mY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// write p4
		gfxDevice.WriteVtx ( p4.mX, p4.mY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// write p6
		gfxDevice.WriteVtx ( p6.mX, p6.mY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		
		gfxDevice.SetPenColor(transColor);
		
		// write p4z
		gfxDevice.WriteVtx ( p4z.mX, p4z.mY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// write p6z
		gfxDevice.WriteVtx ( p6z.mX, p6z.mY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		gfxDevice.EndPrim();
		
		gfxDevice.SetPenColor(penColor);
		
		return;
	}
	// aliased version with six points and one prim.
	
	
	
	gfxDevice.BeginPrim(GL_TRIANGLE_FAN);
		// write p3
		gfxDevice.WriteVtx ( p3.mX, p3.mY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
	
		// write p1
		gfxDevice.WriteVtx ( p1.mX, p1.mY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
	
		// write p2
		gfxDevice.WriteVtx ( p2.mX, p2.mY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
	
		// write p4
		gfxDevice.WriteVtx ( p4.mX, p4.mY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
	
		// write p6
		gfxDevice.WriteVtx ( p6.mX, p6.mY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
	
		// write p5
		gfxDevice.WriteVtx ( p5.mX, p5.mY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
	
	
	gfxDevice.EndPrim();
}

//----------------------------------------------------------------//
void MOAIDraw::DrawBeveledLines(lua_State *L, float lineWidth, float blurMargin){
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	MOAILuaState state ( L );
	
	float p0x, p0y, p1x, p1y, p2x, p2y;
	USVec2D r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, l1, l1n, l2, l2n, q0, q1;
	bool i1, i2;
	
	float lw = lineWidth / 2;
	
	// table at index 1
	const u32 chunk_size = 8;
	
	USLeanArray<float> vertexArray;
	vertexArray.Init(chunk_size);
	
	
	lua_pushnil(L);
	int counter = 0;
	while (lua_next(L, 1) != 0 ) {
		/* `key' is at index -2 and `value' at index -1 */
		u32 arraySize = vertexArray.Size();
		if(arraySize <= (u32) counter){
			vertexArray.Grow(arraySize + chunk_size);
		}
		
		// push value into vertex array
		vertexArray[counter] = state.GetValue <float> (-1, 0.0f);
		
		++counter;
		lua_pop(L, 1);
	}
	if (blurMargin > 0.0f) {
		USVec2D b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, q2, q3;
		bool i3, i4;
		float bw = lw + blurMargin;
		
		// get pen color
		USColorVec penColor = gfxDevice.GetPenColor();
		// make transparent color
		USColorVec transColor(penColor);
		transColor.mA = 0.0f;
		
		for (int i = 0; i < counter - 5; i += 2){
			p0x = vertexArray[i];
			p0y = vertexArray[i+1];
			p1x = vertexArray[i+2];
			p1y = vertexArray[i+3];
			p2x = vertexArray[i+4];
			p2y = vertexArray[i+5];
			
			// calculate line vectors
			l1.Init(p1x - p0x, p1y - p0y);
			if (l1.LengthSquared() == 0) {
				continue;
			}
			l1.Norm();
			l1n.Init(l1);
			l1n.Rotate90Anticlockwise();
			
			l2.Init(p2x - p1x, p2y - p1y);
			if (l2.LengthSquared() == 0) {
				continue;
			}
			l2.Norm();
			l2n.Init(l2);
			l2n.Rotate90Anticlockwise();
			
			
			// calculate render points
			if (i == 0) {
				r0.Init(p0x + lw * l1n.mX, p0y + lw * l1n.mY);
				r2.Init(p0x - lw * l1n.mX, p0y - lw * l1n.mY);
				
				b0.Init(p0x + bw * l1n.mX, p0y + bw * l1n.mY);
				b2.Init(p0x - bw * l1n.mX, p0y - bw * l1n.mY);
			}
			else{
				r0.Init(q0);
				r2.Init(q1);
				
				b0.Init(q2);
				b2.Init(q3);
			}
			
			r1.Init(p1x + lw * l1n.mX, p1y + lw * l1n.mY);
			b1.Init(p1x + bw * l1n.mX, p1y + bw * l1n.mY);
			r3.Init(p1x - lw * l1n.mX, p1y - lw * l1n.mY);
			b3.Init(p1x - bw * l1n.mX, p1y - bw * l1n.mY);
			
			r4.Init(p1x + lw * l2n.mX, p1y + lw * l2n.mY);
			b4.Init(p1x + bw * l2n.mX, p1y + bw * l2n.mY);
			
			r5.Init(p2x + lw * l2n.mX, p2y + lw * l2n.mY);
			b5.Init(p2x + bw * l2n.mX, p2y + bw * l2n.mY);
			
			r6.Init(p1x - lw * l2n.mX, p1y - lw * l2n.mY);
			b6.Init(p1x - bw * l2n.mX, p1y - bw * l2n.mY);
			
			r7.Init(p2x - lw * l2n.mX, p2y - lw * l2n.mY);
			b7.Init(p2x - bw * l2n.mX, p2y - bw * l2n.mY);
			
			
			// find intersection points
			USVec2D::GetLineIntersection(r0, r1, r4, r5, &r8, &i1);
			USVec2D::GetLineIntersection(r2, r3, r6, r7, &r9, &i2);
			USVec2D::GetLineIntersection(b0, b1, b4, b5, &b8, &i3);
			USVec2D::GetLineIntersection(b2, b3, b6, b7, &b9, &i4);
			// fallback for co-linear points
			if (!(i1 && i2 && i3 && i4)) {
				r8.Init(r1);
				r9.Init(r3);
				b8.Init(b1);
				b9.Init(b3);
			}
			// save points r8 and r9 for later use
			q0.Init(r8);
			q1.Init(r9);
			q2.Init(b8);
			q3.Init(b9);
			
			// render the first segment
			
			gfxDevice.BeginPrim(GL_TRIANGLE_STRIP);
			
			
			gfxDevice.SetPenColor(transColor);
			// write b0
			gfxDevice.WriteVtx(b0);
			gfxDevice.WriteFinalColor4b();
			// write b8
			gfxDevice.WriteVtx(b8);
			gfxDevice.WriteFinalColor4b();
			
			gfxDevice.SetPenColor(penColor);
			// write r0
			gfxDevice.WriteVtx(r0);
			gfxDevice.WriteFinalColor4b();
			
			// write r8
			gfxDevice.WriteVtx(r8);
			gfxDevice.WriteFinalColor4b();
			
			// write r2
			gfxDevice.WriteVtx(r2);
			gfxDevice.WriteFinalColor4b();
			
			// write r9
			gfxDevice.WriteVtx(r9);
			gfxDevice.WriteFinalColor4b();
			
			gfxDevice.SetPenColor(transColor);
			
			// write b2
			gfxDevice.WriteVtx(b2);
			gfxDevice.WriteFinalColor4b();
			 
			// write b9
			gfxDevice.WriteVtx(b9);
			gfxDevice.WriteFinalColor4b();
			
			
			gfxDevice.EndPrim();
			
			// render the second segment if at the end.
			if (i + 6 >= counter ) {
				
				gfxDevice.BeginPrim(GL_TRIANGLE_STRIP);
				
				//gfxDevice.SetPenColor(transColor);
				// write b8
				gfxDevice.WriteVtx(b8);
				gfxDevice.WriteFinalColor4b();
				
				// write b5
				gfxDevice.WriteVtx(b5);
				gfxDevice.WriteFinalColor4b();
				
				gfxDevice.SetPenColor(penColor);
				// write r8
				gfxDevice.WriteVtx(r8);
				gfxDevice.WriteFinalColor4b();
				
				// write r5
				gfxDevice.WriteVtx(r5);
				gfxDevice.WriteFinalColor4b();
				
				// write r9
				gfxDevice.WriteVtx(r9);
				gfxDevice.WriteFinalColor4b();
				
				// write r7
				gfxDevice.WriteVtx(r7);
				gfxDevice.WriteFinalColor4b();
				
				gfxDevice.SetPenColor(transColor);
				// write b9
				gfxDevice.WriteVtx(b9);
				gfxDevice.WriteFinalColor4b();
				
				// write b7
				gfxDevice.WriteVtx(b7);
				gfxDevice.WriteFinalColor4b();
				
				
				gfxDevice.EndPrim();
				
			}
			gfxDevice.SetPenColor(penColor);
		}
		
		return;
	}
	for (int i = 0; i < counter - 5; i += 2){
		p0x = vertexArray[i];
		p0y = vertexArray[i+1];
		p1x = vertexArray[i+2];
		p1y = vertexArray[i+3];
		p2x = vertexArray[i+4];
		p2y = vertexArray[i+5];
		
		// calculate line vectors
		l1.Init(p1x - p0x, p1y - p0y);
		if (l1.LengthSquared() == 0) {
			continue;
		}
		l1.Norm();
		l1n.Init(l1);
		l1n.Rotate90Anticlockwise();
		
		l2.Init(p2x - p1x, p2y - p1y);
		if (l2.LengthSquared() == 0) {
			continue;
		}
		l2.Norm();
		l2n.Init(l2);
		l2n.Rotate90Anticlockwise();
		
		
		// calculate render points
		if (i == 0) {
			r0.Init(p0x + lw * l1n.mX, p0y + lw * l1n.mY);
			r2.Init(p0x - lw * l1n.mX, p0y - lw * l1n.mY);
		}
		else{
			r0.Init(q0);
			r2.Init(q1);
		}
		
		r1.Init(p1x + lw * l1n.mX, p1y + lw * l1n.mY);
		r3.Init(p1x - lw * l1n.mX, p1y - lw * l1n.mY);
		
		r4.Init(p1x + lw * l2n.mX, p1y + lw * l2n.mY);
		r5.Init(p2x + lw * l2n.mX, p2y + lw * l2n.mY);
		r6.Init(p1x - lw * l2n.mX, p1y - lw * l2n.mY);
		r7.Init(p2x - lw * l2n.mX, p2y - lw * l2n.mY);
		
		
		// find intersection points
		USVec2D::GetLineIntersection(r0, r1, r4, r5, &r8, &i1);
		USVec2D::GetLineIntersection(r2, r3, r6, r7, &r9, &i2);
		// fallback for co-linear points
		if (!(i1 && i2)) {
			r8.Init(r1);
			r9.Init(r3);
		}
		// save points r8 and r9 for later use
		q0.Init(r8);
		q1.Init(r9);
		
		// render the first segment
		
		
		gfxDevice.BeginPrim(GL_TRIANGLE_STRIP);
		
		// write r0
		gfxDevice.WriteVtx(r0);
		gfxDevice.WriteFinalColor4b();
		
		// write r8
		gfxDevice.WriteVtx(r8);
		gfxDevice.WriteFinalColor4b();
		
		// write r2
		gfxDevice.WriteVtx(r2);
		gfxDevice.WriteFinalColor4b();
		
		// write r9
		gfxDevice.WriteVtx(r9);
		gfxDevice.WriteFinalColor4b();
		
		gfxDevice.EndPrim();
		 
		
		// render the second segment if at the end.
		if (i + 6 >= counter ) {
			gfxDevice.BeginPrim(GL_TRIANGLE_STRIP);
			
			// write r8
			gfxDevice.WriteVtx(r8);
			gfxDevice.WriteFinalColor4b();
			
			// write r5
			gfxDevice.WriteVtx(r5);
			gfxDevice.WriteFinalColor4b();
			
			// write r9
			gfxDevice.WriteVtx(r9);
			gfxDevice.WriteFinalColor4b();
			
			// write r7
			gfxDevice.WriteVtx(r7);
			gfxDevice.WriteFinalColor4b();
			
			gfxDevice.EndPrim();
			 
		}
		
	}
	
}

//----------------------------------------------------------------//
void MOAIDraw::DrawBoxOutline ( const USBox& box ) {

	MOAIDraw::DrawLine ( box.mMin.mX, box.mMin.mY, box.mMin.mZ, box.mMax.mX, box.mMin.mY, box.mMin.mZ );
	MOAIDraw::DrawLine ( box.mMin.mX, box.mMin.mY, box.mMin.mZ, box.mMin.mX, box.mMax.mY, box.mMin.mZ );
	MOAIDraw::DrawLine ( box.mMin.mX, box.mMin.mY, box.mMin.mZ, box.mMin.mX, box.mMin.mY, box.mMax.mZ );
	
	MOAIDraw::DrawLine ( box.mMax.mX, box.mMax.mY, box.mMax.mZ, box.mMin.mX, box.mMax.mY, box.mMax.mZ );
	MOAIDraw::DrawLine ( box.mMax.mX, box.mMax.mY, box.mMax.mZ, box.mMax.mX, box.mMin.mY, box.mMax.mZ );
	MOAIDraw::DrawLine ( box.mMax.mX, box.mMax.mY, box.mMax.mZ, box.mMax.mX, box.mMax.mY, box.mMin.mZ );
	
	MOAIDraw::DrawLine ( box.mMax.mX, box.mMin.mY, box.mMin.mZ, box.mMax.mX, box.mMax.mY, box.mMin.mZ );
	MOAIDraw::DrawLine ( box.mMax.mX, box.mMin.mY, box.mMin.mZ, box.mMax.mX, box.mMin.mY, box.mMax.mZ );

	MOAIDraw::DrawLine ( box.mMin.mX, box.mMax.mY, box.mMin.mZ, box.mMax.mX, box.mMax.mY, box.mMin.mZ );
	MOAIDraw::DrawLine ( box.mMin.mX, box.mMax.mY, box.mMin.mZ, box.mMin.mX, box.mMax.mY, box.mMax.mZ );
	
	MOAIDraw::DrawLine ( box.mMin.mX, box.mMin.mY, box.mMax.mZ, box.mMax.mX, box.mMin.mY, box.mMax.mZ );
	MOAIDraw::DrawLine ( box.mMin.mX, box.mMin.mY, box.mMax.mZ, box.mMin.mX, box.mMax.mY, box.mMax.mZ );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawBezierCurve(float x0, float y0, float x1, float y1, float cx0, float cy0, float cx1, float cy1, u32 steps){
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	
	float t = 0.0f;
	float tStep = 1.0f / (float) steps;
	
	gfxDevice.BeginPrim( GL_LINE_STRIP );
	
	for (u32 i = 0; i <= steps; ++i, t += tStep) {
		gfxDevice.WriteVtx(
			USCurve::Bezier1D(x0, cx0, cx1, x1, t),
			USCurve::Bezier1D(y0, cy0, cy1, y1, t),
			0.0f
		);
		gfxDevice.WriteFinalColor4b();
	}
	
	gfxDevice.EndPrim();
}

//----------------------------------------------------------------//
void MOAIDraw::DrawCardinalSpline(float x0, float y0, float x1, float y1, float cx0, float cy0, float cx1, float cy1, float tension, u32 steps){
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	
	float t = 0.0f;
	float tStep = 1.0f / (float) steps;
	
	gfxDevice.BeginPrim( GL_LINE_STRIP );
	for (u32 i = 0; i <= steps; ++i, t += tStep) {
		gfxDevice.WriteVtx(
			USCurve::CardinalSpline1D(cx0, x0, x1, cx1, tension, t),
			USCurve::CardinalSpline1D(cy0, y0, y1, cy1, tension, t),
			0.0f
		);
		gfxDevice.WriteFinalColor4b();
	}
	
	gfxDevice.EndPrim();
}

//----------------------------------------------------------------//
void MOAIDraw::DrawCatmullRomCurve(float x0, float y0, float x1, float y1, float cx0, float cy0, float cx1, float cy1, u32 steps){
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	
	float t = 0.0f;
	float tStep = 1.0f / (float) steps;
	
	gfxDevice.BeginPrim( GL_LINE_STRIP );
	for (u32 i = 0; i <= steps; ++i, t += tStep) {
		gfxDevice.WriteVtx(
			USCurve::CatmullRom1D(cx0, x0, x1, cx1, t),//USCurve::CardinalSpline1D(cx0, x0, x1, cx1, 0.5, t),
			USCurve::CatmullRom1D(cy0, y0, y1, cy1, t), //USCurve::CardinalSpline1D(cy0, y0, y1, cy1, 0.5, t),
			0.0f
		);
		gfxDevice.WriteFinalColor4b();
	}
	
	gfxDevice.EndPrim();
}

//----------------------------------------------------------------//
void MOAIDraw::DrawEllipseFill ( const USRect& rect, u32 steps ) {

	float xRad = ( rect.mXMax - rect.mXMin ) * 0.5f;
	float yRad = ( rect.mYMax - rect.mYMin ) * 0.5f;

	MOAIDraw::DrawEllipseFill ( rect.mXMin + xRad, rect.mYMin + yRad, xRad, yRad, steps );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawEllipseFill ( float x, float y, float xRad, float yRad, u32 steps ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	float angle = ( float )TWOPI / ( float )steps;
	float angleStep = ( float )PI;
	
	gfxDevice.BeginPrim ( GL_TRIANGLE_FAN );
	
	for ( u32 i = 0; i < steps; ++i, angleStep += angle ) {
		gfxDevice.WriteVtx (
			x + ( Sin ( angleStep ) * xRad ),
			y + ( Cos ( angleStep ) * yRad ),
			0.0f
		);
		gfxDevice.WriteFinalColor4b ();
	}
	gfxDevice.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDraw::DrawEllipseOutline ( const USRect& rect, u32 steps ) {
	
	float xRad = ( rect.mXMax - rect.mXMin ) * 0.5f;
	float yRad = ( rect.mYMax - rect.mYMin ) * 0.5f;

	MOAIDraw::DrawEllipseOutline ( rect.mXMin + xRad, rect.mYMin + yRad, xRad, yRad, steps );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawEllipseOutline ( float x, float y, float xRad, float yRad, u32 steps ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	float angle = ( float )TWOPI / ( float )steps;
	float angleStep = ( float )PI;
	
	gfxDevice.BeginPrim ( GL_LINE_LOOP );
	
	for ( u32 i = 0; i < steps; ++i, angleStep += angle ) {
		gfxDevice.WriteVtx (
			x + ( Sin ( angleStep ) * xRad ),
			y + ( Cos ( angleStep ) * yRad ),
			0.0f
		);
		gfxDevice.WriteFinalColor4b ();
	}
	gfxDevice.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDraw::DrawEllipticalGradientFill(float x, float y, float xRad, float yRad, u32 steps, const USColorVec &centerColor, const USColorVec &edgeColor){
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	float angle = ( float )TWOPI / ( float )steps;
	float angleStep = ( float )PI;
	
	USColorVec penColor = gfxDevice.GetPenColor();
	
	gfxDevice.BeginPrim( GL_TRIANGLE_FAN );
	
	gfxDevice.SetPenColor(centerColor);
	
	gfxDevice.WriteVtx(x, y, 0.0f);
	gfxDevice.WriteFinalColor4b();
	
	
	
	gfxDevice.SetPenColor(edgeColor);
	for (u32 i = 0; i <= steps; ++i, angleStep += angle){
		gfxDevice.WriteVtx (
							x + ( Sin ( angleStep ) * xRad ),
							y + ( Cos ( angleStep ) * yRad ),
							0.0f
							);
		gfxDevice.WriteFinalColor4b ();
	}
	
	// write last point to overlap first edge point
	/*
	gfxDevice.WriteVtx (
						x +  xRad, // x + (Sin (PI) * xRad )
						y , // y + (Cos (PI) * xRad )
						0.0f
						);
	gfxDevice.WriteFinalColor4b ();
	*/
	gfxDevice.EndPrim();
	
	gfxDevice.SetPenColor(penColor);
	
	
}

//----------------------------------------------------------------//
void MOAIDraw::DrawGrid ( const USRect& rect, u32 xCells, u32 yCells ) {

	if ( xCells > 1 ) {
		float xStep = rect.Width () / ( float )xCells;
		for ( u32 i = 1; i < xCells; ++i ) {
			float x = rect.mXMin + (( float )i * xStep );
			USVec2D v0 ( x, rect.mYMin );
			USVec2D v1 ( x, rect.mYMax );
			
			MOAIDraw::DrawLine ( v0, v1 );
		}
	}

	if ( yCells > 1 ) {
		float yStep = rect.Height () / ( float )yCells;
		for ( u32 i = 1; i < yCells; ++i ) {
			float y = rect.mYMin + (( float )i * yStep );
			USVec2D v0 ( rect.mXMin, y );
			USVec2D v1 ( rect.mXMax, y );
			
			MOAIDraw::DrawLine ( v0, v1 );
		}
	}

	MOAIDraw::DrawRectOutline ( rect );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawLine ( const USVec2D& v0, const USVec2D& v1 ) {

	MOAIDraw::DrawLine ( v0.mX, v0.mY, v1.mX, v1.mY );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawLine ( float x0, float y0, float x1, float y1 ) {
	
	MOAIDraw::DrawLine ( x0, y0, 0.0f, x1, y1, 0.0f );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawLine ( const USVec3D& v0, const USVec3D& v1 ) {

	MOAIDraw::DrawLine ( v0.mX, v0.mY, v0.mZ, v1.mX, v1.mY, v1.mZ );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawLine ( float x0, float y0, float z0, float x1, float y1, float z1 ) {
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	gfxDevice.SetPrimType ( GL_LINES );

	gfxDevice.BeginPrim ();
	
		gfxDevice.WriteVtx ( x0, y0, z0 );
		gfxDevice.WriteFinalColor4b ();
		
		gfxDevice.WriteVtx ( x1, y1, z1 );
		gfxDevice.WriteFinalColor4b ();
	
	gfxDevice.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDraw::DrawLuaParams ( lua_State* L, u32 primType ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	MOAILuaState state ( L );

	u32 total = state.GetTop () >> 1;
	
	gfxDevice.BeginPrim ( primType );
	
	for ( u32 i = 0; i < total; ++i ) {
		
		u32 idx = ( i << 1 ) + 1;
		
		float x = state.GetValue < float >( idx, 0.0f );
		float y = state.GetValue < float >( idx + 1, 0.0f );
		
		gfxDevice.WriteVtx ( x, y, 0.0f );
		gfxDevice.WriteFinalColor4b ();
	}
	
	gfxDevice.EndPrim ();
}


//----------------------------------------------------------------//
void MOAIDraw::DrawLuaArray ( lua_State* L, u32 primType ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	MOAILuaState state ( L );

	float x = 0.0f;
	float y = 0.0f;
	
	gfxDevice.BeginPrim ( primType );

	/*
	See http://www.lua.org/manual/5.0/manual.html#3.11 for iterator explanation
	*/
	u32 counter = 0;
	lua_pushnil ( L );
    while ( lua_next ( L, 1 ) != 0 ) {
		// Assuming odd-numbered array entries to be x-coordinate (abscissa),
		// even-numbered array entries to be y-coordinate (oordinate).
		if ( counter % 2 == 0 ) {
			x = state.GetValue < float >( -1, 0.0f );
		} else {
			y = state.GetValue < float >( -1, 0.0f );
			gfxDevice.WriteVtx ( x, y );
			gfxDevice.WriteFinalColor4b ();
		}
		++counter;
		lua_pop ( L, 1 );
	}

	gfxDevice.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDraw::DrawPoint ( const USVec2D& loc ) {

	MOAIDraw::DrawPoint ( loc.mX, loc.mY );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawPoint ( float x, float y ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	gfxDevice.SetPrimType ( GL_POINTS );

	gfxDevice.BeginPrim ();
		gfxDevice.WriteVtx ( x, y, 0.0f );
		gfxDevice.WriteFinalColor4b ();
	gfxDevice.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDraw::DrawQuad ( const USQuad& quad ) {

	MOAIDraw::DrawLine ( quad.mV [ 0 ].mX, quad.mV [ 0 ].mY, quad.mV [ 1 ].mX, quad.mV [ 1 ].mY );
	MOAIDraw::DrawLine ( quad.mV [ 1 ].mX, quad.mV [ 1 ].mY, quad.mV [ 2 ].mX, quad.mV [ 2 ].mY );
	MOAIDraw::DrawLine ( quad.mV [ 2 ].mX, quad.mV [ 2 ].mY, quad.mV [ 3 ].mX, quad.mV [ 3 ].mY );
	MOAIDraw::DrawLine ( quad.mV [ 3 ].mX, quad.mV [ 3 ].mY, quad.mV [ 0 ].mX, quad.mV [ 0 ].mY );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawRay ( float x, float y, float dx, float dy ) {
	
	USVec2D loc ( x, y );
	USVec2D vec ( dx, dy );
	
	USMatrix4x4 mtx = MOAIGfxDevice::Get ().GetViewProjMtx ();
	
	USMatrix4x4 invMtx;
	invMtx.Inverse ( mtx );
	
	mtx.Transform ( loc );
	mtx.TransformVec ( vec );
	
	USRect viewRect;
	viewRect.Init ( -1.0f, -1.0f, 1.0f, 1.0f );
	
	USVec2D p0;
	USVec2D p1;
	
	if ( viewRect.GetIntersection ( loc, vec, p0, p1 )) {
		
		invMtx.Transform ( p0 );
		invMtx.Transform ( p1 );
		
		MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
		
		gfxDevice.BeginPrim ( GL_LINES );
		
			gfxDevice.WriteVtx ( p0.mX, p0.mY, 0.0f );
			gfxDevice.WriteFinalColor4b ();
			
			gfxDevice.WriteVtx ( p1.mX, p1.mY, 0.0f );
			gfxDevice.WriteFinalColor4b ();

		gfxDevice.EndPrim ();
	}
}

//----------------------------------------------------------------//
void MOAIDraw::DrawRectEdges ( USRect rect, u32 edges ) {

	rect.Bless ();
	
	// right
	if ( edges & USRect::kRight ) {
		MOAIDraw::DrawLine ( rect.mXMax, rect.mYMin, rect.mXMax, rect.mYMax );
	}

	// top
	if ( edges & USRect::kTop ) {			
		MOAIDraw::DrawLine ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMin );
	}

	// left
	if ( edges & USRect::kLeft ) {			
		MOAIDraw::DrawLine ( rect.mXMin, rect.mYMin, rect.mXMin, rect.mYMax );
	}

	// bottom
	if ( edges & USRect::kBottom ) {			
		MOAIDraw::DrawLine ( rect.mXMin, rect.mYMax, rect.mXMax, rect.mYMax );
	}	
}

//----------------------------------------------------------------//
void MOAIDraw::DrawRectCenteredGradientFill(float left, float top, float right, float bottom, const USColorVec &centerColor, const USColorVec &edgeColor, float innerWidth, float innerHeight){
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	USColorVec penColor = gfxDevice.GetPenColor();
	
	// make sure left is less than right
	if (left > right) {
		float temp = left;
		left = right;
		right = temp;
	}
	
	// make sure bottom is less than top
	if (bottom > top) {
		float temp = bottom;
		top = bottom;
		bottom = temp;
	}
	
	// calculate center point
	float centerX = (left + right) / 2;
	float centerY = (top + bottom) / 2;
	
	float centerLeft, centerRight, centerTop, centerBottom;
	
	if (innerWidth <= 0.0f && innerHeight <= 0.0f) { // five vertex points
		gfxDevice.BeginPrim( GL_TRIANGLE_FAN );
		
		// center
		gfxDevice.SetPenColor(centerColor);
		gfxDevice.WriteVtx ( centerX, centerY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// top-left
		gfxDevice.SetPenColor(edgeColor);
		gfxDevice.WriteVtx ( left, top, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// top-right
		gfxDevice.WriteVtx ( right, top, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// bottom-right
		gfxDevice.WriteVtx ( right, bottom, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// bottom-left
		gfxDevice.WriteVtx ( left, bottom, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// top-left
		gfxDevice.WriteVtx ( left, top, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		
		gfxDevice.EndPrim();
	}
	else if ( innerHeight <= 0.0f ){ // six vertex points
		centerLeft = centerX - innerWidth / 2.0f;
		centerRight = centerX + innerWidth / 2.0f;
		
		gfxDevice.BeginPrim( GL_TRIANGLE_STRIP );
		
		// top-left
		gfxDevice.SetPenColor(edgeColor);
		gfxDevice.WriteVtx ( left, top, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// center-left
		gfxDevice.SetPenColor(centerColor);
		gfxDevice.WriteVtx ( centerLeft, centerY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// top-right
		gfxDevice.SetPenColor(edgeColor);
		gfxDevice.WriteVtx ( right, top, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// center-right
		gfxDevice.SetPenColor(centerColor);
		gfxDevice.WriteVtx ( centerRight, centerY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// bottom-right
		gfxDevice.SetPenColor(edgeColor);
		gfxDevice.WriteVtx ( right, bottom, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// center-left
		gfxDevice.SetPenColor(centerColor);
		gfxDevice.WriteVtx ( centerLeft, centerY, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// bottom-left
		gfxDevice.SetPenColor(edgeColor);
		gfxDevice.WriteVtx ( left, bottom, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// top-left
		gfxDevice.WriteVtx ( left, top, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		gfxDevice.EndPrim();
	}
	else if ( innerWidth <= 0.0f ){
		centerTop = centerY + innerHeight / 2.0f;
		centerBottom = centerY - innerHeight / 2.0f;
		
		gfxDevice.BeginPrim( GL_TRIANGLE_STRIP );
		
		// top-right
		gfxDevice.SetPenColor(edgeColor);
		gfxDevice.WriteVtx ( right, top, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// center-top
		gfxDevice.SetPenColor(centerColor);
		gfxDevice.WriteVtx ( centerX, centerTop, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// bottom-right
		gfxDevice.SetPenColor(edgeColor);
		gfxDevice.WriteVtx ( right, bottom, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// center-bottom
		gfxDevice.SetPenColor(centerColor);
		gfxDevice.WriteVtx ( centerX, centerBottom, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// bottom-left
		gfxDevice.SetPenColor(edgeColor);
		gfxDevice.WriteVtx ( left, bottom, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// center-top
		gfxDevice.SetPenColor(centerColor);
		gfxDevice.WriteVtx ( centerX, centerTop, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// top-left
		gfxDevice.SetPenColor(edgeColor);
		gfxDevice.WriteVtx ( left, top, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// top-right
		gfxDevice.WriteVtx ( right, top, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		
		
		gfxDevice.EndPrim();
	}
	else{ //eight vertex points
		centerLeft = centerX - innerWidth / 2.0f;
		centerRight = centerX + innerWidth / 2.0f;
		centerTop = centerY + innerHeight / 2.0f;
		centerBottom = centerY - innerHeight / 2.0f;
		
		// render edge
		gfxDevice.BeginPrim( GL_TRIANGLE_STRIP );
		
		// top-left
		gfxDevice.SetPenColor(edgeColor);
		gfxDevice.WriteVtx ( left, top, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// center-top-left
		gfxDevice.SetPenColor(centerColor);
		gfxDevice.WriteVtx ( centerLeft, centerTop, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// top-right
		gfxDevice.SetPenColor(edgeColor);
		gfxDevice.WriteVtx ( right, top, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// center-top-right
		gfxDevice.SetPenColor(centerColor);
		gfxDevice.WriteVtx ( centerRight, centerTop, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// bottom-right
		gfxDevice.SetPenColor(edgeColor);
		gfxDevice.WriteVtx ( right, bottom, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// center-bottom-right
		gfxDevice.SetPenColor(centerColor);
		gfxDevice.WriteVtx ( centerRight, centerBottom, 0.0f );
		gfxDevice.WriteFinalColor4b ();

		// bottom-left
		gfxDevice.SetPenColor(edgeColor);
		gfxDevice.WriteVtx ( left, bottom, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// center-bottom-left
		gfxDevice.SetPenColor(centerColor);
		gfxDevice.WriteVtx ( centerLeft, centerBottom, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// top-left
		gfxDevice.SetPenColor(edgeColor);
		gfxDevice.WriteVtx ( left, top, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// center-top-left
		gfxDevice.SetPenColor(centerColor);
		gfxDevice.WriteVtx ( centerLeft, centerTop, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		
		gfxDevice.EndPrim();
		
		// render center
		gfxDevice.BeginPrim( GL_TRIANGLE_STRIP );
		
		// center-top-left
		gfxDevice.SetPenColor(centerColor);
		gfxDevice.WriteVtx ( centerLeft, centerTop, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// center-top-right
		gfxDevice.WriteVtx ( centerRight, centerTop, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// center-bottom-left
		gfxDevice.WriteVtx ( centerLeft, centerBottom, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		// center-bottom-right
		gfxDevice.WriteVtx ( centerRight, centerBottom, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		gfxDevice.EndPrim();
	}
	gfxDevice.SetPenColor(penColor);
}

//----------------------------------------------------------------//
void MOAIDraw::DrawRectFill ( USRect rect, bool asTriStrip ) {

	rect.Bless ();
	MOAIDraw::DrawRectFill ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMax );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawRectFill ( float left, float top, float right, float bottom ) {
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	gfxDevice.BeginPrim ( GL_TRIANGLE_STRIP );
	
		gfxDevice.WriteVtx ( left, top, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		gfxDevice.WriteVtx ( right, top, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		gfxDevice.WriteVtx ( left, bottom, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		gfxDevice.WriteVtx ( right, bottom, 0.0f );
		gfxDevice.WriteFinalColor4b ();
	
	gfxDevice.EndPrim ();
}

void MOAIDraw::DrawRectHorizontalGradientFill ( float left, float top, float right, float bottom, const USColorVec &leftColor, const USColorVec &rightColor ){
	MOAIGfxDevice &gfxDevice = MOAIGfxDevice::Get();
	USColorVec penColor = gfxDevice.GetPenColor();
	
	gfxDevice.BeginPrim ( GL_TRIANGLE_STRIP );
	gfxDevice.SetPenColor(leftColor);
	gfxDevice.WriteVtx ( left, top, 0.0f );
	gfxDevice.WriteFinalColor4b ();
	
	gfxDevice.WriteVtx ( left, bottom, 0.0f );
	gfxDevice.WriteFinalColor4b ();
	
	gfxDevice.SetPenColor(rightColor);
	gfxDevice.WriteVtx ( right, top, 0.0f );
	gfxDevice.WriteFinalColor4b ();
	
	gfxDevice.WriteVtx ( right, bottom, 0.0f );
	gfxDevice.WriteFinalColor4b ();
	
	gfxDevice.EndPrim ();
	// restore pen color
	gfxDevice.SetPenColor(penColor);
}

//----------------------------------------------------------------//
void MOAIDraw::DrawRectOutline ( const USRect& rect ) {
	
	MOAIDraw::DrawRectOutline ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMax );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawRectOutline ( float left, float top, float right, float bottom ) {
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	gfxDevice.BeginPrim ( GL_LINE_LOOP );
	
		gfxDevice.WriteVtx ( left, top, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		gfxDevice.WriteVtx ( right, top, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		gfxDevice.WriteVtx ( right, bottom, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		gfxDevice.WriteVtx ( left, bottom, 0.0f );
		gfxDevice.WriteFinalColor4b ();
	
	gfxDevice.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDraw::DrawRectVerticalGradientFill(float left, float top, float right, float bottom, const USColorVec &topColor, const USColorVec &bottomColor){
	MOAIGfxDevice &gfxDevice = MOAIGfxDevice::Get();
	USColorVec penColor = gfxDevice.GetPenColor();
	
	gfxDevice.BeginPrim ( GL_TRIANGLE_STRIP );
	
	gfxDevice.SetPenColor(topColor);
	gfxDevice.WriteVtx ( left, top, 0.0f );
	gfxDevice.WriteFinalColor4b ();
	
	gfxDevice.WriteVtx ( right, top, 0.0f );
	gfxDevice.WriteFinalColor4b ();
	
	gfxDevice.SetPenColor(bottomColor);
	gfxDevice.WriteVtx ( left, bottom, 0.0f );
	gfxDevice.WriteFinalColor4b ();
	
	gfxDevice.WriteVtx ( right, bottom, 0.0f );
	gfxDevice.WriteFinalColor4b ();
	
	gfxDevice.EndPrim ();
	// restor pen color
	gfxDevice.SetPenColor(penColor);
}

//----------------------------------------------------------------//
void MOAIDraw::DrawTexture ( float left, float top, float right, float bottom, MOAITexture* texture ) {
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	if ( texture ) {
		
		gfxDevice.Flush ();

		gfxDevice.SetBlendMode ( GL_ONE, GL_ZERO );
		gfxDevice.SetTexture ( texture );
		gfxDevice.SetShaderPreset ( MOAIShaderMgr::DECK2D_SHADER );

		const USColorVec& orgColor = gfxDevice.GetPenColor ();
		gfxDevice.SetPenColor ( 1, 1, 1, 1 );
		
		MOAIQuadBrush::BindVertexFormat ( gfxDevice );

		MOAIQuadBrush quad;
		quad.SetVerts ( left, top, right, bottom );
		quad.SetUVs ( 0, 0, 1, 1 );		
		quad.Draw ();

		gfxDevice.Flush ();
		
		gfxDevice.SetBlendMode ();
		gfxDevice.SetPenColor ( orgColor );
		
		MOAIDraw::Bind ();
	}
}

//----------------------------------------------------------------//
void MOAIDraw::DrawVertexArray ( const USVec3D* verts, u32 count, u32 color, u32 primType ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	gfxDevice.SetPrimType ( primType );
	gfxDevice.SetPenColor ( color );
	
	gfxDevice.BeginPrim ();
	
	for ( u32 i = 0; i < count; ++i ) {
		const USVec3D& vtx = verts [ i ];
		gfxDevice.WriteVtx ( vtx );
		gfxDevice.WriteFinalColor4b ();
	}

	gfxDevice.EndPrim ();
}

//----------------------------------------------------------------//
void MOAIDraw::DrawVertexArray2D ( const float* verts, u32 count, u32 color, u32 primType ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	gfxDevice.SetPrimType ( primType );
	gfxDevice.SetPenColor ( color );
	
	gfxDevice.BeginPrim ();
	
	for ( u32 i = 0; i < count; ++i ) {
		u32 v = i << 1;
		gfxDevice.WriteVtx ( verts [ v ], verts [ v + 1 ], 0.0f );
		gfxDevice.WriteFinalColor4b ();
	}

	gfxDevice.EndPrim ();
}

//----------------------------------------------------------------//
MOAIDraw::MOAIDraw () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIDraw::~MOAIDraw () {
}

//----------------------------------------------------------------//
void MOAIDraw::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );

	luaL_Reg regTable [] = {
		{ "drawAnimCurve",			_drawAnimCurve },
		//{ "drawAxisGrid",			_drawAxisGrid }, // TODO
		{ "drawBeveledCorner",		_drawBeveledCorner },
		{ "drawBeveledLines",		_drawBeveledLines },
		{ "drawBezierCurve",		_drawBezierCurve },
		{ "drawBoxOutline",			_drawBoxOutline },
		{ "drawCardinalSpline",		_drawCardinalSpline },
		{ "drawCatmullRomCurve",	_drawCatmullRomCurve },
		{ "drawCircle",				_drawCircle },
		{ "drawEllipse",			_drawEllipse },
		//{ "drawGrid",				_drawGrid }, // TODO
		{ "drawLine",				_drawLine },
		{ "drawPoints",				_drawPoints },
		{ "drawRay",				_drawRay },
		{ "drawRect",				_drawRect },
		{ "fillCenteredRectangularGradient", _fillCenteredRectangularGradient },
		{ "fillCircle",				_fillCircle },
		{ "fillCircularGradient",	_fillCircularGradient },
		{ "fillEllipse",			_fillEllipse },
		{ "fillEllipticalGradient",	_fillEllipticalGradient },
		{ "fillHorizontalRectangularGradient", _fillHorizontalRectangularGradient },
		{ "fillFan",				_fillFan },
		{ "fillRect",				_fillRect },
		{ "fillVerticalRectangularGradient", _fillVerticalRectangularGradient },
		{ "drawText",				_drawText },
		{ "drawTexture",			_drawTexture },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}
