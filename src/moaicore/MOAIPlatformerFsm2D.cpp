// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIPlatformerBody2D.h>
#include <moaicore/MOAIPlatformerFsm2D.h>
#include <moaicore/MOAISurfaceSampler2D.h>

#ifndef FP_EPSILON
	#define FP_EPSILON 0.0001f
#endif

#define FP_EQUAL(f0,f1) \
	((( f0 + FP_EPSILON ) > f1 ) && (( f1 - FP_EPSILON ) < f0 ))

#define IS_CEILING(surface) \
	( surface.mNorm.mY <= this->mCeilCos )

#define IS_FLOOR(surface) \
	( surface.mNorm.mY >= this->mFloorCos )

#define IS_WALL(surface) \
	(( surface.mNorm.mY < this->mFloorCos ) && ( surface.mNorm.mY > this->mCeilCos ))

#define FLOOR_PAD_LENGTH 0.0001f

//================================================================//
// MOAIPlatformerFsm2D
//================================================================//

//----------------------------------------------------------------//
void MOAIPlatformerFsm2D::CalculateWallDepthInAir () {
	
	this->mWallToLeft = false;
	this->mWallToRight = false;

	this->mLeftWallDepth = 0.0f;
	this->mRightWallDepth = 0.0f;
	
	for ( u32 i = 0; i < this->mSurfaceBuffer.mTop; ++i ) {
		const MOAISurface2D& surface = this->mSurfaceBuffer.mSurfaces [ i ];
		
		if ( IS_FLOOR ( surface )) continue;
		if ( IS_CEILING ( surface )) continue;
		
		float d = this->mLoc.Dot ( surface.mNorm ) + surface.mDist;
		if (( d <= -1.0f ) || ( d >= 1.0f )) continue;
		
		// Get the point of first contact on the polygon...
		USVec2D pofcop = surface.mNorm;
		pofcop.Reverse ();
		pofcop.Add ( this->mLoc );
		surface.ClampPoint ( pofcop );
		
		if ( this->mLoc.DistSqrd ( pofcop ) > 1.0f ) continue;

		// get the distance to the point on the circle
		float a = pofcop.mY - this->mLoc.mY;
		float b = sqrtf ( 1.0f - ( a * a )); // convert sin to cosine
		
		if ( surface.mNorm.mX < 0.0f ) {
			
			b = this->mLoc.mX + b - pofcop.mX;
			
			if ( b >= this->mRightWallDepth ) {
				this->mWallToRight = true;
				this->mRightWallDepth = b;
			}
		}
		else {
			
			b = -( this->mLoc.mX - b - pofcop.mX );
			
			if ( b >= this->mLeftWallDepth ) {
				this->mWallToLeft = true;
				this->mLeftWallDepth = b;
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIPlatformerFsm2D::CalculateWallDepthOnFloor () {

	this->mWallToLeft = false;
	this->mWallToRight = false;

	this->mLeftWallDepth = 0.0f;
	this->mRightWallDepth = 0.0f;
	
	for ( u32 i = 0; i < this->mSurfaceBuffer.mTop; ++i ) {
		const MOAISurface2D& surface = this->mSurfaceBuffer.mSurfaces [ i ];
		if ( IS_FLOOR ( surface )) continue;
		
		// ignore walls facing away from loc
		float dist = USDist::PointToPlane2D ( this->mLoc, surface );
		if ( dist < 0.0f ) continue;
		
		// this gets the distance from the edge of the circle to the closest feature on the surface
		// if the feature is inside the circle a negative value will be returned
		float wallDepth = surface.GetCircleDepthAlongRay ( this->mLoc, this->mFloorTangent ); // TODO: return bool; ignore walls behind circle

		// sort left and right wall info
		// we always want the closest wall
		if ( surface.mNorm.mX > 0.0f ) {
			if ( !this->mWallToLeft || ( wallDepth < this->mLeftWallDepth )) {
				this->mLeftWallDepth = wallDepth;
			}
			this->mWallToLeft = true;
		}
		else {
			if ( !this->mWallToRight || ( wallDepth < this->mRightWallDepth )) {
				this->mRightWallDepth = wallDepth;
			}
			this->mWallToRight = true;
		}
	}
}

//----------------------------------------------------------------//
void MOAIPlatformerFsm2D::CalculateWallShoveInAir () {
	
	this->mShoveDistInAir = 0.0f;
	
	if ( this->mWallToLeft && this->mWallToRight ) {
	
		this->mShoveDistInAir = ( this->mLeftWallDepth - this->mRightWallDepth ) * 0.5f;
	}
	else {
	
		if ( this->mWallToLeft ) {
			this->mShoveDistInAir = this->mLeftWallDepth;
		}
		
		if ( this->mWallToRight ) {
			this->mShoveDistInAir = -this->mRightWallDepth;
		}
	}
}

//----------------------------------------------------------------//
void MOAIPlatformerFsm2D::CalculateWallShoveOnFloor () {
	
	this->mShoveDistOnFloor = 0.0f;
	
	if ( !( this->mWallToLeft || this->mWallToRight )) return;
	
	bool leftShove = ( this->mLeftWallDepth < 0.0f );
	bool rightShove = ( this->mRightWallDepth < 0.0f );
	
	if ( leftShove && rightShove ) {
		this->mShoveDistOnFloor = ( this->mRightWallDepth - this->mLeftWallDepth ) * 0.5f;
		this->mMoveDistOnFloor = 0.0f;
	}
	else if ( leftShove ) {
		
		this->mShoveDistOnFloor = -this->mLeftWallDepth;
		
		if ( this->mMoveDistOnFloor < 0.0f ) {
			this->mMoveDistOnFloor = 0.0f;
		}

		if ( this->mWallToRight ) {
		
			if ( this->mShoveDistOnFloor >= this->mRightWallDepth ) {
				this->mShoveDistOnFloor = this->mRightWallDepth + (( this->mShoveDistOnFloor - this->mRightWallDepth ) * 0.5f );
				this->mMoveDistOnFloor = 0.0f;
			}
			else {
				float gap = this->mRightWallDepth - this->mShoveDistOnFloor;
				if ( this->mMoveDistOnFloor >= gap ) {
					this->mMoveDistOnFloor = gap;
				}
			}
		}
	}
	else if ( rightShove ) {
		
		this->mShoveDistOnFloor = this->mRightWallDepth;
		
		if ( this->mMoveDistOnFloor > 0.0f ) {
			this->mMoveDistOnFloor = 0.0f;
		}
		
		if ( this->mWallToLeft ) {
		
			if ( this->mShoveDistOnFloor <= -this->mLeftWallDepth ) {
				this->mShoveDistOnFloor = (( this->mShoveDistOnFloor + this->mLeftWallDepth ) * 0.5f ) - this->mLeftWallDepth;
				this->mMoveDistOnFloor = 0.0f;
			}
			else {
				float gap = this->mLeftWallDepth - this->mShoveDistOnFloor;
				if ( this->mMoveDistOnFloor <= gap ) {
					this->mMoveDistOnFloor = gap;
				}
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIPlatformerFsm2D::DoMoveInAir () {

	this->mLoc.Add ( this->mMove );
	this->mFoot.Add ( this->mMove );
	
	this->DoWallSnapInAir ();
	
	this->mState = STATE_DONE;
}

//----------------------------------------------------------------//
void MOAIPlatformerFsm2D::DoMoveOnFloor () {
	
	this->CalculateWallDepthOnFloor ();
	this->CalculateWallShoveOnFloor ();
	
	float moveDist = this->mMoveDistOnFloor + this->mShoveDistOnFloor;
	
	if (( moveDist <= 0.0001f ) && ( moveDist >= -0.0001f )) {
		this->mState = STATE_DONE;
		return;
	}
	
	USVec2D move = this->mFloorTangent;
	move.Scale ( moveDist );
	
	float bestTime = 1.0f;
	float bestDot = 0.0f;
	float bridgeTime = 0.0f;
	const MOAISurface2D* hit = 0;
	
	for ( u32 i = 0; i < this->mSurfaceBuffer.mTop; ++i ) {
		
		const MOAISurface2D& surface = this->mSurfaceBuffer.mSurfaces [ i ];
		//if (( &surface == this->mFloor ) || ( !IS_FLOOR ( surface ))) continue;
		if ( !IS_FLOOR ( surface )) continue;
		
		float dot = surface.mNorm.Dot ( move ); // cos between move and surface
		float time = 0.0f;
			
		if (( bestDot < 0.0f ) && ( dot > 0.0f )) continue; // if we've hit a valley, ignore all peaks
		if ( surface.IsLeaving ( this->mFoot, move, -0.0001f )) continue; // ignore any surface we're leaving
		
		// floor is parallel to move, so process as bridge
		if (( dot < 0.000001f ) && ( dot > -0.000001f )) {
			
			// if we're on a bridge, take bridge as new floor (and bridge time) if end is farther than current bridge
			if ( surface.IsBridge ( this->mFoot, move, 0.0001f, time )) {
				if ( time > bridgeTime ) {
					bridgeTime = time;
					this->mFloor = &surface;
				}
			}

			if (( bestDot > 0.0f ) && ( bestTime < ( bridgeTime - 0.0001f ))) {
				bestTime = 1.0f;
				bestDot = 0.0f;
				hit = 0;
			}
		}
		else {
		
			if ( !surface.GetMoveHit ( this->mFoot, move, -0.0001f, time )) continue; // ignore surface w/ no valid hit
			
			//printf ( "RAY HIT: %f %f %p (%f, %f)\n", time, dot, &surface, surface.mNorm.mX, surface.mNorm.mY );
			
			if ( time > 1.0f ) continue; // won't hit this move, so ignore
			if ( time < -0.0001f ) continue; // floor is behind us
			
			// skip if surface is a peak and peak is closer than end of current bridge
			if (( dot > 0.0f ) && ( time < ( bridgeTime - 0.0001f ))) continue;
			
			if (( time < ( bestTime + 0.0001f )) && ( time > ( bestTime - 0.0001f ))) {

				// ignore collision if we're on its edge
				if ( surface.IsOnEdge ( this->mFoot, 0.00001f )) continue;
				
				// break ties by always taking the steepest valley
				if ( dot > bestDot ) continue;
			}
			else {
				
				USVec2D hitLoc = move;
				hitLoc.Scale ( time );
				hitLoc.Add ( this->mFoot );
				
				if ( surface.IsLeaving ( hitLoc, move, -0.0001f )) continue;
				
				if ( dot > 0.0f ) {
					// surface is a peak
					// last found a peak, so only take farther hits UNLESS they are valleys
					if (( bestDot > 0.0f ) && ( dot > 0.0f ) && ( time < bestTime )) continue; 
				}
				else {
					// surface is a valley
					// last found a valley, so only take nearer hits
					// remember: after finding any valley, all peaks will be ignored
					if (( bestDot < 0.0f ) && ( time > bestTime )) continue; 
				}
			}
			
			bestTime = time;
			bestDot = dot;
			hit = &surface;
		}
	}
	
	if ( hit ) {
		
		//printf ( "hit: %p (%f, %f) %f %f\n", hit, hit->mNorm.mX, hit->mNorm.mY, bestTime, bestDot );
		
		if ( bestTime > 0.0f ) {
		
			move.Scale ( bestTime );
			this->mLoc.Add ( move );
			this->mFoot.Add ( move );
			
			float remaining = 1.0f - bestTime;
			float shove = this->mShoveDistOnFloor / moveDist;
			if (( shove > 0.0f ) && ( bestTime > shove )) {
				remaining = ( 1.0f - bestTime ) / ( 1.0f - shove );
				
			}
			this->mMove.Scale ( remaining );
		}
		
		float prevFloorMoveDist = this->mMoveDistOnFloor;
		this->SetFloor ( *hit );
		
		// if next floor reverses sign of last projected move then abort
		if (( prevFloorMoveDist >= 0.0f ) != ( this->mMoveDistOnFloor >= 0.0f )) {
			this->mState = STATE_DONE;
		}
	}
	else {
		this->mLoc.Add ( move );
		this->mFoot.Add ( move );
		this->mState = STATE_DONE;
	}
}

//----------------------------------------------------------------//
void MOAIPlatformerFsm2D::DoWallSnapInAir () {

	this->CalculateWallDepthInAir ();
	this->CalculateWallShoveInAir ();
	
	if ( this->mShoveDistInAir ) {
		this->mLoc.mX += this->mShoveDistInAir;
		this->mFoot.mX += this->mShoveDistInAir;
	}
}

//----------------------------------------------------------------//
void MOAIPlatformerFsm2D::Move ( MOAIPlatformerBody2D& body ) {

	// gather surfaces for move
	// if snap - process snap
	//		gap filling - when snapping, use snap span to place dummy platform
	// process wall overlaps - build up 'push' vector
	// determing if move possible - cannot move into 'push' vector
	// resolve move/push
	//		idea of push is to get platformer out of wall overlap
	//		push gets added to move
	//		move is processed until another wall is hit
	//		when wall is hit, move is nilled out and remainder of push is handled
	//		push is 'soft' - if body is overlapped on boths sides, body should resolve to center between walls

	this->mCeilCos = body.mCeilCos;
	this->mFloorCos = body.mFloorCos;
	
	this->mLoc.Init ( 0.0f, 0.0f );
	this->mFoot.Init ( 0.0f, -(( body.mSkirt / body.mVRad ) + 1.0f ));
	this->mUp.Init ( 0.0f, 1.0f );
	
	this->mMove = body.mMove;
	this->mMove.Scale ( 1.0f / body.mHRad, 1.0f / body.mVRad );
	this->mMoveDistOnFloor = this->mMove.mX;
	
	this->mFloor = 0;
	this->mFloorNorm.Init ( 0.0f, 1.0f );
	this->mFloorTangent.Init ( 1.0f, 0.0f );
	
	USBox bounds;
	body.GetPropBounds ( bounds );
	bounds.Transform ( body.mLocalToWorldMtx );
	bounds.Inflate ( bounds.Width ());
	body.GatherSurfacesForBounds ( this->mSurfaceBuffer, bounds );

	this->SnapUp ();
	this->mState = this->mFloor ? STATE_ON_FLOOR : STATE_IN_AIR;

	// detach (this should be based on dot with 'up' vector being nonzero instead of just 'y' being positive
	//if ( body.mAutoDetach && ( body.mMove.mY > 0.0f ) && ( this->mState == STATE_ON_FLOOR ) && ( this->mMove.Dot ( this->mFloorNorm ) > 0.0001f )) {
	if (( this->mState == STATE_ON_FLOOR ) && ( this->mMove.Dot ( this->mFloorNorm ) > 0.0001f )) {
		this->mState = STATE_IN_AIR;
	}

	u32 steps = 0;
	for ( ; ( this->mState != STATE_DONE ) && ( steps < MAX_STEPS ); ++steps ) {
		
		switch ( this->mState ) {
			
			case STATE_IN_AIR: {
				this->DoMoveInAir ();
				break;
			}
			
			case STATE_ON_FLOOR: {
				this->DoMoveOnFloor ();
				break;
			}
		}
	}
	
	body.mLoc.mX += this->mLoc.mX * body.mHRad;
	body.mLoc.mY += this->mLoc.mY * body.mVRad;
	body.mMove.Init ( 0.0f, 0.0f );
	
	body.mSteps = steps + 1;
	body.mCompleted = this->mState == STATE_DONE;
}

//----------------------------------------------------------------//
void MOAIPlatformerFsm2D::SetFloor ( const MOAISurface2D& floor ) {
		
	this->mFloor = &floor;
	this->mFloorNorm = floor.mNorm;
	this->mFloorTangent = floor.mTangent;
	this->mMoveDistOnFloor = this->mMove.Dot ( this->mFloorTangent );
}

//----------------------------------------------------------------//
void MOAIPlatformerFsm2D::SnapUp () {

	const MOAISurface2D* snapSurface = 0;
	float snapDist = -0.001f;
	float stem = this->mLoc.mY - this->mFoot.mY;

	for ( u32 i = 0; i < this->mSurfaceBuffer.mTop; ++i ) {
		
		const MOAISurface2D& surface = this->mSurfaceBuffer.mSurfaces [ i ];
		
		if ( !IS_FLOOR ( surface )) continue; // not a floor (TODO: pass in floor cos)
		if (( this->mFoot.mX < surface.mXMin ) || ( this->mFoot.mX > surface.mXMax )) continue; // loc not over surface
		if ( surface.IsLeaving ( this->mFoot, this->mMove, -0.0001f )) continue;
		
		float dist;
		if ( surface.GetRayHit ( this->mFoot, this->mUp, dist ))  {
			
			// bail if snap is above us or lower than last best snap
			if (( dist > stem ) || ( dist < snapDist )) continue;
			
			// 'snap' is true if we already have a valid snap
			if ( snapSurface ) {
				
				// looks like we have multiple floors to choose from...
				if ( dist > snapDist ) {
				
					// we have a clear winner
					snapSurface = &surface;
					snapDist = dist;
				}
				else {
					
					// snap is the same as the last snap...
					
					// break the tie
					// if there's a this->mMove, choose the surface with the steepest angle
					// (against the this->mMove)
					if ( this->mMove.mX > 0.0f ) {
						if ( surface.mNorm.mX < snapSurface->mNorm.mX ) {
							snapSurface = &surface;
						}
					}
					else if ( this->mMove.mX < 0.0f ) {
						if ( surface.mNorm.mX > snapSurface->mNorm.mX ) {
							snapSurface = &surface;
						}
					}
				}
			}
			else {
			
				// first floor, so go with it
				snapSurface = &surface;
				snapDist = dist;
			}
		}
	}
		
	if ( snapSurface ) {
	
		snapDist *= stem;
		this->mLoc.mY += snapDist;
		this->mFoot.mY += snapDist;
		
		this->SetFloor ( *snapSurface );
	}
}
