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

	PropIt propIt = this->mProps.Head ();
	while ( propIt ) {
		MOAIProp* prop = propIt->Data ();
		propIt = propIt->Next ();
		prop->SetPartition ( 0 );
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::ExtractProps ( USLeanList < MOAIProp* >& props ) {

	props.Join ( props, this->mProps );
}

//----------------------------------------------------------------//
void MOAIPartitionCell::GatherProps ( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, u32 mask ) {
	
	PropIt propIt = this->mProps.Head ();
	for ( ; propIt; propIt = propIt->Next ()) {
		MOAIProp* prop = propIt->Data ();
		
		if ( prop == ignore ) continue;
		
		if (( mask == 0 ) || ( prop->mMask & mask )) {
			results.PushProp ( *prop );
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::GatherProps ( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, const USVec3D& point, u32 mask ) {

	USRect bounds;

	PropIt propIt = this->mProps.Head ();
	for ( ; propIt; propIt = propIt->Next ()) {
		MOAIProp* prop = propIt->Data ();
		
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

	PropIt propIt = this->mProps.Head ();
	for ( ; propIt; propIt = propIt->Next ()) {
		MOAIProp* prop = propIt->Data ();
		
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

	PropIt propIt = this->mProps.Head ();
	for ( ; propIt; propIt = propIt->Next ()) {
		MOAIProp* prop = propIt->Data ();
		
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
	this->mProps.PushBack ( prop.mLinkInCell );
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
	
	this->mProps.Remove ( prop.mLinkInCell );
	prop.mCell = 0;
}

//----------------------------------------------------------------//
void MOAIPartitionCell::ScheduleProps () {

	PropIt propIt = this->mProps.Head ();
	for ( ; propIt; propIt = propIt->Next ()) {
		MOAIProp* prop = propIt->Data ();
		prop->ScheduleUpdate ();
	}
}