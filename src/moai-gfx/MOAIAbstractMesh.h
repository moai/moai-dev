// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTMESH_H
#define	MOAIABSTRACTMESH_H

#include <moai-gfx/MOAIGfxConsts.h>
#include <moai-gfx/MOAIGfxResource.h>

class MOAIIndexBuffer;
class MOAIVertexBuffer;
class MOAIVertexFormat;

//================================================================//
// MOAIAbstractMesh
//================================================================//
class MOAIAbstractMesh :
	public virtual MOAIGfxResource {
protected:

	//----------------------------------------------------------------//
	virtual void					MOAIAbstractMesh_DrawPrims				( MOAIGfxTopologyEnum::_ primType, u32 base, u32 count ) = 0;
	virtual MOAIIndexBuffer*		MOAIAbstractMesh_GetIndexBuffer			() = 0;
	virtual MOAIVertexBuffer*		MOAIAbstractMesh_GetVertexBuffer		( ZLIndex idx ) = 0;
	virtual MOAIVertexFormat*		MOAIAbstractMesh_GetVertexFormat		( ZLIndex idx ) = 0;
	virtual void					MOAIAbstractMesh_ReserveVertexBuffers	( ZLSize size ) = 0;
	virtual void					MOAIAbstractMesh_SetIndexBuffer			( MOAIIndexBuffer* indexBuffer ) = 0;
	virtual void					MOAIAbstractMesh_SetVertexBuffer		( ZLIndex idx, MOAIVertexBuffer* vtxBuffer, MOAIVertexFormat* vtxFormat ) = 0;

public:

	//----------------------------------------------------------------//
	void					DrawPrims					( MOAIGfxTopologyEnum::_ primType, u32 base, u32 count );
	MOAIIndexBuffer*		GetIndexBuffer				();
	MOAIVertexBuffer*		GetVertexBuffer				( ZLIndex idx = 0 );
	MOAIVertexFormat*		GetVertexFormat				( ZLIndex idx = 0 );
							MOAIAbstractMesh			();
							~MOAIAbstractMesh			();
	void					ReserveVertexBuffers		( ZLSize size );
	void					SetIndexBuffer				( MOAIIndexBuffer* indexBuffer );
	void					SetVertexBuffer				( ZLIndex idx, MOAIVertexBuffer* vtxBuffer, MOAIVertexFormat* vtxFormat );
};

#endif
