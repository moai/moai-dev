// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIPartition.h>
#include <moaicore/MOAIPartitionCell.h>
#include <moaicore/MOAIPartitionLayer.h>
#include <moaicore/MOAIPartitionResultBuffer.h>
#include <moaicore/MOAIProp.h>

//================================================================//
// MOAIPartitionCell
//================================================================//

//----------------------------------------------------------------//
void MOAIPartitionCell::Clear () {

	PrimIt primIt = this->mPrims.Head ();
	while ( primIt ) {
		MOAIProp* prop = primIt->Data ();
		primIt = primIt->Next ();
		prop->SetPartition ( 0 );
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::GatherProps ( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, u32 mask ) {
	
	PrimIt primIt = this->mPrims.Head ();
	for ( ; primIt; primIt = primIt->Next ()) {
		MOAIProp* prop = primIt->Data ();
		
		if ( prop == ignore ) continue;
		
		if (( mask == 0 ) || ( prop->mMask & mask )) {
			results.PushProp ( *prop );
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::GatherProps ( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, const USVec3D& point, u32 mask ) {

	USRect bounds;

	PrimIt primIt = this->mPrims.Head ();
	for ( ; primIt; primIt = primIt->Next ()) {
		MOAIProp* prop = primIt->Data ();
		
		if ( prop == ignore ) continue;
		
		if (( mask == 0 ) || ( prop->mMask & mask )) {
			if ( prop->mCellSize > 0.0f ) {
				
				bounds = prop->mBounds.GetRect ( USBox::PLANE_XY );
				
				if ( bounds.Contains ( point )) {
					if ( prop->Inside ( point, 0.0f )) {
						results.PushProp ( *prop );
					}
				}
			}
			else {
				results.PushProp ( *prop );
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::GatherProps ( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, const USRect& rect, u32 mask ) {

	USRect bounds;

	PrimIt primIt = this->mPrims.Head ();
	for ( ; primIt; primIt = primIt->Next ()) {
		MOAIProp* prop = primIt->Data ();
		
		if ( prop == ignore ) continue;
		
		if (( mask == 0 ) || ( prop->mMask & mask )) {
			if ( prop->mCellSize > 0.0f ) {
				
				bounds = prop->mBounds.GetRect ( USBox::PLANE_XY );
				
				if ( bounds.Overlap ( rect )) {
					results.PushProp ( *prop );
				}
			}
			else {
				results.PushProp ( *prop );
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::GatherProps ( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, const USFrustum& frustum, u32 mask ) {

	PrimIt primIt = this->mPrims.Head ();
	for ( ; primIt; primIt = primIt->Next ()) {
		MOAIProp* prop = primIt->Data ();
		
		if ( prop == ignore ) continue;
		
		if (( mask == 0 ) || ( prop->mMask & mask )) {
			if ( prop->mCellSize > 0.0f ) {
				
				if ( !frustum.Cull ( prop->mBounds )) {
					results.PushProp ( *prop );
				}
			}
			else {
				results.PushProp ( *prop );
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::InsertProp ( MOAIProp& prop ) {

	if ( prop.mCell == this ) return;

	if ( prop.mCell ) {
		prop.mCell->RemoveProp ( prop );
	}
	this->mPrims.PushBack ( prop.mLinkInCell );
	prop.mCell = this;
}

//----------------------------------------------------------------//
MOAIPartitionCell::MOAIPartitionCell () {
}

//----------------------------------------------------------------//
MOAIPartitionCell::~MOAIPartitionCell () {
	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIPartitionCell::RemoveProp ( MOAIProp& prop ) {

	if ( prop.mCell != this ) return;
	
	this->mPrims.Remove ( prop.mLinkInCell );
	prop.mCell = 0;
}

