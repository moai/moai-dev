// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLLISIONSHAPE_H
#define	MOAICOLLISIONSHAPE_H

class MOAITransformBase;

//================================================================//
// MOAICollisionShape
//================================================================//
class MOAICollisionShape {
private:

	// the intent is to determing a fine-gained overlap
	// geometry is given in *local* space
	// to overlap, 'self' and 'other' transforms must be provided
	// it's up to each overlap implementation to do the math in whatever space is appropriate

	enum {
		AABB,
		BOX,
		CONVEX_POLYGON,
		POLYGON,
		QUAD,
		NONE,
	};

	ZLRect	mAABB;
	ZLQuad	mQuad;
	ZLBox	mBox;

	u32		mType;

	//----------------------------------------------------------------//
	static bool		Overlap					( const MOAITransformBase& t0, const ZLQuad& q0, const MOAITransformBase& t1, const ZLQuad& q1, ZLBox& bounds );

public:
	
	//----------------------------------------------------------------//
					MOAICollisionShape		();
					~MOAICollisionShape		();
	bool			Overlap					( const MOAITransformBase& selfTransform, const MOAITransformBase& otherTransform, const MOAICollisionShape& otherShape, ZLBox& bounds ) const;
	void			Set						();
	void			Set						( const ZLRect& aabb );
	void			Set						( const ZLBox& box );
	void			Set						( const ZLQuad& quad );
};

#endif
