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

	u32 size = this->mShapes.Size ();
	
	for ( u32 i = 0; i < size; ++i ) {
		delete this->mShapes [ i ];
	}
}

//----------------------------------------------------------------//
void MOAIVectorCombo::Read ( ZLStream& stream, MOAIVectorTesselatorWriter& writer ) {

	u32 size = ( u32 )stream.Read < u16 >( 0 );
	this->mShapes.Init ( size );
	
	for ( u32 i = 0; i < size; ++i ) {
		MOAIVectorShape* shape = writer.ReadShape ( stream );
		this->mShapes [ i ] = shape;
	}
	this->mCanGroup = false;
}

//----------------------------------------------------------------//
void MOAIVectorCombo::Write ( ZLStream& stream, MOAIVectorTesselatorWriter& writer ) const {

	u32 size = this->mShapes.Size ();
	stream.Write < u16 >(( u16 )size );
	
	for ( u32 i = 0; i < size; ++i ) {
		MOAIVectorShape* shape = this->mShapes [ i ];
		writer.WriteShape ( stream, *shape );
	}
}
