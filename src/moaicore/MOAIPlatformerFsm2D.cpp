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
	(( surface ).mNorm.mY <= this->mCeilCos )

#define IS_FLOOR(surface) \
	(( surface ).mNorm.mY >= this->mFloorCos )

#define IS_WALL(surface) \
	((( surface ).mNorm.mY < this->mFloorCos ) && (( surface ).mNorm.mY > this->mCeilCos ))

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
		if ( !IS_WALL ( surface )) continue;
		
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

	if ( this->mMove.LengthSquared () < 0.00001f ) {
		this->mState = STATE_DONE;
		return;
	}

	USVec2D move = this->mMove;
	USVec2D invMove = move;
	invMove.Reverse ();

	// head is at top of unit circle
	USVec2D head = this->mLoc;
	head.mY += 1.0f;

	// foot is at bottom of unit circle
	USVec2D foot = this->mLoc;
	foot.mY -= 1.0f;
	
	USVec2D spine ( 0.0f, 2.0f );
	
	enum {
		HEAD,
		FOOT,
		SPINE,
	};
	
	float time = 0.0f;
	u32 contactType = 0;
	const MOAISurface2D* hit = 0;
	float hitTime = 1.0f;
	
	for ( u32 i = 0; i < this->mSurfaceBuffer.mTop; ++i ) {
		const MOAISurface2D& surface = this->mSurfaceBuffer.mSurfaces [ i ];
		
		float dot = surface.mNorm.Dot ( move ); // cos between move and surface
		if ( dot >= -0.000001f ) continue;
		
		// check foot against floors and walls
		if ( !IS_CEILING ( surface )) {
			if ( surface.GetRayHit ( foot, move, 0.0001f, time )) {
				if (( time >= -0.0001f ) && ( time < hitTime )) {
					hitTime = time;
					hit = &surface;
					contactType = FOOT;
				}
			}
		}
		
		// check head against ceilings and walls
		if ( !IS_FLOOR ( surface )) {
			if ( surface.GetRayHit ( head, move, 0.0001f, time )) {
				if (( time >= -0.0001f ) && ( time < hitTime )) {
					hitTime = time;
					hit = &surface;
					contactType = HEAD;
				}
			}
		}
		
		// check spine against surfaces entirely on one side or the other
		if (( foot.mX <= surface.mXMin ) || ( foot.mX >= surface.mXMax )) {
			
			float uB;
			
			if ( USSect::RayToRay ( surface.mV0, invMove, foot, spine, time, uB ) == USSect::SECT_HIT ) {
				if (( time >= 0.0f ) && ( time < hitTime )) {
					hitTime = time;
					hit = &surface;
					contactType = SPINE;
				}
			}
			
			if ( USSect::RayToRay ( surface.mV1, invMove, foot, spine, time, uB ) == USSect::SECT_HIT ) {
				if (( time >= 0.0f ) && ( time < hitTime )) {
					hitTime = time;
					hit = &surface;
					contactType = SPINE;
				}
			}
		}
	}
	
	if ( hit ) {
		
		// bring body flush with surface
		move.Scale ( hitTime );
		this->mLoc.Add ( move );
		this->mMove.Scale ( 1.0f - hitTime );
		
		// adjust the move vector
		if ( contactType == SPINE ) {
			this->mMove.mX = 0.0f;
		}
		else {
			
			this->mMove.PerpProject ( hit->mNorm );
		
			if ( IS_FLOOR ( *hit )) {
				this->mState = STATE_ON_FLOOR;
			}
		}
	}
	else {
		this->mLoc.Add ( move );
		this->DoWallSnapInAir ();
		this->mState = STATE_DONE;
	}
}

//----------------------------------------------------------------//
void MOAIPlatformerFsm2D::DoMoveOnFloor () {
	
	this->CalculateWallDepthOnFloor ();
	this->CalculateWallShoveOnFloor ();
	this->mShoveDistOnFloor = 0.0f;
	float moveDist = this->mMoveDistOnFloor + this->mShoveDistOnFloor;
	
	if (( moveDist <= 0.0001f ) && ( moveDist >= -0.0001f )) {
		this->mState = STATE_DONE;
		return;
	}
	
	USVec2D move = this->mFloorTangent;
	move.Scale ( moveDist );
	
	// head is at top of unit circle
	USVec2D head = this->mLoc;
	head.mY += 1.0f;

	// foot is at bottom of unit circle
	USVec2D foot = this->mLoc;
	foot.mY -= 1.0f;
	
	float bestFloorTime = 1.0f;
	float bestFloorDot = 0.0f;
	float bridgeTime = 0.0f;
	const MOAISurface2D* floorHit = 0;
	
	float bestCeilingTime = 1.0f;
	const MOAISurface2D* ceilingHit = 0;
	
	for ( u32 i = 0; i < this->mSurfaceBuffer.mTop; ++i ) {
		
		const MOAISurface2D& surface = this->mSurfaceBuffer.mSurfaces [ i ];
		
		float dot = surface.mNorm.Dot ( move ); // cos between move and surface
		float time = 0.0f;
		
		if ( IS_FLOOR ( surface )) {
			
			if (( bestFloorDot < 0.0f ) && ( dot > 0.0f )) continue; // if we've previously hit a valley, ignore all peaks
			if ( surface.IsLeaving ( foot, move, -0.0001f )) continue; // ignore any surface we're leaving
			
			// if we have not hit a valley and floor is parallel to move, process as bridge
			if (( bestFloorDot <= 0.0f ) && ( dot < 0.000001f ) && ( dot > -0.000001f )) {
				
				if ( surface.IsBridge ( foot, move, 0.0001f, time )) {
				
					if ( time > 1.0f ) continue;
					if (( bestFloorDot > 0.0f ) && ( bestFloorTime > ( time - 0.0001f ))) continue;
				
					if ( time > bridgeTime ) {
						bridgeTime = time;
						bestFloorTime = time;
						bestFloorDot = 0.0f;
						floorHit = &surface;
					}
				}
			}
			else {
			
				if ( !surface.GetMoveHit ( foot, move, -0.0001f, time )) continue; // ignore surface w/ no valid hit
				
				//printf ( "RAY HIT: %f %f %p (%f, %f)\n", time, dot, &surface, surface.mNorm.mX, surface.mNorm.mY );
				
				if ( time > 1.0f ) continue; // won't hit this move, so ignore
				if ( time < -0.0001f ) continue; // floor is behind us
				
				// skip if surface is a peak and peak is closer than end of current bridge
				if (( dot > 0.0f ) && ( time < ( bridgeTime - 0.0001f ))) continue;
				
				if (( time < ( bestFloorTime + 0.0001f )) && ( time > ( bestFloorTime - 0.0001f ))) {

					// ignore collision if we're on its edge
					if ( surface.IsOnEdge ( foot, 0.00001f )) continue;
					
					// break ties by always taking the steepest valley
					if ( dot > bestFloorDot ) continue;
				}
				else {
					
					USVec2D hitLoc = move;
					hitLoc.Scale ( time );
					hitLoc.Add ( foot );
					
					if ( surface.IsLeaving ( hitLoc, move, -0.0001f )) continue;
					
					if ( dot > 0.0f ) {
						// surface is a peak
						// last found a peak, so only take farther hits UNLESS they are valleys
						if (( bestFloorDot > 0.0f ) && ( dot > 0.0f ) && ( time < bestFloorTime )) continue; 
					}
					else {
						// surface is a valley
						// last found a valley, so only take nearer hits
						// remember: after finding any valley, all peaks will be ignored
						if (( bestFloorDot < 0.0f ) && ( time > bestFloorTime )) continue; 
					}
				}
				
				bridgeTime = time;
				bestFloorTime = time;
				bestFloorDot = dot;
				floorHit = &surface;
			}
		}
		else if ( IS_CEILING ( surface )) {
		
			if ( dot >= 0.0f ) continue; // don't care about ceilings behind us
			
			if ( !surface.GetMoveHit ( head, move, -0.0001f, time )) continue; // ignore surface w/ no valid hit
			
			if ( time <= bestCeilingTime ) {
				bestCeilingTime = time;
				ceilingHit = &surface;
			}
		}
	}
	
	// if we hit a ceiling before a floor clear out the floor hit
	// also make assert that we have a ceiling hit!
	if ( bestCeilingTime < bestFloorTime ) {
		assert ( ceilingHit );
		floorHit = 0;
	}
	
	if ( floorHit ) {
		
		//printf ( "hit: %p (%f, %f) %f %f\n", hit, hit->mNorm.mX, hit->mNorm.mY, bestTime, bestDot );
		
		if ( bestFloorTime > 0.0f ) {
			
			move.Scale ( bestFloorTime );
			this->mLoc.Add ( move );
			
			float remaining = 1.0f - bestFloorTime;
			float shove = this->mShoveDistOnFloor / moveDist;
			if (( shove > 0.0f ) && ( bestFloorTime > shove )) {
				remaining = ( 1.0f - bestFloorTime ) / ( 1.0f - shove );
				
			}
			this->mMove.Scale ( remaining );
		}
		
		float prevFloorMoveDist = this->mMoveDistOnFloor;
		this->SetFloor ( *floorHit );
		
		// if we hit a ceiling at the same time as the floor then abort
		// if next floor reverses sign of last projected move then abort
		if (( bestCeilingTime <= bestFloorTime ) && (( prevFloorMoveDist >= 0.0f ) != ( this->mMoveDistOnFloor >= 0.0f ))) {
			this->mState = STATE_DONE;
		}
	}
	else if ( ceilingHit ) {
		printf ( "HIT A CEILING - STOPPING\n" );
		move.Scale ( bestCeilingTime );
		this->mLoc.Add ( move );
		this->mState = STATE_DONE;
	}
	else {
		this->mLoc.Add ( move );
		this->mState = STATE_DONE;
	}
}

//----------------------------------------------------------------//
void MOAIPlatformerFsm2D::DoWallSnapInAir () {

	this->CalculateWallDepthInAir ();
	this->CalculateWallShoveInAir ();
	
	if ( this->mShoveDistInAir ) {
		this->mLoc.mX += this->mShoveDistInAir;
	}
}

//----------------------------------------------------------------//
void MOAIPlatformerFsm2D::DoVerticalSnap () {

	float bestFloorSnapDist = -1.001f;
	const MOAISurface2D* bestFloor = 0;
	
	float bestCeilingSnapDist = 1.001f;
	const MOAISurface2D* bestCeiling = 0;

	for ( u32 i = 0; i < this->mSurfaceBuffer.mTop; ++i ) {
		
		const MOAISurface2D& surface = this->mSurfaceBuffer.mSurfaces [ i ];
		
		if ( IS_WALL ( surface )) continue; // not a floor or ceiling (TODO: pass in floor cos)
		if (( this->mLoc.mX < ( surface.mXMin - 0.0001f )) || ( this->mLoc.mX > ( surface.mXMax + 0.0001f ))) continue; // loc not over surface
		if ( surface.IsLeaving ( this->mLoc, this->mMove, -0.0001f )) continue;
		
		float dist;
		if ( surface.GetRayHit ( this->mLoc, this->mUp, dist ))  {
			
			if ( IS_FLOOR ( surface ) && ( dist <= 0.0f ) && ( dist >= bestFloorSnapDist )) {
				if ( bestFloor && ( dist == bestFloorSnapDist )) {
					
					// we have multiple floors to choose from so break the tie
					// if there's a this->mMove, choose the surface with the steepest angle
					// (against the this->mMove)
					if ( this->mMove.mX > 0.0f ) {
						if ( surface.mNorm.mX < bestFloor->mNorm.mX ) {
							bestFloor = &surface;
						}
					}
					else if ( this->mMove.mX < 0.0f ) {
						if ( surface.mNorm.mX > bestFloor->mNorm.mX ) {
							bestFloor = &surface;
						}
					}
				}
				else {
				
					// first floor, so go with it
					bestFloor = &surface;
					bestFloorSnapDist = dist;
				}
			}
			else if (( dist >= 0.0f ) && ( dist < bestCeilingSnapDist )) {
				// just take the lowest ceiling - don't care about tie-breaking
				bestCeiling = &surface;
				bestCeilingSnapDist = dist;
			}
		}
	}
	
	if ( bestFloor ) {
	
		bestFloorSnapDist = 1.0f + bestFloorSnapDist;
		this->mLoc.mY += bestFloorSnapDist;
		this->SetFloor ( *bestFloor );
		
		if ( bestCeiling ) {
			this->SetCeiling ( *bestCeiling );
		}
	}
	else if ( bestCeiling ) {
	
		bestCeilingSnapDist = 1.0f - bestCeilingSnapDist;
		this->mLoc.mY -= bestCeilingSnapDist;
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

	printf ( "loc: (%f, %f) move: (%f, %f)\n", body.mLoc.mX, body.mLoc.mY, body.mMove.mX, body.mMove.mY );

	this->mCeilCos = body.mCeilCos;
	this->mFloorCos = body.mFloorCos;
	
	this->mLoc.Init ( 0.0f, 0.0f );
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

	this->DoVerticalSnap ();
	this->mState = this->mFloor ? STATE_ON_FLOOR : STATE_IN_AIR;

	// detach (this should be based on dot with 'up' vector being nonzero instead of just 'y' being positive
	if (( this->mState == STATE_ON_FLOOR ) && ( this->mMove.Dot ( this->mFloorNorm ) > 0.0001f )) {
		if (( body.mDetachMode == MOAIPlatformerBody2D::DETACH_ON_ANY ) || (( body.mDetachMode == MOAIPlatformerBody2D::DETACH_ON_UP ) && ( this->mMove.Dot ( this->mUp ) > 0.0001f ))) {
			this->mState = STATE_IN_AIR;
		}
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
	
	//if ( this->mFloor ) {
	//	float dist;
	//	this->mFloor->GetRayHit ( this->mLoc, this->mUp, dist );
	//	printf ( "dist: %f\n", dist );
	//}
	
	body.mLoc.mX += this->mLoc.mX * body.mHRad;
	body.mLoc.mY += this->mLoc.mY * body.mVRad;
	body.mMove.Init ( 0.0f, 0.0f );
	
	body.mSteps = steps + 1;
	body.mCompleted = this->mState == STATE_DONE;
}

//----------------------------------------------------------------//
void MOAIPlatformerFsm2D::SetCeiling ( const MOAISurface2D& ceiling ) {

	if ((( this->mMove.mX < 0.0f ) && ( ceiling.mNorm.mX > 0.0f )) || (( this->mMove.mX > 0.0f ) && ( ceiling.mNorm.mX < 0.0f ))) {
		this->mMove.mX = 0.0f;
		this->mMoveDistOnFloor = 0.0f;
	}
}

//----------------------------------------------------------------//
void MOAIPlatformerFsm2D::SetFloor ( const MOAISurface2D& floor ) {
	
	//printf ( "FLOOR: %p\n", &floor );
	
	this->mFloor = &floor;
	this->mFloorNorm = floor.mNorm;
	this->mFloorTangent = floor.mTangent;
	this->mMoveDistOnFloor = this->mMove.Dot ( this->mFloorTangent );
}

