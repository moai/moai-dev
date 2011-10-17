// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIAttrOp.h>

//================================================================//
// MOAIAttrOp
//================================================================//

//----------------------------------------------------------------//
template <>
bool MOAIAttrOp::GetValue < bool >() {

	return this->mType == TYPE_NUMBER ? ( this->mNumber == 0.0f ? false : true ) : false;
}

//----------------------------------------------------------------//
template <>
float MOAIAttrOp::GetValue < float >() {

	return this->mType == TYPE_NUMBER ? this->mNumber : 0.0f;
}

//----------------------------------------------------------------//
template <>
int MOAIAttrOp::GetValue < int >() {

	return this->mType == TYPE_NUMBER ? USFloat::ToInt ( this->mNumber ) : 0;
}

//----------------------------------------------------------------//
template <>
u32 MOAIAttrOp::GetValue < u32 >() {

	return this->mType == TYPE_NUMBER ? ( u32 )USFloat::ToInt ( this->mNumber ) : 0;
}
