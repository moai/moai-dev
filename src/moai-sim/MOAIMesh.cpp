// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <tesselator.h>

#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGfxResourceClerk.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIMesh.h>
#include <moai-sim/MOAIMeshSparseQuadTree.h>
#include <moai-sim/MOAIMeshTernaryTree.h>
#include <moai-sim/MOAIProp.h>
#include <moai-sim/MOAIRegion.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAIVectorUtil.h>
#include <moai-sim/MOAIVertexFormat.h>

//================================================================//
// MOAIMeshPrim
//================================================================//
	
//----------------------------------------------------------------//
ZLBox MOAIMeshPrimCoords::GetBounds () {

	ZLBox bounds;
	bounds.Init ( this->mCoords [ 0 ]);
	
	if ( this->mPrimSize > 1 ) {
		bounds.Grow ( this->mCoords [ 1 ]);
	
		if ( this->mPrimSize > 2 ) {
			bounds.Grow ( this->mCoords [ 2 ]);
		}
	}
	return bounds;
}

//================================================================//
// MOAIMeshPrimReader
//================================================================//

//----------------------------------------------------------------//
bool MOAIMeshPrimReader::GetPrimCoords ( u32 idx, MOAIMeshPrimCoords& prim ) const {

	assert ( this->mMesh && this->mVertexFormat && this->mVertexBuffer );
	
	prim.mIndex = idx;
	
	switch ( this->mMesh->mPrimType ) {
		
		case ZGL_PRIM_POINTS: {
		
			prim.mPrimSize = 1;
			prim.mCoords [ 0 ] = this->ReadCoord ( idx );
			return true;
		}
		
		case ZGL_PRIM_LINES:
		
			idx = idx * 2;

		case ZGL_PRIM_LINE_LOOP:
		case ZGL_PRIM_LINE_STRIP: {
		
			prim.mPrimSize = 2;
			
			prim.mCoords [ 0 ] = this->ReadCoord ( idx++ );
			prim.mCoords [ 1 ] = this->ReadCoord ( idx );
			return true;
		}
		
		case ZGL_PRIM_TRIANGLES: {
		
			prim.mPrimSize = 3;
		
			idx = idx * 3;
			
			prim.mCoords [ 0 ] = this->ReadCoord ( idx++ );
			prim.mCoords [ 1 ] = this->ReadCoord ( idx++ );
			prim.mCoords [ 2 ] = this->ReadCoord ( idx );
			return true;
		}
		
		case ZGL_PRIM_TRIANGLE_FAN: {
		
			prim.mPrimSize = 3;
		
			prim.mCoords [ 0 ] = this->ReadCoord ( 0 );
		
			idx = idx + 1;
		
			prim.mCoords [ 1 ] = this->ReadCoord ( idx++ );
			prim.mCoords [ 2 ] = this->ReadCoord ( idx );
			return true;
		}
		
		case ZGL_PRIM_TRIANGLE_STRIP: {
		
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
bool MOAIMeshPrimReader::Init ( MOAIMesh& mesh, u32 vertexBufferIndex ) {

	MOAIVertexFormat* vertexFormat = mesh.GetVertexFormat ( vertexBufferIndex );
	MOAIVertexBuffer* vertexBuffer = mesh.GetVertexBuffer ( vertexBufferIndex );

	if ( !vertexFormat && vertexBuffer ) return false;

	if ( !vertexFormat->CountAttributesByUse ( MOAIVertexFormat::ATTRIBUTE_COORD )) return false;

	this->mMesh				= &mesh;
	this->mVertexFormat		= vertexFormat;
	
	this->mAttribute		= &vertexFormat->GetAttributeByUse ( MOAIVertexFormat::ATTRIBUTE_COORD, 0 );
	this->mVertexBuffer		= vertexBuffer->ZLCopyOnWrite::GetBuffer ();
	this->mIndexBuffer		= mesh.mIndexBuffer;
	
	this->mTotalPrims = 0;
	
	switch ( this->mMesh->mPrimType ) {
		
		case ZGL_PRIM_POINTS:
		case ZGL_PRIM_LINE_LOOP:
			this->mTotalPrims = mesh.mTotalElements;
			break;
		
		case ZGL_PRIM_LINES:
			this->mTotalPrims = mesh.mTotalElements / 2;
			break;
		
		case ZGL_PRIM_LINE_STRIP:
			this->mTotalPrims = mesh.mTotalElements - 1;
			break;
		
		case ZGL_PRIM_TRIANGLES:
			this->mTotalPrims = mesh.mTotalElements / 3;
			break;
		
		case ZGL_PRIM_TRIANGLE_FAN:
		case ZGL_PRIM_TRIANGLE_STRIP:
			this->mTotalPrims = mesh.mTotalElements - 2;
			break;
	}
	
	return true;
}

//----------------------------------------------------------------//
ZLVec3D MOAIMeshPrimReader::ReadCoord ( u32 idx ) const {

	assert ( this->mMesh && this->mVertexFormat && this->mAttribute && this->mVertexBuffer );

	idx %= this->mMesh->mTotalElements;

	if ( this->mIndexBuffer ) {
		idx = this->mIndexBuffer->GetIndex ( idx );
	}
	
	const void* packedCoord = this->mVertexFormat->GetAttributeAddress ( *this->mAttribute, this->mVertexBuffer, idx );
	ZLVec4D coord = this->mVertexFormat->UnpackCoord ( packedCoord, *this->mAttribute );
	
	return ZLVec3D ( coord.mX, coord.mY, coord.mZ );
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_buildQuadTree ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )

	u32 targetPrimsPerNode		= state.GetValue < u32 >( 1, MOAIMeshSparseQuadTree::DEFAULT_TARGET_PRIMS_PER_NODE );
	u32 vertexBufferIndex		= state.GetValue < u32 >( 2, 1 ) - 1;

	MOAIMeshPrimReader coordReader;
	
	if ( coordReader.Init ( *self, vertexBufferIndex )) {
	
		MOAIMeshSparseQuadTree* quadTree = new MOAIMeshSparseQuadTree ();
		quadTree->Init ( coordReader, targetPrimsPerNode );
		self->mPartition = quadTree;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_buildTernaryTree ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	u32 axisMask				= state.GetValue < u32 >( 2, MOAIMeshTernaryTree::AXIS_MASK_ALL );
	u32 targetPrimsPerNode		= state.GetValue < u32 >( 3, MOAIMeshTernaryTree::DEFAULT_TARGET_PRIMS_PER_NODE );
	u32 vertexBufferIndex		= state.GetValue < u32 >( 4, 1 ) - 1;
	
	MOAIMeshPrimReader coordReader;
	
	if ( coordReader.Init ( *self, vertexBufferIndex )) {
	
		MOAIMeshTernaryTree* ternaryTree = new MOAIMeshTernaryTree ();
		ternaryTree->Init ( coordReader, targetPrimsPerNode, axisMask );
		self->mPartition = ternaryTree;
	}
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

	ZLBox meshBounds = self->GetBounds ();
	if ((( is3D ) && meshBounds.Contains ( point )) || meshBounds.Contains ( point, ZLBox::PLANE_XY )) {
		
		if ( primReader.Init ( *self, 0 )) {
			
			u32 basePrim = state.GetValue < u32 >( 5, 1 ) - 1;
			u32 nPrims = state.GetValue < u32 >( 6, primReader.GetTotalPrims ());
			
			for ( u32 i = basePrim; i < nPrims; ++i ) {
				
				MOAIMeshPrimCoords prim;
				if ( primReader.GetPrimCoords ( i, prim )) {
					
					if ((
						( is3D )
						&&
						ZLBarycentric::PointInTriangle (
							prim.mCoords [ 0 ],
							prim.mCoords [ 1 ],
							prim.mCoords [ 2 ],
							point ))
						||
						ZLBarycentric::PointInTriangle (
							prim.mCoords [ 0 ].Vec2D (),
							prim.mCoords [ 1 ].Vec2D (),
							prim.mCoords [ 2 ].Vec2D (),
							point.Vec2D ())
						) {
					
						state.Push ( i + 1 );
						totalPrims++;
					}
				}
			}
		}
	}
	return totalPrims;
}

//----------------------------------------------------------------//
int MOAIMesh::_getRegionForPrim ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )

	// TODO: non-triangle meshes? need to support or error out

	u32 basePrimID = state.GetValue < u32 >( 2, 1 ) - 1;
	u32 nPrims = state.GetValue < u32 >( 3, 1 );

	SafeTesselator tesselator;

	MOAIMeshPrimReader primReader;
	
	if ( primReader.Init ( *self, 0 )) {
		
		for ( u32 i = 0; i < nPrims; ++i ) {
		
			MOAIMeshPrimCoords prim;
			if ( primReader.GetPrimCoords ( basePrimID + i, prim )) {
				
				assert ( prim.mPrimSize == 3 );
				
				ZLVec2D triangle [ 3 ];
				
				triangle [ 0 ] = prim.mCoords [ 0 ].Vec2D ();
				triangle [ 1 ] = prim.mCoords [ 1 ].Vec2D ();
				triangle [ 2 ] = prim.mCoords [ 2 ].Vec2D ();
				
				tesselator.AddContour ( 2, triangle, sizeof ( ZLVec2D ), 3 );
			}
		}
	}
	
	tesselator.Tesselate ( TESS_WINDING_NONZERO, TESS_BOUNDARY_CONTOURS, 0, 0 );

	MOAIRegion* region = state.GetLuaObject < MOAIRegion >( 4, false );
	region = region ? region : new MOAIRegion ();

	region->Copy ( tesselator );

	state.Push ( region );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_intersectRay ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	// TODO: this is a naive first pass. need to use the partition if one has been created.
	// TODO: handle all prim types or bail if not triangles
	
	MOAIMeshPrimReader primReader;
	
	ZLVec3D loc		= state.GetValue < ZLVec3D >( 2, ZLVec3D::ORIGIN );
	ZLVec3D vec		= state.GetValue < ZLVec3D >( 5, ZLVec3D::ORIGIN );
	
	bool hasHit = false;
	float bestTime = 0.0f;
	ZLVec3D bestHit;
	
	if ( primReader.Init ( *self, 0 )) {
	
		u32 totalMeshPrims = primReader.GetTotalPrims ();
		
		for ( u32 i = 0; i < totalMeshPrims; ++i ) {
		
			MOAIMeshPrimCoords prim;
			if ( primReader.GetPrimCoords ( i, prim )) {
				
				float time;
				ZLVec3D hit;
				
				if ( ZLSect::VecToTriangle ( loc, vec, prim.mCoords [ 0 ], prim.mCoords [ 1 ], prim.mCoords [ 2 ], time, hit ) == ZLSect::SECT_HIT ) {
				
					if (( !hasHit ) || ( time < bestTime )) {
						bestTime = time;
						bestHit = hit;
						hasHit = true;
					}
				}
			}
		}
	}
	
	if ( hasHit ) {
		state.Push ( bestTime );
		state.Push ( bestHit );
		return 4;
	}
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

	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	
	if ( stream ) {
	
		u32 basePrimID = state.GetValue < u32 >( 3, 1 ) - 1;
		u32 nPrims = state.GetValue < u32 >( 4, 1 );

		MOAIMeshPrimReader primReader;
		
		if ( primReader.Init ( *self, 0 )) {
			
			for ( u32 i = 0; i < nPrims; ++i ) {
			
				MOAIMeshPrimCoords prim;
				if ( primReader.GetPrimCoords ( basePrimID + i, prim )) {
					
					stream->Write < ZLVec3D >( prim.mCoords [ 0 ]);
					
					if ( prim.mPrimSize > 1 ) {
					
						stream->Write < ZLVec3D >( prim.mCoords [ 1 ]);
						
						if ( prim.mPrimSize > 2 ) {
						
							stream->Write < ZLVec3D >( prim.mCoords [ 2 ]);
						}
					}
				}
			}
		}
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_setBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	self->ClearBounds ();
	
	if ( state.CheckParams ( 2, "NNNNNN-" )) {
		self->SetBounds ( state.GetValue < ZLBox >( 2, self->mBounds ));
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setIndexBuffer
	@text	Set the index buffer to render.
	
	@in		MOAIMesh self
	@in		MOAIGfxBuffer indexBuffer
	@out	nil
*/
int MOAIMesh::_setIndexBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	self->SetIndexBuffer ( state.GetLuaObject < MOAIIndexBuffer >( 2, true ));
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
	
	self->SetPrimType ( state.GetValue < u32 >( 2, 0 ));
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

	this->mHasBounds = false;
	this->mBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
u32 MOAIMesh::CountPrims () const {

	return 0;
}

//----------------------------------------------------------------//
ZLBox MOAIMesh::ComputeMaxBounds () {
	return this->GetItemBounds ( 0 );
}

//----------------------------------------------------------------//
void MOAIMesh::DrawIndex ( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale ) {

	this->DrawIndex ( idx, 0, materials, offset, scale );
}

//----------------------------------------------------------------//
void MOAIMesh::DrawIndex ( u32 idx, MOAIMeshSpan* span, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale ) {
	UNUSED ( offset );
	UNUSED ( scale );

	if ( !materials.LoadGfxState ( this, idx, MOAIShaderMgr::MESH_SHADER )) return;

	// TODO: make use of offset and scale

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	if ( gfxMgr.mGfxState.BindVertexArray ( this )) {

		ZLGfx& gfx = gfxMgr.GetDrawingAPI ();

		// I am super lazy, so set this up here instead of adding if's below
		MOAIMeshSpan defaultSpan;
		if ( !span ) {
			defaultSpan.mBase = 0;
			defaultSpan.mTop = this->mTotalElements;
			defaultSpan.mNext = 0;
			span = &defaultSpan;
		}
		
		gfxMgr.mGfxState.SetPenWidth ( this->mPenWidth );
		
		gfxMgr.mGfxState.UpdateAndBindUniforms ();
		
		if ( this->mIndexBuffer ) {
			
			// TODO: turns out we can bind this inside the VAO as well. so there.
			if ( gfxMgr.mGfxState.BindIndexBuffer ( this->mIndexBuffer )) {
			
				size_t indexSizeInBytes = this->mIndexBuffer->GetIndexSize ();
				
				for ( ; span; span = span->mNext ) {
				
					if ( span->mBase == span->mTop ) continue;
					assert (( span->mBase < span->mTop ) && ( span->mTop <= this->mTotalElements ));
				
					gfx.DrawElements (
						this->mPrimType,
						( u32 )( span->mTop - span->mBase ),
						indexSizeInBytes == 2 ? ZGL_TYPE_UNSIGNED_SHORT : ZGL_TYPE_UNSIGNED_INT,
						this->mIndexBuffer->GetBuffer (),
						span->mBase * indexSizeInBytes
					);
				}
				gfxMgr.mGfxState.BindIndexBuffer ();
			}
		}
		else {
		
			for ( ; span; span = span->mNext ) {
			
				if ( span->mBase == span->mTop ) continue;
				assert (( span->mBase < span->mTop ) && ( span->mTop <= this->mTotalElements ));
			
				gfx.DrawArrays ( this->mPrimType, ( u32 )span->mBase, ( u32 )( span->mTop - span->mBase ));
			}
		}
		gfxMgr.mGfxState.BindVertexArray ();
	}
}

//----------------------------------------------------------------//
ZLBox MOAIMesh::GetItemBounds ( u32 idx ) {
	UNUSED ( idx );

	return this->mBounds;
}

//----------------------------------------------------------------//
MOAIMesh::MOAIMesh () :
	mTotalElements ( 0 ),
	mPrimType ( ZGL_PRIM_TRIANGLES ),
	mPenWidth ( 1.0f ),
	mPartition ( 0 ) {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIStandardDeck )
		RTTI_EXTEND ( MOAIVertexArray )
	RTTI_END
	
	this->ClearBounds ();
}

//----------------------------------------------------------------//
MOAIMesh::~MOAIMesh () {

	this->SetIndexBuffer ( 0 );
}

//----------------------------------------------------------------//
void MOAIMesh::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIStandardDeck::RegisterLuaClass ( state );
	MOAIVertexArray::RegisterLuaClass ( state );
	
	state.SetField ( -1, "GL_POINTS",			( u32 )ZGL_PRIM_POINTS );
	state.SetField ( -1, "GL_LINES",			( u32 )ZGL_PRIM_LINES );
	state.SetField ( -1, "GL_TRIANGLES",		( u32 )ZGL_PRIM_TRIANGLES );
	state.SetField ( -1, "GL_LINE_LOOP",		( u32 )ZGL_PRIM_LINE_LOOP );
	state.SetField ( -1, "GL_LINE_STRIP",		( u32 )ZGL_PRIM_LINE_STRIP );
	state.SetField ( -1, "GL_TRIANGLE_FAN",		( u32 )ZGL_PRIM_TRIANGLE_FAN );
	state.SetField ( -1, "GL_TRIANGLE_STRIP",	( u32 )ZGL_PRIM_TRIANGLE_STRIP );
	
	state.SetField ( -1, "X_AXIS_MASK",			( u32 )MOAIMeshTernaryTree::X_AXIS_MASK );
	state.SetField ( -1, "Y_AXIS_MASK",			( u32 )MOAIMeshTernaryTree::Y_AXIS_MASK );
	state.SetField ( -1, "Z_AXIS_MASK",			( u32 )MOAIMeshTernaryTree::Z_AXIS_MASK );
	state.SetField ( -1, "AXIS_MASK_ALL",		( u32 )MOAIMeshTernaryTree::AXIS_MASK_ALL );
}

//----------------------------------------------------------------//
void MOAIMesh::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIStandardDeck::RegisterLuaFuncs ( state );
	MOAIVertexArray::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "buildQuadTree",				_buildQuadTree },
		{ "buildTernaryTree",			_buildTernaryTree },
		{ "getPrimsForPoint",			_getPrimsForPoint },
		{ "getRegionForPrim",			_getRegionForPrim },
		{ "intersectRay",				_intersectRay },
		{ "printPartition",				_printPartition },
		{ "readPrimCoords",				_readPrimCoords },
		{ "reserveVAOs",				_reserveVAOs },
		{ "reserveVertexBuffers",		_reserveVertexBuffers },
		{ "setBounds",					_setBounds },
		{ "setIndexBuffer",				_setIndexBuffer },
		{ "setPenWidth",				_setPenWidth },
		{ "setPrimType",				_setPrimType },
		{ "setTotalElements",			_setTotalElements },
		{ "setVertexBuffer",			_setVertexBuffer },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIMesh::ReserveVAOs ( u32 total ) {

	for ( size_t i = 0; i < this->mVAOs.Size (); ++i ) {
		MOAIGfxResourceClerk::DeleteOrDiscardHandle ( this->mVAOs [ i ], false );
	}
	this->mVAOs.Init ( total );
}

//----------------------------------------------------------------//
void MOAIMesh::ReserveVertexBuffers ( u32 total ) {

	for ( size_t i = 0; i < this->mVertexBuffers.Size (); ++i ) {
		this->mVertexBuffers [ i ].SetBufferAndFormat ( *this, 0, 0 );
	}
	this->mVertexBuffers.Init ( total );
}

//----------------------------------------------------------------//
void MOAIMesh::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {

	MOAIVertexArray::SerializeIn ( state, serializer );

	this->SetIndexBuffer ( serializer.MemberIDToObject < MOAIIndexBuffer >( state.GetField < MOAISerializer::ObjID >( -1, "mIndexBuffer", 0 )));
	
	this->mTotalElements = state.GetField < u32 >( -1, "mTotalElements", 0 );
	
	this->mHasBounds = state.GetField < bool >( -1, "mHasBounds", 0 );
	
	if ( state.GetFieldWithType ( -1, "mBounds", LUA_TTABLE )) {
		
		this->mBounds.mMin.mX	= state.GetField < float >( -1, "mMinX", 0 );
		this->mBounds.mMin.mY	= state.GetField < float >( -1, "mMinY", 0 );
		this->mBounds.mMin.mZ	= state.GetField < float >( -1, "mMinZ", 0 );
		
		this->mBounds.mMax.mX	= state.GetField < float >( -1, "mMaxX", 0 );
		this->mBounds.mMax.mY	= state.GetField < float >( -1, "mMaxY", 0 );
		this->mBounds.mMax.mZ	= state.GetField < float >( -1, "mMaxZ", 0 );
		
		state.Pop ();
	}
	
	this->mPenWidth = state.GetField < float >( -1, "mPenWidth", 0 );
}

//----------------------------------------------------------------//
void MOAIMesh::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

	MOAIVertexArray::SerializeOut ( state, serializer );

	state.SetField ( -1, "mIndexBuffer", serializer.AffirmMemberID ( this->mIndexBuffer ));
	
	state.SetField < u32 >( -1, "mTotalElements", this->mTotalElements );
	
	state.SetField < bool >( -1, "mHasBounds", this->mHasBounds );
	
	lua_newtable ( state );
	
		state.SetField < float >( -1, "mMinX", this->mBounds.mMin.mX );
		state.SetField < float >( -1, "mMinY", this->mBounds.mMin.mY );
		state.SetField < float >( -1, "mMinZ", this->mBounds.mMin.mZ );
	
		state.SetField < float >( -1, "mMaxX", this->mBounds.mMax.mX );
		state.SetField < float >( -1, "mMaxY", this->mBounds.mMax.mY );
		state.SetField < float >( -1, "mMaxZ", this->mBounds.mMax.mZ );
	
	lua_setfield ( state, -2, "mBounds" );
	
	state.SetField < u32 >( -1, "mPenWidth", ( u32 )this->mPenWidth );
}

//----------------------------------------------------------------//
void MOAIMesh::SetBounds ( const ZLBox& bounds ) {

	this->mBounds = bounds;
	this->mHasBounds = true;
}

//----------------------------------------------------------------//
void MOAIMesh::SetIndexBuffer ( MOAIIndexBuffer* indexBuffer ) {

	this->mIndexBuffer.Set ( *this, indexBuffer );
}
