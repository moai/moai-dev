// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAINodeHarness.h>
#include <moaicore/MOAITransformBase2D.h>

//================================================================//
// MOAIDrawingMtx2D
//================================================================//

//----------------------------------------------------------------//
void MOAIDrawingMtx2D::Append ( const USAffine2D& mtx ) {

	this->mLocalToWorldMtx.Append ( mtx );
}

//----------------------------------------------------------------//
const USAffine2D& MOAIDrawingMtx2D::GetLocalToWorldMtx () {

	return this->mLocalToWorldMtx;
}

//----------------------------------------------------------------//
USAffine2D MOAIDrawingMtx2D::GetLocalToWorldMtxNoStretch () {

	USAffine2D mtx;
	mtx.Scale ( 1.0f / this->mStretch.mX, 1.0f / this->mStretch.mY );
	
	mtx.Append ( this->mLocalToWorldMtx );
	return mtx;
}

//----------------------------------------------------------------//
MOAIDrawingMtx2D::MOAIDrawingMtx2D () :
	mStretch ( 1.0f, 1.0f ) {
}

//----------------------------------------------------------------//
MOAIDrawingMtx2D::~MOAIDrawingMtx2D () {
}

//----------------------------------------------------------------//
void MOAIDrawingMtx2D::SetLocalToWorldMtx ( const USAffine2D& localToWorldMtx ) {

	this->mLocalToWorldMtx = localToWorldMtx;
}

//================================================================//
// MOAITransformBase2D
//================================================================//

//----------------------------------------------------------------//
const USAffine2D& MOAITransformBase2D::GetWorldToLocalMtx () {

	return this->mWorldToLocalMtx;
}

//----------------------------------------------------------------//
MOAITransformBase2D::MOAITransformBase2D () {
	
	RTTI_SINGLE ( MOAINode )
	
	this->mLocalToWorldMtx.Ident ();
	this->mWorldToLocalMtx.Ident ();
}

//----------------------------------------------------------------//
MOAITransformBase2D::~MOAITransformBase2D () {
}

//----------------------------------------------------------------//
STLString MOAITransformBase2D::ToString () {

	STLString repr;
	PRETTY_PRINT ( repr, mStretch )
	return repr;
}
