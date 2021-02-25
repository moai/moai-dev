// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIMeshPrimReader.h>
#include <moai-gfx/MOAIIndexBuffer.h>
#include <moai-gfx/MOAIVertexBuffer.h>
#include <moai-gfx/MOAIVertexFormat.h>

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
