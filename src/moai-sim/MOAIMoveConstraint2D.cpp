// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIMoveConstraint2D.h>

// uncomment me to debug log
//#define MOAIMOVECONSTRAINT2D_DEBUG

#ifdef MOAIMOVECONSTRAINT2D_DEBUG
	#define DEBUG_LOG printf
#else
	#define DEBUG_LOG(...)
#endif

using namespace MOAICollisionConsts;

//================================================================//
// MOAIMoveConstraintAccumulator2D
//================================================================//

//----------------------------------------------------------------//
void MOAIMoveConstraintAccumulator2D::FindConstraints ( const ZLQuad& p0, const ZLQuad& p1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {

	ZLQuad tq0 = p0;
	tq0.Transform ( t0 );

	ZLQuad tq1 = p1;
	tq1.Transform ( t1 );

	this->FindConstraints ( tq0.mV, 4, tq1.mV, 4 );
}

//----------------------------------------------------------------//
MOAIMoveConstraintAccumulator2D::MOAIMoveConstraintAccumulator2D ( MOAIMoveConstraint2D* contacts, u32 max ) :
	mContacts ( contacts ),
	mTop ( 0 ),
	mMax ( max ) {
}

//----------------------------------------------------------------//
void MOAIMoveConstraintAccumulator2D::Push ( const ZLVec2D& point, const ZLVec2D& tangent, const ZLVec2D& normal, ZLVec2D v0, ZLVec2D v1, u32 type ) {
	
	assert ( this->mContacts );
	
	if ( this->mTop < this->mMax ) {
		
		MOAIMoveConstraint2D& constraint = this->mContacts [ this->mTop++ ];
		
		constraint.mType = type;
		constraint.mPoint = point;
		constraint.mTangent = tangent;
		constraint.mNormal = normal;
		
		v0.Sub ( point );
		v1.Sub ( point );
		
		constraint.mNegD = constraint.mTangent.Dot ( v0 );
		constraint.mPosD = constraint.mTangent.Dot ( v1 );
		
		constraint.mNegD = ABS ( constraint.mNegD );
		constraint.mPosD = ABS ( constraint.mPosD );
		
		#ifdef MOAICOLLISIONWORLD_DEBUG
		
			cc8* typeString = "";
			
			switch ( type ) {
			
				case MOAIContactPoint2D::LEAVING:
					typeString = "LEAVING";
					break;
					
				case MOAIContactPoint2D::CROSSING:
					typeString = "CROSSING";
					break;
					
				case MOAIContactPoint2D::PARALLEL:
					typeString = "PARALLEL";
					break;
					
				case MOAIContactPoint2D::CORNER:
					typeString = "CORNER";
					break;
			}
			
			printf (
				"  %s: (%g, %g) T: (%g, %g) N: (%g, %g) -%g %g\n",
				typeString,
				point.mX,
				point.mY,
				tangent.mX,
				tangent.mY,
				normal.mX,
				normal.mY,
				constraint.mNegD,
				constraint.mPosD
			);
		
		#endif
	}
}

//----------------------------------------------------------------//
void MOAIMoveConstraintAccumulator2D::PushCorner ( const ZLVec2D& point, const ZLVec2D& tangent, const ZLVec2D& normal, const ZLVec2D& v0, const ZLVec2D& v1, const ZLVec2D& edgeNormal, const ZLVec2D& cornerTangent ) {

	if ( this->mTop < this->mMax ) {
		this->Push ( point, tangent, normal, v0, v1, MOAIMoveConstraint2D::CORNER );
		this->mContacts [ this->mTop - 1 ].mEdgeNormal = edgeNormal;
		this->mContacts [ this->mTop - 1 ].mCornerTangent = cornerTangent;
	}
}

//----------------------------------------------------------------//
void MOAIMoveConstraintAccumulator2D::Reset () {

	this->mTop = 0;
}

//----------------------------------------------------------------//
u32 MOAIMoveConstraintAccumulator2D::Top () {

	return this->mTop;
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
void MOAIMoveConstraintAccumulator2D::MOAIOverlapShapeVisitor_Process ( const MOAIOverlapShape& shape0, const MOAIOverlapShape& shape1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {

	switch ( COLLISION_COMBO_CODE ( shape0.mType, shape1.mType )) {
	
		case QUAD__QUAD:			FindConstraints ( *( ZLQuad* )shape0.mPtr, *( ZLQuad* )shape1.mPtr, t0, t1 );
	}
}
