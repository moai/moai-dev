// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionCell.h>
#include <moai-sim/MOAIPartitionLevel.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIPartitionHull.h>

//================================================================//
// MOAIPartitionCell
//================================================================//

//----------------------------------------------------------------//
void MOAIPartitionCell::Clear () {

	HullIt propIt = this->mHulls.Head ();
	while ( propIt ) {
		MOAIPartitionHull* hull = propIt->Data ();
		propIt = propIt->Next ();
		hull->SetPartition ( 0 );
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::ExtractProps ( MOAIPartitionCell& cell, MOAIPartitionLevel* level ) {

	if ( &cell != this ) {
	
		HullIt propIt = this->mHulls.Head ();
		for ( ; propIt; propIt = propIt->Next ()) {
			MOAIPartitionHull* hull = propIt->Data ();
			hull->mCell = &cell;
			hull->mLevel = level;
		}
	
		cell.mHulls.Join ( cell.mHulls, this->mHulls );
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::GatherHulls ( MOAIPartitionResultBuffer& results, const MOAIPartitionHull* ignore, u32 interfaceMask, u32 queryMask ) {
	
	HullIt propIt = this->mHulls.Head ();
	for ( ; propIt; propIt = propIt->Next ()) {
		MOAIPartitionHull* hull = propIt->Data ();
		
		if (( hull != ignore ) && ( hull->mInterfaceMask & interfaceMask ) && ( hull->mQueryMask & queryMask )) {
			hull->AddToSortBuffer ( results );
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::GatherHulls ( MOAIPartitionResultBuffer& results, const MOAIPartitionHull* ignore, const ZLVec3D& point, u32 interfaceMask, u32 queryMask ) {

	HullIt propIt = this->mHulls.Head ();
	for ( ; propIt; propIt = propIt->Next ()) {
		MOAIPartitionHull* hull = propIt->Data ();
		
		if (( hull != ignore ) && ( hull->mInterfaceMask & interfaceMask ) && ( hull->mQueryMask & queryMask )) {
			if ( hull->mWorldBounds.mAABB.Contains ( point )) {
				if ( hull->Inside ( point, 0.0f )) {
					hull->AddToSortBuffer ( results );
				}
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::GatherHulls ( MOAIPartitionResultBuffer& results, const MOAIPartitionHull* ignore, const ZLVec3D& point, const ZLVec3D& orientation, u32 interfaceMask, u32 queryMask ) {
	HullIt propIt = this->mHulls.Head ();
	for ( ; propIt; propIt = propIt->Next ()) {
		MOAIPartitionHull* hull = propIt->Data ();
		
		float t;
		if (( hull != ignore ) && ( hull->mInterfaceMask & interfaceMask ) && ( hull->mQueryMask & queryMask )) {
			if ( !ZLSect::RayToBox( hull->mWorldBounds.mAABB, point, orientation, t )) {
				hull->AddToSortBuffer ( results, ZLFloat::FloatToIntKey ( t ));
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::GatherHulls ( MOAIPartitionResultBuffer& results, const MOAIPartitionHull* ignore, const ZLRect& rect, u32 interfaceMask, u32 queryMask ) {

	HullIt propIt = this->mHulls.Head ();
	for ( ; propIt; propIt = propIt->Next ()) {
		MOAIPartitionHull* hull = propIt->Data ();
		
		if (( hull != ignore ) && ( hull->mInterfaceMask & interfaceMask ) && ( hull->mQueryMask & queryMask )) {
			ZLRect bounds = hull->mWorldBounds.mAABB.GetRect ( ZLBox::PLANE_XY );
			if ( bounds.Overlap ( rect )) {
				hull->AddToSortBuffer ( results );
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::GatherHulls ( MOAIPartitionResultBuffer& results, const MOAIPartitionHull* ignore, const ZLBox& box, u32 interfaceMask, u32 queryMask ) {

	HullIt propIt = this->mHulls.Head ();
	for ( ; propIt; propIt = propIt->Next ()) {
		MOAIPartitionHull* hull = propIt->Data ();
		
		if (( hull != ignore ) && ( hull->mInterfaceMask & interfaceMask ) && ( hull->mQueryMask & queryMask )) {
			if ( hull->mWorldBounds.mAABB.Overlap ( box )) {
				hull->AddToSortBuffer ( results );
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::GatherHulls ( MOAIPartitionResultBuffer& results, const MOAIPartitionHull* ignore, const ZLFrustum& frustum, u32 interfaceMask, u32 queryMask ) {

	HullIt propIt = this->mHulls.Head ();
	for ( ; propIt; propIt = propIt->Next ()) {
		MOAIPartitionHull* hull = propIt->Data ();
		
		if (( hull != ignore ) && ( hull->mInterfaceMask & interfaceMask ) && ( hull->mQueryMask & queryMask )) {
			if ( !frustum.Cull ( hull->mWorldBounds.mAABB )) {
				hull->AddToSortBuffer ( results );
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::InsertHull ( MOAIPartitionHull& hull ) {

	if ( hull.mCell == this ) return;

	if ( hull.mCell ) {
		hull.mCell->RemoveHull ( hull );
	}
	this->mHulls.PushBack ( hull.mLinkInCell );
	hull.mCell = this;
}

//----------------------------------------------------------------//
MOAIPartitionCell::MOAIPartitionCell () {
}

//----------------------------------------------------------------//
MOAIPartitionCell::~MOAIPartitionCell () {
	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIPartitionCell::RemoveHull ( MOAIPartitionHull& hull ) {

	if ( hull.mCell != this ) return;
	
	this->mHulls.Remove ( hull.mLinkInCell );
	hull.mCell = 0;
}

//----------------------------------------------------------------//
void MOAIPartitionCell::ScheduleProps () {

	HullIt propIt = this->mHulls.Head ();
	for ( ; propIt; propIt = propIt->Next ()) {
		MOAIPartitionHull* hull = propIt->Data ();
		hull->ScheduleUpdate ();
	}
}
