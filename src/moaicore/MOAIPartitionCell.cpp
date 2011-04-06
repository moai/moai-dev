// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIPartition.h>
#include <moaicore/MOAIPartitionCell.h>
#include <moaicore/MOAIPartitionLayer.h>
#include <moaicore/MOAIProp.h>

//================================================================//
// MOAIPartitionCell
//================================================================//

//----------------------------------------------------------------//
void MOAIPartitionCell::GatherProps ( MOAIPartition& partition, MOAIProp* ignore, u32 mask ) {
	
	PrimIt primIt = this->mPrims.Head ();
	for ( ; primIt; primIt = primIt->Next ()) {
		MOAIProp* prop = primIt->Data ();
		
		if ( prop == ignore ) continue;
		
		if (( mask == 0 ) || ( prop->mMask & mask )) {
			partition.PushResult ( *prop );
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::GatherProps ( MOAIPartition& partition, MOAIProp* ignore, USVec2D& point, u32 mask ) {

	PrimIt primIt = this->mPrims.Head ();
	for ( ; primIt; primIt = primIt->Next ()) {
		MOAIProp* prop = primIt->Data ();
		
		if ( prop == ignore ) continue;
		
		if (( mask == 0 ) || ( prop->mMask & mask )) {
			if ( prop->mCellSize > 0.0f ) {
		
				if ( prop->mBounds.Contains ( point )) {
					partition.PushResult ( *prop );
				}
			}
			else {
				partition.PushResult ( *prop );
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::GatherProps ( MOAIPartition& partition, MOAIProp* ignore, USRect& rect, u32 mask ) {

	PrimIt primIt = this->mPrims.Head ();
	for ( ; primIt; primIt = primIt->Next ()) {
		MOAIProp* prop = primIt->Data ();
		
		if ( prop == ignore ) continue;
		
		if (( mask == 0 ) || ( prop->mMask & mask )) {
		
			if ( prop->mCellSize > 0.0f ) {
				
				if ( prop->mBounds.Overlap ( rect )) {
					partition.PushResult ( *prop );
				}
			}
			else {
				partition.PushResult ( *prop );
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
void MOAIPartitionCell::RemoveProp ( MOAIProp& prop ) {

	if ( prop.mCell != this ) return;
	
	this->mPrims.Remove ( prop.mLinkInCell );
	prop.mCell = 0;
}

//----------------------------------------------------------------//
MOAIPartitionCell::MOAIPartitionCell () {
}

//----------------------------------------------------------------//
MOAIPartitionCell::~MOAIPartitionCell () {
}
