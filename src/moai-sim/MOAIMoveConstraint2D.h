// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMOVECONSTRAINT2D_H
#define	MOAIMOVECONSTRAINT2D_H

#include <moai-sim/MOAICollisionPrim.h>

//================================================================//
// MOAIMoveConstraint2D
//================================================================//
class MOAIMoveConstraint2D {
public:

	enum {
		LEAVING,
		CROSSING,
		PARALLEL,
		CORNER,
	};

	u32			mType;
	ZLVec2D		mPoint;
	ZLVec2D		mTangent;
	ZLVec2D		mNormal;
	
	ZLVec2D		mEdgeNormal;
	ZLVec2D		mCornerTangent;
	
	float		mNegD; // the negative distance the point can travel along the tangent before reach the end
	float		mPosD; // the positive distance the point can travel along the tangent before reach the end
};

//================================================================//
// MOAIMoveConstraintAccumulator2D
//================================================================//
class MOAIMoveConstraintAccumulator2D :
	public MOAICollisionPrimVisitor {
private:

	MOAIMoveConstraint2D*	mContacts;
	u32						mTop;
	u32						mMax;

	//----------------------------------------------------------------//
	void		FindConstraints						( const ZLVec2D* poly0, size_t nPoly0, const ZLVec2D* poly1, size_t nPoly1 );
	void		Push								( const ZLVec2D& point, const ZLVec2D& tangent, const ZLVec2D& normal, ZLVec2D v0, ZLVec2D v1, u32 type );
	void		PushCorner							( const ZLVec2D& point, const ZLVec2D& tangent, const ZLVec2D& normal, const ZLVec2D& v0, const ZLVec2D& v1, const ZLVec2D& edgeNormal, const ZLVec2D& cornerTangent );

	//----------------------------------------------------------------//
	void		MOAICollisionPrimVisitor_Process			( const MOAICollisionPrim& shape0, const MOAICollisionPrim& shape1, const ZLAffine3D& t0, const ZLAffine3D& t1 );




public:

	//----------------------------------------------------------------//
	void		FindConstraints						( const ZLQuad& p0, const ZLQuad& p1, const ZLAffine3D& t0, const ZLAffine3D& t1 );
				MOAIMoveConstraintAccumulator2D		( MOAIMoveConstraint2D* contacts, u32 max );
	void		Reset								();
	u32			Top									();
};

#endif
