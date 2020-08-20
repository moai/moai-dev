// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICollisionPrim.h>
#include <moai-sim/MOAICollisionShape.h>
#include <moai-sim/MOAIMoveConstraint2D.h>
#include <moai-sim/MOAIOverlap.h>
#include <moai-sim/MOAIOverlapResolver.h>

//================================================================//
// MOAICollisionShape
//================================================================//

//----------------------------------------------------------------//
void MOAICollisionShape::Bless () {

	ZLBox aabb;
	for ( ZLIndex i = 0; i < this->mShapes.Size (); ++i ) {
		aabb.Grow ( this->mShapes [ i ]->mAABB, i > 0 );
	}
	this->mBounds.Init ( aabb );
}

//----------------------------------------------------------------//
void MOAICollisionShape::Clear () {

	for ( ZLIndex i = 0; i < this->mShapes.Size (); ++i ) {
		if ( this->mShapes [ i ]) {
			delete ( this->mShapes [ i ]);
		}
	}
	this->mShapes.Clear ();
}

//----------------------------------------------------------------//
void MOAICollisionShape::Draw ( const ZLAffine3D& localToWorldMtx ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	gfxMgr.SetUVTransform ( gfxMgr.GetMtx ( MOAIGfxMgr::UV_TO_MODEL_MTX ));

	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008

	draw.BindVectorPresets ();

	for ( ZLIndex i = 0; i < this->mShapes.Size (); ++i ) {

		MOAICollisionPrim* shape = this->mShapes [ i ];

		switch ( shape->mType ) {

			case MOAICollisionConsts::BOX: {

				gfxMgr.SetVertexTransform ( MOAIGfxMgr::WORLD_TO_CLIP_MTX );

				ZLBox box = *( ZLBox* )shape->mPtr;
				box.Transform ( localToWorldMtx );
				draw.DrawBoxOutline ( box );
				break;
			}
			case MOAICollisionConsts::CIRCLE: {

				gfxMgr.SetVertexTransform ( MOAIGfxMgr::MODEL_TO_CLIP_MTX );

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

				gfxMgr.SetVertexTransform ( MOAIGfxMgr::MODEL_TO_CLIP_MTX );
				draw.DrawQuadOutline ( *( ZLQuad* )shape->mPtr );
				break;
			}
			case MOAICollisionConsts::RECT: {

				gfxMgr.SetVertexTransform ( MOAIGfxMgr::WORLD_TO_CLIP_MTX );

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
MOAICollisionShape::MOAICollisionShape () {

	this->mBounds = ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
MOAICollisionShape::~MOAICollisionShape () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAICollisionShape::Set ( ZLIndex idx, const ZLBox& box ) {

	this->mShapes.Resize (( ZLSize )idx + 1, 0 );
	if ( this->mShapes [ idx ]) {
		delete this->mShapes [ idx ];
	}
	MOAIOverlapBox* overlap = new MOAIOverlapBox ();
	overlap->mShape = box;
	overlap->mAABB.Init ( box );
	this->mShapes [ idx ] = overlap;
	
	this->Bless ();
}

//----------------------------------------------------------------//
void MOAICollisionShape::Set ( ZLIndex idx, const ZLQuad& quad ) {

	this->mShapes.Resize (( ZLSize )idx + 1, 0 );
	if ( this->mShapes [ idx ]) {
		delete this->mShapes [ idx ];
	}
	MOAIOverlapQuad* overlap = new MOAIOverlapQuad ();
	overlap->mShape = quad;
	overlap->mAABB.Init ( quad.GetFrame ());
	this->mShapes [ idx ] = overlap;
	
	this->Bless ();
}

//----------------------------------------------------------------//
void MOAICollisionShape::Set ( ZLIndex idx, const ZLRect& rect ) {

	this->mShapes.Resize (( ZLSize )idx + 1, 0 );
	if ( this->mShapes [ idx ]) {
		delete this->mShapes [ idx ];
	}
	MOAIOverlapRect* overlap = new MOAIOverlapRect ();
	overlap->mShape = rect;
	overlap->mAABB.Init ( rect );
	this->mShapes [ idx ] = overlap;
	
	this->Bless ();
}
