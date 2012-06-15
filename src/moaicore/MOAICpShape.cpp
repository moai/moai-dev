// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAICpBody.h>
#include <moaicore/MOAICpShape.h>
#include <moaicore/MOAILogMessages.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_CHIPMUNK

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	areaForCircle
	@text	Returns the area for a ring or circle.
	
	@overload
	
		@in		number radius
		@out	number area

	@overload
	
		@in		number innerRadius
		@in		number outerRadius
		@out	number area
*/ 
int MOAICpShape::_areaForCircle ( lua_State* L ) {
	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "N" )) return 0;

	cpFloat r1;
	cpFloat r2;

	if ( state.GetTop () >= 2 ) {
		r1 = state.GetValue < cpFloat >( 1, 0 );
		r2 = state.GetValue < cpFloat >( 2, 0 );
		
	}
	else {
		r1 = 0;
		r2 = state.GetValue < cpFloat >( 1, 0 );
	}

	lua_pushnumber ( L, cpAreaForCircle ( r1, r2 ));
	return 1;
}

//----------------------------------------------------------------//
/**	@name	areaForCircle
	@text	Returns the area for a polygon.
	
	@in		table vertices Array containg vertex coordinate components ( t[1] = x0, t[2] = y0, t[3] = x1, t[4] = y1... )
	@out	number area
*/ 
int MOAICpShape::_areaForPolygon ( lua_State* L ) {
	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "T" )) return 0;

	cpVect verts [ MAX_POLY_VERTS ];
	int numVerts = MOAICpShape::LoadVerts ( state, 1, verts, MAX_POLY_VERTS );
			
	if ( numVerts && cpPolyValidate ( verts, numVerts )) {
		cpFloat area = cpAreaForPoly ( numVerts, verts );
		area = area < 0 ? -area : area;
		lua_pushnumber ( L, area );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	areaForRect
	@text	Returns the area for the specified rectangle.

	@in		number x1
	@in		number y1
	@in		number x2
	@in		number y2
	@out	number area			The calculated area.
*/
int MOAICpShape::_areaForRect ( lua_State* L ) {
	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "NNNN" )) return 0;

	USMetaRect < cpFloat > rect = state.GetRect < cpFloat >( 1 );
	rect.Bless ();
	
	lua_pushnumber ( L, rect.Area ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	areaForSegment
	@text	Returns the area for the specified segment.

	@in		number x1
	@in		number y1
	@in		number x2
	@in		number y2
	@in		number r
	@out	number area			The calculated area.
*/
int MOAICpShape::_areaForSegment ( lua_State* L ) {
	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "UUNNNN" )) return 0;
	
	cpVect a;
	a.x = state.GetValue < cpFloat >( 1, 0 );
	a.y = state.GetValue < cpFloat >( 2, 0 );
	
	cpVect b;
	b.x = state.GetValue < cpFloat >( 3, 0 );
	b.y = state.GetValue < cpFloat >( 4, 0 );

	cpFloat r = state.GetValue < cpFloat >( 5, 0 );

	lua_pushnumber ( L, cpAreaForSegment ( a, b, r ));
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getBody
	@text	Returns the current body for the shape.

	@in		MOAICpShape self
	@out	MOAICpBody body			The body.
*/
int MOAICpShape::_getBody ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpShape, "U" )
	
	if ( self->mShape && self->mShape->body ) {
		(( MOAICpBody* )self->mShape->body->data )->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getElasticity
	@text	Returns the current elasticity.

	@in		MOAICpShape self
	@out	number elasticity			The elasticity.
*/
int MOAICpShape::_getElasticity ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpShape, "U" )
	
	if ( self->mShape ) {
		lua_pushnumber ( L, self->mShape->e );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getFriction
	@text	Returns the current friction.

	@in		MOAICpShape self
	@out	number friction			The friction.
*/
int MOAICpShape::_getFriction ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpShape, "U" )
	
	if ( self->mShape ) {
		lua_pushnumber ( L, self->mShape->u );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getGroup
	@text	Returns the current group ID.

	@in		MOAICpShape self
	@out	number group			The group ID.
*/
int MOAICpShape::_getGroup ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpShape, "U" )
	
	if ( self->mShape ) {
		lua_pushnumber ( L, self->mShape->group );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getLayers
	@text	Returns the current layer ID.

	@in		MOAICpShape self
	@out	number layer			The layer ID.
*/
int MOAICpShape::_getLayers ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpShape, "U" )
	
	if ( self->mShape ) {
		lua_pushnumber ( L, self->mShape->layers );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getSurfaceVel
	@text	Returns the current surface velocity?

	@in		MOAICpShape self
	@out	number x				The X component of the surface velocity.
	@out	number y				The Y component of the surface velocity.
*/
int MOAICpShape::_getSurfaceVel ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpShape, "UNN" )
	
	if ( self->mShape ) {
		lua_pushnumber ( L, self->mShape->surface_v.x );
		lua_pushnumber ( L, self->mShape->surface_v.y );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getType
	@text	Returns the current collision type.

	@in		MOAICpShape self
	@out	number type				The collision type.
*/
int MOAICpShape::_getType ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpShape, "U" )
	
	if ( self->mShape ) {
		lua_pushnumber ( L, self->mShape->collision_type );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	inside
	@text	Returns whether the specified point is inside the shape.

	@in		MOAICpShape self
	@in		number x
	@in		number y
	@out	boolean inside			Whether the point is inside the shape.
*/
int MOAICpShape::_inside ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpShape, "UNN" )
	
	if ( self->mShape ) {
	
		cpVect p;
		p.x = state.GetValue < cpFloat >( 2, 0 );
		p.y = state.GetValue < cpFloat >( 3, 0 );
		
		bool inside = cpShapePointQuery ( self->mShape, p ) == cpTrue ? true : false;
		lua_pushboolean ( state, inside );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	isSensor
	@text	Returns whether the current shape is a sensor.

	@in		MOAICpShape self
	@out	boolean sensor			Whether the shape is a sensor.
*/
int MOAICpShape::_isSensor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpShape, "U" )
	
	if ( self->mShape ) {
		lua_pushboolean ( state, ( self->mShape->sensor == cpTrue ));
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	momentForCircle
	@text	Return the moment of inertia for the circle.

	@in		number m
	@opt	number r1
	@in		number r2
	@in		number ox
	@in		number oy
	@out	number moment
*/
int MOAICpShape::_momentForCircle ( lua_State* L ) {
	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "NN" )) return 0;
	int p = 1;

	cpFloat m = state.GetValue < cpFloat >( p++, 0 );

	cpFloat r1 = 0;
	cpFloat r2 = 0;
	
	if ( state.GetTop () >= 5 ) {
		r1 = state.GetValue < cpFloat >( p++, 0 );
		r2 = state.GetValue < cpFloat >( p++, 0 );
	}
	else {
		r2 = state.GetValue < cpFloat >( p++, 0 );
	}
	
	cpVect offset;
	offset.x = state.GetValue < cpFloat >( p++, 0 );
	offset.y = state.GetValue < cpFloat >( p, 0 );

	lua_pushnumber ( L, cpMomentForCircle ( m, r1, r2, offset ));
	return 1;
}

//----------------------------------------------------------------//
/**	@name	momentForPolygon
	@text	Returns the moment of intertia for the polygon.

	@in		number m
	@in		table polygon
	@out	number moment
*/
int MOAICpShape::_momentForPolygon ( lua_State* L ) {
	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "NT" )) return 0;

	cpFloat m = state.GetValue < cpFloat >( 1, 0 );

	cpVect verts [ MAX_POLY_VERTS ];
	int numVerts = MOAICpShape::LoadVerts ( state, 2, verts, MAX_POLY_VERTS );
			
	if ( numVerts && cpPolyValidate ( verts, numVerts )) {
	
		cpVect offset;
		offset.x = 0;
		offset.y = 0;
	
		lua_pushnumber ( L, cpMomentForPoly ( m, numVerts, verts, offset ));
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	momentForRect
	@text	Returns the moment of intertia for the rect.

	@in		number m
	@in		number x1
	@in		number y1
	@in		number x2
	@in		number y2
	@out	number moment
*/
int MOAICpShape::_momentForRect ( lua_State* L ) {
	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "NNNNN" )) return 0;

	cpFloat m = state.GetValue < cpFloat >( 1, 0 );

	USMetaRect < cpFloat > rect = state.GetRect < cpFloat >( 2 );
	rect.Bless ();
	
	lua_pushnumber ( L, cpMomentForBox( m, rect.Width (), rect.Height ()));
	return 1;
}

//----------------------------------------------------------------//
/**	@name	momentForSegment
	@text	Returns the moment of intertia for the segment.

	@in		number m
	@in		number x1
	@in		number y1
	@in		number x2
	@in		number y2
	@out	number moment
*/
int MOAICpShape::_momentForSegment ( lua_State* L ) {
	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "UUNNNNN" )) return 0;
	
	cpFloat m = state.GetValue < cpFloat >( 1, 0 );
	
	cpVect a;
	a.x = state.GetValue < cpFloat >( 2, 0 );
	a.y = state.GetValue < cpFloat >( 3, 0 );
	
	cpVect b;
	b.x = state.GetValue < cpFloat >( 4, 0 );
	b.y = state.GetValue < cpFloat >( 5, 0 );

	lua_pushnumber ( L, cpMomentForSegment ( m, a, b ));
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setElasticity
	@text	Sets the current elasticity.

	@in		MOAICpShape self
	@in		number elasticity			The elasticity.
	@out	nil
*/
int MOAICpShape::_setElasticity ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpShape, "UN" )
	
	if ( self->mShape ) {
		self->mShape->e = state.GetValue < cpFloat >( 2, self->mShape->e );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setFriction
	@text	Sets the current friction.

	@in		MOAICpShape self
	@in		number friction			The friction.
	@out	nil
*/
int MOAICpShape::_setFriction ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpShape, "UN" )
	
	if ( self->mShape ) {
		self->mShape->u = state.GetValue < cpFloat >( 2, self->mShape->u );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setGroup
	@text	Sets the current group ID.

	@in		MOAICpShape self
	@in		number group			The group ID.
	@out	nil
*/
int MOAICpShape::_setGroup ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpShape, "UN" )
	
	if ( self->mShape ) {
		self->mShape->group = state.GetValue < cpGroup >( 2, self->mShape->group );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setIsSensor
	@text	Sets whether this shape is a sensor.

	@in		MOAICpShape self
	@in		boolean sensor			Whether this shape is a sensor.
	@out	nil
*/
int MOAICpShape::_setIsSensor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpShape, "UB" )
	
	if ( self->mShape ) {
		self->mShape->sensor = state.GetValue < bool >( 2, false ) ? cpTrue : cpFalse;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLayers
	@text	Sets the current layer ID.

	@in		MOAICpShape self
	@in		number layer			The layer ID.
	@out	nil
*/
int MOAICpShape::_setLayers ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpShape, "UN" )
	
	if ( self->mShape ) {
		self->mShape->layers = state.GetValue < cpLayers >( 2, self->mShape->layers );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setSurfaceVel
	@text	Sets the current surface velocity.

	@in		MOAICpShape self
	@in		number x				The X component of the surface velocity.
	@in		number y				The Y component of the surface velocity.
	@out	nil
*/
int MOAICpShape::_setSurfaceVel ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpShape, "UNN" )
	
	if ( !self->mShape ) {
		cpVect surface_v;
		surface_v.x = state.GetValue < cpFloat >( 2, 0 );
		surface_v.y = state.GetValue < cpFloat >( 3, 0 );
		self->mShape->surface_v = surface_v;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setType
	@text	Sets the current collision type.

	@in		MOAICpShape self
	@in		number type				The collision type.
	@out	nil
*/
int MOAICpShape::_setType ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpShape, "UN" )
	
	if ( self->mShape ) {
		self->mShape->collision_type = state.GetValue < cpCollisionType >( 2, self->mShape->collision_type );
	}
	return 0;
}

//================================================================//
// MOAICpShape
//================================================================//

//----------------------------------------------------------------//
void MOAICpShape::CpAddToSpace ( cpSpace* space ) {

	assert ( space );
	assert ( this->mShape );
	cpSpaceAddShape ( space, this->mShape );
}

//----------------------------------------------------------------//
void MOAICpShape::CpRemoveFromSpace ( cpSpace* space ) {

	assert ( space );
	assert ( this->mShape );
	cpSpaceRemoveShape ( space, this->mShape );
}

//----------------------------------------------------------------//
u32 MOAICpShape::LoadVerts ( MOAILuaState& state, int idx, cpVect* verts, u32 max  ) {
	
	int itr = state.PushTableItr ( idx );
	idx = 0;
	
	u32 total = 0;
	for ( ; state.TableItrNext ( itr ) && ( total < max ); ++idx ) {
		
		cpFloat val = state.GetValue < cpFloat >( -1, 0 );
	
		if ( idx & 0x01 ) {
			verts [ total ].y = val;
			total++;
		}
		else {
			verts [ total ].x = val;
		}
	}
	return total;
}

//----------------------------------------------------------------//
MOAICpShape::MOAICpShape () :
	mShape ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAICpPrim )
	RTTI_END
	
	this->mLinkInBody.Data ( this );
}

//----------------------------------------------------------------//
MOAICpShape::~MOAICpShape () {

	if ( this->mShape ) {
		cpShapeFree ( this->mShape );
	}
}

//----------------------------------------------------------------//
void MOAICpShape::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "areaForCircle",		_areaForCircle },
		{ "areaForPolygon",		_areaForPolygon },
		{ "areaForRect",		_areaForRect },
		{ "areaForSegment",		_areaForSegment },
		{ "momentForCircle",	_momentForCircle },
		{ "momentForPolygon",	_momentForPolygon },
		{ "momentForRect",		_momentForRect },
		{ "momentForSegment",	_momentForSegment },
		{ "new",				MOAILogMessages::_alertNewIsUnsupported },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAICpShape::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "getBody",			_getBody },
		{ "getElasticity",		_getElasticity },
		{ "getFriction",		_getFriction },
		{ "getGroup",			_getGroup },
		{ "getLayers",			_getLayers },
		{ "getSurfaceVel",		_getSurfaceVel },
		{ "getType",			_getType },
		{ "inside",				_inside },
		{ "isSensor",			_isSensor },
		{ "setElasticity",		_setElasticity },
		{ "setFriction",		_setFriction },
		{ "setGroup",			_setGroup },
		{ "setIsSensor",		_setIsSensor },
		{ "setLayers",			_setLayers },
		{ "setSurfaceVel",		_setSurfaceVel },
		{ "setType",			_setType },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

#endif