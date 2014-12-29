// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionCell.h>
#include <moai-sim/MOAIPartitionLevel.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIProp.h>

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
void MOAIPartitionCell::ExtractProps ( MOAIPartitionCell& cell, MOAIPartitionLevel* level ) {

	if ( &cell != this ) {
	
		PropIt propIt = this->mProps.Head ();
		for ( ; propIt; propIt = propIt->Next ()) {
			MOAIProp* prop = propIt->Data ();
			prop->mCell = &cell;
			prop->mLevel = level;
		}
	
		cell.mProps.Join ( cell.mProps, this->mProps );
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::GatherProps ( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, const ZLVec3D& point, const ZLVec3D& orientation, u32 interfaceMask, u32 queryMask ) {
	PropIt propIt = this->mProps.Head ();
	for ( ; propIt; propIt = propIt->Next ()) {
		MOAIProp* prop = propIt->Data ();
		
		float t;
		if (( prop != ignore ) && ( prop->mInterfaceMask & interfaceMask ) && ( prop->mQueryMask & queryMask )) {
			if ( !ZLSect::RayToBox( prop->mWorldBounds, point, orientation, t )) {
				prop->AddToSortBuffer ( results, ZLFloat::FloatToIntKey ( t ));
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::GatherProps ( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, u32 interfaceMask, u32 queryMask ) {
	
	PropIt propIt = this->mProps.Head ();
	for ( ; propIt; propIt = propIt->Next ()) {
		MOAIProp* prop = propIt->Data ();
		
		if (( prop != ignore ) && ( prop->mInterfaceMask & interfaceMask ) && ( prop->mQueryMask & queryMask )) {
			prop->AddToSortBuffer ( results );
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::GatherProps ( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, const ZLVec3D& point, u32 interfaceMask, u32 queryMask ) {

	PropIt propIt = this->mProps.Head ();
	for ( ; propIt; propIt = propIt->Next ()) {
		MOAIProp* prop = propIt->Data ();
		
		if (( prop != ignore ) && ( prop->mInterfaceMask & interfaceMask ) && ( prop->mQueryMask & queryMask )) {
			if ( prop->mWorldBounds.Contains ( point )) {
				if ( prop->Inside ( point, 0.0f )) {
					prop->AddToSortBuffer ( results );
				}
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::GatherProps ( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, const ZLBox& box, u32 interfaceMask, u32 queryMask ) {

	PropIt propIt = this->mProps.Head ();
	for ( ; propIt; propIt = propIt->Next ()) {
		MOAIProp* prop = propIt->Data ();
		
		if (( prop != ignore ) && ( prop->mInterfaceMask & interfaceMask ) && ( prop->mQueryMask & queryMask )) {
			if ( prop->mWorldBounds.Overlap ( box )) {
				prop->AddToSortBuffer ( results );
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionCell::GatherProps ( MOAIPartitionResultBuffer& results, const MOAIProp* ignore, const ZLFrustum& frustum, u32 interfaceMask, u32 queryMask ) {

	PropIt propIt = this->mProps.Head ();
	for ( ; propIt; propIt = propIt->Next ()) {
		MOAIProp* prop = propIt->Data ();
		
		if (( prop != ignore ) && ( prop->mInterfaceMask & interfaceMask ) && ( prop->mQueryMask & queryMask )) {
			if ( !frustum.Cull ( prop->mWorldBounds )) {
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