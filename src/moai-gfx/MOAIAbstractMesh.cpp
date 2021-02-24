// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractMesh.h>

//================================================================//
// MOAIAbstractMesh
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractMesh::DrawPrims ( MOAIGfxTopologyEnum::_ primType, u32 base, u32 count ) {

	this->MOAIAbstractMesh_DrawPrims ( primType, base, count );
}

//----------------------------------------------------------------//
MOAIIndexBuffer* MOAIAbstractMesh::GetIndexBuffer () {

	return this->MOAIAbstractMesh_GetIndexBuffer ();
}

//----------------------------------------------------------------//
MOAIVertexBuffer* MOAIAbstractMesh::GetVertexBuffer ( ZLIndex idx ) {

	return this->MOAIAbstractMesh_GetVertexBuffer ( idx );
}

//----------------------------------------------------------------//
MOAIVertexFormat* MOAIAbstractMesh::GetVertexFormat ( ZLIndex idx ) {

	return this->MOAIAbstractMesh_GetVertexFormat ( idx );
}

//----------------------------------------------------------------//
MOAIAbstractMesh::MOAIAbstractMesh () {
	
	RTTI_BEGIN ( MOAIAbstractMesh )
		RTTI_EXTEND ( MOAIGfxResource )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractMesh::~MOAIAbstractMesh () {
}

//----------------------------------------------------------------//
void MOAIAbstractMesh::ReserveVertexBuffers ( ZLSize size ) {

	this->MOAIAbstractMesh_ReserveVertexBuffers ( size );
}

//----------------------------------------------------------------//
void MOAIAbstractMesh::SetIndexBuffer ( MOAIIndexBuffer* indexBuffer ) {

	this->MOAIAbstractMesh_SetIndexBuffer ( indexBuffer );
}

//----------------------------------------------------------------//
void MOAIAbstractMesh::SetVertexBuffer ( ZLIndex idx, MOAIVertexBuffer* vtxBuffer, MOAIVertexFormat* vtxFormat ) {

	this->MOAIAbstractMesh_SetVertexBuffer ( idx, vtxBuffer, vtxFormat );
}
