// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAnimChannel.h>
#include <moai-sim/MOAIAttribute.h>

//================================================================//
// MOAIAnimChannel
//================================================================//
	
//----------------------------------------------------------------//
void MOAIAnimChannel::Accumulate ( const ZLColorVec& value, float opacity ) {

	if (( opacity > 0.0f ) && ( this->mType == CHANNEL_TYPE_COLOR )) {
	
		assert ( this->mBuffer );
		ZLColorVec& result = *( ZLColorVec* )this->mBuffer;
	
		if ( opacity < 1.0f ) {
			result.Lerp ( ZLInterpolate::kLinear, result, value, opacity );
		}
		else {
			result = value;
		}
	}
}

//----------------------------------------------------------------//
void MOAIAnimChannel::Accumulate ( const float& value, float opacity ) {

	if (( opacity > 0.0f ) && ( this->mType == CHANNEL_TYPE_FLOAT )) {
	
		assert ( this->mBuffer );
		float& result = *( float* )this->mBuffer;
		result = opacity < 1.0f ? ZLInterpolate::Interpolate ( ZLInterpolate::kLinear, result, value, opacity ) : value;
	}
}

//----------------------------------------------------------------//
void MOAIAnimChannel::Accumulate ( const ZLQuaternion& value, float opacity ) {

	if (( opacity > 0.0f ) && ( this->mType == CHANNEL_TYPE_COLOR )) {
		
		assert ( this->mBuffer );
		ZLQuaternion& result = *( ZLQuaternion* )this->mBuffer;
		
		if ( opacity < 1.0f ) {
			result.Slerp ( result, value, opacity );
		}
		else {
			result = value;
		}
	}
}

//----------------------------------------------------------------//
void MOAIAnimChannel::Accumulate ( const ZLVec3D& value, float opacity ) {

	if (( opacity > 0.0f ) && ( this->mType == CHANNEL_TYPE_COLOR )) {
		
		assert ( this->mBuffer );
		ZLVec3D& result = *( ZLVec3D* )this->mBuffer;
		
		if ( opacity < 1.0f ) {
			result.mX = ZLInterpolate::Interpolate ( ZLInterpolate::kLinear, result.mX, value.mX, opacity );
			result.mY = ZLInterpolate::Interpolate ( ZLInterpolate::kLinear, result.mY, value.mY, opacity );
			result.mZ = ZLInterpolate::Interpolate ( ZLInterpolate::kLinear, result.mZ, value.mZ, opacity );
		}
		else {
			result = value;
		}
	}
}

//----------------------------------------------------------------//
size_t MOAIAnimChannel::GetSize () {

	switch ( this->mType ) {
		case CHANNEL_TYPE_COLOR:	return sizeof ( ZLColorVec );
		case CHANNEL_TYPE_FLOAT:	return sizeof ( float );
		case CHANNEL_TYPE_QUAT:		return sizeof ( ZLQuaternion );
		case CHANNEL_TYPE_VEC:		return sizeof ( ZLVec3D );
		case CHANNEL_TYPE_UNKNOWN:
		default:
			return 0;
	}
	return 0;
}

//----------------------------------------------------------------//
bool MOAIAnimChannel::GetValue ( MOAIAttribute& attr ) {

	switch ( this->mType ) {
	
		case CHANNEL_TYPE_COLOR:
			attr.SetValue ( *( ZLColorVec* )this->mBuffer );
			return true;
			
		case CHANNEL_TYPE_FLOAT:
			attr.SetValue ( *( float* )this->mBuffer );
			return true;
		
		case CHANNEL_TYPE_QUAT:
			attr.SetValue ( *( ZLQuaternion* )this->mBuffer );
			return true;
		
		case CHANNEL_TYPE_VEC:
			attr.SetValue ( *( ZLVec3D* )this->mBuffer );
			return true;
	}
	return false;
}

//----------------------------------------------------------------//
MOAIAnimChannel::MOAIAnimChannel () :
	mType ( CHANNEL_TYPE_UNKNOWN ),
	mBuffer ( 0 ) {
}

//----------------------------------------------------------------//
MOAIAnimChannel::~MOAIAnimChannel () {
}
