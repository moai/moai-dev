// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIAnimCurve.h>
#include <moaicore/MOAIDraw.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIShaderMgr.h>
#include <moaicore/MOAIVertexFormatMgr.h>

#define DEFAULT_ELLIPSE_STEPS 64
#define DEFAULT_CURVE_STEPS 32

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
int MOAIDraw::_drawBeveledCorner ( lua_State *L ) {
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
/** @name	drawBeveledLine
	@text	when provided with three or more points, creates line segments that meet at corners.
 
	@in		table vertices
	@in		number lineWidth
	@opt	number blurMargin	default to 1.0
	@out	nil
*/
int MOAIDraw::_drawBeveledLine ( lua_State *L ) {
	
	MOAILuaState state( L );
	
	float lineWidth = state.GetValue < float > (2, 1.0f);
	float blurMargin = state.GetValue < float > (3, 1.0f);
	
	MOAIDraw::DrawBeveledLines(L, lineWidth, blurMargin);
	
	return 0;
}

//----------------------------------------------------------------//
/** @name	drawBeveledLineLoop
	@text	Draw a loop of line segments that meet at corners.
	@in		table vertices
	@in		number lineWidth
	@opt	number blurMargin	default to 1.0
	@out	nil
 
*/
int MOAIDraw::_drawBeveledLineLoop ( lua_State *L ) {
	MOAILuaState state( L );
	
	float lineWidth = state.GetValue < float > (2, 1.0f);
	float blurMargin = state.GetValue < float > (3, 1.0f);
	
	MOAIDraw::DrawBeveledLineLoop(L, lineWidth, blurMargin);
	
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
int MOAIDraw::_drawBezierCurve ( lua_State *L ) {
	MOAILuaState state ( L );
	
	float x0 = state.GetValue < float > (1, 0.0f);
	float y0 = state.GetValue < float > (2, 0.0f);
	float x1 = state.GetValue < float > (3, 0.0f);
	float y1 = state.GetValue < float > (4, 0.0f);
	float cx0 = state.GetValue < float > (5, x0);
	float cy0 = state.GetValue < float > (6, y0);
	float cx1 = state.GetValue < float > (7, x1);
	float cy1 = state.GetValue < float > (8, y1);
	u32	steps = state.GetValue < u32 > (9, DEFAULT_CURVE_STEPS);
	
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
int MOAIDraw::_drawCardinalSpline ( lua_State* L ) {
	
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
	u32	steps = state.GetValue < u32 > (10, DEFAULT_CURVE_STEPS);
	
	MOAIDraw::DrawCardinalSpline(x0, y0, x1, y1, cx0, cy0, cx1, cy1, tension, steps);
	return 0;
}

//----------------------------------------------------------------//
/** @name	drawCardinalSpineLoop
	@text	Draw a cardinal spline curve through the array of points with 
 
	@overload
	@in		...
	@out	nil
 
	@overload
	@in		table	vertices
	@opt	number	tension
	@opt	number	steps
	@out	nil
 
 */
int MOAIDraw::_drawCardinalSplineLoop( lua_State *L ){
	u32 steps = DEFAULT_CURVE_STEPS;
	float tension = 0.0f;
	MOAILuaState state( L );
	if ( lua_istable ( L, 1 ) ) {
		tension = state.GetValue < float > (2, 0.0f);
		steps = state.GetValue < u32 > (3, DEFAULT_CURVE_STEPS);
		//MOAIDraw::DrawLuaArray( L, GL_LINE_STRIP );
	}
	MOAIDraw::DrawCardinalSplineLoop(L, tension, steps);
	
	return 0;
}

//----------------------------------------------------------------//
/** @name	drawCardinalSplineStrip
	@text	Draw a cardinal spline curve through the array of points.
 
	@overload
	@in		...
	@out	nil
 
	@overload
	@in		table	vertices
	@opt	number	tension
	@opt	number	steps
	@out	nil
 */

int	MOAIDraw::_drawCardinalSplineStrip (  lua_State* L ) {
	u32 steps = DEFAULT_CURVE_STEPS;
	float tension = 0.0f;
	MOAILuaState state( L );
	if ( lua_istable ( L, 1 ) ) {
		tension = state.GetValue < float > (2, 0.0f);
		steps = state.GetValue < u32 > (3, DEFAULT_CURVE_STEPS);
		//MOAIDraw::DrawLuaArray( L, GL_LINE_STRIP );
	}
	MOAIDraw::DrawCardinalSplineStrip(L, tension, steps);
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
int MOAIDraw::_drawCatmullRomCurve ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	float x0 = state.GetValue < float > (1, 0.0f);
	float y0 = state.GetValue < float > (2, 0.0f);
	float x1 = state.GetValue < float > (3, 0.0f);
	float y1 = state.GetValue < float > (4, 0.0f);
	float cx0 = state.GetValue < float > (5, x0);
	float cy0 = state.GetValue < float > (6, y0);
	float cx1 = state.GetValue < float > (7, x1);
	float cy1 = state.GetValue < float > (8, y1);
	u32	steps = state.GetValue < u32 > (9, DEFAULT_CURVE_STEPS);
	
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
/** @name	drawJoinedCorner
	@text	Draws a corner similar to MOAIDraw.drawBeveledCorner(), but without the extruding point.
			More expensive in terms of vertices and draws.
 
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
int MOAIDraw::_drawJoinedCorner ( lua_State *L ) {
	MOAILuaState state ( L );
	
	float x0 = state.GetValue < float > (1, 0.0f);
	float y0 = state.GetValue < float > (2, 0.0f);
	float x1 = state.GetValue < float > (3, 0.0f);
	float y1 = state.GetValue < float > (4, 0.0f);
	float x2 = state.GetValue < float > (5, 0.0f);
	float y2 = state.GetValue < float > (6, 0.0f);
	float lineWidth = state.GetValue < float > (7, 1.0f);
	float blurMargin = state.GetValue <float> (8, 0.0f);
	
	MOAIDraw::DrawJoinedCorner(x0, y0, x1, y1, x2, y2, lineWidth, blurMargin);
	
	return 0;
}
//----------------------------------------------------------------//
/** @name	drawJoinedLine
	@text	Draw a line with corners like those produced by MOAIDraw.drawJoinedCorner().
 
	@in		table
	@in		number		lineWidth
	@opt	number		blurMargin	default to 1.0
	@out	nil
 */
int MOAIDraw::_drawJoinedLine ( lua_State *L ) {
	MOAILuaState state ( L );
	
	float lineWidth = state.GetValue < float > (2, 1.0f);
	float blurMargin = state.GetValue < float > (3, 1.0f);
	
	MOAIDraw::DrawJoinedLine(L, lineWidth, blurMargin);
	return 0;
}

//----------------------------------------------------------------//
/** @name	drawJoinedLineLoop
	@text	Draw a closed line loop with truncated corners.
 
	@in		table		vertices
	@in		number		lineWidth
	@opt	number		blurMargin	default to 1.0
	@out	nil
 */
int MOAIDraw::_drawJoinedLineLoop ( lua_State *L ){
	MOAILuaState state ( L );
	
	float lineWidth = state.GetValue < float > (2, 1.0f);
	float blurMargin = state.GetValue < float > (3, 1.0f);
	
	MOAIDraw::DrawJoinedLineLoop(L, lineWidth, blurMargin);
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
/**	@name	drawRoundBeveledLine
	@text	Draw a line with rounded corners and endpoints.
			With obtuse angles, there corner may look exactly like 
			the joined line corner.  With sharp angles, the corner 
			may have as many sections as the endpoints.
 
	@in		table		vertices
	@in		number		lineWidth
	@opt	number		blurMargin	default to 1.0
	@opt	number		steps		
	@out	nil
 */

int MOAIDraw::_drawRoundBeveledLine ( lua_State *L ) {
	MOAILuaState state ( L );
	
	float lineWidth = state.GetValue < float > (2, 1.0f);
	float blurMargin = state.GetValue < float > (3, 1.0f);
	u32 steps = state.GetValue < u32 > (4, DEFAULT_CURVE_STEPS);
	
	MOAIDraw::DrawRoundBeveledLine(L, lineWidth, blurMargin, steps);
	
	return 0;
}

//----------------------------------------------------------------//
/** @name	fillCenteredRectangularGradient
	@text	Draw a rectangle with the color at the edges fading to a central
			color.  The width and height of the central rectangle can be specified
			after the colors.
 
	@overload
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
 
	@overload
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@in		MOAIColor centerColor
	@in		MOAIColor edgeColor
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
	
	MOAIColor *color1, *color2;
	USColorVec centerColor, edgeColor;
	
	float innerWidth, innerHeight;
	
	if ( ( color1 = state.GetLuaObject < MOAIColor > ( 5, false ) ) &&
		 ( color2 = state.GetLuaObject < MOAIColor > ( 6, false ) ) ) {
		
		centerColor = color1->GetColorTrait();
		edgeColor = color2->GetColorTrait();
		
		innerWidth = state.GetValue < float > (7, 0.0f);
		innerHeight = state.GetValue < float > (8, 0.0f);
	}
	else {
		float centerR = state.GetValue < float > (5, 1.0f);
		float centerG = state.GetValue < float > (6, 1.0f);
		float centerB = state.GetValue < float > (7, 1.0f);
		float centerA = state.GetValue < float > (8, 1.0f);
		
		float edgeR = state.GetValue < float > (9,  1.0f);
		float edgeG = state.GetValue < float > (10, 1.0f);
		float edgeB = state.GetValue < float > (11, 1.0f);
		float edgeA = state.GetValue < float > (12, 1.0f);
		
		innerWidth = state.GetValue < float > (13, 0.0f);
		innerHeight = state.GetValue < float > (14, 0.0f);
		
		centerColor.Set(centerR, centerG, centerB, centerA);
		edgeColor.Set(edgeR, edgeG, edgeB, edgeA);
	}
	
	
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
	
	@overload
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
 
	@overload
	@in		number x		x-coordinate of circle
	@in		number y		y-coordinate of circle
	@in		number r		radius of circle
	@in		number steps
	@in		MOAIColor centerColor
	@in		MOAIColor edgeColor
	@out	nil
 */

int MOAIDraw::_fillCircularGradient ( lua_State *L ) {
	MOAILuaState state ( L );
	
	float x0	= state.GetValue < float >( 1, 0.0f );
	float y0	= state.GetValue < float >( 2, 0.0f );
	float r		= state.GetValue < float >( 3, 0.0f );
	u32 steps	= state.GetValue < u32 >( 4, DEFAULT_ELLIPSE_STEPS );
	
	MOAIColor *color1, *color2;
	USColorVec centerColor, edgeColor;
	
	if ( ( color1 = state.GetLuaObject < MOAIColor > ( 5, false ) ) &&
		( color2 = state.GetLuaObject < MOAIColor > ( 6, false ) ) ) {
		centerColor = color1->GetColorTrait();
		edgeColor = color2->GetColorTrait();
	}
	else {
		float centerR = state.GetValue < float > (5, 1.0f);
		float centerG = state.GetValue < float > (6, 1.0f);
		float centerB = state.GetValue < float > (7, 1.0f);
		float centerA = state.GetValue < float > (8, 1.0f);
		
		float edgeR = state.GetValue < float > (9, 1.0f);
		float edgeG = state.GetValue < float > (10, 1.0f);
		float edgeB = state.GetValue < float > (11, 1.0f);
		float edgeA = state.GetValue < float > (12, 1.0f);
		
		centerColor.Set(centerR, centerG, centerB, centerA);
		edgeColor.Set(edgeR, edgeG, edgeB, edgeA);
	}
	
	MOAIDraw::DrawEllipticalGradientFill(x0, y0, r, r, steps, centerColor, edgeColor);
	
	return 0;
}

//----------------------------------------------------------------//
/** @name	fillCircularSlice
	@text	Draw a pie slice of a circle with the specified center, radius and angle in degrees.
			Optional parameters include blur margin, angle offset and steps.  By default
			the left leg of the slice starts out vertical pointing up and goes clockwise.  A
			ninety degree slice at the origin would occupy the upper right quadrant.  The blur
			extends from the curved portion, not the legs.
 
	@in		number x			x-coordinate of center
	@in		number y			y-coordinate of center
	@in		number radius		radius of circle
	@in		number angle		angle of the slice of the circle in degrees
	@opt	number offset		the offset clockwise from positive y axis in degrees.
	@opt	number blurMargin	default to zero
	@opt	number steps		the number of segments needed to make the slice
	@out	nil
 
 */
int MOAIDraw::_fillCircularSlice ( lua_State *L ) {
	
	MOAILuaState state ( L );
	
	float x				= state.GetValue < float >( 1, 0.0f );
	float y				= state.GetValue < float >( 2, 0.0f );
	float radius		= state.GetValue < float >( 3, 0.0f );
	float angle			= state.GetValue < float >( 4, 0.0f );
	float offset		= state.GetValue < float >( 5, 0.0f );
	float blurMargin	= state.GetValue < float >( 6, 0.0f );
	
	u32	steps			= state.GetValue < u32 > ( 7, DEFAULT_ELLIPSE_STEPS );
	
	MOAIDraw::DrawEllipticalSliceFill(x, y, radius, radius, angle, offset, blurMargin, steps);
	
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
 
	@overload
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
	@out	nil
 
	@overload
	@in		number x		x-coordinate of ellipse
	@in		number y		y-coordinate of ellipse
	@in		number xRad		radius of ellipse on x-axis
	@in		number yRad r	adius of ellipse on y-axis
	@in		number steps
	@in		MOAIColor centerColor
	@in		MOAIColor edgeColor
	@out	nil
*/
int MOAIDraw::_fillEllipticalGradient ( lua_State *L ) {
	MOAILuaState state ( L );
	
	float x		= state.GetValue < float >( 1, 0.0f );
	float y		= state.GetValue < float >( 2, 0.0f );
	float xRad	= state.GetValue < float >( 3, 0.0f );
	float yRad	= state.GetValue < float >( 4, 0.0f );
	
	u32 steps = state.GetValue < u32 >( 5, DEFAULT_ELLIPSE_STEPS );
	
	USColorVec centerColor, edgeColor;
	MOAIColor *color1, *color2;
	
	if ( ( color1 = state.GetLuaObject < MOAIColor > ( 6, false ) ) &&
		( color2 = state.GetLuaObject < MOAIColor > ( 7, false ) ) ) {
		centerColor = color1->GetColorTrait();
		edgeColor = color2->GetColorTrait();
	}
	else {
		float centerR = state.GetValue < float > (6, 1.0f);
		float centerG = state.GetValue < float > (7, 1.0f);
		float centerB = state.GetValue < float > (8, 1.0f);
		float centerA = state.GetValue < float > (9, 1.0f);
		
		float edgeR = state.GetValue < float > (10, 1.0f);
		float edgeG = state.GetValue < float > (11, 1.0f);
		float edgeB = state.GetValue < float > (12, 1.0f);
		float edgeA = state.GetValue < float > (13, 1.0f);
		
		centerColor.Set(centerR, centerG, centerB, centerA);
		edgeColor.Set(edgeR, edgeG, edgeB, edgeA);
	}
	
	MOAIDraw::DrawEllipticalGradientFill(x, y, xRad, yRad, steps, centerColor, edgeColor);
	
	return 0;
}

//----------------------------------------------------------------//
/** @name	fillEllipticalSlice
	@text	
 
	@in		number x			x-coordinate of center
	@in		number y			y-coordinate of center
	@in		number xRad			radius of ellipse in x direction
	@in		number yRad
	@in		number angle		angle of the slice of the ellipse in degrees
	@opt	number offset		the offset clockwise from positive y axis in degrees.
	@opt	number blurMargin	default to zero
	@opt	number steps		the number of segments needed to make the slice
	@out	nil
 */
int MOAIDraw::_fillEllipticalSlice ( lua_State *L ) {
	MOAILuaState state ( L );
	
	float x				= state.GetValue < float > ( 1, 0.0f );
	float y				= state.GetValue < float > ( 2, 0.0f );
	float xRad			= state.GetValue < float > ( 3, 0.0f );
	float yRad			= state.GetValue < float > ( 4, 0.0f );
	float angle			= state.GetValue < float > ( 5, 0.0f );
	float offset		= state.GetValue < float > ( 6, 0.0f );
	float blurMargin	= state.GetValue < float > ( 7, 0.0f );
	
	u32	steps			= state.GetValue < u32 > ( 8, DEFAULT_ELLIPSE_STEPS );
	
	MOAIDraw::DrawEllipticalSliceFill(x, y, xRad, yRad, angle, offset, blurMargin, steps);
	
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
 
	@overload
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
 
	@overload
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@in		MOAIColor	leftColor
	@in		MOAIColor	rightColor
	@out	nil
*/
int MOAIDraw::_fillHorizontalRectangularGradient ( lua_State *L ) {
	
	MOAILuaState state ( L );
	
	float x0 = state.GetValue < float >( 1, 0.0f );
	float y0 = state.GetValue < float >( 2, 0.0f );
	float x1 = state.GetValue < float >( 3, 0.0f );
	float y1 = state.GetValue < float >( 4, 0.0f );
	
	float r1, g1, b1, a1, r2, g2, b2, a2;
	USColorVec leftColor, rightColor;
	MOAIColor *color1, *color2;
	if ( ( color1 = state.GetLuaObject < MOAIColor > ( 5, false ) ) &&
		 ( color2 = state.GetLuaObject < MOAIColor > ( 6, false ) ) ) {
		leftColor = color1->GetColorTrait();
		rightColor = color2->GetColorTrait();
	}
	else{
		if ( state.GetTop() >= 11 ){
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
		leftColor.Set(r1, g1, b1, a1);
		rightColor.Set(r2, g2, b2, a2);
	}
	
	
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
/** @name	fillTriangularGradient
	@text	Draw a filled triangle with a different color for each vertex.
 
	@overload
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@in		number x2
	@in		number y2
	@in		number r1	First point color red
	@in		number g1
	@in		number b1
	@in		number a1
	@in		number r2	Second point color red
	@in		number g2
	@in		number b2
	@in		number a2
	@in		number r3	First point color red
	@in		number g3
	@in		number b3
	@in		number a3
	@out	nil
 
	@overload
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@in		number x2
	@in		number y2
	@in		MOAIColor color0
	@in		MOAIColor color1
	@in		MOAIColor color2
	@out	buk
 */
int MOAIDraw::_fillTriangularGradient ( lua_State *L ) {
	MOAILuaState state ( L );
	
	USVec2D vec0 = state.GetVec2D<float>(1);
	USVec2D vec1 = state.GetVec2D<float>(3);
	USVec2D vec2 = state.GetVec2D<float>(5);
	
	USColorVec color0, color1, color2;
	MOAIColor *moaiColor0, *moaiColor1, *moaiColor2;
	if ( (moaiColor0 = state.GetLuaObject < MOAIColor > ( 7, false ) ) &&
		 (moaiColor1 = state.GetLuaObject < MOAIColor > ( 8, false ) ) &&
		 (moaiColor2 = state.GetLuaObject < MOAIColor > ( 9, false ) ) ) {
		
		color0 = moaiColor0->GetColorTrait();
		color1 = moaiColor1->GetColorTrait();
		color2 = moaiColor2->GetColorTrait();
		
	}
	else{
		color0 = state.GetColor(7, 1.0f, 1.0f, 1.0f, 1.0f);
		color1 = state.GetColor(11, 1.0f, 1.0f, 1.0f, 1.0f);
		color2 = state.GetColor(15, 1.0f, 1.0f, 1.0f, 1.0f);
	}
	
	MOAIDraw::DrawTriangularGradientFill(vec0, vec1, vec2, color0, color1, color2);
	
	return 0;
}


//----------------------------------------------------------------//
/** @name	fillVerticalRectangularGradient
	@text	Draw a filled rectangle with a gradient between two colors
			going from top to bottom.
 
	@overload
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
 
	@overload
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@in		MOAIColor topColor
	@in		MOAIColor bottomColor
 */

int MOAIDraw::_fillVerticalRectangularGradient ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	float x0 = state.GetValue < float >( 1, 0.0f );
	float y0 = state.GetValue < float >( 2, 0.0f );
	float x1 = state.GetValue < float >( 3, 0.0f );
	float y1 = state.GetValue < float >( 4, 0.0f );
	
	USColorVec topColor, bottomColor;
	MOAIColor *color1, *color2;
	
	if ( ( color1 = state.GetLuaObject < MOAIColor > ( 5, false ) ) &&
		 ( color2 = state.GetLuaObject < MOAIColor > ( 6, false ) ) ) {
		topColor = color1->GetColorTrait();
		bottomColor = color2->GetColorTrait();
	}
	else {
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
		
		topColor.Set(r1, g1, b1, a1);
		bottomColor.Set(r2, g2, b2, a2);
	}
	
	MOAIDraw::DrawRectVerticalGradientFill ( x0, y0, x1, y1, topColor, bottomColor );
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

void MOAIDraw::DrawAntiAliasedLineSegment( float x0, float y0, float x1, float y1, float lineWidth, float blurMargin){
	// find the normalized vector
	
	MOAIGfxDevice &gfxDevice = MOAIGfxDevice::Get();
	
	USVec2D vec;
	vec.Init(x1 - x0, y1 - y0);
	
	// don't draw lines with zero length
	if (vec.LengthSquared() == 0.0) {
		return;
	}
	
	// normalize the vector
	vec.Norm();
	
	// rotate vector anti-clockwise 90 degrees
	vec.Rotate90Anticlockwise();
	
	// half line width to multiply with vec for determining point locations
	float lw = lineWidth / 2;
	
	// half line width plus blur margin
	float bw = lw + blurMargin;
	
	// declare points
	USVec2D p1;// "north" of (x0, y0), opacity of 0
	p1.Init(x0 + bw * vec.mX, y0 + bw * vec.mY);
	
	USVec2D p2; // "north" of (x1, y1), opacity of 0
	p2.Init(x1 + bw * vec.mX, y1 + bw * vec.mY);
	
	USVec2D p3; // "north" of (x0, y0), opacity of 1
	p3.Init(x0 + lw * vec.mX, y0 + lw * vec.mY);
	
	USVec2D p4; // "north" of (x1, y1), opacity of 1
	p4.Init(x1 + lw * vec.mX, y1 + lw * vec.mY);
	
	USVec2D p5; // "south" of (x0, y0), opacity of 1
	p5.Init(x0 - lw * vec.mX, y0 - lw * vec.mY);
	
	USVec2D p6; // "south" of (x1, y1), opacity of 1
	p6.Init(x1 - lw * vec.mX, y1 - lw * vec.mY);
	
	USVec2D p7; // "south" of (x0, y0), opacity of 0
	p7.Init(x0 - bw * vec.mX, y0 - bw * vec.mY);
	
	USVec2D p8; // "south" of (x1, y1), opacity of 0
	p8.Init(x1 - bw * vec.mX, y1 - bw * vec.mY);
	
	// get pen color
	USColorVec penColor = gfxDevice.GetPenColor();
	// make transparent color
	USColorVec transColor(penColor);
	transColor.mA = 0.0f;
	if ( MOAIGfxDevice::Get ().GetColorPremultiply () ) {
		transColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
	}
	
	
	// draw triangle strip
	gfxDevice.BeginPrim(GL_TRIANGLE_STRIP);
	
	gfxDevice.SetPenColor(transColor);
	
	// write p1
	gfxDevice.WriteVtx ( p1.mX, p1.mY, 0.0f );
	gfxDevice.WriteFinalColor4b ();
	
	// write p2
	gfxDevice.WriteVtx ( p2.mX, p2.mY, 0.0f );
	gfxDevice.WriteFinalColor4b ();
	
	gfxDevice.SetPenColor(penColor);
	
	// write p3
	gfxDevice.WriteVtx ( p3.mX, p3.mY, 0.0f );
	gfxDevice.WriteFinalColor4b ();
	
	// write p4
	gfxDevice.WriteVtx ( p4.mX, p4.mY, 0.0f );
	gfxDevice.WriteFinalColor4b ();
	
	// write p5
	gfxDevice.WriteVtx ( p5.mX, p5.mY, 0.0f );
	gfxDevice.WriteFinalColor4b ();
	
	// write p6
	gfxDevice.WriteVtx ( p6.mX, p6.mY, 0.0f );
	gfxDevice.WriteFinalColor4b ();
	
	gfxDevice.SetPenColor(transColor);
	
	// write p7
	gfxDevice.WriteVtx ( p7.mX, p7.mY, 0.0f );
	gfxDevice.WriteFinalColor4b ();
	
	// write p8
	gfxDevice.WriteVtx ( p8.mX, p8.mY, 0.0f );
	gfxDevice.WriteFinalColor4b ();
	
	gfxDevice.EndPrim();
	
	// restore original pen color
	gfxDevice.SetPenColor(penColor);
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
		if ( MOAIGfxDevice::Get ().GetColorPremultiply () ) {
			transColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
		}
		
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
void MOAIDraw::DrawBeveledLineLoop(lua_State *L, float lineWidth, float blurMargin){
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	MOAILuaState state ( L );
	
	float p0x, p0y, p1x, p1y, p2x, p2y;
	// r0 to r3 are the points defining the rectangle of the first segment
	// r4 to r7 are the points defining the rectangle of the second segment
	// r8 and r9 are the corner intersection points
	// line1Normal and line2Normal are the normalized line vectors.  line1 defines p0p1 vector. line2 defines p1p2 vector.
	// line1Normal and line2Normal are the normalized vectors anti-clockwise from line1 and line2 respectively
	// q0 and q1 are storage variables for the next segment, to be used as the values of r0 and r2
	USVec2D r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, line1, line1Normal, line2, line2Normal, q0, q1;
	bool i1, i2, j1, j2;
	
	float lw = lineWidth / 2;
	bool renderBlur = blurMargin > 0.0f;
	
	// the b variables are the blur boundary counterparts of the r variables
	USVec2D b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, q2, q3;
	// Additional boolean variables for lines defining northern and southern
	bool i3, i4, j3, j4;
	// blur width
	float bw = lw + blurMargin;
	
	// the first intersection points saved for rendering the segment between points 0 and 1 in the final step
	USVec2D firstSolidIntNorth, firstSolidIntSouth, firstBlurIntNorth, firstBlurIntSouth;
	
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
	
	// get pen color
	USColorVec penColor = gfxDevice.GetPenColor();
	// make transparent color
	USColorVec transColor(penColor);
	transColor.mA = 0.0f;
	if ( MOAIGfxDevice::Get ().GetColorPremultiply () ) {
		transColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
	}
	
	for (int i = 0; i < counter - 1; i += 2) {
		p0x = vertexArray[i];
		p0y = vertexArray[i+1];
		
		
		if (i < counter - 5) {
			p2x = vertexArray[i+4];
			p2y = vertexArray[i+5];
		}
		else{
			p2x = vertexArray[0];
			p2y = vertexArray[1];
		}
		
		if (i < counter - 3){
			p1x = vertexArray[i+2];
			p1y = vertexArray[i+3];
		}
		else{
			p1x = vertexArray[0];
			p1y = vertexArray[1];
			p2x = vertexArray[2];
			p2y = vertexArray[3];
		}
		
		
		// calculate line vectors
		line1.Init(p1x - p0x, p1y - p0y);
		if (line1.LengthSquared() == 0) {
			continue;
		}
		line1.Norm();
		line1Normal.Init(line1);
		line1Normal.Rotate90Anticlockwise();
		
		line2.Init(p2x - p1x, p2y - p1y);
		if (line2.LengthSquared() == 0) {
			continue;
		}
		line2.Norm();
		line2Normal.Init(line2);
		line2Normal.Rotate90Anticlockwise();
		
		
		// calculate render points
		if (i == 0) {
			// "northwest" corner or first segment solid boundary
			r0.Init(p0x + lw * line1Normal.mX, p0y + lw * line1Normal.mY);
			// "southwest" corner of first segment solid boundary
			r2.Init(p0x - lw * line1Normal.mX, p0y - lw * line1Normal.mY);
			
			// "northwest" corner or first segment blur boundary
			b0.Init(p0x + bw * line1Normal.mX, p0y + bw * line1Normal.mY);
			// "southwest" corner or first segment blur boundary
			b2.Init(p0x - bw * line1Normal.mX, p0y - bw * line1Normal.mY);
		}
		else{
			r0.Init(q0);
			r2.Init(q1);
			
			b0.Init(q2);
			b2.Init(q3);
		}
		
		// "northeast" corner of first segment
		r1.Init(p1x + lw * line1Normal.mX, p1y + lw * line1Normal.mY);
		b1.Init(p1x + bw * line1Normal.mX, p1y + bw * line1Normal.mY);
		
		// "southeast" corner of first segment
		r3.Init(p1x - lw * line1Normal.mX, p1y - lw * line1Normal.mY);
		b3.Init(p1x - bw * line1Normal.mX, p1y - bw * line1Normal.mY);
		
		// "northwest" corner of second segment
		r4.Init(p1x + lw * line2Normal.mX, p1y + lw * line2Normal.mY);
		b4.Init(p1x + bw * line2Normal.mX, p1y + bw * line2Normal.mY);
		
		// "southwest" corner of second segment
		r6.Init(p1x - lw * line2Normal.mX, p1y - lw * line2Normal.mY);
		b6.Init(p1x - bw * line2Normal.mX, p1y - bw * line2Normal.mY);
		
		if (i < counter - 3) {
			// "northeast" corner of second segment
			r5.Init(p2x + lw * line2Normal.mX, p2y + lw * line2Normal.mY);
			b5.Init(p2x + bw * line2Normal.mX, p2y + bw * line2Normal.mY);
			
			// "southeat" corner of second segment
			r7.Init(p2x - lw * line2Normal.mX, p2y - lw * line2Normal.mY);
			b7.Init(p2x - bw * line2Normal.mX, p2y - bw * line2Normal.mY);
		}
		else{
			// initialize the points in 
			r5.Init(firstSolidIntNorth);
			b5.Init(firstBlurIntNorth);
			
			r7.Init(firstSolidIntSouth);
			b7.Init(firstBlurIntSouth);
		}
		
		
		
		// find intersection points
		j1 = USVec2D::GetLineIntersection(r0, r1, r4, r5, &r8, &i1);
		j2 = USVec2D::GetLineIntersection(r2, r3, r6, r7, &r9, &i2);
		j3 = USVec2D::GetLineIntersection(b0, b1, b4, b5, &b8, &i3);
		j4 = USVec2D::GetLineIntersection(b2, b3, b6, b7, &b9, &i4);
		
		bool allPointsFound = (i1 && i2 && i3 && i4);
		bool blurIntersection = (j3 || j4);
		bool solidIntersection = (j1 || j2);
		
		// fallback for co-linear points, parallel and anti-parallel cases
		if ( !allPointsFound ) {
			r8.Init(r1);
			r9.Init(r3);
			b8.Init(b1);
			b9.Init(b3);
			
			q0.Init(r4);
			q1.Init(r6);
			q2.Init(b4);
			q3.Init(b6);
		}
		// case where neither pair of blur boundaries intersects
		else if ( !blurIntersection ) {
			// find out which intersection point to reassign.
			// b9 in right-handed corners, b8 in left-handed corners
			
			
			bool rightHanded = line1.Cross(line2) > 0.0f;
			// right-handed case where cross product of line1 with line2 is positive
			if ( rightHanded ) {
				b9.Init(b3);
				
				q2.Init(b8);
				q3.Init(b6);
			}
			// left-handed case
			else{
				b8.Init(b1);
				
				q2.Init(b4);
				q3.Init(b9);
			}
			
			// case where neither pair of solid boundaries intersects
			if ( !solidIntersection ) {
				if (rightHanded) {
					r9.Init(r3);
					
					q0.Init(r8);
					q1.Init(r6);
				}
				else{
					r8.Init(r1);
					
					q0.Init(r4);
					q1.Init(r9);
				}
			}
			else{
				q0.Init(r8);
				q1.Init(r9);
			}
			
		}
		else{
			// save points r8 and r9 for later use
			q0.Init(r8);
			q1.Init(r9);
			q2.Init(b8);
			q3.Init(b9);
		}
		
		if (i == 0) {
			firstBlurIntNorth.Init(b8);
			firstBlurIntSouth.Init(b9);
			firstSolidIntNorth.Init(r8);
			firstSolidIntSouth.Init(r9);
			continue;
		}
		
		// render first segment.  No rendering during first run
		
		// render second segment if at end
		
		gfxDevice.BeginPrim(GL_TRIANGLE_STRIP);
		
		if (renderBlur) {
			gfxDevice.SetPenColor(transColor);
			// write b0
			gfxDevice.WriteVtx(b0);
			gfxDevice.WriteFinalColor4b();
			// write b8
			gfxDevice.WriteVtx(b8);
			gfxDevice.WriteFinalColor4b();
		}
		
		
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
		
		if (renderBlur) {
			gfxDevice.SetPenColor(transColor);
			
			// write b2
			gfxDevice.WriteVtx(b2);
			gfxDevice.WriteFinalColor4b();
			
			// write b9
			gfxDevice.WriteVtx(b9);
			gfxDevice.WriteFinalColor4b();
		}
		
		
		gfxDevice.EndPrim();
		
		// render the second segment if at the end.
		if (i + 2 >= counter ) {
			
			gfxDevice.BeginPrim(GL_TRIANGLE_STRIP);
			if (renderBlur) {
				//gfxDevice.SetPenColor(transColor);
				// write q2 AKA b8 or b4
				gfxDevice.WriteVtx(q2);
				gfxDevice.WriteFinalColor4b();
				
				// write b5
				gfxDevice.WriteVtx(b5);
				gfxDevice.WriteFinalColor4b();
				
				gfxDevice.SetPenColor(penColor);
			}
			
			// write q0 AKA r8 or r4
			gfxDevice.WriteVtx(q0);
			gfxDevice.WriteFinalColor4b();
			
			// write r5
			gfxDevice.WriteVtx(r5);
			gfxDevice.WriteFinalColor4b();
			
			// write q1 AKA r9 or r6
			gfxDevice.WriteVtx(q1);
			gfxDevice.WriteFinalColor4b();
			
			// write r7
			gfxDevice.WriteVtx(r7);
			gfxDevice.WriteFinalColor4b();
			
			if (renderBlur) {
				gfxDevice.SetPenColor(transColor);
				// write q3 AKA b9 or b6
				gfxDevice.WriteVtx(q3);
				gfxDevice.WriteFinalColor4b();
				
				// write b7
				gfxDevice.WriteVtx(b7);
				gfxDevice.WriteFinalColor4b();
				
				gfxDevice.SetPenColor(penColor);
			}
			
			gfxDevice.EndPrim();
			
		}
		
	}
	
	
	
}

//----------------------------------------------------------------//
void MOAIDraw::DrawBeveledLines(lua_State *L, float lineWidth, float blurMargin){
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	MOAILuaState state ( L );
	
	// TODO: rename the float, boolean and vector variables here to make more sense
	
	// the x and y components of the three points making up the corner
	float p0x, p0y, p1x, p1y, p2x, p2y;
	// r0 to r3 are the points defining the rectangle of the first segment
	// r4 to r7 are the points defining the rectangle of the second segment
	// r8 and r9 are the corner intersection points
	// line1Normal and line2Normal are the normalized line vectors.  line1 defines p0p1 vector. line2 defines p1p2 vector.
	// line1Normal and line2Normal are the normalized vectors anti-clockwise from line1 and line2 respectively
	// q0 and q1 are storage variables for the next segment, to be used as the values of r0 and r2
	USVec2D r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, line1, line1Normal, line2, line2Normal, q0, q1;
	
	// i1 and i2 are the boolean variables used to determine if an intersection point exists.
	bool i1, i2;
	
	// j1 and j2 are the boolean variables used to determine if the line segments intersect.  At most, one of them should be true
	bool j1, j2;
	
	// lw is half the line width.
	float lw = lineWidth / 2;
	
	// render the blur boundaries when blurMargin parameter is greater than zero.
	bool renderBlur = blurMargin > 0.0f;
	
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
	
	// the b variables are the blur boundary counterparts of the r variables
	USVec2D b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, q2, q3;
	// Additional boolean variables for lines defining northern and southern 
	bool i3, i4, j3, j4;
	// blur width 
	float bw = lw + blurMargin;
	
	// get pen color
	USColorVec penColor = gfxDevice.GetPenColor();
	// make transparent color
	USColorVec transColor(penColor);
	transColor.mA = 0.0f;
	if ( MOAIGfxDevice::Get ().GetColorPremultiply () ) {
		transColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
	}
	
	for (int i = 0; i < counter - 5; i += 2){
		p0x = vertexArray[i];
		p0y = vertexArray[i+1];
		p1x = vertexArray[i+2];
		p1y = vertexArray[i+3];
		p2x = vertexArray[i+4];
		p2y = vertexArray[i+5];
		
		// calculate line vectors
		line1.Init(p1x - p0x, p1y - p0y);
		if (line1.LengthSquared() == 0) {
			continue;
		}
		line1.Norm();
		line1Normal.Init(line1);
		line1Normal.Rotate90Anticlockwise();
		
		line2.Init(p2x - p1x, p2y - p1y);
		if (line2.LengthSquared() == 0) {
			continue;
		}
		line2.Norm();
		line2Normal.Init(line2);
		line2Normal.Rotate90Anticlockwise();
		
		
		// calculate render points
		if (i == 0) {
			// "northwest" corner or first segment solid boundary
			r0.Init(p0x + lw * line1Normal.mX, p0y + lw * line1Normal.mY);
			// "southwest" corner of first segment solid boundary
			r2.Init(p0x - lw * line1Normal.mX, p0y - lw * line1Normal.mY);
			
			// "northwest" corner or first segment blur boundary
			b0.Init(p0x + bw * line1Normal.mX, p0y + bw * line1Normal.mY);
			// "southwest" corner or first segment blur boundary
			b2.Init(p0x - bw * line1Normal.mX, p0y - bw * line1Normal.mY);
		}
		else{
			r0.Init(q0);
			r2.Init(q1);
			
			b0.Init(q2);
			b2.Init(q3);
		}
		
		// "northeast" corner of first segment
		r1.Init(p1x + lw * line1Normal.mX, p1y + lw * line1Normal.mY);
		b1.Init(p1x + bw * line1Normal.mX, p1y + bw * line1Normal.mY);
		
		// "southeast" corner of first segment
		r3.Init(p1x - lw * line1Normal.mX, p1y - lw * line1Normal.mY);
		b3.Init(p1x - bw * line1Normal.mX, p1y - bw * line1Normal.mY);
		
		// "northwest" corner of second segment
		r4.Init(p1x + lw * line2Normal.mX, p1y + lw * line2Normal.mY);
		b4.Init(p1x + bw * line2Normal.mX, p1y + bw * line2Normal.mY);
		
		// "northeast" corner of second segment
		r5.Init(p2x + lw * line2Normal.mX, p2y + lw * line2Normal.mY);
		b5.Init(p2x + bw * line2Normal.mX, p2y + bw * line2Normal.mY);
		
		// "southeast" corner of second segment
		r6.Init(p1x - lw * line2Normal.mX, p1y - lw * line2Normal.mY);
		b6.Init(p1x - bw * line2Normal.mX, p1y - bw * line2Normal.mY);
		
		// "southwest" corner of second segment
		r7.Init(p2x - lw * line2Normal.mX, p2y - lw * line2Normal.mY);
		b7.Init(p2x - bw * line2Normal.mX, p2y - bw * line2Normal.mY);
		
		
		// find intersection points
		j1 = USVec2D::GetLineIntersection(r0, r1, r4, r5, &r8, &i1);
		j2 = USVec2D::GetLineIntersection(r2, r3, r6, r7, &r9, &i2);
		j3 = USVec2D::GetLineIntersection(b0, b1, b4, b5, &b8, &i3);
		j4 = USVec2D::GetLineIntersection(b2, b3, b6, b7, &b9, &i4);
		
		bool allPointsFound = (i1 && i2 && i3 && i4);
		bool blurIntersection = (j3 || j4);
		bool solidIntersection = (j1 || j2);
		
		// fallback for co-linear points, parallel and anti-parallel cases
		if ( !allPointsFound ) {
			r8.Init(r1);
			r9.Init(r3);
			b8.Init(b1);
			b9.Init(b3);
			
			q0.Init(r4);
			q1.Init(r6);
			q2.Init(b4);
			q3.Init(b6);
		}
		// case where neither pair of blur boundaries intersects
		else if ( !blurIntersection ) {
			// find out which intersection point to reassign.
			// b9 in right-handed corners, b8 in left-handed corners
			
			
			bool rightHanded = line1.Cross(line2) > 0.0f;
			// right-handed case where cross product of line1 with line2 is positive
			if ( rightHanded ) {
				b9.Init(b3);
				
				q2.Init(b8);
				q3.Init(b6);
			}
			// left-handed case
			else{
				b8.Init(b1);
				
				q2.Init(b4);
				q3.Init(b9);
			}
			
			// case where neither pair of solid boundaries intersects
			if ( !solidIntersection ) {
				if (rightHanded) {
					r9.Init(r3);
					
					q0.Init(r8);
					q1.Init(r6);
				}
				else{
					r8.Init(r1);
					
					q0.Init(r4);
					q1.Init(r9);
				}
			}
			else{
				q0.Init(r8);
				q1.Init(r9);
			}
			
		}
		else{
			// save points r8 and r9 for later use
			q0.Init(r8);
			q1.Init(r9);
			q2.Init(b8);
			q3.Init(b9);
		}
		
		// render the first segment
		
		gfxDevice.BeginPrim(GL_TRIANGLE_STRIP);
		
		if (renderBlur) {
			gfxDevice.SetPenColor(transColor);
			// write b0
			gfxDevice.WriteVtx(b0);
			gfxDevice.WriteFinalColor4b();
			// write b8
			gfxDevice.WriteVtx(b8);
			gfxDevice.WriteFinalColor4b();
		}
		
		
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
		
		if (renderBlur) {
			gfxDevice.SetPenColor(transColor);
			
			// write b2
			gfxDevice.WriteVtx(b2);
			gfxDevice.WriteFinalColor4b();
			
			// write b9
			gfxDevice.WriteVtx(b9);
			gfxDevice.WriteFinalColor4b();
		}
		
		
		gfxDevice.EndPrim();
		
		// render the second segment if at the end.
		if (i + 6 >= counter ) {
			
			gfxDevice.BeginPrim(GL_TRIANGLE_STRIP);
			if (renderBlur) {
				//gfxDevice.SetPenColor(transColor);
				// write q2 AKA b8 or b4
				gfxDevice.WriteVtx(q2);
				gfxDevice.WriteFinalColor4b();
				
				// write b5
				gfxDevice.WriteVtx(b5);
				gfxDevice.WriteFinalColor4b();
				
				gfxDevice.SetPenColor(penColor);
			}
			
			// write q0 AKA r8 or r4
			gfxDevice.WriteVtx(q0);
			gfxDevice.WriteFinalColor4b();
			
			// write r5
			gfxDevice.WriteVtx(r5);
			gfxDevice.WriteFinalColor4b();
			
			// write q1 AKA r9 or r6
			gfxDevice.WriteVtx(q1);
			gfxDevice.WriteFinalColor4b();
			
			// write r7
			gfxDevice.WriteVtx(r7);
			gfxDevice.WriteFinalColor4b();
			
			if (renderBlur) {
				gfxDevice.SetPenColor(transColor);
				// write q3 AKA b9 or b6
				gfxDevice.WriteVtx(q3);
				gfxDevice.WriteFinalColor4b();
				
				// write b7
				gfxDevice.WriteVtx(b7);
				gfxDevice.WriteFinalColor4b();
				
				gfxDevice.SetPenColor(penColor);
			}
			
			gfxDevice.EndPrim();
			
		}
		
	}
		
		
	/*
	for (int i = 0; i < counter - 5; i += 2){
		p0x = vertexArray[i];
		p0y = vertexArray[i+1];
		p1x = vertexArray[i+2];
		p1y = vertexArray[i+3];
		p2x = vertexArray[i+4];
		p2y = vertexArray[i+5];
		
		// calculate line vectors
		line1.Init(p1x - p0x, p1y - p0y);
		if (line1.LengthSquared() == 0) {
			continue;
		}
		line1.Norm();
		line1Normal.Init(line1);
		line1Normal.Rotate90Anticlockwise();
		
		line2.Init(p2x - p1x, p2y - p1y);
		if (line2.LengthSquared() == 0) {
			continue;
		}
		line2.Norm();
		line2Normal.Init(line2);
		line2Normal.Rotate90Anticlockwise();
		
		
		// calculate render points
		if (i == 0) {
			r0.Init(p0x + lw * line1Normal.mX, p0y + lw * line1Normal.mY);
			r2.Init(p0x - lw * line1Normal.mX, p0y - lw * line1Normal.mY);
		}
		else{
			r0.Init(q0);
			r2.Init(q1);
		}
		
		r1.Init(p1x + lw * line1Normal.mX, p1y + lw * line1Normal.mY);
		r3.Init(p1x - lw * line1Normal.mX, p1y - lw * line1Normal.mY);
		
		r4.Init(p1x + lw * line2Normal.mX, p1y + lw * line2Normal.mY);
		r5.Init(p2x + lw * line2Normal.mX, p2y + lw * line2Normal.mY);
		r6.Init(p1x - lw * line2Normal.mX, p1y - lw * line2Normal.mY);
		r7.Init(p2x - lw * line2Normal.mX, p2y - lw * line2Normal.mY);
		
		
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
	*/
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
void MOAIDraw::DrawCardinalSplineLoop(lua_State *L, float tension, u32 steps){
	USLeanArray<float> vertexArray;
	const u32 chunk_size = 8;
	MOAILuaState state ( L );
	
	vertexArray.Init(chunk_size);
	float p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y;
	int counter = 0;
	
	if (lua_istable(L, 1)) {
		lua_pushnil(L);
		while (lua_next(L, 1) != 0 ) {
			u32 arraySize = vertexArray.Size();
			if(arraySize <= (u32) counter){
				vertexArray.Grow(arraySize + chunk_size);
			}
			
			// push value into vertex array
			vertexArray[counter] = state.GetValue <float> (-1, 0.0f);
			++counter;
			lua_pop(L, 1);
		}
		
	}
	else{
		u32 total = state.GetTop ();
		
		for (u32 j = 1; j < total; ++j, ++counter) {
			u32 arraySize = vertexArray.Size();
			if(arraySize <= (u32) counter){
				vertexArray.Grow(arraySize + chunk_size);
			}
			
			vertexArray[counter] = state.GetValue < float > (j, 0.0f);
		}
	}
	
	if (counter < 4) {
		return;
	}
	
	for (int i = 0; i < counter - 1; i += 2) {
		
		p1x = vertexArray[i+0];
		p1y = vertexArray[i+1];
		if (i >= 2){
			p0x = vertexArray[i-2];
			p0y = vertexArray[i-1];
		}
		else{
			// set to last point
			p0x = vertexArray[counter - 2];
			p0y = vertexArray[counter - 1];
		}
		
		if (i < counter - 5) {
			p3x = vertexArray[i+4];
			p3y = vertexArray[i+5];
		}
		else{
			// set to first point
			p3x = vertexArray[0];
			p3y = vertexArray[1];
		}
		if (i < counter - 3){
			p2x = vertexArray[i+2];
			p2y = vertexArray[i+3];
		}
		else{
			// set to first point
			p2x = vertexArray[0];
			p2y = vertexArray[1];
			// set to second point
			p3x = vertexArray[2];
			p3y = vertexArray[3];
		}
		MOAIDraw::DrawCardinalSpline(p1x, p1y, p2x, p2y, p0x, p0y, p3x, p3y, tension, steps);
	}
}

//----------------------------------------------------------------//
void MOAIDraw::DrawCardinalSplineStrip(lua_State *L, float tension, u32 steps){
	USLeanArray<float> vertexArray;
	const u32 chunk_size = 8;
	MOAILuaState state ( L );
	
	vertexArray.Init(chunk_size);
	float p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y;
	int counter = 0;
	
	if (lua_istable(L, 1)) {
		lua_pushnil(L);
		while (lua_next(L, 1) != 0 ) {
			u32 arraySize = vertexArray.Size();
			if(arraySize <= (u32) counter){
				vertexArray.Grow(arraySize + chunk_size);
			}
			
			// push value into vertex array
			vertexArray[counter] = state.GetValue <float> (-1, 0.0f);
			++counter;
			lua_pop(L, 1);
		}
		
	}
	else{
		u32 total = state.GetTop ();
		
		for (u32 j = 1; j < total; ++j, ++counter) {
			u32 arraySize = vertexArray.Size();
			if(arraySize <= (u32) counter){
				vertexArray.Grow(arraySize + chunk_size);
			}
			
			vertexArray[counter] = state.GetValue < float > (j, 0.0f);
		}
	}
	
	if (counter < 4) {
		return;
	}
	
	for (int i = 0; i < counter - 3; i += 2) {
		
		p1x = vertexArray[i+0];
		p1y = vertexArray[i+1];
		if (i >= 2){
			p0x = vertexArray[i-2];
			p0y = vertexArray[i-1];
		}
		else{
			p0x = p1x;
			p0y = p1y;
		}
		p2x = vertexArray[i+2];
		p2y = vertexArray[i+3];
		
		if (i < counter - 5) {
			p3x = vertexArray[i+4];
			p3y = vertexArray[i+5];
		}
		else{
			p3x = p2x;
			p3y = p2y;
		}
		
		MOAIDraw::DrawCardinalSpline(p1x, p1y, p2x, p2y, p0x, p0y, p3x, p3y, tension, steps);
	}
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

void MOAIDraw::DrawEllipticalSliceFill ( float x, float y, float xRad, float yRad, float angle, float offset, float blurMargin, u32 steps ) {
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	bool renderBlur = blurMargin > 0.0f;
	
	
	float theta = angle * ( float )D2R / ( float )steps;
	float thetaStep = offset * (float)D2R;
	
	USColorVec penColor = gfxDevice.GetPenColor();
	
	gfxDevice.BeginPrim( GL_TRIANGLE_FAN );
	
	gfxDevice.WriteVtx(x, y, 0.0f);
	gfxDevice.WriteFinalColor4b();
	
	u32 i;
	for (i = 0; i <= steps; ++i, thetaStep += theta) {
		gfxDevice.WriteVtx (
							x + ( Sin ( thetaStep ) * xRad ),
							y + ( Cos ( thetaStep ) * yRad ),
							0.0f
							);
		gfxDevice.WriteFinalColor4b ();
	}
	
	gfxDevice.EndPrim();
	
	if (renderBlur) {
		USColorVec transColor(penColor);
		transColor.mA = 0.0f;
		if (gfxDevice.GetColorPremultiply()) {
			transColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
		}
		
		thetaStep = offset * (float)D2R;
		// render the arc section
		gfxDevice.BeginPrim( GL_TRIANGLE_STRIP );
		for (i = 0; i <= steps; ++i, thetaStep += theta ) {
			
			// point at blur margin
			gfxDevice.SetPenColor(transColor);
			gfxDevice.WriteVtx (
								x + ( Sin ( thetaStep ) * (xRad + blurMargin) ),
								y + ( Cos ( thetaStep ) * (yRad + blurMargin) ),
								0.0f
								);
			gfxDevice.WriteFinalColor4b ();
			
			
			gfxDevice.SetPenColor(penColor);
			gfxDevice.WriteVtx (
								x + ( Sin ( thetaStep ) * xRad ),
								y + ( Cos ( thetaStep ) * yRad ),
								0.0f
								);
			gfxDevice.WriteFinalColor4b ();
			
		}
		
		gfxDevice.EndPrim();
		
	}
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
void MOAIDraw::DrawJoinedCorner(float x0, float y0, float x1, float y1, float x2, float y2, float lineWidth, float blurMargin){
	
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
	bool i1, i2;
	i2 = USVec2D::GetLineIntersection(p1, p3a, p3b, p5, &p3, &i1);
	
	USVec2D p4; // intersection of p2-p4a and p4b-p6
	bool i3, i4;
	i4 = USVec2D::GetLineIntersection(p2, p4a, p4b, p6, &p4, &i3);
	
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
	USVec2D::GetLineIntersection(p2z, p4za, p4zb, p6z, &p4z, &i3);
	
	
	if (i1 && i2) {
		p4.Init(p4a);
		p4z.Init(p4za);
	}else if (i3 && i4){
		p3.Init(p3a);
		p3z.Init(p3za);
	}else{
		p4.Init(p4a);
		p4z.Init(p4za);
		p3.Init(p3a);
		p3z.Init(p3za);
	}
	MOAIGfxDevice &gfxDevice = MOAIGfxDevice::Get();
	// get pen color
	USColorVec penColor = gfxDevice.GetPenColor();
	// make transparent color
	USColorVec transColor(penColor);
	transColor.mA = 0.0f;
	if ( MOAIGfxDevice::Get ().GetColorPremultiply () ) {
		transColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
	}
	
	bool renderBlur = blurMargin > 0.0f;
	
	
	// render the L1 segment
	gfxDevice.BeginPrim(GL_TRIANGLE_STRIP);
	if (renderBlur) {
		gfxDevice.SetPenColor(transColor);
		// write p1z
		gfxDevice.WriteVtx(p1z);
		gfxDevice.WriteFinalColor4b ();
		
		// write p3z
		gfxDevice.WriteVtx ( p3z );
		gfxDevice.WriteFinalColor4b ();
		
	}
	
	gfxDevice.SetPenColor(penColor);
	
	// write p1
	gfxDevice.WriteVtx ( p1 );
	gfxDevice.WriteFinalColor4b ();
	
	// write p3
	gfxDevice.WriteVtx ( p3 );
	gfxDevice.WriteFinalColor4b ();
	
	
	// write p2
	gfxDevice.WriteVtx ( p2 );
	gfxDevice.WriteFinalColor4b ();
	
	// write p4
	gfxDevice.WriteVtx ( p4 );
	gfxDevice.WriteFinalColor4b ();
	
	if (renderBlur) {
		gfxDevice.SetPenColor(transColor);
		
		// write p2z
		gfxDevice.WriteVtx ( p2z );
		gfxDevice.WriteFinalColor4b ();
		
		// write p4z
		gfxDevice.WriteVtx ( p4z );
		gfxDevice.WriteFinalColor4b ();
	}
	
	
	gfxDevice.EndPrim();
	
	
	if (i1 && i2) {
		p4.Init(p4b);
		p4z.Init(p4zb);
	}else if (i3 && i4){
		p3.Init(p3b);
		p3z.Init(p3zb);
	}else{
		p4.Init(p4b);
		p4z.Init(p4zb);
		p3.Init(p3b);
		p3z.Init(p3zb);
	}

	 
	// render the L2 segment
	gfxDevice.BeginPrim(GL_TRIANGLE_STRIP);
	
	if (renderBlur) {
		gfxDevice.SetPenColor(transColor);
		
		// write p3z
		gfxDevice.WriteVtx ( p3z );
		gfxDevice.WriteFinalColor4b ();
		
		// write p5z
		gfxDevice.WriteVtx ( p5z );
		gfxDevice.WriteFinalColor4b ();
	}
	
	gfxDevice.SetPenColor(penColor);
	// write p3
	gfxDevice.WriteVtx ( p3 );
	gfxDevice.WriteFinalColor4b ();
	
	// write p5
	gfxDevice.WriteVtx ( p5 );
	gfxDevice.WriteFinalColor4b ();
	
	// write p4
	gfxDevice.WriteVtx ( p4 );
	gfxDevice.WriteFinalColor4b ();
	
	// write p6
	gfxDevice.WriteVtx ( p6 );
	gfxDevice.WriteFinalColor4b ();
	
	if (renderBlur) {
		gfxDevice.SetPenColor(transColor);
		
		// write p4z
		gfxDevice.WriteVtx ( p4z );
		gfxDevice.WriteFinalColor4b ();
		
		// write p6z
		gfxDevice.WriteVtx ( p6z );
		gfxDevice.WriteFinalColor4b ();
	}
	
	gfxDevice.EndPrim();
	
	
	
	// render the corner
	if (i1 && i2) {
		gfxDevice.BeginPrim(GL_TRIANGLE_STRIP);
		
		if (renderBlur){
			gfxDevice.SetPenColor(transColor);
			// write p4za
			gfxDevice.WriteVtx ( p4za );
			gfxDevice.WriteFinalColor4b ();
			
			// write p4zb
			gfxDevice.WriteVtx ( p4zb );
			gfxDevice.WriteFinalColor4b ();
		}
		
		gfxDevice.SetPenColor(penColor);
		
		// write p4a
		gfxDevice.WriteVtx ( p4a );
		gfxDevice.WriteFinalColor4b ();
		
		// write p4b
		gfxDevice.WriteVtx ( p4b );
		gfxDevice.WriteFinalColor4b ();
		
		// write p3
		gfxDevice.WriteVtx ( p3 );
		gfxDevice.WriteFinalColor4b ();
		
		
		gfxDevice.EndPrim();
	}
	else if (i3 && i4){
		gfxDevice.BeginPrim(GL_TRIANGLE_STRIP);
		
		if (renderBlur){
			gfxDevice.SetPenColor(transColor);
			
			// write p3za
			gfxDevice.WriteVtx ( p3za );
			gfxDevice.WriteFinalColor4b ();
			
			// write p3zb
			gfxDevice.WriteVtx ( p3zb );
			gfxDevice.WriteFinalColor4b ();
		
		}
		
		gfxDevice.SetPenColor(penColor);
		
		// write p3a
		gfxDevice.WriteVtx ( p3a );
		gfxDevice.WriteFinalColor4b ();
		
		// write p3b
		gfxDevice.WriteVtx ( p3b );
		gfxDevice.WriteFinalColor4b ();
		
		// write p4
		gfxDevice.WriteVtx ( p4 );
		gfxDevice.WriteFinalColor4b ();
		
		
		gfxDevice.EndPrim();
	}
	
	
	gfxDevice.SetPenColor(penColor);
	
}

//----------------------------------------------------------------//
void MOAIDraw::DrawJoinedLine(lua_State *L, float lineWidth, float blurMargin){
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	MOAILuaState state ( L );
	
	// TODO: rename the float, boolean and vector variables here to make more sense
	
	// the x and y components of the three points making up the corner
	float p0x, p0y, p1x, p1y, p2x, p2y;
	
	// r0 to r3 are the points defining the rectangle of the first segment
	// r4 to r7 are the points defining the rectangle of the second segment
	// r8 and r9 are the corner intersection points
	// line1 and line2 are the normalized line vectors.  line1 defines p0p1 vector. line2 defines p1p2 vector.
	// line1Normal and line2Normal are the normalized vectors anti-clockwise from line1 and line2 respectively
	// q0 and q1 are storage variables for the next segment, to be used as the values of r0 and r2
	USVec2D r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, line1, line1Normal, line2, line2Normal, q0, q1;
	
	USVec2D b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, q2, q3;
	
	// i1 to i4 are the boolean variables used to determine if an intersection point exists.
	bool i1, i2, i3, i4;
	
	// j1 to j4 are the boolean variables used to determine if the line segments intersect.  At most, one of them should be true
	bool j1, j2, j3, j4;
	
	// render the blur boundaries when blurMargin parameter is greater than zero.
	bool renderBlur = blurMargin > 0.0f;
	
	// lw is half the line width.
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
	
	float bw = lw + blurMargin;
	
	// get pen color
	USColorVec penColor = gfxDevice.GetPenColor();
	// make transparent color
	USColorVec transColor(penColor);
	transColor.mA = 0.0f;
	if ( MOAIGfxDevice::Get ().GetColorPremultiply () ) {
		transColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
	}
	
	for (int i = 0; i < counter - 5; i += 2){
		p0x = vertexArray[i];
		p0y = vertexArray[i+1];
		p1x = vertexArray[i+2];
		p1y = vertexArray[i+3];
		p2x = vertexArray[i+4];
		p2y = vertexArray[i+5];
		
		// calculate line vectors
		line1.Init(p1x - p0x, p1y - p0y);
		if (line1.LengthSquared() == 0) {
			continue;
		}
		line1.Norm();
		line1Normal.Init(line1);
		line1Normal.Rotate90Anticlockwise();
		
		line2.Init(p2x - p1x, p2y - p1y);
		if (line2.LengthSquared() == 0) {
			continue;
		}
		line2.Norm();
		line2Normal.Init(line2);
		line2Normal.Rotate90Anticlockwise();
		
		
		// calculate render points
		if (i == 0) {
			r0.Init(p0x + lw * line1Normal.mX, p0y + lw * line1Normal.mY);
			r2.Init(p0x - lw * line1Normal.mX, p0y - lw * line1Normal.mY);
			
			b0.Init(p0x + bw * line1Normal.mX, p0y + bw * line1Normal.mY);
			b2.Init(p0x - bw * line1Normal.mX, p0y - bw * line1Normal.mY);
		}
		else{
			r0.Init(q0);
			r2.Init(q1);
			
			b0.Init(q2);
			b2.Init(q3);
		}
		
		r1.Init(p1x + lw * line1Normal.mX, p1y + lw * line1Normal.mY);
		b1.Init(p1x + bw * line1Normal.mX, p1y + bw * line1Normal.mY);
		r3.Init(p1x - lw * line1Normal.mX, p1y - lw * line1Normal.mY);
		b3.Init(p1x - bw * line1Normal.mX, p1y - bw * line1Normal.mY);
		
		r4.Init(p1x + lw * line2Normal.mX, p1y + lw * line2Normal.mY);
		b4.Init(p1x + bw * line2Normal.mX, p1y + bw * line2Normal.mY);
		
		r5.Init(p2x + lw * line2Normal.mX, p2y + lw * line2Normal.mY);
		b5.Init(p2x + bw * line2Normal.mX, p2y + bw * line2Normal.mY);
		
		r6.Init(p1x - lw * line2Normal.mX, p1y - lw * line2Normal.mY);
		b6.Init(p1x - bw * line2Normal.mX, p1y - bw * line2Normal.mY);
		
		r7.Init(p2x - lw * line2Normal.mX, p2y - lw * line2Normal.mY);
		b7.Init(p2x - bw * line2Normal.mX, p2y - bw * line2Normal.mY);
		
		
		// find intersection points
		j1 = USVec2D::GetLineIntersection(r0, r1, r4, r5, &r8, &i1);
		j2 = USVec2D::GetLineIntersection(r2, r3, r6, r7, &r9, &i2);
		j3 = USVec2D::GetLineIntersection(b0, b1, b4, b5, &b8, &i3);
		j4 = USVec2D::GetLineIntersection(b2, b3, b6, b7, &b9, &i4);
		
		bool allPointsFound = (i1 && i2 && i3 && i4);
		bool blurIntersection = (j3 || j4);
		bool solidIntersection = (j1 || j2);
		
		bool rightHanded = line1.Cross(line2) > 0.0f;
		bool leftHanded = line1.Cross(line2) < 0.0f;
		
		// fallback for co-linear points
		if ( !allPointsFound ) {
			r8.Init(r1);
			r9.Init(r3);
			b8.Init(b1);
			b9.Init(b3);
			
			// find out if in parallel or anti-parallel case
			bool isParallel = line1.Dot(line2) > 0.0f;
			
			if (isParallel) {
				q0.Init(r8);
				q1.Init(r9);
				q2.Init(b8);
				q3.Init(b9);
			}
			else{
				q0.Init(r9);
				q1.Init(r8);
				q2.Init(b9);
				q3.Init(b8);
			}
		}
		// right-hand case
		else if ( rightHanded ) { // j2 && j4
			
			if (! blurIntersection) {
				
				b9.Init(b3);
			
				q3.Init(b6);
				
				if (! solidIntersection ){
					r9.Init(r3);
					
					q1.Init(r6);
				}
				else{
					q1.Init(r9);
				}
				
			}
			else{
				
				q1.Init(r9);
				q3.Init(b9);
			}
			
			r8.Init(r1);
			b8.Init(b1);
			
			q0.Init(r4);
			q2.Init(b4);
		}
		// left-handed case
		else { // j1 && j3
			if (! blurIntersection) {
				
				b8.Init(b1);
				
				q2.Init(b4);
				
				if (! solidIntersection) {
					r8.Init(r1);
					
					q0.Init(r4);
				}
				else{
					q0.Init(r8);
				}
				
			}
			else {
				
				
				q0.Init(r8);
				
				q2.Init(b8);
				
			}
			r9.Init(r3);
			b9.Init(b3);
			
			q1.Init(r6);
			q3.Init(b6);
			
		}
		
		
		// render the first segment
		
		gfxDevice.BeginPrim(GL_TRIANGLE_STRIP);
		
		if (renderBlur) {
			gfxDevice.SetPenColor(transColor);
			// write b0
			gfxDevice.WriteVtx(b0);
			gfxDevice.WriteFinalColor4b();
			// write b8
			gfxDevice.WriteVtx(b8);
			gfxDevice.WriteFinalColor4b();
		}
		
		
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
		
		if ( renderBlur ) {
			gfxDevice.SetPenColor(transColor);
			
			// write b2
			gfxDevice.WriteVtx(b2);
			gfxDevice.WriteFinalColor4b();
			
			// write b9
			gfxDevice.WriteVtx(b9);
			gfxDevice.WriteFinalColor4b();
		}
		
		gfxDevice.EndPrim();
		
		// render corner
		gfxDevice.BeginPrim(GL_TRIANGLE_STRIP);
		if (leftHanded) { // j1
			if (renderBlur) {
				//gfxDevice.SetPenColor(transColor);
				// write b3
				gfxDevice.WriteVtx(b3);
				gfxDevice.WriteFinalColor4b();
				
				// write b6
				gfxDevice.WriteVtx(b6);
				gfxDevice.WriteFinalColor4b();
				
			}
			gfxDevice.SetPenColor(penColor);
			// write r3
			gfxDevice.WriteVtx(r3);
			gfxDevice.WriteFinalColor4b();
			
			// write r6
			gfxDevice.WriteVtx(r6);
			gfxDevice.WriteFinalColor4b();
			
			// write r8
			gfxDevice.WriteVtx(r8);
			gfxDevice.WriteFinalColor4b();
		}
		else if (rightHanded){
			if (renderBlur) {
				// write b1
				gfxDevice.WriteVtx(b1);
				gfxDevice.WriteFinalColor4b();
				
				// write b4
				gfxDevice.WriteVtx(b4);
				gfxDevice.WriteFinalColor4b();
			}
			gfxDevice.SetPenColor(penColor);
			// write r1
			gfxDevice.WriteVtx(r1);
			gfxDevice.WriteFinalColor4b();
			
			// write r4
			gfxDevice.WriteVtx(r4);
			gfxDevice.WriteFinalColor4b();
			
			// write r9
			gfxDevice.WriteVtx(r9);
			gfxDevice.WriteFinalColor4b();
			
		}
		gfxDevice.EndPrim();
		
		// render the second segment if at the end.
		if (i + 6 >= counter ) {
			
			if ( rightHanded ) { //( j2 && j4 ) {
				r8.Init(r4);
				b8.Init(b4);
				
				if (!blurIntersection) {
					b9.Init(b6);
					if (!solidIntersection) {
						r9.Init(r6);
					}
				}
				
				
			}
			else /* if ( j1 && j3 ) */ {
				r9.Init(r6);
				b9.Init(b6);
				
				if (!blurIntersection) {
					b8.Init(b4);
					if (!solidIntersection) {
						r8.Init(r4);
					}
				}
				
			}
			
			gfxDevice.BeginPrim(GL_TRIANGLE_STRIP);
			
			if ( renderBlur ) {
				gfxDevice.SetPenColor(transColor);
				// write b8
				gfxDevice.WriteVtx(b8);
				gfxDevice.WriteFinalColor4b();
				
				// write b5
				gfxDevice.WriteVtx(b5);
				gfxDevice.WriteFinalColor4b();
			
			}
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
			
			if ( renderBlur ) {
				gfxDevice.SetPenColor(transColor);
				// write b9
				gfxDevice.WriteVtx(b9);
				gfxDevice.WriteFinalColor4b();
				
				// write b7
				gfxDevice.WriteVtx(b7);
				gfxDevice.WriteFinalColor4b();
			}
			
			gfxDevice.EndPrim();
			
		}
		gfxDevice.SetPenColor(penColor);
	}
	
}

//----------------------------------------------------------------//
void MOAIDraw::DrawJoinedLineLoop(lua_State *L, float lineWidth, float blurMargin){
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	MOAILuaState state ( L );
	
	// the x and y components of the three points making up the corner
	float p0x, p0y, p1x, p1y, p2x, p2y;
	
	// r0 to r3 are the points defining the rectangle of the first segment
	// r4 to r7 are the points defining the rectangle of the second segment
	// r8 and r9 are the corner intersection points
	// line1 and line2 are the normalized line vectors.  line1 defines p0p1 vector. line2 defines p1p2 vector.
	// line1Normal and line2Normal are the normalized vectors anti-clockwise from line1 and line2 respectively
	// q0 and q1 are storage variables for the next segment, to be used as the values of r0 and r2
	USVec2D r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, line1, line1Normal, line2, line2Normal, q0, q1;
	
	USVec2D b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, q2, q3;
	
	// i1 to i4 are the boolean variables used to determine if an intersection point exists.
	bool i1, i2, i3, i4;
	
	// j1 to j4 are the boolean variables used to determine if the line segments intersect.  At most, one of them should be true
	bool j1, j2, j3, j4;
	
	// render the blur boundaries when blurMargin parameter is greater than zero.
	bool renderBlur = blurMargin > 0.0f;
	
	// lw is half the line width.
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
	
	float bw = lw + blurMargin;
	
	// the first intersection points saved for rendering the segment between points 0 and 1 in the final step
	USVec2D firstSolidIntNorth, firstSolidIntSouth, firstBlurIntNorth, firstBlurIntSouth;
	
	// get pen color
	USColorVec penColor = gfxDevice.GetPenColor();
	// make transparent color
	USColorVec transColor(penColor);
	transColor.mA = 0.0f;
	if ( MOAIGfxDevice::Get ().GetColorPremultiply () ) {
		transColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
	}
	
	for (int i = 0; i < counter - 1; i += 2) {
		p0x = vertexArray[i];
		p0y = vertexArray[i+1];
		
		
		if (i < counter - 5) {
			p2x = vertexArray[i+4];
			p2y = vertexArray[i+5];
		}
		else{
			p2x = vertexArray[0];
			p2y = vertexArray[1];
		}
		
		if (i < counter - 3){
			p1x = vertexArray[i+2];
			p1y = vertexArray[i+3];
		}
		else{
			p1x = vertexArray[0];
			p1y = vertexArray[1];
			p2x = vertexArray[2];
			p2y = vertexArray[3];
		}
		
		
		// calculate line vectors
		line1.Init(p1x - p0x, p1y - p0y);
		if (line1.LengthSquared() == 0) {
			continue;
		}
		line1.Norm();
		line1Normal.Init(line1);
		line1Normal.Rotate90Anticlockwise();
		
		line2.Init(p2x - p1x, p2y - p1y);
		if (line2.LengthSquared() == 0) {
			continue;
		}
		line2.Norm();
		line2Normal.Init(line2);
		line2Normal.Rotate90Anticlockwise();
		
		
		// calculate render points
		if (i == 0) {
			// "northwest" corner of fist segment
			r0.Init(p0x + lw * line1Normal.mX, p0y + lw * line1Normal.mY);
			b0.Init(p0x + bw * line1Normal.mX, p0y + bw * line1Normal.mY);
			
			// "southwest" corner of fist segment
			r2.Init(p0x - lw * line1Normal.mX, p0y - lw * line1Normal.mY);
			b2.Init(p0x - bw * line1Normal.mX, p0y - bw * line1Normal.mY);
		}
		else{
			r0.Init(q0);
			b0.Init(q2);
			
			r2.Init(q1);
			b2.Init(q3);
		}
		// "northeast" corner of first segment
		r1.Init(p1x + lw * line1Normal.mX, p1y + lw * line1Normal.mY);
		b1.Init(p1x + bw * line1Normal.mX, p1y + bw * line1Normal.mY);
		
		// "southeast" corner of first segment
		r3.Init(p1x - lw * line1Normal.mX, p1y - lw * line1Normal.mY);
		b3.Init(p1x - bw * line1Normal.mX, p1y - bw * line1Normal.mY);
		
		// "northwest" corner of second segment
		r4.Init(p1x + lw * line2Normal.mX, p1y + lw * line2Normal.mY);
		b4.Init(p1x + bw * line2Normal.mX, p1y + bw * line2Normal.mY);
		
		// "southwest" corner of second segment
		r6.Init(p1x - lw * line2Normal.mX, p1y - lw * line2Normal.mY);
		b6.Init(p1x - bw * line2Normal.mX, p1y - bw * line2Normal.mY);
		
		
		if (i < counter - 3) {
			// "northeast" corner of second segment
			r5.Init(p2x + lw * line2Normal.mX, p2y + lw * line2Normal.mY);
			b5.Init(p2x + bw * line2Normal.mX, p2y + bw * line2Normal.mY);
			
			// "southeat" corner of second segment
			r7.Init(p2x - lw * line2Normal.mX, p2y - lw * line2Normal.mY);
			b7.Init(p2x - bw * line2Normal.mX, p2y - bw * line2Normal.mY);
		}
		else{
			r5.Init(firstSolidIntNorth);
			b5.Init(firstBlurIntNorth);
			
			r7.Init(firstSolidIntSouth);
			b7.Init(firstBlurIntSouth);
		}
		
		
		
		// find intersection points
		j1 = USVec2D::GetLineIntersection(r0, r1, r4, r5, &r8, &i1);
		j2 = USVec2D::GetLineIntersection(r2, r3, r6, r7, &r9, &i2);
		j3 = USVec2D::GetLineIntersection(b0, b1, b4, b5, &b8, &i3);
		j4 = USVec2D::GetLineIntersection(b2, b3, b6, b7, &b9, &i4);
		
		bool allPointsFound = (i1 && i2 && i3 && i4);
		bool blurIntersection = (j3 || j4);
		bool solidIntersection = (j1 || j2);
		
		bool rightHanded = line1.Cross(line2) > 0.0f;
		bool leftHanded = line1.Cross(line2) < 0.0f;
		
		// fallback for co-linear points
		if ( !allPointsFound ) {
			r8.Init(r1);
			r9.Init(r3);
			b8.Init(b1);
			b9.Init(b3);
			
			// find out if in parallel or anti-parallel case
			bool isParallel = line1.Dot(line2) > 0.0f;
			
			if (isParallel) {
				q0.Init(r8);
				q1.Init(r9);
				q2.Init(b8);
				q3.Init(b9);
			}
			else{
				q0.Init(r9);
				q1.Init(r8);
				q2.Init(b9);
				q3.Init(b8);
			}
		}
		// right-hand case
		else if ( rightHanded ) { // j2 && j4
			
			if (! blurIntersection) {
				
				b9.Init(b3);
				
				q3.Init(b6);
				
				if (! solidIntersection ){
					r9.Init(r3);
					
					q1.Init(r6);
				}
				else{
					q1.Init(r9);
				}
				
			}
			else{
				
				q1.Init(r9);
				q3.Init(b9);
			}
			
			r8.Init(r1);
			b8.Init(b1);
			
			q0.Init(r4);
			q2.Init(b4);
		}
		// left-handed case
		else { // j1 && j3
			if (! blurIntersection) {
				
				b8.Init(b1);
				
				q2.Init(b4);
				
				if (! solidIntersection) {
					r8.Init(r1);
					
					q0.Init(r4);
				}
				else{
					q0.Init(r8);
				}
				
			}
			else {
				
				
				q0.Init(r8);
				
				q2.Init(b8);
				
			}
			r9.Init(r3);
			b9.Init(b3);
			
			q1.Init(r6);
			q3.Init(b6);
			
		}
		
		if (i == 0) {
			firstBlurIntNorth.Init(b8);
			firstBlurIntSouth.Init(b9);
			firstSolidIntNorth.Init(r8);
			firstSolidIntSouth.Init(r9);
		}
		
		
		// render first segment if after the first calculation
		if (i > 0){
			gfxDevice.BeginPrim(GL_TRIANGLE_STRIP);
			
			if (renderBlur) {
				gfxDevice.SetPenColor(transColor);
				// write b0
				gfxDevice.WriteVtx(b0);
				gfxDevice.WriteFinalColor4b();
				// write b8
				gfxDevice.WriteVtx(b8);
				gfxDevice.WriteFinalColor4b();
			}
			
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
			
			if ( renderBlur ) {
				gfxDevice.SetPenColor(transColor);
				
				// write b2
				gfxDevice.WriteVtx(b2);
				gfxDevice.WriteFinalColor4b();
				
				// write b9
				gfxDevice.WriteVtx(b9);
				gfxDevice.WriteFinalColor4b();
			}
			
			
			gfxDevice.EndPrim();
			
		}
		
		// render corner
		gfxDevice.BeginPrim(GL_TRIANGLE_STRIP);
		if (leftHanded) { // j1
			if (renderBlur) {
				//gfxDevice.SetPenColor(transColor);
				// write b3
				gfxDevice.WriteVtx(b3);
				gfxDevice.WriteFinalColor4b();
				
				// write b6
				gfxDevice.WriteVtx(b6);
				gfxDevice.WriteFinalColor4b();
				
			}
			gfxDevice.SetPenColor(penColor);
			// write r3
			gfxDevice.WriteVtx(r3);
			gfxDevice.WriteFinalColor4b();
			
			// write r6
			gfxDevice.WriteVtx(r6);
			gfxDevice.WriteFinalColor4b();
			
			// write r8
			gfxDevice.WriteVtx(r8);
			gfxDevice.WriteFinalColor4b();
		}
		else if (rightHanded){
			if (renderBlur) {
				// write b1
				gfxDevice.WriteVtx(b1);
				gfxDevice.WriteFinalColor4b();
				
				// write b4
				gfxDevice.WriteVtx(b4);
				gfxDevice.WriteFinalColor4b();
			}
			gfxDevice.SetPenColor(penColor);
			// write r1
			gfxDevice.WriteVtx(r1);
			gfxDevice.WriteFinalColor4b();
			
			// write r4
			gfxDevice.WriteVtx(r4);
			gfxDevice.WriteFinalColor4b();
			
			// write r9
			gfxDevice.WriteVtx(r9);
			gfxDevice.WriteFinalColor4b();
			
		}

		gfxDevice.EndPrim();
		
		
		// render second segment if at the end
		if (i + 2 >= counter) {
			
			if ( rightHanded ) { //( j2 && j4 ) {
				r8.Init(r4);
				b8.Init(b4);
				
				if (!blurIntersection) {
					b9.Init(b6);
					if (!solidIntersection) {
						r9.Init(r6);
					}
				}
				
				
			}
			else /* if ( j1 && j3 ) */ {
				r9.Init(r6);
				b9.Init(b6);
				
				if (!blurIntersection) {
					b8.Init(b4);
					if (!solidIntersection) {
						r8.Init(r4);
					}
				}
				
			}
			
			gfxDevice.BeginPrim(GL_TRIANGLE_STRIP);
			
			if ( renderBlur ) {
				gfxDevice.SetPenColor(transColor);
				// write b8
				gfxDevice.WriteVtx(b8);
				gfxDevice.WriteFinalColor4b();
				
				// write b5
				gfxDevice.WriteVtx(b5);
				gfxDevice.WriteFinalColor4b();
				
			}
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
			
			if ( renderBlur ) {
				gfxDevice.SetPenColor(transColor);
				// write b9
				gfxDevice.WriteVtx(b9);
				gfxDevice.WriteFinalColor4b();
				
				// write b7
				gfxDevice.WriteVtx(b7);
				gfxDevice.WriteFinalColor4b();
			}
			
			gfxDevice.EndPrim();
		}
		
		gfxDevice.SetPenColor(penColor);
	}
	
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
	MOAIDraw::DrawRectFill ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMax, asTriStrip );
}

//----------------------------------------------------------------//
void MOAIDraw::DrawRectFill ( float left, float top, float right, float bottom, bool asTriStrip ) {
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	if ( asTriStrip ) {
		
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
	else {
		
		// Tri 1
		gfxDevice.BeginPrim ( GL_TRIANGLES );
		
		gfxDevice.WriteVtx ( left, top, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		gfxDevice.WriteVtx ( right, top, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		gfxDevice.WriteVtx ( right, bottom, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		gfxDevice.EndPrim ();
		
		// Tri 2
		gfxDevice.BeginPrim ( GL_TRIANGLES );
		
		gfxDevice.WriteVtx ( right, bottom, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		gfxDevice.WriteVtx ( left, bottom, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		gfxDevice.WriteVtx ( left, top, 0.0f );
		gfxDevice.WriteFinalColor4b ();
		
		gfxDevice.EndPrim ();
	}
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
void MOAIDraw::DrawRoundBeveledLine(lua_State *L, float lineWidth, float blurMargin, u32 steps){
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	MOAILuaState state ( L );
	
	// the x and y components of the three points making up the corner
	float p0x, p0y, p1x, p1y, p2x, p2y;
	
	// r0 to r3 are the points defining the rectangle of the first segment
	// r4 to r7 are the points defining the rectangle of the second segment
	// r8 and r9 are the corner intersection points
	// line1 and line2 are the normalized line vectors.  line1 defines p0p1 vector. line2 defines p1p2 vector.
	// line1Normal and line2Normal are the normalized vectors anti-clockwise from line1 and line2 respectively
	// q0 and q1 are storage variables for the next segment, to be used as the values of r0 and r2
	USVec2D r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, line1, line1Normal, line2, line2Normal, q0, q1;
	
	USVec2D b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, q2, q3;
	
	// i1 to i4 are the boolean variables used to determine if an intersection point exists.
	bool i1, i2, i3, i4;
	
	// j1 to j4 are the boolean variables used to determine if the line segments intersect.  At most, one of them should be true
	bool j1, j2, j3, j4;
	
	// render the blur boundaries when blurMargin parameter is greater than zero.
	bool renderBlur = blurMargin > 0.0f;
	
	// lw is half the line width.
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
	
	float bw = lw + blurMargin;
	
	// get pen color
	USColorVec penColor = gfxDevice.GetPenColor();
	// make transparent color
	USColorVec transColor(penColor);
	transColor.mA = 0.0f;
	if ( MOAIGfxDevice::Get ().GetColorPremultiply () ) {
		transColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
	}
	
	for (int i = 0; i < counter - 5; i += 2){
		p0x = vertexArray[i];
		p0y = vertexArray[i+1];
		p1x = vertexArray[i+2];
		p1y = vertexArray[i+3];
		p2x = vertexArray[i+4];
		p2y = vertexArray[i+5];
		
		// calculate line vectors
		line1.Init(p1x - p0x, p1y - p0y);
		if (line1.LengthSquared() == 0) {
			continue;
		}
		line1.Norm();
		line1Normal.Init(line1);
		line1Normal.Rotate90Anticlockwise();
		
		line2.Init(p2x - p1x, p2y - p1y);
		if (line2.LengthSquared() == 0) {
			continue;
		}
		line2.Norm();
		line2Normal.Init(line2);
		line2Normal.Rotate90Anticlockwise();
		
		
		// calculate render points
		if (i == 0) {
			r0.Init(p0x + lw * line1Normal.mX, p0y + lw * line1Normal.mY);
			r2.Init(p0x - lw * line1Normal.mX, p0y - lw * line1Normal.mY);
			
			b0.Init(p0x + bw * line1Normal.mX, p0y + bw * line1Normal.mY);
			b2.Init(p0x - bw * line1Normal.mX, p0y - bw * line1Normal.mY);
		}
		else{
			r0.Init(q0);
			r2.Init(q1);
			
			b0.Init(q2);
			b2.Init(q3);
		}
		
		r1.Init(p1x + lw * line1Normal.mX, p1y + lw * line1Normal.mY);
		b1.Init(p1x + bw * line1Normal.mX, p1y + bw * line1Normal.mY);
		r3.Init(p1x - lw * line1Normal.mX, p1y - lw * line1Normal.mY);
		b3.Init(p1x - bw * line1Normal.mX, p1y - bw * line1Normal.mY);
		
		r4.Init(p1x + lw * line2Normal.mX, p1y + lw * line2Normal.mY);
		b4.Init(p1x + bw * line2Normal.mX, p1y + bw * line2Normal.mY);
		
		r5.Init(p2x + lw * line2Normal.mX, p2y + lw * line2Normal.mY);
		b5.Init(p2x + bw * line2Normal.mX, p2y + bw * line2Normal.mY);
		
		r6.Init(p1x - lw * line2Normal.mX, p1y - lw * line2Normal.mY);
		b6.Init(p1x - bw * line2Normal.mX, p1y - bw * line2Normal.mY);
		
		r7.Init(p2x - lw * line2Normal.mX, p2y - lw * line2Normal.mY);
		b7.Init(p2x - bw * line2Normal.mX, p2y - bw * line2Normal.mY);
		
		
		// find intersection points
		j1 = USVec2D::GetLineIntersection(r0, r1, r4, r5, &r8, &i1);
		j2 = USVec2D::GetLineIntersection(r2, r3, r6, r7, &r9, &i2);
		j3 = USVec2D::GetLineIntersection(b0, b1, b4, b5, &b8, &i3);
		j4 = USVec2D::GetLineIntersection(b2, b3, b6, b7, &b9, &i4);
		
		bool allPointsFound = (i1 && i2 && i3 && i4);
		bool blurIntersection = (j3 || j4);
		bool solidIntersection = (j1 || j2);
		
		bool rightHanded = line1.Cross(line2) > 0.0f;
		bool leftHanded = line1.Cross(line2) < 0.0f;
		
		// fallback for co-linear points
		if ( !allPointsFound ) {
			r8.Init(r1);
			r9.Init(r3);
			b8.Init(b1);
			b9.Init(b3);
			
			// find out if in parallel or anti-parallel case
			bool isParallel = line1.Dot(line2) > 0.0f;
			
			if (isParallel) {
				q0.Init(r8);
				q1.Init(r9);
				q2.Init(b8);
				q3.Init(b9);
			}
			else{
				q0.Init(r9);
				q1.Init(r8);
				q2.Init(b9);
				q3.Init(b8);
			}
		}
		// right-hand case
		else if ( rightHanded ) { // j2 && j4
			
			if (! blurIntersection) {
				
				b9.Init(b3);
				
				q3.Init(b6);
				
				if (! solidIntersection ){
					r9.Init(r3);
					
					q1.Init(r6);
				}
				else{
					q1.Init(r9);
				}
				
			}
			else{
				
				q1.Init(r9);
				q3.Init(b9);
			}
			
			r8.Init(r1);
			b8.Init(b1);
			
			q0.Init(r4);
			q2.Init(b4);
		}
		// left-handed case
		else { // j1 && j3
			if (! blurIntersection) {
				
				b8.Init(b1);
				
				q2.Init(b4);
				
				if (! solidIntersection) {
					r8.Init(r1);
					
					q0.Init(r4);
				}
				else{
					q0.Init(r8);
				}
				
			}
			else {
				
				
				q0.Init(r8);
				
				q2.Init(b8);
				
			}
			r9.Init(r3);
			b9.Init(b3);
			
			q1.Init(r6);
			q3.Init(b6);
			
		}
		
		
		// render first endpoint if at beginning
		if (i == 0) {
			float angleInRadians = atan2f(line1Normal.mX, line1Normal.mY);
			float offsetAngle = angleInRadians * (float)R2D;
			float sliceAngle = 180.0f;
			//u32 steps = 16;
			MOAIDraw::DrawEllipticalSliceFill(p0x, p0y, lw, lw, sliceAngle, offsetAngle, blurMargin, steps);
		}
		
		
		
		// render the first segment
		
		gfxDevice.BeginPrim(GL_TRIANGLE_STRIP);
		
		if (renderBlur) {
			gfxDevice.SetPenColor(transColor);
			// write b0
			gfxDevice.WriteVtx(b0);
			gfxDevice.WriteFinalColor4b();
			// write b8
			gfxDevice.WriteVtx(b8);
			gfxDevice.WriteFinalColor4b();
		}
		
		
		gfxDevice.SetPenColor(penColor);
		// write r0
		gfxDevice.WriteVtx(r0);
		gfxDevice.WriteFinalColor4b();
		
		// write r8
		gfxDevice.WriteVtx(r8);
		gfxDevice.WriteFinalColor4b();
		
		// write p0
		gfxDevice.WriteVtx(p0x, p0y);
		gfxDevice.WriteFinalColor4b();
		
		// write p1
		gfxDevice.WriteVtx(p1x, p1y);
		gfxDevice.WriteFinalColor4b();
		
		// write r2
		gfxDevice.WriteVtx(r2);
		gfxDevice.WriteFinalColor4b();
		
		// write r9
		gfxDevice.WriteVtx(r9);
		gfxDevice.WriteFinalColor4b();
		
		if ( renderBlur ) {
			gfxDevice.SetPenColor(transColor);
			
			// write b2
			gfxDevice.WriteVtx(b2);
			gfxDevice.WriteFinalColor4b();
			
			// write b9
			gfxDevice.WriteVtx(b9);
			gfxDevice.WriteFinalColor4b();
			
			gfxDevice.SetPenColor(penColor);
		}
		
		gfxDevice.EndPrim();
		
		// render corner
		if (leftHanded) {
			float cross = line1.Cross(line2);
			float dot = line1.Dot(line2);
			// r3 r6 p1
			float angle = (atan2f(dot, cross) - M_PI_2)  * (float)R2D;
			// angle should be positive
			if (angle < 0.0f) {
				angle += 360.0f;
			}
			
			float offset = atan2f(-line1Normal.mX, -line1Normal.mY) * (float)R2D;
			
			u32 angleSteps = (u32) ceilf (steps * (angle / 180.0f) );
			
			
			MOAIDraw::DrawEllipticalSliceFill(p1x, p1y, lw, lw, angle, offset, blurMargin, angleSteps);
			
		}
		else if (rightHanded){
			float cross = line1.Cross(line2);
			float dot = line1.Dot(line2);
			
			// r1 r4 p1
			float angle = (atan2f(dot, cross) - M_PI_2)  * (float)R2D;
			// angle should be negative
			
			float offset = atan2f(line1Normal.mX, line1Normal.mY) * (float)R2D;
			
			u32 angleSteps = (u32) ceilf (steps * (-angle / 180.0f) );
			
			MOAIDraw::DrawEllipticalSliceFill(p1x, p1y, lw, lw, angle, offset, blurMargin, angleSteps);
			
		}
		
		
		// render the second segment if at the end.
		if (i + 6 >= counter ) {
			
			if ( rightHanded ) { //( j2 && j4 ) {
				r8.Init(r4);
				b8.Init(b4);
				
				if (!blurIntersection) {
					b9.Init(b6);
					if (!solidIntersection) {
						r9.Init(r6);
					}
				}
				
				
			}
			else /* if ( j1 && j3 ) */ {
				r9.Init(r6);
				b9.Init(b6);
				
				if (!blurIntersection) {
					b8.Init(b4);
					if (!solidIntersection) {
						r8.Init(r4);
					}
				}
				
			}
			
			gfxDevice.BeginPrim(GL_TRIANGLE_STRIP);
			
			if ( renderBlur ) {
				gfxDevice.SetPenColor(transColor);
				// write b8
				gfxDevice.WriteVtx(b8);
				gfxDevice.WriteFinalColor4b();
				
				// write b5
				gfxDevice.WriteVtx(b5);
				gfxDevice.WriteFinalColor4b();
				
			}
			gfxDevice.SetPenColor(penColor);
			// write r8
			gfxDevice.WriteVtx(r8);
			gfxDevice.WriteFinalColor4b();
			
			// write r5
			gfxDevice.WriteVtx(r5);
			gfxDevice.WriteFinalColor4b();
			
			// write p1
			gfxDevice.WriteVtx(p1x, p1y);
			gfxDevice.WriteFinalColor4b();
			
			// write p2
			gfxDevice.WriteVtx(p2x, p2y);
			gfxDevice.WriteFinalColor4b();
			
			// write r9
			gfxDevice.WriteVtx(r9);
			gfxDevice.WriteFinalColor4b();
			
			// write r7
			gfxDevice.WriteVtx(r7);
			gfxDevice.WriteFinalColor4b();
			
			if ( renderBlur ) {
				gfxDevice.SetPenColor(transColor);
				// write b9
				gfxDevice.WriteVtx(b9);
				gfxDevice.WriteFinalColor4b();
				
				// write b7
				gfxDevice.WriteVtx(b7);
				gfxDevice.WriteFinalColor4b();
				
				gfxDevice.SetPenColor(penColor);
			}
			
			gfxDevice.EndPrim();
			
			
			// render second endpoint
			float angleInRadians = atan2f(- line2Normal.mX, - line2Normal.mY);
			float offsetAngle = angleInRadians * (float)R2D;
			float sliceAngle = 180.0f;
			//u32 steps = 16;
			MOAIDraw::DrawEllipticalSliceFill(p2x, p2y, lw, lw, sliceAngle, offsetAngle, blurMargin, steps);
			
			
		}
		gfxDevice.SetPenColor(penColor);
	}
}

//----------------------------------------------------------------//
void MOAIDraw::DrawTriangularGradientFill(const USVec2D& v0, const USVec2D& v1, const USVec2D& v2, const USColorVec &color0, const USColorVec &color1, const USColorVec &color2){
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	USColorVec penColor = gfxDevice.GetPenColor();
	
	
	gfxDevice.BeginPrim(GL_TRIANGLE_STRIP);
	
	gfxDevice.SetPenColor(color0);
	gfxDevice.WriteVtx(v0);
	gfxDevice.WriteFinalColor4b();
	
	gfxDevice.SetPenColor(color1);
	gfxDevice.WriteVtx(v1);
	gfxDevice.WriteFinalColor4b();
	
	gfxDevice.SetPenColor(color2);
	gfxDevice.WriteVtx(v2);
	gfxDevice.WriteFinalColor4b();
	
	gfxDevice.EndPrim();
	
	gfxDevice.SetPenColor(penColor);
	
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
		{ "drawAntialiasedLineSegment", _drawAntialiasedLineSegment },
		//{ "drawAxisGrid",			_drawAxisGrid }, // TODO
		{ "drawBeveledCorner",		_drawBeveledCorner },
		{ "drawBeveledLine",		_drawBeveledLine },
		{ "drawBeveledLineLoop",	_drawBeveledLineLoop },
		{ "drawBezierCurve",		_drawBezierCurve },
		{ "drawBoxOutline",			_drawBoxOutline },
		{ "drawCardinalSpline",		_drawCardinalSpline },
		{ "drawCardinalSplineLoop",	_drawCardinalSplineLoop },
		{ "drawCardinalSplineStrip", _drawCardinalSplineStrip },
		{ "drawCatmullRomCurve",	_drawCatmullRomCurve },
		{ "drawCircle",				_drawCircle },
		{ "drawEllipse",			_drawEllipse },
		//{ "drawGrid",				_drawGrid }, // TODO
		{ "drawJoinedCorner",		_drawJoinedCorner },
		{ "drawJoinedLine",			_drawJoinedLine },
		{ "drawJoinedLineLoop",		_drawJoinedLineLoop },
		{ "drawLine",				_drawLine },
		{ "drawPoints",				_drawPoints },
		{ "drawRay",				_drawRay },
		{ "drawRect",				_drawRect },
		{ "drawRoundBeveledLine",	_drawRoundBeveledLine },
		{ "fillCenteredRectangularGradient", _fillCenteredRectangularGradient },
		{ "fillCircle",				_fillCircle },
		{ "fillCircularGradient",	_fillCircularGradient },
		{ "fillCircularSlice",		_fillCircularSlice },
		{ "fillEllipse",			_fillEllipse },
		{ "fillEllipticalGradient",	_fillEllipticalGradient },
		{ "fillEllipticalSlice",	_fillEllipticalSlice },
		{ "fillHorizontalRectangularGradient", _fillHorizontalRectangularGradient },
		{ "fillFan",				_fillFan },
		{ "fillRect",				_fillRect },
		{ "fillTriangularGradient", _fillTriangularGradient },
		{ "fillVerticalRectangularGradient", _fillVerticalRectangularGradient },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}
