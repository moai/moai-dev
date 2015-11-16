// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDraw.h>
#include <moai-sim/MOAIGfxDevice.h>
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
int MOAIRegion::_bless ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRegion, "U" )
	
	self->Bless ();
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRegion::_boolean ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRegion, "UUU" )

	MOAIRegion* regionA		= state.GetLuaObject < MOAIRegion >( 2, false );
	MOAIRegion* regionB		= state.GetLuaObject < MOAIRegion >( 3, false );

	u32 operation			= state.GetValue < u32 >( 4, BOOLEAN_OR );

	if ( regionA && regionB ) {
		self->Boolean ( *regionA, *regionB, operation );
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIRegion::_copy ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRegion, "UU" )

	MOAIRegion* region = state.GetLuaObject < MOAIRegion >( 2, false );

	if ( region ) {
		self->Copy ( *region );
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIRegion::_cull ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRegion, "U" )
	
	MOAIRegion* region		= state.GetLuaObject < MOAIRegion >( 2, false );
	
	if ( region ) {
	
		u32 flag			= state.GetValue < u32 >( 3, ZLPolygon2D::IS_CLOCKWISE );

		bool checkArea		= state.IsType ( 4, LUA_TNUMBER );
		float minArea		= state.GetValue < float >( 4, 0.0f );

		self->Cull ( *region, flag, checkArea, minArea );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRegion::_drawDebug ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRegion, "U" )

	self->DrawDebug ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRegion::_edge ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRegion, "U" )
	
	MOAIRegion* region		= state.GetLuaObject < MOAIRegion >( 2, false );
	ZLVec2D point			= state.GetValue < ZLVec2D >( 3, ZLVec2D ( 0.0f, 0.0f ));
	
	if ( region ) {
		self->Edge ( *region, point );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRegion::_getDistance ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRegion, "U" )

	ZLVec2D point = state.GetValue < ZLVec2D >( 2, ZLVec2D ( 0.0f, 0.0f ));

	float d;
	ZLVec2D p;
	
	if ( self->GetDistance ( point, d, p )) {
	
		state.Push ( d );
		state.Push ( p.mX );
		state.Push ( p.mY );
		
		return 3;
	}

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRegion::_getTriangles ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRegion, "U" )
	
	u32 base = 0;
	int totalElements = 0;
	
	MOAIVertexBuffer* vtxBuffer		= state.GetLuaObject < MOAIVertexBuffer >( 2, false );
	MOAIIndexBuffer* idxBuffer		= state.GetLuaObject < MOAIIndexBuffer >( 3, false );

	if ( vtxBuffer && idxBuffer ) {
	
		u32 idxSizeInBytes = state.GetValue < u32 >( 4, 4 );
		MOAIVertexFormat* format = state.GetLuaObject < MOAIVertexFormat >( 5, false );
		
		base = idxBuffer->GetCursor () / idxSizeInBytes;
		totalElements = self->GetTriangles ( *format, *vtxBuffer, *idxBuffer, idxSizeInBytes );
	}
	else {
		
		MOAIStream* vtxStream		= state.GetLuaObject < MOAIStream >( 2, false );
		MOAIStream* idxStream		= state.GetLuaObject < MOAIStream >( 3, false );
		MOAIVertexFormat* format	= state.GetLuaObject < MOAIVertexFormat >( 4, false );
		
		if ( vtxStream && idxStream && format ) {
			base = idxStream->GetCursor () / 4;
			totalElements = self->GetTriangles ( *format, *vtxStream, *idxStream );
		}
	}
	
	state.Push ( totalElements );
	state.Push ( base );
	state.Push ( base + totalElements );
	return 3;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRegion::_pointInside ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRegion, "UNN" )
	
	float x		= state.GetValue < float >( 2, 0.0f );
	float y		= state.GetValue < float >( 3, 0.0f );
	float pad	= state.GetValue < float >( 4, 0.0f );

	state.Push ( self->PointInside ( ZLVec2D ( x, y ), pad ));
	
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRegion::_print ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRegion, "U" )
	
	self->Print ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRegion::_reservePolygons ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRegion, "U" )
	
	self->ReservePolygons ( state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRegion::_reserveVertices ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRegion, "UN" )
	
	u32 idx		= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 size	= state.GetValue < u32 >( 3, 0 );
	
	self->mPolygons [ idx ].ReserveVertices ( size );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRegion::_reverseWinding ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRegion, "U" )

	MOAIRegion* region		= state.GetLuaObject < MOAIRegion >( 2, false );
	
	if ( region ) {
		self->ReverseWinding ( *region );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRegion::_setVertex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRegion, "UNNNN" )
	
	u32 polyIdx		= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 vertIdx		= state.GetValue < u32 >( 3, 1 ) - 1;
	
	float x			= state.GetValue < float >( 4, 0.0f );
	float y			= state.GetValue < float >( 5, 0.0f );
	
	self->mPolygons [ polyIdx ].SetVert ( vertIdx, x, y );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIRegion::_setWinding ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRegion, "U" )
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRegion::_snap ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRegion, "U" )

	MOAIRegion* region		= state.GetLuaObject < MOAIRegion >( 2, false );

	if ( region ) {
	
		float xSnap = state.GetValue < float >( 3, 0.0f );
		float ySnap = state.GetValue < float >( 4, 0.0f );

		self->Snap ( *region, xSnap, ySnap );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRegion::_stroke ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRegion, "U" )

	MOAIRegion* region		= state.GetLuaObject < MOAIRegion >( 2, false );
	
	float exterior			= 0.0;
	bool strokeExterior		= false;
	
	float interior			= 0.0;
	bool strokeInterior		= false;
	
	if ( state.IsType ( 3, LUA_TNUMBER )) {
		exterior = state.GetValue < float >( 3, 0.0f );
		strokeExterior = true;
	}
	
	if ( strokeExterior && state.IsType ( 4, LUA_TNUMBER )) {
	
		interior = exterior;
		strokeInterior = true;
	
		exterior = state.GetValue < float >( 4, 0.0f );
	}
	
	if ( region ) {
	
		if ( strokeExterior || strokeInterior ) {
			self->Stroke ( *region, exterior, strokeExterior, interior, strokeInterior );
		}
		else {
			self->Copy ( *region );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIRegion::_tesselate ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRegion, "U" )
	
	MOAIRegion* regionA		= state.GetLuaObject < MOAIRegion >( 2, false );

	if ( regionA ) {
	
		MOAIRegion* regionB		= state.GetLuaObject < MOAIRegion >( 3, false );
	
		if ( regionB ) {
		
			u32 windingRule		= state.GetValue < u32 >( 4, TESS_WINDING_ODD );
			self->CombineAndTesselate ( *regionA, *regionB, windingRule );
		}
		else {
		
			u32 windingRule		= state.GetValue < u32 >( 3, TESS_WINDING_ODD );
			self->Tesselate ( *regionA, windingRule );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIRegion::_translate ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIRegion, "U" )
	
	MOAIRegion* region = state.GetLuaObject < MOAIRegion >( 2, false );
	
	if ( region ) {
	
		float xOff = state.GetValue < float >( 2, 0.0f );
		float yOff = state.GetValue < float >( 3, 0.0f );
		
		ZLAffine2D mtx;
		mtx.Translate ( xOff, yOff );
		
		self->Transform ( *region, mtx );
	}
	return 0;
}

//================================================================//
// MOAIRegion
//================================================================//

//----------------------------------------------------------------//
int MOAIRegion::AddFillContours ( SafeTesselator& tess, u32 mask ) const {

	size_t size = this->mPolygons.Size ();
	
	for ( size_t i = 0; i < size; ++i ) {
		ZLPolygon2D& polygon = this->mPolygons [ i ];
		if ( polygon.GetInfo () & mask ) {
			tess.AddPolygon ( this->mPolygons [ i ]);
		}
	}
	
	return 0; // since we're not tesselating here (and thus do not have any error case) we always report no error
}

//----------------------------------------------------------------//
void MOAIRegion::Bless () {

	size_t size = this->mPolygons.Size ();
	
	for ( size_t i = 0; i < size; ++i ) {
		this->mPolygons [ i ].Bless ();
	}
}

//----------------------------------------------------------------//
void MOAIRegion::Boolean ( const MOAIRegion& regionA, const MOAIRegion& regionB, u32 operation ) {

	switch ( operation ) {
	
		case BOOLEAN_AND:
			this->BooleanAnd ( regionA, regionB );
			break;
		
		case BOOLEAN_NOT:
			this->BooleanNot ( regionA, regionB );
			break;
		
		case BOOLEAN_OR:
			this->BooleanOr ( regionA, regionB );
			break;
			
		case BOOLEAN_XOR:
			this->BooleanXor ( regionA, regionB );
			break;
	}
}

//----------------------------------------------------------------//
void MOAIRegion::BooleanAnd ( const MOAIRegion& regionA, const MOAIRegion& regionB ) {
	
	this->CombineAndTesselate ( regionA, regionB, TESS_WINDING_ABS_GEQ_TWO );
}

//----------------------------------------------------------------//
void MOAIRegion::BooleanNot ( const MOAIRegion& regionA, const MOAIRegion& regionB ) {

	MOAIRegion regionOr;
	int error = regionOr.CombineAndTesselate ( regionA, regionB, TESS_WINDING_POSITIVE );

	if ( !error ) {
		this->CombineAndTesselate ( regionOr, regionB, TESS_WINDING_ODD );
	}
}

//----------------------------------------------------------------//
void MOAIRegion::BooleanOr ( const MOAIRegion& regionA, const MOAIRegion& regionB ) {

	this->CombineAndTesselate ( regionA, regionB, TESS_WINDING_POSITIVE );
	this->Print ();
}

//----------------------------------------------------------------//
void MOAIRegion::BooleanXor ( const MOAIRegion& regionA, const MOAIRegion& regionB ) {

	this->CombineAndTesselate ( regionA, regionB, TESS_WINDING_ODD );
}

//----------------------------------------------------------------//
int MOAIRegion::CombineAndTesselate ( const MOAIRegion& regionA, const MOAIRegion& regionB, int windingRule ) {

	SafeTesselator tess;
	
	regionA.AddFillContours ( tess );
	regionB.AddFillContours ( tess );
	
	int error = tess.Tesselate ( windingRule, TESS_BOUNDARY_CONTOURS, 0, 0 );

	if ( !error ) {
		this->Copy ( tess );
		this->Bless ();
		this->Cull ( *this, ZLPolygon2D::IS_CORRUPT );
	}
	return error;
}

//----------------------------------------------------------------//
void MOAIRegion::Copy ( const MOAIRegion& region ) {

	if ( this != &region ) {

		size_t size = region.mPolygons.Size ();

		this->mPolygons.Init ( size );
		
		for ( size_t i = 0; i < size; ++i ) {
			this->mPolygons [ i ].Copy ( region.mPolygons [ i ]);
		}
	}
}

//----------------------------------------------------------------//
void MOAIRegion::Copy ( const SafeTesselator& tess ) {

	const int* elems	= tessGetElements ( tess.mTess );
	int nelems			= tessGetElementCount ( tess.mTess );
	const float* verts	= tessGetVertices ( tess.mTess );

	// each elem is an edge loop
	this->ReservePolygons ( nelems );
	
	for ( int i = 0; i < nelems; ++i ) {
	
		ZLPolygon2D& poly = this->GetPolygon ( i );
		
		int b = elems [( i * 2 )];
		int n = elems [( i * 2 ) + 1 ];
		
		poly.SetVertices (( ZLVec2D* )&verts [ b * 2 ], n );
	}
}

//----------------------------------------------------------------//
void MOAIRegion::Cull ( const MOAIRegion& region, u32 flag, bool checkArea, float minArea ) {

	size_t count	= 0;
	size_t size		= region.mPolygons.Size ();
	
	for ( size_t i = 0; i < size; ++i ) {
		ZLPolygon2D& poly = region.mPolygons [ i ];
		if ( !this->ShouldCull ( poly, flag, checkArea, minArea )) {
			count++;
		}
	}
	
	if ( count == region.mPolygons.Size ()) {
		this->Copy ( region );
		return;
	}
	
	const MOAIRegion* srcRegion = &region;
	MOAIRegion copyRegion;
	
	if ( this == srcRegion ) {
		copyRegion.Copy ( region );
		srcRegion = &copyRegion;
	}
	
	this->ReservePolygons ( count );
	
	count = 0;
	
	for ( size_t i = 0; i < size; ++i ) {
		ZLPolygon2D& poly = srcRegion->mPolygons [ i ];
		if ( !this->ShouldCull ( poly, flag, checkArea, minArea )) {
			this->mPolygons [ count++ ].Copy ( poly );
		}
	}
}

//----------------------------------------------------------------//
void MOAIRegion::DrawDebug () const {

	static u32 POLY_UNKNOWN_COLOR					= ZLColor::PackRGBA ( 0.5f, 0.5f, 0.5f, 1.0f );
	static u32 POLY_COMPLEX_COLOR					= ZLColor::PackRGBA ( 1.0f, 0.0f, 1.0f, 1.0f );
	
	static u32 POLY_ANTICLOCKWISE_CONVEX_COLOR		= ZLColor::PackRGBA ( 0.0f, 1.0f, 1.0f, 1.0f );
	static u32 POLY_ANTICLOCKWISE_CONCAVE_COLOR		= ZLColor::PackRGBA ( 0.0f, 0.5f, 1.0f, 1.0f );
	
	static u32 POLY_CLOCKWISE_CONVEX_COLOR			= ZLColor::PackRGBA ( 0.5f, 1.0f, 0.0f, 1.0f );
	static u32 POLY_CLOCKWISE_CONCAVE_COLOR			= ZLColor::PackRGBA ( 0.0f, 1.0f, 0.0f, 1.0f );
	
	static u32 POLY_CORRUPT_COLOR					= ZLColor::PackRGBA ( 1.0f, 0.0f, 0.0f, 1.0f );

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	MOAIDraw::Bind ();

	u32 nPolys = this->mPolygons.Size ();
	for ( u32 i = 0; i < nPolys; ++i ) {
		const ZLPolygon2D& poly = this->mPolygons [ i ];
		
		switch ( poly.GetInfo ()) {
		
			case ZLPolygon2D::POLY_UNKNOWN: {
				gfxDevice.SetPenColor ( POLY_UNKNOWN_COLOR );
				gfxDevice.SetPenWidth ( 1.0f );
				break;
			}
			case ZLPolygon2D::POLY_COMPLEX_BIT: {
				gfxDevice.SetPenColor ( POLY_COMPLEX_COLOR );
				gfxDevice.SetPenWidth ( 1.0f );
				break;
			}
			case ZLPolygon2D::POLY_ANTICLOCKWISE_CONVEX: {
				gfxDevice.SetPenColor ( POLY_ANTICLOCKWISE_CONVEX_COLOR );
				gfxDevice.SetPenWidth ( 2.0f );
				break;
			}
			case ZLPolygon2D::POLY_ANTICLOCKWISE_CONCAVE: {
				gfxDevice.SetPenColor ( POLY_ANTICLOCKWISE_CONCAVE_COLOR );
				gfxDevice.SetPenWidth ( 2.0f );
				break;
			}
			case ZLPolygon2D::POLY_CLOCKWISE_CONVEX: {
				gfxDevice.SetPenColor ( POLY_CLOCKWISE_CONVEX_COLOR );
				gfxDevice.SetPenWidth ( 1.0f );
				break;
			}
			case ZLPolygon2D::POLY_CLOCKWISE_CONCAVE: {
				gfxDevice.SetPenColor ( POLY_CLOCKWISE_CONCAVE_COLOR );
				gfxDevice.SetPenWidth ( 1.0f );
				break;
			}
			case ZLPolygon2D::POLY_CORRUPT: {
				gfxDevice.SetPenColor ( POLY_CORRUPT_COLOR );
				gfxDevice.SetPenWidth ( 1.0f );
				break;
			}
		}
		
		MOAIDraw::DrawPolyOutline ( poly );
	}
}

//----------------------------------------------------------------//
void MOAIRegion::Edge ( const MOAIRegion& region, const ZLVec2D& offset ) {

	SafeTesselator tess;
	
	size_t size = region.mPolygons.Size ();

	for ( size_t i = 0; i < size; ++i ) {
		ZLPolygon2D& polygon = region.mPolygons [ i ];
		
		size_t nVerts = polygon.GetSize ();
		
		for ( size_t j = 0; j < nVerts; ++j ) {
			
			ZLVec2D contour [ 4 ];
			
			contour [ 0 ] = polygon.GetVertex ( j );
			contour [ 1 ] = polygon.GetVertex (( j + 1 ) % nVerts );
			
			ZLVec2D edgeVec = contour [ 1 ];
			edgeVec.Sub ( contour [ 0 ]);
			
			edgeVec.Rotate90Clockwise ();
			if ( edgeVec.Dot ( offset ) < 0.0f ) {
			
				ZLVec2D swap = contour [ 0 ];
				contour [ 0 ] = contour [ 1 ];
				contour [ 1 ] = swap;
			}
			
			contour [ 2 ] = contour [ 1 ];
			contour [ 3 ] = contour [ 0 ];
			
			contour [ 2 ].Add ( offset );
			contour [ 3 ].Add ( offset );
			
			ZLVec2D d0 = contour [ 2 ];
			d0.Sub ( contour [ 0 ]);
			
			ZLVec2D d1 = contour [ 3 ];
			d1.Sub ( contour [ 1 ]);
			
			if ( ABS ( d0.Cross ( d1 )) > FLT_EPSILON ) {
				tess.AddContour ( 2, contour, sizeof ( ZLVec2D ), 4 );
			}
		}
	}
	
	int error = tess.Tesselate ( TESS_WINDING_POSITIVE, TESS_BOUNDARY_CONTOURS, 0, 0 );
	if ( !error ) {
		this->Copy ( tess );
		this->Bless ();
	}
}

//----------------------------------------------------------------//
bool MOAIRegion::GetDistance ( const ZLVec2D& point, float& d ) const {

	ZLVec2D unused;
	return this->GetDistance ( point, d, unused );
}

//----------------------------------------------------------------//
bool MOAIRegion::GetDistance ( const ZLVec2D& point, float& d, ZLVec2D& p ) const {

	bool foundResult = false;

	for ( size_t i = 0; i < this->mPolygons.Size (); ++i ) {
	
		ZLPolygon2D& poly = this->mPolygons [ i ];
		
		float		candidateD;
		ZLVec2D		candidateP;
		
		if ( poly.GetDistance ( point, candidateD, candidateP )) {
		
			if (( !foundResult ) || ( candidateD < d )) {
				d = candidateD;
				p = candidateP;
				foundResult = true;
			}
		}
	}
	return foundResult;
}

//----------------------------------------------------------------//
ZLPolygon2D& MOAIRegion::GetPolygon ( u32 idx ) {

	return this->mPolygons [ idx ];
}

//----------------------------------------------------------------//
const ZLPolygon2D& MOAIRegion::GetPolygon ( u32 idx ) const {

	return this->mPolygons [ idx ];
}

//----------------------------------------------------------------//
u32 MOAIRegion::GetTriangles ( SafeTesselator& tess ) const {

	u32 nPolys = this->mPolygons.Size ();
	for ( u32 i = 0; i < nPolys; ++i ) {
		const ZLPolygon2D& poly = this->mPolygons [ i ];
		tess.AddContour ( 2, poly.GetVertices (), sizeof ( ZLVec2D ), poly.GetSize ());
	}
	
	return tess.Tesselate ( TESS_WINDING_NONZERO, TESS_POLYGONS, 3, 2 );
}

//----------------------------------------------------------------//
u32 MOAIRegion::GetTriangles ( MOAIVertexFormat& format, ZLStream& vtxStream, ZLStream& idxStream ) const {

	SafeTesselator tess;
	int error = this->GetTriangles ( tess );
	
	if ( !error ) {
		return tess.GetTriangles ( format, vtxStream, idxStream );
	}
	return 0;
}

//----------------------------------------------------------------//
u32 MOAIRegion::GetTriangles ( MOAIVertexFormat& format, MOAIVertexBuffer& vtxBuffer, MOAIIndexBuffer& idxBuffer, u32 idxSizeInBytes ) const {

	SafeTesselator tess;
	int error = this->GetTriangles ( tess );
	
	if ( !error ) {
		return tess.GetTriangles ( format, vtxBuffer, idxBuffer, idxSizeInBytes );
	}
	return 0;
}

//----------------------------------------------------------------//
MOAIRegion::MOAIRegion () {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIRegion::~MOAIRegion () {
}

//----------------------------------------------------------------//
bool MOAIRegion::PointInside ( const ZLVec2D& p, float pad ) const {

	u32 nPolys = this->mPolygons.Size ();

	if ( pad != 0.0f ) {
		float dist = 0.0f;
		if ( this->GetDistance ( p, dist )) {
			if ( dist <= ABS ( pad ) ) {
				return pad < 0.0f ? false : true;
			}
		}
	}

	bool inside = false;

	for ( u32 i = 0; i < nPolys; ++i ) {
		
		switch ( this->mPolygons [ i ].PointInside ( p )) {
		
			case ZLPolygon2D::POINT_ON_EDGE:
				return true;
			
			case ZLPolygon2D::POINT_INSIDE:
				inside = !inside;
				break;
		}
	}
	return inside;
}

//----------------------------------------------------------------//
void MOAIRegion::Print () const {

	size_t nPolys = this->mPolygons.Size ();
	for ( size_t i = 0; i < nPolys; ++i ) {
	
		const ZLPolygon2D& poly = this->mPolygons [ i ];
		
		printf ( "poly %d:\n", i );
	
		size_t nVerts = poly.GetSize ();
		for ( size_t j = 0; j < nVerts; ++j ) {
		
			const ZLVec2D& v = poly.GetVertex ( j );
			printf ( "\t%d: (%f, %f)\n", j, v.mX, v.mY );
		}
	}
}

//----------------------------------------------------------------//
void MOAIRegion::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "BOOLEAN_AND",					( u32 )BOOLEAN_AND );
	state.SetField ( -1, "BOOLEAN_NOT",					( u32 )BOOLEAN_NOT );
	state.SetField ( -1, "BOOLEAN_OR",					( u32 )BOOLEAN_OR );
	state.SetField ( -1, "BOOLEAN_XOR",					( u32 )BOOLEAN_XOR );
	
	state.SetField ( -1, "TESS_WINDING_ODD",			( u32 )TESS_WINDING_ODD );
	state.SetField ( -1, "TESS_WINDING_NONZERO",		( u32 )TESS_WINDING_NONZERO );
	state.SetField ( -1, "TESS_WINDING_POSITIVE",		( u32 )TESS_WINDING_POSITIVE );
	state.SetField ( -1, "TESS_WINDING_NEGATIVE",		( u32 )TESS_WINDING_NEGATIVE );
	state.SetField ( -1, "TESS_WINDING_ABS_GEQ_TWO",	( u32 )TESS_WINDING_ABS_GEQ_TWO );
	
	state.SetField ( -1, "IS_COMPLEX",					( u32 )ZLPolygon2D::IS_COMPLEX );
	state.SetField ( -1, "IS_CORRUPT",					( u32 )ZLPolygon2D::IS_CORRUPT );
	state.SetField ( -1, "IS_UNKNOWN",					( u32 )ZLPolygon2D::IS_UNKNOWN );
	state.SetField ( -1, "IS_CONVEX",					( u32 )ZLPolygon2D::IS_CONVEX );
	state.SetField ( -1, "IS_CONCAVE",					( u32 )ZLPolygon2D::IS_CONCAVE );
	state.SetField ( -1, "IS_CLOCKWISE",				( u32 )ZLPolygon2D::IS_CLOCKWISE );
	state.SetField ( -1, "IS_ANTICLOCKWISE",			( u32 )ZLPolygon2D::IS_ANTICLOCKWISE );
	state.SetField ( -1, "IS_CLOCKWISE_CONVEX",			( u32 )ZLPolygon2D::IS_CLOCKWISE_CONVEX );
	state.SetField ( -1, "IS_CLOCKWISE_CONCAVE",		( u32 )ZLPolygon2D::IS_CLOCKWISE_CONCAVE );
	state.SetField ( -1, "IS_ANTICLOCKWISE_CONVEX",		( u32 )ZLPolygon2D::IS_ANTICLOCKWISE_CONVEX );
	state.SetField ( -1, "IS_ANTICLOCKWISE_CONCAVE",	( u32 )ZLPolygon2D::IS_ANTICLOCKWISE_CONCAVE );
	
	state.SetField ( -1, "MAKE_CONVEX",					( u32 )ZLPolygon2D::IS_CONVEX );
	state.SetField ( -1, "MAKE_CONCAVE",				( u32 )ZLPolygon2D::IS_CONCAVE );
}

//----------------------------------------------------------------//
void MOAIRegion::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "bless",				_bless },
		{ "boolean",			_boolean },
		{ "copy",				_copy },
		{ "cull",				_cull },
		{ "drawDebug",			_drawDebug },
		{ "edge",				_edge },
		{ "getDistance",		_getDistance },
		{ "getTriangles",		_getTriangles },
		{ "pointInside",		_pointInside },
		{ "print",				_print },
		{ "reservePolygons",	_reservePolygons },
		{ "reserveVertices",	_reserveVertices },
		{ "reverseWinding",		_reverseWinding },
		{ "setVertex",			_setVertex },
		{ "setWinding",			_setWinding },
		{ "snap",				_snap },
		{ "stroke",				_stroke },
		{ "tesselate",			_tesselate },
		{ "translate",			_translate },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIRegion::ReservePolygons ( u32 size ) {

	this->mPolygons.Init ( size );
}

//----------------------------------------------------------------//
void MOAIRegion::ReverseWinding ( const MOAIRegion& region ) {

	this->Copy ( region );

	size_t size = this->mPolygons.Size ();
	
	for ( size_t i = 0; i < size; ++i ) {
		ZLPolygon2D& polygon = this->mPolygons [ i ];
		polygon.ReverseWinding ();
	}
}

//----------------------------------------------------------------//
void MOAIRegion::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( serializer );

	size_t nPolys = lua_objlen ( state, -1 );
	this->mPolygons.Init ( nPolys );
	
	for ( size_t i = 0; i < nPolys; ++i ) {
		ZLPolygon2D& poly = this->mPolygons [ i ];
	
		state.GetField ( -1, i + 1 );
	
		size_t len = 0;
		const void* vertices = lua_tolstring ( state, -1, &len );
		
		size_t nVertices = len / sizeof ( ZLVec2D );
		
		poly.SetVertices (( ZLVec2D* )vertices, nVertices );
		poly.Bless ();
		
		state.Pop ( 1 );
	}
}

//----------------------------------------------------------------//
void MOAIRegion::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( serializer );
	
	size_t nPolys = this->mPolygons.Size ();
	for ( size_t i = 0; i < nPolys; ++i ) {
		const ZLPolygon2D& poly = this->mPolygons [ i ];

		state.Push (( u32 )i + 1 );
		lua_pushlstring ( state, ( cc8* )poly.GetVertices (), poly.GetSize () * sizeof ( ZLVec2D ));
		lua_settable ( state, -3 );
	}
}

//----------------------------------------------------------------//
bool MOAIRegion::ShouldCull ( const ZLPolygon2D& poly, u32 flag, bool checkArea, float minArea ) {
	
	return poly.Check ( flag ) || (( checkArea == true ) && ( !poly.Check ( ZLPolygon2D::HAS_AREA ) || poly.GetArea () < minArea ));
}

//----------------------------------------------------------------//
void MOAIRegion::Snap ( const MOAIRegion& region, float xSnap, float ySnap ) {

	this->Copy ( region );
	
	size_t size = this->mPolygons.Size ();
	
	for ( size_t i = 0; i < size; ++i ) {
		this->mPolygons [ i ].Snap ( xSnap, ySnap );
	}
}

//----------------------------------------------------------------//
void MOAIRegion::Stroke ( const MOAIRegion& region, float exterior, bool strokeExterior, float interior, bool strokeInterior ) {

	SafeTesselator tess;
	
	size_t size = region.mPolygons.Size ();

	MOAIVectorStyle style;
	style.Default ();

	for ( size_t i = 0; i < size; ++i ) {
		ZLPolygon2D& polygon = region.mPolygons [ i ];
		
		size_t nVerts = polygon.GetSize ();
		
		MOAIVectorLineJoin* joins = ( MOAIVectorLineJoin* )alloca ( sizeof ( MOAIVectorLineJoin ) * nVerts );
		
		// stroke the exterior
		if ( strokeExterior ) {
		
			MOAIVectorUtil::ComputeLineJoins ( joins, polygon.GetVertices (), nVerts, false, true, false );
			
			bool exact = ( exterior == 0.0f );
		
			int contourVerts = MOAIVectorUtil::StrokeLine ( style, 0, joins, nVerts, exterior, exact );
			ZLVec2D* contour = ( ZLVec2D* )alloca ( sizeof ( ZLVec2D ) * contourVerts );
			MOAIVectorUtil::StrokeLine ( style, contour, joins, nVerts, exterior, exact );
			tess.AddContour ( 2, contour, sizeof ( ZLVec2D ), contourVerts );
		}
		
		// stroke the interior
		if ( strokeInterior ) {
		
			MOAIVectorUtil::ComputeLineJoins ( joins, polygon.GetVertices (), nVerts, false, false, true );
			
			bool exact = ( interior == 0.0f );
			
			int contourVerts = MOAIVectorUtil::StrokeLine ( style, 0, joins, nVerts, interior, exact );
			ZLVec2D* contour = ( ZLVec2D* )alloca ( sizeof ( ZLVec2D ) * contourVerts );
			MOAIVectorUtil::StrokeLine ( style, contour, joins, nVerts, interior, exact );
			tess.AddContour ( 2, contour, sizeof ( ZLVec2D ), contourVerts );
		}
	}
	
	int error = tess.Tesselate ( TESS_WINDING_POSITIVE, TESS_BOUNDARY_CONTOURS, 0, 0 );
	if ( !error ) {
		this->Copy ( tess );
		this->Bless ();
	}
}

//----------------------------------------------------------------//
int MOAIRegion::Tesselate ( const MOAIRegion& region, int windingRule ) {

	SafeTesselator tess;
	
	region.AddFillContours ( tess );
	
	int error = tess.Tesselate ( windingRule, TESS_BOUNDARY_CONTOURS, 0, 0 );

	if ( !error ) {
		this->Copy ( tess );
		this->Bless ();
	}
	return error;
}

//----------------------------------------------------------------//
void MOAIRegion::Transform ( const MOAIRegion& region, const ZLAffine2D& transform ) {

	this->Copy ( region );

	size_t nPolys = this->mPolygons.Size ();
	for ( size_t i = 0; i < nPolys; ++i ) {
		this->mPolygons [ i ].Transform ( transform );
	}
}
