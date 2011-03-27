// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAINodeHarness.h>
#include <moaicore/MOAITransformBase2D.h>

//================================================================//
// MOAITransformBase2D
//================================================================//

//----------------------------------------------------------------//
const USAffine2D& MOAITransformBase2D::GetLocalToWorldMtx () {

	return this->mLocalToWorldMtx;
}

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
	return repr;
}
