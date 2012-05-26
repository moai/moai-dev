// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIAnimSample.h>
#include <moaicore/MOAINode.h>

#define AS_TYPE(type) ( *( type* )this->mBuffer )

#define SETUP_VAL(type, s0, s1)				\
	type v0 = *( type* )s0.mBuffer;			\
	type v1 = *( type* )s1.mBuffer;			\
	type& r = *( type* )this->mBuffer;

//================================================================//
// MOAIAnimSample
//================================================================//

//----------------------------------------------------------------//
void MOAIAnimSample::Add ( const MOAIAnimSample& s0, const MOAIAnimSample& s1 ) {

	assert ( s0.mType == s1.mType );
	this->mType = s0.mType;

	switch ( this->mType ) {
		case TYPE_FLOAT: {
			SETUP_VAL ( float, s0, s1 )
			r = v0 + v1;
			break;
		}
		case TYPE_VEC2: {
			SETUP_VAL ( USVec2D, s0, s1 )
			r = v0;
			r.Add ( v1 );
			break;
		}
		case TYPE_VEC3: {
			SETUP_VAL ( USVec3D, s0, s1 )
			r = v0;
			r.Add ( v1 );
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIAnimSample::Clear () {

	this->mType = TYPE_UNKNOWN;
	memset ( this->mBuffer, 0, this->mSize );
}

//----------------------------------------------------------------//
bool MOAIAnimSample::Equals ( const MOAIAnimSample& s0 ) {

	return ( memcmp ( this->mBuffer, s0.mBuffer, this->mSize ) == 0 );
}

//----------------------------------------------------------------//
void MOAIAnimSample::GetAttrOp ( MOAIAttrOp& attrOp ) {

	switch ( this->mType ) {
		
		//case TYPE_BOOL:
		
		case TYPE_FLOAT:
			attrOp.SetValue ( AS_TYPE ( float ));
			break;
		
		//case TYPE_INDEX:
		//case TYPE_INT:
		//case TYPE_QUATERNION:
		//case TYPE_TRANSFORM_2D:
		//case TYPE_TRANSFORM_3D:
		
		case TYPE_VEC2:
			attrOp.SetValue ( AS_TYPE ( USVec2D ));
			break;
			
		case TYPE_VEC3:
			attrOp.SetValue ( AS_TYPE ( USVec3D ));
			break;
	}
}

//----------------------------------------------------------------//
void MOAIAnimSample::GetBuffer ( void* buffer ) const {

	memcpy ( buffer, this->mBuffer, this->mSize );
}

//----------------------------------------------------------------//
size_t MOAIAnimSample::GetSize ( u32 type ) {

	switch ( type ) {
		//case TYPE_BOOL:				return sizeof ( u8 );
		case TYPE_FLOAT:			return sizeof ( float );
		//case TYPE_INDEX:			return sizeof ( u32 );
		//case TYPE_INT:				return sizeof ( s32 );
		//case TYPE_QUATERNION:
		//case TYPE_TRANSFORM_2D:
		//case TYPE_TRANSFORM_3D:
		case TYPE_VEC2:				return sizeof ( USVec2D );
		case TYPE_VEC3:				return sizeof ( USVec3D );
	}
	return 0;
}

//----------------------------------------------------------------//
void MOAIAnimSample::HandleAttrOp ( MOAIAttrOp& attrOp, u32 op ) {

	switch ( this->mType ) {
		
		case TYPE_FLOAT:
			AS_TYPE ( float ) = attrOp.Apply ( AS_TYPE ( float ), op, MOAINode::ATTR_READ_WRITE );
			break;
		
		case TYPE_VEC2:
			attrOp.Apply < USVec2D >( AS_TYPE ( USVec2D ), op, MOAINode::ATTR_READ );
			break;
		
		case TYPE_VEC3:
			attrOp.Apply < USVec3D >( AS_TYPE ( USVec3D ), op, MOAINode::ATTR_READ );
			break;
	}
}

//----------------------------------------------------------------//
void MOAIAnimSample::Interpolate ( const MOAIAnimSample& s0, const MOAIAnimSample& s1, u32 mode, float t, float w ) {
	
	assert ( s0.mType == s1.mType );
	this->mType = s0.mType;
	
	switch ( this->mType ) {
		case TYPE_FLOAT: {
			SETUP_VAL ( float, s0, s1 )
			r = USInterpolate::Interpolate ( mode, v0, v1, t, w );
			break;
		}
		case TYPE_VEC2: {
			SETUP_VAL ( USVec2D, s0, s1 )
			r.mX = USInterpolate::Interpolate ( mode, v0.mX, v1.mX, t, w );
			r.mY = USInterpolate::Interpolate ( mode, v0.mY, v1.mY, t, w );
			break;
		}
		case TYPE_VEC3: {
			SETUP_VAL ( USVec3D, s0, s1 )
			r.mX = USInterpolate::Interpolate ( mode, v0.mX, v1.mX, t, w );
			r.mY = USInterpolate::Interpolate ( mode, v0.mY, v1.mY, t, w );
			r.mZ = USInterpolate::Interpolate ( mode, v0.mZ, v1.mZ, t, w );
			break;
		}
	}
}

//----------------------------------------------------------------//
u32 MOAIAnimSample::Push ( MOAILuaState& state ) {

	switch ( this->mType ) {
		case TYPE_FLOAT: {
			state.Push ( AS_TYPE ( float ));
			return 1;
		}
		case TYPE_VEC2: {
			const USVec2D& vec = AS_TYPE ( USVec2D );
			state.Push ( vec.mX );
			state.Push ( vec.mY );
			return 2;
		}
		case TYPE_VEC3: {
			const USVec3D& vec = AS_TYPE ( USVec3D );
			state.Push ( vec.mX );
			state.Push ( vec.mY );
			state.Push ( vec.mZ );
			return 3;
		}
	}

	return 0;
}

//----------------------------------------------------------------//
void MOAIAnimSample::Scale ( float s ) {

	switch ( this->mType ) {
		case TYPE_FLOAT: {
			AS_TYPE ( float ) *= s;
			break;
		}
		case TYPE_VEC2: {
			AS_TYPE ( USVec2D ).Scale ( s );
			break;
		}
		case TYPE_VEC3: {
			AS_TYPE ( USVec3D ).Scale ( s );
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIAnimSample::Set ( u32 type ) {

	this->mType = type;
	
	if ( type == TYPE_UNKNOWN ) {
		this->mSize = 0;
		memset ( this->mBuffer, 0, BUFFER_SIZE );
	}
	else {
		this->mSize = this->GetSize ( type );
		memset ( this->mBuffer, 0, this->mSize );
	}
}

//----------------------------------------------------------------//
void MOAIAnimSample::Set ( float value ) {

	this->mType = TYPE_FLOAT;
	this->mSize = sizeof ( float );
	AS_TYPE ( float ) = value;
}

//----------------------------------------------------------------//
void MOAIAnimSample::Set ( const void* buffer, u32 type ) {

	this->mType = type;
	this->mSize = this->GetSize ( type );
	memcpy ( this->mBuffer, buffer, this->mSize );
}

//----------------------------------------------------------------//
void MOAIAnimSample::Set ( MOAILuaState& state, int idx, u32 type ) {

	this->mType = type;
	this->mSize = this->GetSize ( type );

	switch ( this->mType ) {
		case TYPE_FLOAT: {
			AS_TYPE ( float ) = state.GetValue < float >( idx, 0.0f );
			break;
		}
		case TYPE_VEC2: {
			AS_TYPE ( USVec2D ) = state.GetVec2D < float >( idx );
			break;
		}
		case TYPE_VEC3: {
			AS_TYPE ( USVec3D ) = state.GetVec3D < float >( idx );
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIAnimSample::Sub ( const MOAIAnimSample& s0, const MOAIAnimSample& s1 ) {

	assert ( s0.mType == s1.mType );
	this->mType = s0.mType;

	switch ( this->mType ) {
		case TYPE_FLOAT: {
			SETUP_VAL ( float, s0, s1 )
			r = v0 - v1;
			break;
		}
		case TYPE_VEC2: {
			SETUP_VAL ( USVec2D, s0, s1 )
			r = v0;
			r.Sub ( v1 );
			break;
		}
		case TYPE_VEC3: {
			SETUP_VAL ( USVec3D, s0, s1 )
			r = v0;
			r.Sub ( v1 );
			break;
		}
	}
}
