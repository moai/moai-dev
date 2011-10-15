// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USAttrOp.h>

//================================================================//
// USAttrOp
//================================================================//

//----------------------------------------------------------------//
template <>
bool USAttrOp::GetValue < bool >() {

	return this->mType == TYPE_NUMBER ? ( this->mNumber == 0.0f ? false : true ) : false;
}

//----------------------------------------------------------------//
template <>
float USAttrOp::GetValue < float >() {

	return this->mType == TYPE_NUMBER ? this->mNumber : 0.0f;
}

//----------------------------------------------------------------//
template <>
int USAttrOp::GetValue < int >() {

	return this->mType == TYPE_NUMBER ? USFloat::ToInt ( this->mNumber ) : 0;
}

//----------------------------------------------------------------//
template <>
u32 USAttrOp::GetValue < u32 >() {

	return this->mType == TYPE_NUMBER ? ( u32 )USFloat::ToInt ( this->mNumber ) : 0;
}
