#include "pch.h"
#include <moaicore/MOAIDraw.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAISurfaceSampler2D.h>

//================================================================//
// MOAISurfaceBuffer2D
//================================================================//

//----------------------------------------------------------------//
MOAISurfaceSnap2D MOAISurfaceBuffer2D::GetSnapUp ( USVec2D loc, USVec2D move, float stem ) {

	loc.mY -= stem;

	MOAISurfaceSnap2D snap;
	snap.mSurface = 0;
	snap.mDist = 0.0f;

	for ( u32 i = 0; i < this->mTop; ++i ) {
		
		const MOAISurface2D& surface = this->mSurfaces [ i ];
		
		if ( surface.mNorm.mY < 0.0f ) continue;
		if ( !surface.IsOver ( loc )) continue;
		if ( surface.IsLeaving ( loc, move )) continue;

		USVec2D up ( 0.0f, 1.0f );
		float dist = 0.0f; // dist will be returned here
		if ( surface.GetRayHit ( loc, up, dist ))  {
			
			if ( dist > stem ) continue; // bail if snap is above us
			if ( dist < snap.mDist ) continue; // bail if snap is lower than last best snap
			
			// 'snap' is true if we already have a valid snap
			if ( snap.mSurface ) {
				
				// looks like we have multiple floors to choose from...
				if ( dist > snap.mDist ) {
				
					// we have a clear winner
					snap.mSurface = &surface;
					snap.mDist = dist;
				}
				else {
					
					// snap is the same as the last snap...
					
					// break the tie
					// if there's a move, choose the surface with the steepest angle
					// (against the move)
					if ( move.mX > 0.0f ) {
						if ( surface.mNorm.mX < snap.mSurface->mNorm.mX ) {
							snap.mSurface = &surface;
						}
					}
					else if ( move.mX < 0.0f ) {
						if ( surface.mNorm.mX > snap.mSurface->mNorm.mX ) {
							snap.mSurface = &surface;
						}
					}
				}
			}
			else {
			
				// first floor, so go with it
				snap.mSurface = &surface;
				snap.mDist = dist;
			}
			
			if ( dist > snap.mDist ) {
				snap.mDist = dist;
			}
		}
	}
	
	snap.mDist *= stem;
	return snap;
}

//----------------------------------------------------------------//
const MOAISurface2D& MOAISurfaceBuffer2D::GetSurface ( u32 id ) const {

	assert ( id < this->mTop );
	return this->mSurfaces [ id ];
}

//----------------------------------------------------------------//
MOAISurfaceBuffer2D::MOAISurfaceBuffer2D () :
	mTop ( 0 ) {
}

//----------------------------------------------------------------//
MOAISurfaceBuffer2D::~MOAISurfaceBuffer2D () {
}

//================================================================//
// MOAISurfaceSampler2D
//================================================================//

//----------------------------------------------------------------//
void MOAISurfaceSampler2D::Init ( MOAISurfaceBuffer2D& buffer, const USRect& sampleRect, const USAffine3D& worldToSampler ) {

	this->mBuffer = &buffer;
	this->mSampleRect = sampleRect;
	
	this->mSourceProp = 0;
	
	this->mWorldToSampler = worldToSampler;
	this->mTransform.Ident ();
}

//----------------------------------------------------------------//
MOAISurfaceSampler2D::MOAISurfaceSampler2D () {
}

//----------------------------------------------------------------//
MOAISurfaceSampler2D::~MOAISurfaceSampler2D	 () {
}

//----------------------------------------------------------------//
void MOAISurfaceSampler2D::PushSurface ( const MOAISurfaceEdge2D& surface ) {

	if ( this->mBuffer->mTop < MOAISurfaceBuffer2D::MAX_SURFACES ) {
		
		USVec2D v0 = surface.mV0;
		USVec2D v1 = surface.mV1;
		
		this->mTransform.Transform ( v0 );
		this->mTransform.Transform ( v1 );
		
		USRect& sampleRect = this->mSampleRect;

		if (( v0.mX <= sampleRect.mXMin ) && ( v1.mX <= sampleRect.mXMin )) return;
		if (( v0.mX >= sampleRect.mXMax ) && ( v1.mX >= sampleRect.mXMax )) return;
		if (( v0.mY <= sampleRect.mYMin ) && ( v1.mY <= sampleRect.mYMin )) return;
		if (( v0.mY >= sampleRect.mYMax ) && ( v1.mY >= sampleRect.mYMax )) return;
		
		MOAISurface2D& surface = this->mBuffer->mSurfaces [ this->mBuffer->mTop++ ];
		surface.Init ( v0, v1 );
	}
}

//----------------------------------------------------------------//
void MOAISurfaceSampler2D::SetSourceProp ( MOAIProp* prop ) {

	this->mSourceProp = prop;
	this->mTransform = this->mWorldToSampler;

	if ( prop ) {
		USAffine3D propToWorld;
		propToWorld.Init ( prop->GetLocalToWorldMtx ());
		this->mTransform.Prepend ( propToWorld );
	}
}
