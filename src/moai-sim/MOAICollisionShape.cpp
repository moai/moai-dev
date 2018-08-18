// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICollisionPrim.h>
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

	MOAIGfxState& gfxState = MOAIGfxMgr::Get ().mGfxState;

	gfxState.SetUVTransform ( gfxState.GetMtx ( MOAIGfxState::UV_TO_MODEL_MTX ));

	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008
	
	draw.Bind ();
	
	for ( u32 i = 0; i < this->mShapes.Size (); ++i ) {
	
		MOAICollisionPrim* shape = this->mShapes [ i ];
	
		switch ( shape->mType ) {
			
			case MOAICollisionConsts::BOX: {
			
				gfxState.SetVertexTransform ( MOAIGfxState::WORLD_TO_CLIP_MTX );
			
				ZLBox box = *( ZLBox* )shape->mPtr;
				box.Transform ( localToWorldMtx );
				draw.DrawBoxOutline ( box );
				break;
			}
			case MOAICollisionConsts::CIRCLE: {
			
				gfxState.SetVertexTransform ( MOAIGfxState::MODEL_TO_CLIP_MTX );
			
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
			
				gfxState.SetVertexTransform ( MOAIGfxState::MODEL_TO_CLIP_MTX );
				draw.DrawQuadOutline ( *( ZLQuad* )shape->mPtr );
				break;
			}
			case MOAICollisionConsts::RECT: {
			
				gfxState.SetVertexTransform ( MOAIGfxState::WORLD_TO_CLIP_MTX );
			
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
