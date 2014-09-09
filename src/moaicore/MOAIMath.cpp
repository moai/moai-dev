// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIMath.h>

extern "C" {
	#include <SFMT.h>
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/** @name	pointsForBezierCurve
	@text	Returns a table containing the x and y coordinates of all
			the points on the cubic Bezier curve given two endpoints,
			two control points and the number of subdivisions for the segment.
 
	@in		number	x0
	@in		number	y0
	@in		number	x1
	@in		number	y1
	@in		number	cx0
	@in		number	cy0
	@in		number	cx1
	@in		number	cy0
	@opt	number	subdivision			default 16.
	@out	table	curvePoints
*/

int MOAIMath::_pointsForBezierCurve ( lua_State *L ) {
	MOAILuaState state ( L );
	
	float x0 = state.GetValue < float > (1, 0.0f);
	float y0 = state.GetValue < float > (2, 0.0f);
	float x1 = state.GetValue < float > (3, 0.0f);
	float y1 = state.GetValue < float > (4, 0.0f);
	float cx0 = state.GetValue < float > (5, x0);
	float cy0 = state.GetValue < float > (6, y0);
	float cx1 = state.GetValue < float > (7, x1);
	float cy1 = state.GetValue < float > (8, y1);
	u32	steps = state.GetValue < u32 > (9, 16);
	
	float t = 0.0f;
	float tStep = 1.0f / (float) steps;
	
	if (steps == 0){
		steps = 1;
	}
	
	u32 arraySize = 2 * steps + 2; // == 2 * steps + 2
	
	// create a table
	lua_createtable(L, arraySize, 0);
	u32 idx = 1;
	int i;
	for (i = 0; i < (int)steps; ++i, t += tStep, idx += 2 ){
		
		// push x
		state.Push( USCurve::Bezier1D( x0, cx0, cx1, x1, t ) );
		lua_rawseti(L, -2, idx);
		
		// push y
		state.Push( USCurve::Bezier1D( y0, cy0, cy1, y1, t ) );
		lua_rawseti(L, -2, idx + 1);
	}
	
	// push final point
	state.Push( x1 );
	lua_rawseti(L, -2, idx);
	
	state.Push( y1 );
	lua_rawseti(L, -2, idx + 1);
	
	state.Push(arraySize);
	
	return 2;
}

//----------------------------------------------------------------//
/** @name	pointsForCardinalSpline
	@text	Returns a table containing the x and y coordinates of all the points
			in a cardinal spline curve given at least three key points and the 
			subdivisions per segment.  Also returns the number of points in the 
			returned array, half the number of entries.
 
	@in		table	vertices
	@opt	number	subdivision		default 16.
	@opt	number	tension			default 0.0
	@out	table	splinePoints
	@out	number	arraySize
 */
int MOAIMath::_pointsForCardinalSpline(lua_State *L){
	MOAILuaState state ( L );
	
	u32 subdivision = state.GetValue < u32 > (2, 16);
	float tension = state.GetValue < float > (3, 0.0f);
	
	return MOAIMath::PointsForCardinalSpline(L, subdivision, tension);
}

//----------------------------------------------------------------//
/** @name	pointsForCardinalSplineLoop
	@text	Returns a table containing the x and y coordinats of all the points
			in a looping cardinal spline curve given at least three key points
			and the subdivisions per segments.
 
	@in		table	vertices
	@opt	number	subdivision		default 16.
	@opt	number	tension			default 0.0
	@out	table	splinePoints
	@out	number	arraySize
 */
int MOAIMath::_pointsForCardinalSplineLoop(lua_State *L){
	MOAILuaState state ( L );
	
	u32 subdivision = state.GetValue < u32 > (2, 16);
	float tension = state.GetValue < float > (3, 0.0f);
	
	return MOAIMath::PointsForCardinalSplineLoop(L, subdivision, tension);
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMath::_randSFMT ( lua_State* L ) {
	MOAILuaState state ( L );
	
	double lower = 0.0;
	double upper = 1.0;
	
	if ( state.IsType ( 1, LUA_TNUMBER )) {
	
		upper = state.GetValue < double >( 1, 0.0 );
	
		if ( state.IsType ( 2, LUA_TNUMBER )) {
			lower = upper;
			upper = state.GetValue < double >( 2, 0.0 );
		}
	}
	
	double r = sfmt_genrand_real1 ( MOAIMath::Get ().mSFMT ); // [0, 1]
	state.Push ( lower + ( r * ( upper - lower )));
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMath::_seedSFMT ( lua_State* L ) {
	MOAILuaState state ( L );
	
	u32 seed = state.GetValue < u32 >( 1, 0 );
	sfmt_init_gen_rand ( MOAIMath::Get ().mSFMT, seed );
	
	return 0;
}

//================================================================//
// MOAIMath
//================================================================//

//----------------------------------------------------------------//
MOAIMath::MOAIMath () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
	
	this->mSFMT = ( SFMT_T* )calloc ( 1, sizeof ( SFMT_T ));
}

//----------------------------------------------------------------//
MOAIMath::~MOAIMath () {

	free ( this->mSFMT );
}

//----------------------------------------------------------------//
int MOAIMath::PointsForCardinalSpline(lua_State *L, u32 subdivide, float tension){
	MOAILuaState state ( L );
	
	USLeanArray<float> vertexArray;
	const u32 chunk_size = 8;
	
	vertexArray.Init(chunk_size);
	float p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y;
	int counter = 0;
	
	
	float tStep = 1.0f / (float) subdivide;
	
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
		
		u32 arraySize = (u32) counter;
		
		if (subdivide == 0){
			subdivide = 1;
		}
		
		if (counter >= 4) {
			arraySize = (arraySize - 2) * subdivide + 2;
		}
		
		// create a table
		lua_createtable(L, arraySize, 0);
		
		int i;
		for (i = 0; i < counter - 3; i += 2) {
			
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
			
			float t = 0.0f;
			// subdivide == 2
			// in i == 0, j == 0; idx == 1
			// i == 0, j == 1; idx == 3
			// i == 2, j == 0; idx == 5
			// i == 2, j == 1; idx == 7
			// i == 4, j == 0; idx == 9
			// i == 4, j == 1; idx == 11
			
			for (u32 j = 0; j < subdivide; ++j, t += tStep){
				u32 idx = 2 * ((i>>1) * subdivide + j) + 1;
				// push x
				state.Push( USCurve::CardinalSpline1D(p0x, p1x, p2x, p3x, tension, t) );
				lua_rawseti(L, -2, idx );
				
				// push y
				state.Push( USCurve::CardinalSpline1D(p0y, p1y, p2y, p3y, tension, t) );
				lua_rawseti(L, -2, idx + 1);
			}
			
			
		}
		// push last point
		p0x = vertexArray[i];
		p0y = vertexArray[i + 1];
		
		state.Push(p0x);
		lua_rawseti(L, -2, 2 * ((i>>1) * subdivide) + 1 );
		
		state.Push(p0y);
		lua_rawseti(L, -2, 2 * ((i>>1) * subdivide) + 2 );
		
		
		state.Push(arraySize);
		
		return 2;
	}
	
	
	
	return 0;
}
//----------------------------------------------------------------//
int MOAIMath::PointsForCardinalSplineLoop(lua_State *L, u32 subdivide, float tension){
	
	MOAILuaState state ( L );
	
	USLeanArray<float> vertexArray;
	const u32 chunk_size = 8;
	
	vertexArray.Init(chunk_size);
	float p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y;
	int counter = 0;
	
	
	float tStep = 1.0f / (float) subdivide;
	
	
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
		
		u32 arraySize = (u32) counter;
		
		if (subdivide == 0){
			subdivide = 1;
		}
		
		if (counter >= 4) {
			arraySize = arraySize * subdivide;
		}
		
		// create a table
		lua_createtable(L, arraySize, 0);
		
		int i;
		for (i = 0; i < counter - 1; i += 2) {
			
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
			
			float t = 0.0f;
			// subdivide == 2
			// in i == 0, j == 0; idx == 1
			// i == 0, j == 1; idx == 3
			// i == 2, j == 0; idx == 5
			// i == 2, j == 1; idx == 7
			// i == 4, j == 0; idx == 9
			// i == 4, j == 1; idx == 11
			
			for (u32 j = 0; j < subdivide; ++j, t += tStep){
				u32 idx = 2 * ((i>>1) * subdivide + j) + 1;
				// push x
				state.Push( USCurve::CardinalSpline1D(p0x, p1x, p2x, p3x, tension, t) );
				lua_rawseti(L, -2, idx );
				
				// push y
				state.Push( USCurve::CardinalSpline1D(p0y, p1y, p2y, p3y, tension, t) );
				lua_rawseti(L, -2, idx + 1);
			}
			
			
		}
		
		
		state.Push(arraySize);
		
		return 2;
	}
	
	return 0;
}


//----------------------------------------------------------------//
void MOAIMath::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "pointsForBezierCurve",			_pointsForBezierCurve },
		{ "pointsForCardinalSpline",		_pointsForCardinalSpline },
		{ "pointsForCardinalSplineLoop",	_pointsForCardinalSplineLoop },
		{ "randSFMT",						_randSFMT },
		{ "seedSFMT",						_seedSFMT },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
