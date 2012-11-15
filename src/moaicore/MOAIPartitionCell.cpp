// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIPartition.h>
#include <moaicore/MOAIPartitionCell.h>
#include <moaicore/MOAIPartitionLevel.h>
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
void MOAIPartitionCell::ExtractProps ( MOAIPartitionCell& cell, MOAIPartitionLevel* layer ) {

	if ( &cell != this ) {
	
		PropIt propIt = this->mProps.Head ();
		for ( ; propIt; propIt = propIt->Next ()) {
			MOAIProp* prop = propIt->Data ();
			prop->mCell = &cell;
			prop->mLayer = layer;
		}
	
		cell.mProps.Join ( cell.mProps, this->mProps );
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::GatherProps ( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, const USVec3D& point, const USVec3D& orientation, u32 mask ) {
	PropIt propIt = this->mProps.Head ();
	for ( ; propIt; propIt = propIt->Next ()) {
		MOAIProp* prop = propIt->Data ();
		
		if ( prop == ignore ) continue;
		
		float t;
		if (( mask == 0 ) || ( prop->mMask & mask )) {
			if ( !USSect::RayToBox( prop->mBounds, point, orientation, t )) {
				prop->AddToSortBuffer ( results, USFloat::FloatToIntKey ( t ));
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::GatherProps ( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, u32 mask ) {
	
	PropIt propIt = this->mProps.Head ();
	for ( ; propIt; propIt = propIt->Next ()) {
		MOAIProp* prop = propIt->Data ();
		
		if ( prop == ignore ) continue;
		
		if (( mask == 0 ) || ( prop->mMask & mask )) {
			prop->AddToSortBuffer ( results );
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::GatherProps ( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, const USVec3D& point, u32 mask ) {

	PropIt propIt = this->mProps.Head ();
	for ( ; propIt; propIt = propIt->Next ()) {
		MOAIProp* prop = propIt->Data ();
		
		if ( prop == ignore ) continue;
		
		if (( mask == 0 ) || ( prop->mMask & mask )) {
			if ( prop->mBounds.Contains ( point )) {
				if ( prop->Inside ( point, 0.0f )) {
					prop->AddToSortBuffer ( results );
				}
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::GatherProps ( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, const USBox& box, u32 mask ) {

	PropIt propIt = this->mProps.Head ();
	for ( ; propIt; propIt = propIt->Next ()) {
		MOAIProp* prop = propIt->Data ();
		
		if ( prop == ignore ) continue;
		
		if (( mask == 0 ) || ( prop->mMask & mask )) {	
			if ( prop->mBounds.Overlap ( box )) {
				prop->AddToSortBuffer ( results );
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
			if ( !frustum.Cull ( prop->mBounds )) {
				prop->AddToSortBuffer ( results );
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