// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORPOLY_H
#define	MOAIVECTORPOLY_H

#include <moai-sim/MOAIVectorShape.h>

//================================================================//
// MOAIVectorPoly
//================================================================//
class MOAIVectorPoly :
	public MOAIVectorShape {
protected:

	ZLLeanArray < ZLVec2D > mVertices;
	bool mIsClosed;

public:
	
	MOAI_VECTOR_SHAPE_DECL_TYPE ( MOAIVectorShape::POLY )
	
	//----------------------------------------------------------------//
	int				AddFillContours			( SafeTesselator& tess );
	int				AddStrokeContours		( SafeTesselator& tess, bool inside, bool outside );
	bool			IsClosed				();
					MOAIVectorPoly			();
					~MOAIVectorPoly			();
	void			Read					( ZLStream& stream, MOAIVectorTesselatorWriter& writer );
	bool			SetVertices				( const ZLVec2D* vertices, u32 total, bool closed );
	void			Write					( ZLStream& stream, MOAIVectorTesselatorWriter& writer ) const;
};

#endif
