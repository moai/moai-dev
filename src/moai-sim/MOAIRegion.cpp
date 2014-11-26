// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIRegion.h>
#include <moai-sim/MOAIVectorUtil.h>
#include <moai-sim/MOAIVertexBuffer.h>
#include <tesselator.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRegion::_getTriangles ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRegion, "U" )

	MOAIVertexBuffer* vtxBuffer		= state.GetLuaObject < MOAIVertexBuffer >( 2, true );
	MOAIIndexBuffer* idxBuffer		= state.GetLuaObject < MOAIIndexBuffer >( 3, true );

	if ( vtxBuffer && idxBuffer ) {
		self->GetTriangles ( *vtxBuffer, *idxBuffer );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRegion::_pointInside ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRegion, "UNN" )
	
	float x = state.GetValue < float >( 2, 0.0f );
	float y = state.GetValue < float >( 3, 0.0f );

	state.Push ( self->PointInside ( ZLVec2D ( x, y )));
	
	return 1;
}

//================================================================//
// MOAIRegion
//================================================================//

//----------------------------------------------------------------//
void MOAIRegion::GetTriangles ( MOAIVertexBuffer& vtxBuffer, MOAIIndexBuffer& idxBuffer ) {

	SafeTesselator tesselator;

	u32 nPolys = this->Size ();
	for ( u32 i = 0; i < nPolys; ++i ) {
		const ZLPolygon2D& poly = ( *this )[ i ];
		tessAddContour ( tesselator.mTess, 2, poly.Data (), sizeof ( ZLVec2D ), poly.Size ());
	}
	
	int error = tesselator.Tesselate ( TESS_WINDING_ODD, TESS_POLYGONS, 3, 2 );
	if ( !error ) {
		tesselator.GetTriangles ( vtxBuffer, idxBuffer );
	}
}

//----------------------------------------------------------------//
MOAIRegion::MOAIRegion () {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIRegion::~MOAIRegion () {
}

//----------------------------------------------------------------//
bool MOAIRegion::PointInside ( const ZLVec2D& p ) {

	bool inside = false;

	u32 nPolys = this->Size ();
	for ( u32 i = 0; i < nPolys; ++i ) {
		const ZLPolygon2D& poly = ( *this )[ i ];
		if ( poly.PointInside ( p )) {
			inside = !inside;
		}
	}
	return inside;
}

//----------------------------------------------------------------//
void MOAIRegion::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIRegion::RegisterLuaFuncs ( MOAILuaState& state ) {

	// here are the instance methods:
	luaL_Reg regTable [] = {
		{ "getTriangles",		_getTriangles },
		{ "pointInside",		_pointInside },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIRegion::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( serializer );

	size_t nPolys = lua_objlen ( state, -1 );
	this->Init ( nPolys );
	
	for ( size_t i = 0; i < nPolys; ++i ) {
		ZLPolygon2D& poly = ( *this )[ i ];
	
		state.GetField ( -1, i + 1 );
	
		size_t len = 0;
		const void* vertices = lua_tolstring ( state, -1, &len );
		
		poly.Init ( len / sizeof ( ZLVec2D ));
		memcpy ( poly.Data (), vertices, len );
		poly.Bless ();
		
		state.Pop ( 1 );
	}
}

//----------------------------------------------------------------//
void MOAIRegion::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( serializer );
	
	size_t nPolys = this->Size ();
	for ( size_t i = 0; i < nPolys; ++i ) {
		const ZLPolygon2D& poly = ( *this )[ i ];

		state.Push (( u32 )i + 1 );
		lua_pushlstring ( state, ( cc8* )poly.Data (), poly.Size () * sizeof ( ZLVec2D ));
		lua_settable ( state, -3 );
	}
}
