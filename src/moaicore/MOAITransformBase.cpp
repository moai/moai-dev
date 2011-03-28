// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAITransformBase.h>

//================================================================//
// MOAITransformBase
//================================================================//

//----------------------------------------------------------------//
const USAffine2D& MOAITransformBase::GetLocalToWorldMtx () {

	return this->mLocalToWorldMtx;
}

//----------------------------------------------------------------//
const USAffine2D& MOAITransformBase::GetWorldToLocalMtx () {

	return this->mWorldToLocalMtx;
}

//----------------------------------------------------------------//
MOAITransformBase::MOAITransformBase () {
	
	RTTI_SINGLE ( MOAINode )
	
	this->mLocalToWorldMtx.Ident ();
	this->mWorldToLocalMtx.Ident ();
}

//----------------------------------------------------------------//
MOAITransformBase::~MOAITransformBase () {
}

//----------------------------------------------------------------//
STLString MOAITransformBase::ToString () {

	STLString repr;
	return repr;
}
