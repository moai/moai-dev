// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USAttrOp.h>

//================================================================//
// USAttrOp
//================================================================//
	
//----------------------------------------------------------------//
template <>
bool USAttrOp::Get < bool > () {
	return ( this->mFloat == 0.0f ) ? false : true;
}

//----------------------------------------------------------------//
template <>
float USAttrOp::Get < float > () {
	return this->mFloat;
}

//----------------------------------------------------------------//
template <>
int USAttrOp::Get < int > () {
	return ( int )this->mFloat;
}

//----------------------------------------------------------------//
template <>
u32 USAttrOp::Get < u32 > () {
	return ( u32 )this->mFloat;
}

//----------------------------------------------------------------//
template <>
void USAttrOp::Set ( bool value ) {
	this->mFloat = value ? 1.0f : 0.0f;
	this->mType = TYPE_BOOLEAN;
}

//----------------------------------------------------------------//
template <>
void USAttrOp::Set ( float value ) {
	this->mFloat = value;
	this->mType = TYPE_FLOAT;
}

//----------------------------------------------------------------//
template <>
void USAttrOp::Set ( int value ) {
	this->mFloat = ( float )value;
	this->mType = TYPE_INT;
}

//----------------------------------------------------------------//
template <>
void USAttrOp::Set ( u32 value ) {
	this->mFloat = ( float )value;
	this->mType = TYPE_INDEX;
}
