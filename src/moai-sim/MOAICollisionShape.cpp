// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICollisionPrims.h>
#include <moai-sim/MOAICollisionShape.h>
#include <moai-sim/MOAIDraw.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIMoveConstraint2D.h>
#include <moai-sim/MOAIOverlap.h>
#include <moai-sim/MOAIOverlapResolver.h>
#include <moai-sim/MOAITransformBase.h>

//================================================================//
// MOAICollisionShape
//================================================================//

//----------------------------------------------------------------//
void MOAICollisionShape::Bless () {

	for ( u32 i = 0; i < this->mShapes.Size (); ++i ) {

		this->mBounds.Grow ( this->mShapes [ i ]->mBounds, i == 0 );
	}
}

//----------------------------------------------------------------//
void MOAICollisionShape::Clear () {

	for ( u32 i = 0; i < this->mShapes.Size (); ++i ) {
		if ( this->mShapes [ i ]) {
			delete ( this->mShapes [ i ]);
		}
	}
	this->mShapes.Clear ();
}

//----------------------------------------------------------------//
void MOAICollisionShape::Draw ( const ZLAffine3D& localToWorldMtx ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	gfxMgr.mVertexCache.SetUVTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::UV_TO_MODEL_MTX ));

	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008
	
	draw.Bind ();
	
	for ( u32 i = 0; i < this->mShapes.Size (); ++i ) {
	
		MOAIOverlapShape* shape = this->mShapes [ i ];
	
		switch ( shape->mType ) {
			
			case MOAICollisionConsts::BOX: {
			
				gfxMgr.mVertexCache.SetVertexTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::WORLD_TO_CLIP_MTX ));
			
				ZLBox box = *( ZLBox* )shape->mPtr;
				box.Transform ( localToWorldMtx );
				draw.DrawBoxOutline ( box );
				break;
			}
			case MOAICollisionConsts::CIRCLE: {
			
				gfxMgr.mVertexCache.SetVertexTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::MODEL_TO_CLIP_MTX ));
			
				ZLCircle circle = *( ZLCircle* )shape->mPtr;
				draw.DrawEllipseOutline ( circle.mCenter.mX, circle.mCenter.mY, circle.mRadius, circle.mRadius, 32 );
				break;
			}
			case MOAICollisionConsts::FRUSTUM:
				break;
			
			case MOAICollisionConsts::POLYGON:
				break;
			
			case MOAICollisionConsts::PRISM:
				break;
			
			case MOAICollisionConsts::QUAD: {
			
				gfxMgr.mVertexCache.SetVertexTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::MODEL_TO_CLIP_MTX ));
				draw.DrawQuadOutline ( *( ZLQuad* )shape->mPtr );
				break;
			}
			case MOAICollisionConsts::RECT: {
			
				gfxMgr.mVertexCache.SetVertexTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::WORLD_TO_CLIP_MTX ));
			
				ZLRect rect = *( ZLRect* )shape->mPtr;
				localToWorldMtx.Transform ( rect );
				draw.DrawRectOutline ( rect );
				break;
			}
			case MOAICollisionConsts::SPHERE:
				break;
			
			default:
				break;
		}
	}
}

//----------------------------------------------------------------//
void MOAICollisionShape::Process ( MOAIOverlapShapeVisitor& visitor, const ZLBox& otherBounds, const ZLAffine3D& t0, const ZLAffine3D& t1 ) const {

	MOAIOverlapBox otherShape;
	otherShape.mShape = otherBounds;
	otherShape.mBounds = otherBounds;

	size_t selfShapeCount = this->mShapes.Size ();

	if ( selfShapeCount == 1 ) {
	
		visitor.Process ( *this->mShapes [ 0 ], otherShape, t0, t1 );
	}
	else {
	
		for ( size_t i = 0; i < selfShapeCount; ++i ) {
			visitor.Process ( *this->mShapes [ i ], otherShape, t0, t1 );
		}
	}
}

//----------------------------------------------------------------//
void MOAICollisionShape::Process ( MOAIOverlapShapeVisitor& visitor, const MOAICollisionShape& otherShape, const ZLAffine3D& t0, const ZLAffine3D& t1 ) const {

	MOAICollisionShape::Process ( visitor, *this, otherShape, t0, t1 );
}

//----------------------------------------------------------------//
void MOAICollisionShape::Process ( MOAIOverlapShapeVisitor& visitor, const MOAICollisionShape& shape0, const MOAICollisionShape& shape1, const ZLAffine3D& t0, const ZLAffine3D& t1 ) {

	size_t shapeCount0 = shape0.mShapes.Size ();
	size_t shapeCount1 = shape1.mShapes.Size ();
	
	if (( shapeCount0 == 1 ) || ( shapeCount1 == 1 )) {
	
		if (( shapeCount0 == 1 ) && ( shapeCount1 == 1 )) {
		
			visitor.Process ( *shape0.mShapes [ 0 ], *shape1.mShapes [ 0 ], t0, t1 );
		}
		else if ( shapeCount1 == 1 ) {
		
			for ( size_t i = 0; i < shapeCount0; ++i ) {
				visitor.Process ( *shape0.mShapes [ i ], *shape1.mShapes [ 0 ], t0, t1 );
			}
		}
		else {
		
			for ( size_t i = 0; i < shapeCount1; ++i ) {
				visitor.Process ( *shape0.mShapes [ 0 ], *shape1.mShapes [ i ], t0, t1 );
			}
		}
	}
	else {
	
		for ( size_t i = 0; i < shapeCount0; ++i ) {
			for ( size_t j = 0; j < shapeCount1; ++j ) {
				visitor.Process ( *shape0.mShapes [ i ], *shape1.mShapes [ j ], t0, t1 );
			}
		}
	}
}

//----------------------------------------------------------------//
MOAICollisionShape::MOAICollisionShape () {

	this->mBounds = ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
MOAICollisionShape::~MOAICollisionShape () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAICollisionShape::Set ( u32 idx, const ZLBox& box ) {

	this->mShapes.Resize ( idx + 1, 0 );
	if ( this->mShapes [ idx ]) {
		delete this->mShapes [ idx ];
	}
	MOAIOverlapBox* overlap = new MOAIOverlapBox ();
	overlap->mShape = box;
	overlap->mBounds.Init ( box );
	this->mShapes [ idx ] = overlap;
	
	this->Bless ();
}

//----------------------------------------------------------------//
void MOAICollisionShape::Set ( u32 idx, const ZLQuad& quad ) {

	this->mShapes.Resize ( idx + 1, 0 );
	if ( this->mShapes [ idx ]) {
		delete this->mShapes [ idx ];
	}
	MOAIOverlapQuad* overlap = new MOAIOverlapQuad ();
	overlap->mShape = quad;
	overlap->mBounds.Init ( quad.GetBounds ());
	this->mShapes [ idx ] = overlap;
	
	this->Bless ();
}

//----------------------------------------------------------------//
void MOAICollisionShape::Set ( u32 idx, const ZLRect& rect ) {

	this->mShapes.Resize ( idx + 1, 0 );
	if ( this->mShapes [ idx ]) {
		delete this->mShapes [ idx ];
	}
	MOAIOverlapRect* overlap = new MOAIOverlapRect ();
	overlap->mShape = rect;
	overlap->mBounds.Init ( rect );
	this->mShapes [ idx ] = overlap;
	
	this->Bless ();
}
