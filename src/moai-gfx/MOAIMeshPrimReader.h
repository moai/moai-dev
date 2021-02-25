// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMESHPRIMREADER_H
#define	MOAIMESHPRIMREADER_H

#include <moai-gfx/MOAIGfxConsts.h>

class MOAIIndexBuffer;
class MOAIVertexAttribute;
class MOAIVertexBuffer;
class MOAIVertexFormat;

//================================================================//
// MOAIMeshPrimCoords
//================================================================//
class MOAIMeshPrimCoords {
public:
	
	static const u32 PRIM_POINT			= 1;
	static const u32 PRIM_LINE			= 2;
	static const u32 PRIM_TRIANGLE		= 3;
	static const u32 MAX_COORDS			= 3;
	
	u32			mIndex;
	u32			mPrimSize;
	ZLVec3D		mCoords [ MAX_COORDS ];
	
	//----------------------------------------------------------------//
	ZLBox		GetAABB		();
};

//================================================================//
// MOAIMeshPrimReader
//================================================================//
class MOAIMeshPrimReader {
private:

	friend class MOAIMeshDeck;

	MOAIVertexFormat*			mVertexFormat;
	MOAIGfxTopologyEnum::_		mPrimType;
	ZLSize						mTotalElements;
	ZLSize						mTotalPrims;
	
	const MOAIVertexAttribute*	mAttribute;
	const void*					mVertexBuffer;
	MOAIIndexBuffer*			mIndexBuffer;

	//----------------------------------------------------------------//
	bool		Init			( MOAIVertexFormat& vertexFormat, MOAIVertexBuffer& vertexBuffer, MOAIGfxTopologyEnum::_ primType, u32 totalElements, MOAIIndexBuffer* indexBuffer = NULL );
	ZLVec3D		ReadCoord		( u32 idx ) const;

public:

	GET_CONST ( ZLSize, TotalPrims, mTotalPrims )

	//----------------------------------------------------------------//
	bool		GetPrimCoords	( u32 idx, MOAIMeshPrimCoords& prim ) const;
};

#endif
