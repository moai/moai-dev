// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLLISIONWORLD_H
#define	MOAICOLLISIONWORLD_H

#include <moai-sim/MOAIAction.h>
#include <moai-sim/MOAICollisionProp.h>
#include <moai-sim/MOAIDrawable.h>
#include <moai-sim/MOAIDrawShapeRetained.h>
#include <moai-sim/MOAIPartition.h>

class MOAIPartitionHull;
class MOAICollisionWorld;

//================================================================//
// MOAIContactPoint2D
//================================================================//
class MOAIContactPoint2D {
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
	ZLVec2D		mCornerTangent;
	
	float		mNegD; // the negative distance the point can travel along the tangent before reach the end
	float		mPosD; // the positive distance the point can travel along the tangent before reach the end
	
	//float		mDist0;
	//float		mDist1;
};

//================================================================//
// MOAIContactPointAccumulator2D
//================================================================//
class MOAIContactPointAccumulator2D {
private:

	MOAIContactPoint2D*		mContacts;
	u32						mTop;
	u32						mMax;

public:
	
	//----------------------------------------------------------------//
	void Push ( const ZLVec2D& point, const ZLVec2D& tangent, const ZLVec2D& normal, ZLVec2D v0, ZLVec2D v1, u32 type ) {
		
		assert ( this->mContacts );
		
		if ( this->mTop < this->mMax ) {
			
			MOAIContactPoint2D& contact = this->mContacts [ this->mTop++ ];
			
			contact.mType = type;
			contact.mPoint = point;
			contact.mTangent = tangent;
			contact.mNormal = normal;
			
			v0.Sub ( point );
			v1.Sub ( point );
			
			contact.mNegD = contact.mTangent.Dot ( v0 );
			contact.mPosD = contact.mTangent.Dot ( v1 );
			
			contact.mNegD = ABS ( contact.mNegD );
			contact.mPosD = ABS ( contact.mPosD );
			
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
				contact.mNegD,
				contact.mPosD
			);
		}
	}
	
	//----------------------------------------------------------------//
	void PushCorner ( const ZLVec2D& point, const ZLVec2D& tangent, const ZLVec2D& normal, const ZLVec2D& v0, const ZLVec2D& v1, ZLVec2D cornerTangent ) {
	
		if ( this->mTop < this->mMax ) {
			this->Push ( point, tangent, normal, v0, v1, MOAIContactPoint2D::CORNER );
			this->mContacts [ this->mTop - 1 ].mCornerTangent = cornerTangent;
		}
	}
	
	//----------------------------------------------------------------//
	MOAIContactPointAccumulator2D ( MOAIContactPoint2D* contacts, u32 max ) :
		mContacts ( contacts ),
		mTop ( 0 ),
		mMax ( max ) {
	}
	
	//----------------------------------------------------------------//
	void Reset () {
	
		this->mTop = 0;
	}
	
	//----------------------------------------------------------------//
	u32 Top () {
	
		return this->mTop;
	}
};

//================================================================//
// MOAIVectorAccumulator
//================================================================//
class MOAIVectorAccumulator {
private:

	ZLVec3D			mAccumulator;
	float			mCount;

public:
	
	//----------------------------------------------------------------//
	void			Accumulate					( float x, float y, float z = 0.0f );
	ZLVec3D			GetAverage					();
					MOAIVectorAccumulator		();
};

//================================================================//
// MOAIOverlapHandler
//================================================================//
class MOAIOverlapHandler {
private:

	friend class MOAICollisionWorld;

	MOAICollisionProp&			mProp0;
	MOAICollisionProp&			mProp1;

public:
	
	const MOAITransformBase&	mTransform0;
	const MOAITransformBase&	mTransform1;
	
	ZLBounds					mBounds;
	bool						mCalculateBounds;
	
	//----------------------------------------------------------------//
					MOAIOverlapHandler		( MOAICollisionProp& prop0, MOAICollisionProp& prop1 );
	void			HandleOverlap			();
};

//================================================================//
// MOAICollisionWorld
//================================================================//
class MOAICollisionWorld :
	public virtual MOAIAction,
	public virtual MOAIDrawShapeRetained,
	public virtual MOAIPartition {
private:

	friend class MOAICollisionProp;
	friend class MOAIOverlapHandler;

	bool	mUpdated;
	u32		mOverlapPass;

	typedef ZLLeanList < MOAICollisionProp* >::Iterator ActiveListIt;
	ZLLeanList < MOAICollisionProp* > mActiveList;
	
	typedef ZLLeanList < MOAIPropOverlap* >::Iterator OverlapListIt;
	ZLLeanList < MOAIPropOverlap* > mOverlapList;
	
	ZLLeanPool < MOAIPropOverlap > mOverlapPool;

	MOAILuaStrongRef mCallback;

	//----------------------------------------------------------------//
	static int			_insertProp				( lua_State* L );
	static int			_processOverlaps		( lua_State* L );
	static int			_setCallback			( lua_State* L );

	//----------------------------------------------------------------//
	void				AffirmOverlap			( MOAICollisionProp& prop0, u32 type0, MOAICollisionProp& prop1, u32 type1, const ZLBounds& bounds );
	void				ClearOverlap			( MOAICollisionProp& prop0, MOAICollisionProp& prop1 );
	void				ClearOverlaps			( MOAICollisionProp& prop );
	void				DoCallback				( u32 eventID, MOAICollisionProp& prop0, MOAICollisionProp& prop1 );
	void				DoCallback				( u32 eventID, MOAICollisionProp& prop0, MOAICollisionProp& prop1, const ZLBounds& bounds );
	void				HandleOverlap			( MOAICollisionProp& prop0, u32 type0, MOAICollisionProp& prop1, u32 type1, const ZLBounds& bounds );
	bool				IsDone					();
	void				InvalidateOverlaps		( MOAICollisionProp& prop, u32 nextPass );
	void				MakeActive				( MOAICollisionProp& prop0 );
	void				MakeInactive			( MOAICollisionProp& prop0 );
	void				ProcessOverlaps			();
	void				PruneOverlaps			( MOAICollisionProp& prop );
	void				Render					();
	void				RemoveHull				( MOAICollisionProp& prop );

	//----------------------------------------------------------------//
	void				MOAIAction_Update					( double step );
	void				MOAIPartition_DrawDebugFront		();
	void				MOAIPartition_OnInsertHull			( MOAIPartitionHull& hull );
	void				MOAIPartition_OnRemoveHull			( MOAIPartitionHull& hull );
	void				MOAIPartition_OnUpdateHull			( MOAIPartitionHull& hull );

public:
	
	DECL_LUA_FACTORY ( MOAICollisionWorld )
	
	enum {
		OVERLAP_BEGIN,
		OVERLAP_END,
		OVERLAP_UPDATE,
	};
	
	static const u32 OVERLAP_PASS_INIT = 0;
	
	GET ( u32, CurrentPass, mOverlapPass )
	GET ( u32, NextPass, ( mOverlapPass + 1 ))
	
	//----------------------------------------------------------------//
					MOAICollisionWorld		();
					~MOAICollisionWorld		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
