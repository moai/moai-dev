// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLLISIONSHAPE_H
#define	MOAICOLLISIONSHAPE_H

#include <moai-sim/MOAIOverlap.h>

class MOAIOverlapHandler;
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
	void			FindOverlaps			( const ZLBox& otherBounds, MOAIOverlapHandler& handler ) const;
	void			FindOverlaps			( const MOAICollisionShape& otherShape, MOAIOverlapHandler& handler ) const;
					MOAICollisionShape		();
					~MOAICollisionShape		();
	void			ReserveShapes			( u32 totalShapes );
	void			Set						( u32 idx, const ZLBox& box );
	void			Set						( u32 idx, const ZLQuad& quad );
	void			Set						( u32 idx, const ZLRect& rect );
};

#endif
