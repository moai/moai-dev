// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLLISIONSHAPE_H
#define	MOAICOLLISIONSHAPE_H

#include <moai-sim/MOAIOverlap.h>

class MOAITransformBase;

//================================================================//
// MOAICollisionShape
//================================================================//
class MOAICollisionShape :
	public ZLRefCountedObject {
private:

	ZLBounds		mBounds;
	
	ZLLeanArray < MOAIOverlapShape* >	mShapes;

	//----------------------------------------------------------------//
	void			Bless					();

public:
	
	GET ( ZLBounds&, Bounds, mBounds )
	
	//----------------------------------------------------------------//
	void			Clear					();
	void			Draw					( const ZLAffine3D& localToWorldMtx );
					MOAICollisionShape		();
					~MOAICollisionShape		();
	bool			Overlap					( const ZLBox& otherBounds, const MOAITransformBase& selfTransform, const MOAITransformBase& otherTransform, ZLBox& bounds ) const;
	bool			Overlap					( const MOAICollisionShape& otherShape, const MOAITransformBase& selfTransform, const MOAITransformBase& otherTransform, ZLBox& bounds ) const;
	void			ReserveShapes			( u32 totalShapes );
	void			Set						( u32 idx, const ZLBox& box );
	void			Set						( u32 idx, const ZLQuad& quad );
	void			Set						( u32 idx, const ZLRect& rect );
};

#endif
