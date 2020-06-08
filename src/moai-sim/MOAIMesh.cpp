// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <tesselator.h>

#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIMesh.h>
#include <moai-sim/MOAIMeshSparseQuadTree.h>
#include <moai-sim/MOAIMeshTernaryTree.h>
#include <moai-sim/MOAIRegion.h>
#include <moai-sim/MOAIVectorUtil.h>

//================================================================//
// MOAIMeshCallable
//================================================================//
class MOAIMeshCallable :
	public MOAIAbstractGfxScriptCallback {
public:

	MOAIGfxTopologyEnum::_		mPrimType;
	MOAIMeshSpan*				mSpan;
	u32							mTotalElements;

	//----------------------------------------------------------------//
	void MOAIAbstractGfxScriptCallback_Call () {
	
		MOAIMeshSpan* span = this->mSpan;
	
		for ( ; span; span = span->mNext ) {
			if ( span->mBase == span->mTop ) continue;
			assert (( span->mBase < span->mTop ) && ( span->mTop <= this->mTotalElements ));
			MOAIGfxMgr::Get ().DrawPrims ( this->mPrimType, span->mBase, ( u32 )( span->mTop - span->mBase ));
		}
	}
};

//================================================================//
// MOAIMeshPrim
//================================================================//
	
//----------------------------------------------------------------//
ZLBox MOAIMeshPrimCoords::GetAABB () {

	ZLBox aabb;
	aabb.Init ( this->mCoords [ 0 ]);
	
	if ( this->mPrimSize > 1 ) {
	
		aabb.Grow ( this->mCoords [ 1 ]);
	
		if ( this->mPrimSize > 2 ) {
			aabb.Grow ( this->mCoords [ 2 ]);
		}
	}
	return aabb;
}

//================================================================//
// MOAIMeshPrimReader
//================================================================//

//----------------------------------------------------------------//
bool MOAIMeshPrimReader::GetPrimCoords ( u32 idx, MOAIMeshPrimCoords& prim ) const {

	// TODO: should return index instead of bool?
	
	assert ( this->mVertexFormat && this->mVertexBuffer );
	
	prim.mIndex = idx;
	
	switch ( this->mPrimType ) {
		
		case MOAIGfxTopologyEnum::POINT_LIST: {
		
			prim.mPrimSize = 1;
			prim.mCoords [ 0 ] = this->ReadCoord ( idx );
			return true;
		}
		
		case MOAIGfxTopologyEnum::LINE_LIST:
		
			idx = idx * 2;

		case MOAIGfxTopologyEnum::LINE_LOOP:
		case MOAIGfxTopologyEnum::LINE_STRIP: {
		
			prim.mPrimSize = 2;
			
			prim.mCoords [ 0 ] = this->ReadCoord ( idx++ );
			prim.mCoords [ 1 ] = this->ReadCoord ( idx );
			return true;
		}
		
		case MOAIGfxTopologyEnum::TRIANGLE_LIST: {
		
			prim.mPrimSize = 3;
		
			idx = idx * 3;
			
			prim.mCoords [ 0 ] = this->ReadCoord ( idx++ );
			prim.mCoords [ 1 ] = this->ReadCoord ( idx++ );
			prim.mCoords [ 2 ] = this->ReadCoord ( idx );
			return true;
		}
		
		case MOAIGfxTopologyEnum::TRIANGLE_FAN: {
		
			prim.mPrimSize = 3;
		
			prim.mCoords [ 0 ] = this->ReadCoord ( 0 );
		
			idx = idx + 1;
		
			prim.mCoords [ 1 ] = this->ReadCoord ( idx++ );
			prim.mCoords [ 2 ] = this->ReadCoord ( idx );
			return true;
		}
		
		case MOAIGfxTopologyEnum::TRIANGLE_STRIP: {
		
			// 0   1   2   3   4   5   6
			// 012 213 234 435 456 657 678
		
			prim.mPrimSize = 3;
		
			if ( idx & 1 ) {
				
				// odd
				prim.mCoords [ 0 ] = this->ReadCoord ( idx + 1 );
				prim.mCoords [ 1 ] = this->ReadCoord ( idx );
				prim.mCoords [ 2 ] = this->ReadCoord ( idx + 2 );
			}
			else {
			
				// even
				prim.mCoords [ 0 ] = this->ReadCoord ( idx++ );
				prim.mCoords [ 1 ] = this->ReadCoord ( idx++ );
				prim.mCoords [ 2 ] = this->ReadCoord ( idx );
			}
			return true;
		}
	}
	
	return false;
}

//----------------------------------------------------------------//
bool MOAIMeshPrimReader::Init ( MOAIVertexFormat& vertexFormat, MOAIVertexBuffer& vertexBuffer, MOAIGfxTopologyEnum::_ primType, u32 totalElements, MOAIIndexBuffer* indexBuffer ) {

	if ( !vertexFormat.CountAttributesByUse ( MOAIVertexFormat::ATTRIBUTE_COORD )) return false;

	this->mVertexFormat		= &vertexFormat;
	
	this->mAttribute		= vertexFormat.GetAttributeByUse ( MOAIVertexFormat::ATTRIBUTE_COORD, 0 );
	this->mVertexBuffer		= vertexBuffer.GetConstBuffer ();
	this->mIndexBuffer		= indexBuffer;
	
	this->mPrimType			= primType;
	this->mTotalElements	= totalElements;
	this->mTotalPrims		= 0;
	
	switch ( this->mPrimType ) {
		
		case MOAIGfxTopologyEnum::POINT_LIST:
		case MOAIGfxTopologyEnum::LINE_LOOP:
			this->mTotalPrims = totalElements;
			break;
		
		case MOAIGfxTopologyEnum::LINE_LIST:
			this->mTotalPrims = totalElements / 2;
			break;
		
		case MOAIGfxTopologyEnum::LINE_STRIP:
			this->mTotalPrims = totalElements - 1;
			break;
		
		case MOAIGfxTopologyEnum::TRIANGLE_LIST:
			this->mTotalPrims = totalElements / 3;
			break;
		
		case MOAIGfxTopologyEnum::TRIANGLE_FAN:
		case MOAIGfxTopologyEnum::TRIANGLE_STRIP:
			this->mTotalPrims = totalElements - 2;
			break;
	}
	
	return true;
}

//----------------------------------------------------------------//
ZLVec3D MOAIMeshPrimReader::ReadCoord ( u32 idx ) const {

	if ( !( this->mVertexFormat && this->mAttribute && this->mVertexBuffer )) return ZLVec3D::ORIGIN; // TODO: report error

	idx %= this->mTotalElements;

	if ( this->mIndexBuffer ) {
		idx = this->mIndexBuffer->GetIndex ( idx );
	}
	
	const void* packedCoord = this->mVertexFormat->GetAttributeAddress ( *this->mAttribute, this->mVertexBuffer, idx );
	ZLVec4D coord = this->mVertexFormat->UnpackCoord ( packedCoord, *this->mAttribute );
	
	return ZLVec3D ( coord.mX, coord.mY, coord.mZ );
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_buildQuadTree ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )

	u32 targetPrimsPerNode			= state.GetValue < u32 >( 1, MOAIMeshSparseQuadTree::DEFAULT_TARGET_PRIMS_PER_NODE );
	ZLIndex vertexBufferIndex		= state.GetValue < MOAILuaIndex >( 2, 0 );

	MOAIMeshPrimReader coordReader;
	
//	if ( coordReader.Init ( *self, vertexBufferIndex )) {
//
//		MOAIMeshSparseQuadTree* quadTree = new MOAIMeshSparseQuadTree ();
//		quadTree->Init ( coordReader, targetPrimsPerNode );
//		self->mPartition = quadTree;
//	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_buildTernaryTree ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	u32 axisMask				= state.GetValue < u32 >( 2, MOAIMeshTernaryTree::AXIS_MASK_ALL );
	u32 targetPrimsPerNode		= state.GetValue < u32 >( 3, MOAIMeshTernaryTree::DEFAULT_TARGET_PRIMS_PER_NODE );
	ZLIndex vertexBufferIndex	= state.GetValue < MOAILuaIndex >( 4, 0 );
	
	MOAIMeshPrimReader coordReader;
	
//	if ( coordReader.Init ( *self, vertexBufferIndex )) {
//	
//		MOAIMeshTernaryTree* ternaryTree = new MOAIMeshTernaryTree ();
//		ternaryTree->Init ( coordReader, targetPrimsPerNode, axisMask );
//		self->mPartition = ternaryTree;
//	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIMesh::_getPrimsForPoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	// TODO: this is a naive first pass. need to use the partition if one has been created.
	// TODO: handle all prim types or bail if not triangles
	
	MOAIMeshPrimReader primReader;
	
	bool is3D = state.CheckParams ( 2, "NNN", false );
	
	ZLVec3D point = state.GetValue < ZLVec3D >( 2, ZLVec3D::ORIGIN );

	u32 totalPrims = 0;

//	ZLBox meshBounds = self->GetBounds ().mAABB;
//	if ((( is3D ) && meshBounds.Contains ( point )) || meshBounds.Contains ( point, ZLBox::PLANE_XY )) {
//
//		if ( primReader.Init ( *self, 0 )) {
//
//			u32 basePrim = state.GetValue < u32 >( 5, 1 ) - 1;
//			u32 nPrims = state.GetValue < u32 >( 6, primReader.GetTotalPrims ());
//
//			for ( u32 i = basePrim; i < nPrims; ++i ) {
//
//				MOAIMeshPrimCoords prim;
//				if ( primReader.GetPrimCoords ( i, prim )) {
//
//					if ((
//						( is3D )
//						&&
//						ZLBarycentric::PointInTriangle (
//							prim.mCoords [ 0 ],
//							prim.mCoords [ 1 ],
//							prim.mCoords [ 2 ],
//							point ))
//						||
//						ZLBarycentric::PointInTriangle (
//							prim.mCoords [ 0 ].Vec2D (),
//							prim.mCoords [ 1 ].Vec2D (),
//							prim.mCoords [ 2 ].Vec2D (),
//							point.Vec2D ())
//						) {
//
//						state.Push ( i + 1 );
//						totalPrims++;
//					}
//				}
//			}
//		}
//	}
	return totalPrims;
}

//----------------------------------------------------------------//
int MOAIMesh::_getRegionForPrim ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )

	// TODO: non-triangle meshes? need to support or error out

	u32 basePrimID = state.GetValue < u32 >( 2, 1 ) - 1;
	u32 nPrims = state.GetValue < u32 >( 3, 1 );

//	SafeTesselator tesselator;
//
//	MOAIMeshPrimReader primReader;
//
//	if ( primReader.Init ( *self, 0 )) {
//
//		for ( u32 i = 0; i < nPrims; ++i ) {
//
//			MOAIMeshPrimCoords prim;
//			if ( primReader.GetPrimCoords ( basePrimID + i, prim )) {
//
//				assert ( prim.mPrimSize == 3 );
//
//				ZLVec2D triangle [ 3 ];
//
//				triangle [ 0 ] = prim.mCoords [ 0 ].Vec2D ();
//				triangle [ 1 ] = prim.mCoords [ 1 ].Vec2D ();
//				triangle [ 2 ] = prim.mCoords [ 2 ].Vec2D ();
//
//				tesselator.AddContour2D ( triangle, 3 );
//			}
//		}
//	}
//
//	tesselator.Tesselate ( TESS_WINDING_NONZERO, TESS_BOUNDARY_CONTOURS, 0, 0 );
//
//	MOAIRegion* region = state.GetLuaObject < MOAIRegion >( 4, false );
//	region = region ? region : new MOAIRegion ();
//
//	region->Copy ( tesselator );
//
//	state.Push ( region );
//	return 1;

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_intersectRay ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	// TODO: this is a naive first pass. need to use the partition if one has been created.
	// TODO: handle all prim types or bail if not triangles
	
//	MOAIMeshPrimReader primReader;
//
//	ZLVec3D loc		= state.GetValue < ZLVec3D >( 2, ZLVec3D::ORIGIN );
//	ZLVec3D vec		= state.GetValue < ZLVec3D >( 5, ZLVec3D::ORIGIN );
//
//	bool hasHit = false;
//	float bestTime = 0.0f;
//	ZLVec3D bestHit;
//
//	if ( primReader.Init ( *self, 0 )) {
//
//		u32 totalMeshPrims = primReader.GetTotalPrims ();
//
//		for ( u32 i = 0; i < totalMeshPrims; ++i ) {
//
//			MOAIMeshPrimCoords prim;
//			if ( primReader.GetPrimCoords ( i, prim )) {
//
//				float time;
//				ZLVec3D hit;
//
//				if ( ZLSect::VecToTriangle ( loc, vec, prim.mCoords [ 0 ], prim.mCoords [ 1 ], prim.mCoords [ 2 ], time, hit ) == ZLSect::SECT_HIT ) {
//
//					if (( !hasHit ) || ( time < bestTime )) {
//						bestTime = time;
//						bestHit = hit;
//						hasHit = true;
//					}
//				}
//			}
//		}
//	}
//
//	if ( hasHit ) {
//		state.Push ( bestTime );
//		state.Push ( bestHit );
//		return 4;
//	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_printPartition ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )

	if ( self->mPartition ) {
		self->mPartition->Print ();
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_readPrimCoords ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )

//	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
//
//	if ( stream ) {
//
//		u32 basePrimID = state.GetValue < u32 >( 3, 1 ) - 1;
//		u32 nPrims = state.GetValue < u32 >( 4, 1 );
//
//		MOAIMeshPrimReader primReader;
//
//		if ( primReader.Init ( *self, 0 )) {
//
//			for ( u32 i = 0; i < nPrims; ++i ) {
//
//				MOAIMeshPrimCoords prim;
//				if ( primReader.GetPrimCoords ( basePrimID + i, prim )) {
//
//					stream->Write < ZLVec3D >( prim.mCoords [ 0 ]);
//
//					if ( prim.mPrimSize > 1 ) {
//
//						stream->Write < ZLVec3D >( prim.mCoords [ 1 ]);
//
//						if ( prim.mPrimSize > 2 ) {
//
//							stream->Write < ZLVec3D >( prim.mCoords [ 2 ]);
//						}
//					}
//				}
//			}
//		}
//	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_setBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	self->ClearBounds ();
	
	if ( state.CheckParams ( 2, "NNNNNN-" )) {
		self->SetBounds ( state.GetValue < ZLBox >( 2, ZLBox::EMPTY ));
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPenWidth
	@text	Sets the pen with for drawing prims in this vertex buffer.
			Only valid with prim types GL_LINES, GL_LINE_LOOP, GL_LINE_STRIP.
	
	@in		MOAIMesh self
	@in		number penWidth
	@out	nil
*/
int MOAIMesh::_setPenWidth ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "UN" )
	
	self->mPenWidth = state.GetValue < float >( 2, 1.0f );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPrimType
	@text	Sets the prim type the buffer represents.
	
	@in		MOAIMesh self
	@in		number primType		One of MOAIMesh GL_POINTS, GL_LINES, GL_TRIANGLES, GL_LINE_LOOP,
								GL_LINE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLE_STRIP
	@out	nil
*/
int MOAIMesh::_setPrimType ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "UN" )
	
	self->SetPrimType (( MOAIGfxTopologyEnum::_ )state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------///
// TODO: doxygen
int MOAIMesh::_setTotalElements ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )

	self->SetTotalElements ( state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//================================================================//
// MOAIMesh
//================================================================//

//----------------------------------------------------------------//
void MOAIMesh::ClearBounds () {

	this->mBounds = ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
u32 MOAIMesh::CountPrims () const {

	return 0;
}

//----------------------------------------------------------------//
void MOAIMesh::DrawIndex ( ZLIndex idx, MOAIMeshSpan* span ) {
	UNUSED ( idx );

	MOAIAbstractGfxScript* gfxScript = this->GetGfxScript ();
	if ( !gfxScript ) return;

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	gfxMgr.SetShader ( MOAIShaderPresetEnum::MESH_SHADER );

	MOAIMeshSpan defaultSpan;
	if ( !span ) {
		defaultSpan.mBase = 0;
		defaultSpan.mTop = this->mTotalElements;
		defaultSpan.mNext = 0;
		span = &defaultSpan;
	}
	
	MOAIMeshCallable callable;
	callable.mPrimType 			= this->mPrimType;
	callable.mSpan	 			= span;
	callable.mTotalElements		= this->mTotalElements;
	gfxScript->RunScript ( &callable, MOAIGfxScriptRetained::CALL_FROM_SHADER );
}

//----------------------------------------------------------------//
MOAIMesh::MOAIMesh () :
	mTotalElements ( 0 ),
	mPrimType ( MOAIGfxTopologyEnum::TRIANGLE_LIST ),
	mPenWidth ( 1.0f ),
	mPartition ( 0 ) {

	RTTI_BEGIN ( MOAIMesh )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIMesh >)
		RTTI_VISITOR ( MOAIAbstractLuaSerializationVisitor, MOAILuaSerializationVisitor < MOAIMesh >)
		RTTI_EXTEND ( MOAIDeck )
		RTTI_EXTEND ( MOAIHasGfxScript )
	RTTI_END

	this->ClearBounds ();
}

//----------------------------------------------------------------//
MOAIMesh::~MOAIMesh () {
}

//----------------------------------------------------------------//
void MOAIMesh::SetBounds ( const ZLBox& aabb ) {

	this->mBounds.Init ( aabb );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIMesh::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
	
	state.SetField ( -1, "GL_LINES",			( u32 )MOAIGfxTopologyEnum::LINE_LIST );
	state.SetField ( -1, "GL_LINE_LOOP",		( u32 )MOAIGfxTopologyEnum::LINE_LOOP );
	state.SetField ( -1, "GL_LINE_STRIP",		( u32 )MOAIGfxTopologyEnum::LINE_STRIP );
	state.SetField ( -1, "GL_POINTS",			( u32 )MOAIGfxTopologyEnum::POINT_LIST );
	state.SetField ( -1, "GL_TRIANGLE_FAN",		( u32 )MOAIGfxTopologyEnum::TRIANGLE_FAN );
	state.SetField ( -1, "GL_TRIANGLES",		( u32 )MOAIGfxTopologyEnum::TRIANGLE_LIST );
	state.SetField ( -1, "GL_TRIANGLE_STRIP",	( u32 )MOAIGfxTopologyEnum::TRIANGLE_STRIP );
	
	state.SetField ( -1, "X_AXIS_MASK",			( u32 )MOAIMeshTernaryTree::X_AXIS_MASK );
	state.SetField ( -1, "Y_AXIS_MASK",			( u32 )MOAIMeshTernaryTree::Y_AXIS_MASK );
	state.SetField ( -1, "Z_AXIS_MASK",			( u32 )MOAIMeshTernaryTree::Z_AXIS_MASK );
	state.SetField ( -1, "AXIS_MASK_ALL",		( u32 )MOAIMeshTernaryTree::AXIS_MASK_ALL );
}

//----------------------------------------------------------------//
void MOAIMesh::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "buildQuadTree",				_buildQuadTree },
		{ "buildTernaryTree",			_buildTernaryTree },
		{ "getPrimsForPoint",			_getPrimsForPoint },
		{ "getRegionForPrim",			_getRegionForPrim },
		{ "intersectRay",				_intersectRay },
		{ "printPartition",				_printPartition },
		{ "readPrimCoords",				_readPrimCoords },
		{ "setBounds",					_setBounds },
		{ "setIndexBuffer",				_setIndexBuffer },
		{ "setPenWidth",				_setPenWidth },
		{ "setPrimType",				_setPrimType },
		{ "setTotalElements",			_setTotalElements },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIMesh::_SerializeIn ( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer ) {
	if ( history.DidVisit ( *this )) return;

//	this->SetIndexBuffer (
//		serializer.MemberIDToObject < MOAIIndexBuffer >( state.GetFieldValue < cc8*, MOAISerializer::ObjID >( -1, "mIndexBuffer", 0 ))
//	);
	
	this->mTotalElements = state.GetFieldValue < cc8*, u32 >( -1, "mTotalElements", 0 );
	
	if ( state.PushFieldWithType ( -1, "mBounds", LUA_TTABLE )) {
		
		ZLBox aabb;
		
		aabb.mMin.mX	= state.GetFieldValue < cc8*, float >( -1, "mMinX", 0 );
		aabb.mMin.mY	= state.GetFieldValue < cc8*, float >( -1, "mMinY", 0 );
		aabb.mMin.mZ	= state.GetFieldValue < cc8*, float >( -1, "mMinZ", 0 );
		
		aabb.mMax.mX	= state.GetFieldValue < cc8*, float >( -1, "mMaxX", 0 );
		aabb.mMax.mY	= state.GetFieldValue < cc8*, float >( -1, "mMaxY", 0 );
		aabb.mMax.mZ	= state.GetFieldValue < cc8*, float >( -1, "mMaxZ", 0 );
		
		this->mBounds.Init ( aabb );
		
		this->mBounds.UpdateStatus ();
		
		state.Pop ();
	}
	
	this->mPenWidth = state.GetFieldValue < cc8*, float >( -1, "mPenWidth", 0 );
}

//----------------------------------------------------------------//
void MOAIMesh::_SerializeOut ( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer ) {
	if ( history.DidVisit ( *this )) return;

//	state.SetField ( -1, "mIndexBuffer", serializer.AffirmMemberID ( this->mIndexBuffer ));
	
	state.SetField < cc8*, u32 >( -1, "mTotalElements", this->mTotalElements );
	
	lua_newtable ( state );
	
		state.SetField < cc8*, float >( -1, "mMinX", this->mBounds.mAABB.mMin.mX );
		state.SetField < cc8*, float >( -1, "mMinY", this->mBounds.mAABB.mMin.mY );
		state.SetField < cc8*, float >( -1, "mMinZ", this->mBounds.mAABB.mMin.mZ );
	
		state.SetField < cc8*, float >( -1, "mMaxX", this->mBounds.mAABB.mMax.mX );
		state.SetField < cc8*, float >( -1, "mMaxY", this->mBounds.mAABB.mMax.mY );
		state.SetField < cc8*, float >( -1, "mMaxZ", this->mBounds.mAABB.mMax.mZ );
	
	lua_setfield ( state, -2, "mBounds" );
	
	state.SetField < cc8*, u32 >( -1, "mPenWidth", ( u32 )this->mPenWidth );
}

//----------------------------------------------------------------//
ZLBounds MOAIMesh::MOAIDeck_ComputeMaxAABB () {

	return this->mBounds;
}

//----------------------------------------------------------------//
void MOAIMesh::MOAIDeck_Draw ( ZLIndex idx ) {

	this->DrawIndex ( idx, 0 );
}

//----------------------------------------------------------------//
ZLBounds MOAIMesh::MOAIDeck_GetBounds ( ZLIndex idx ) {
	UNUSED ( idx );

	return this->mBounds;
}
