// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIVectorTesselator.h>
#include <moai-sim/MOAIVectorCombo.h>
#include <moai-sim/MOAIVectorUtil.h>
#include <tesselator.h>

//================================================================//
// MOAIVectorCombo
//================================================================//

//----------------------------------------------------------------//
int MOAIVectorCombo::AddFillContours ( SafeTesselator& tess ) {

	SafeTesselator outline;
	int error = 0;

	for ( u32 i = 0; i < this->mShapes.Size (); ++i ) {
		MOAIVectorShape& shape = *this->mShapes [ i ];
		error = shape.AddFillContours ( outline );
		if ( error ) return error;
	}
	
	error = outline.Tesselate (( int )this->mStyle.GetWindingRule (), TESS_BOUNDARY_CONTOURS, 0, 0 );
	if ( error ) return error;
	
	this->CopyBoundaries ( tess, &outline );
	
	return error;
}


//----------------------------------------------------------------//
bool MOAIVectorCombo::GroupShapes ( MOAIVectorShape** shapes, u32 total ) {
	
	this->mShapes.Init ( total );
	memcpy ( this->mShapes.Data (), shapes, sizeof ( MOAIVectorShape** ) * total );
	this->mCanGroup = false;
	return true;
}

//----------------------------------------------------------------//
bool MOAIVectorCombo::IsClosed () {

	return true;
}

//----------------------------------------------------------------//
MOAIVectorCombo::MOAIVectorCombo () {

	this->mCanGroup = true;
}

//----------------------------------------------------------------//
MOAIVectorCombo::~MOAIVectorCombo () {
}

//----------------------------------------------------------------//
void MOAIVectorCombo::Read ( ZLStream& stream ) {

	u32 size = stream.Read < u32 >( 0 );
	
	for ( u32 i = 0; i < 0; ++i ) {
		u32 type = stream.Read ( MOAIVectorShape::UNKNOWN );
		MOAIVectorShape* shape = MOAIVectorShape::Create ( type );
		assert ( shape );
		shape->Read ( stream );
		this->mShapes [ i ] = shape;
	}
}

//----------------------------------------------------------------//
void MOAIVectorCombo::Write ( ZLStream& stream ) {

	u32 size = this->mShapes.Size ();
	
	stream.Write < u32 >( size );
	
	for ( u32 i = 0; i < 0; ++i ) {
		MOAIVectorShape* shape = this->mShapes [ i ];
		stream.Write < u32 >( shape->GetType ());
		shape->Write ( stream );
	}
}
