// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLLISIONSHAPE_H
#define	MOAICOLLISIONSHAPE_H

class MOAIOverlap;
class MOAIOverlapResolver;
class MOAICollisionPrim;
class MOAICollisionPrimVisitor;
class MOAIMoveConstraintAccumulator2D;

//================================================================//
// MOAICollisionShape
//================================================================//
class MOAICollisionShape :
	public ZLRefCountedObject {
private:

	friend class MOAICollisionPrimVisitor;

	ZLBounds		mBounds;
	
	ZLLeanArray < MOAICollisionPrim* >	mShapes;

	//----------------------------------------------------------------//
	void			Bless					();

public:
	
	GET ( ZLBounds&, Bounds, mBounds )
	
	//----------------------------------------------------------------//
	void			Clear							();
	void			Draw							( const ZLAffine3D& localToWorldMtx );
					MOAICollisionShape				();
					~MOAICollisionShape				();
	void			Set								( ZLIndex idx, const ZLBox& box );
	void			Set								( ZLIndex idx, const ZLQuad& quad );
	void			Set								( ZLIndex idx, const ZLRect& rect );
};

#endif
