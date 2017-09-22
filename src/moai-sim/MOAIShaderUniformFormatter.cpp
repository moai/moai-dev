// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAttribute.h>
#include <moai-sim/MOAIShaderUniformFormatter.h>

//================================================================//
// MOAIShaderUniformFormatter
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderUniformFormatter::AddValue ( void* buffer, const MOAIAttribute& attr ) const {

	switch ( attr.GetTypeID ()) {

		case MOAIAttribute::ATTR_TYPE_FLOAT_32: {

			float value = attr.GetValue ( 0.0f );

			if ( value != 0.0f ) {
				*( float* )buffer += value;
			}
			break;
		}
		case MOAIAttribute::ATTR_TYPE_INT_32: {

			int value = ( int )attr.GetValue ( 0 );

			if ( value != 0 ) {
				*( s32* )buffer += value;
			}
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniformFormatter::Default ( void* buffer, u32 count ) const {
	
	if ( this->mWidth == 9 ) {
	
		for ( u32 i = 0; i < count; ++i ) {
			this->SetValue ( this->GetArrayItem ( buffer, i ), ZLMatrix3x3::IDENT );
		}
	}
	else if ( this->mWidth == 16 ) {
	
		for ( u32 i = 0; i < count; ++i ) {
			this->SetValue ( this->GetArrayItem ( buffer, i ), ZLMatrix4x4::IDENT );
		}
	}
	else {
	
		memset ( buffer, 0, this->mWidth * count * ELEMENT_SIZE );
	}
}

//----------------------------------------------------------------//
void* MOAIShaderUniformFormatter::GetArrayItem ( void* buffer, u32 index ) const {

	return ( void* )(( size_t )buffer + ( this->mWidth * ELEMENT_SIZE * index ));
}

//----------------------------------------------------------------//
const void* MOAIShaderUniformFormatter::GetArrayItem ( const void* buffer, u32 index ) const {

	return ( const void* )(( size_t )buffer + ( this->mWidth * ELEMENT_SIZE * index ));
}

//----------------------------------------------------------------//
size_t MOAIShaderUniformFormatter::GetSize () const {

	return this->mWidth * ELEMENT_SIZE;
}

//----------------------------------------------------------------//
bool MOAIShaderUniformFormatter::Init ( u32 type, u32 width ) {
	
	this->mType		= type;
	this->mWidth	= width;
	
	return true;
}

//----------------------------------------------------------------//
MOAIShaderUniformFormatter::MOAIShaderUniformFormatter () :
	mType ( UNIFORM_TYPE_FLOAT ),
	mWidth ( 1 ) {
}

//----------------------------------------------------------------//
MOAIShaderUniformFormatter::~MOAIShaderUniformFormatter () {
}

//----------------------------------------------------------------//
void MOAIShaderUniformFormatter::SetRawValue ( void* buffer, const void* srcBuffer ) const {
	
	memcpy ( buffer, srcBuffer, this->mWidth * ELEMENT_SIZE );
}

//----------------------------------------------------------------//
void MOAIShaderUniformFormatter::SetValue ( void* buffer, const s32* srcBuffer ) const {

	if ( this->mType == UNIFORM_TYPE_FLOAT ) {
	
		float fBuffer [ MAX_BUFFER_WIDTH ];
		for ( size_t i = 0; i < this->mWidth; ++i ) {
		
			fBuffer [ i ] = ( float )srcBuffer [ i ];
		}
		return this->SetRawValue ( buffer, fBuffer );
	}
	return this->SetRawValue ( buffer, srcBuffer );
}

//----------------------------------------------------------------//
void MOAIShaderUniformFormatter::SetValue ( void* buffer, const float* srcBuffer ) const {

	if ( this->mType == UNIFORM_TYPE_INT ) {
	
		s32 iBuffer [ MAX_BUFFER_WIDTH ];
		for ( size_t i = 0; i < this->mWidth; ++i ) {
		
			iBuffer [ i ] = ( s32 )srcBuffer [ i ];
		}
		return this->SetRawValue ( buffer, iBuffer );
	}
	return this->SetRawValue ( buffer, srcBuffer );
}

//----------------------------------------------------------------//
void MOAIShaderUniformFormatter::SetValue ( void* buffer, float value ) const {

	if ( this->mType == MOAIShaderUniformFormatter::UNIFORM_TYPE_INT ) {
		this->SetValue ( buffer, ( s32 )value );
		return;
	}
	
	*( float* )buffer = value;
}

//----------------------------------------------------------------//
void MOAIShaderUniformFormatter::SetValue ( void* buffer, s32 value ) const {

	if ( this->mType == MOAIShaderUniformFormatter::UNIFORM_TYPE_FLOAT ) {
		this->SetValue ( buffer, ( float )value );
		return;
	}

	*( s32* )buffer = value;
}

//----------------------------------------------------------------//
void MOAIShaderUniformFormatter::SetValue ( void* buffer, const MOAIAttribute& attr ) const {

	switch ( attr.GetTypeID ()) {

		case MOAIAttribute::ATTR_TYPE_COLOR_VEC_4: {
			ZLColorVec color = attr.GetValue ( ZLColorVec::WHITE );
			this->SetValue ( buffer, color );
			break;
		}
		
		case MOAIAttribute::ATTR_TYPE_FLOAT_32: {
			this->SetValue ( buffer, ( float )attr.GetValue ( 0.0f ));
			break;
		}
		
		case MOAIAttribute::ATTR_TYPE_INT_32: {
			return this->SetValue ( buffer, ( int )attr.GetValue ( 0 ));
			break;
		}
		
		case MOAIAttribute::ATTR_TYPE_AFFINE_3D: {
			this->SetValue ( buffer, attr.GetValue ( ZLAffine3D::IDENT ));
			break;
		}
		
		case MOAIAttribute::ATTR_TYPE_MATRIX_3X3: {
			this->SetValue ( buffer, attr.GetValue ( ZLMatrix3x3::IDENT ));
			break;
		}
		
		case MOAIAttribute::ATTR_TYPE_MATRIX_4X4: {
			this->SetValue ( buffer, attr.GetValue ( ZLMatrix4x4::IDENT ));
			break;
		}
		
		case MOAIAttribute::ATTR_TYPE_VEC_3: {
			ZLVec3D vec3 = attr.GetValue ( ZLVec3D::ORIGIN );
			ZLVec4D vec4 ( vec3.mX, vec3.mY, vec3.mZ, 0.0f );
			this->SetValue ( buffer, vec4 );
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniformFormatter::SetValue ( void* buffer, const ZLColorVec& value ) const {

	if ( this->mWidth == 1 ) {
		this->SetValue ( buffer, value.mR ); // TODO: this right?
		return;
	}

	float m [ 4 ];

	m [ 0 ]		= value.mR;
	m [ 1 ]		= value.mG;
	m [ 2 ]		= value.mB;
	m [ 3 ]		= value.mA;

	this->SetValue ( buffer, m );
}

//----------------------------------------------------------------//
void MOAIShaderUniformFormatter::SetValue ( void* buffer, const ZLIntVec4D& value ) const {

	if ( this->mWidth == 1 ) {
		this->SetValue ( buffer, value.mX );
		return;
	}

	s32 m [ 4 ];
	
	m [ 0 ]		= value.mX;
	m [ 1 ]		= value.mY;
	m [ 2 ]		= value.mZ;
	m [ 3 ]		= value.mW;

	this->SetValue ( buffer, m );
}

//----------------------------------------------------------------//
void MOAIShaderUniformFormatter::SetValue ( void* buffer, const ZLAffine2D& value ) const {

	if ( this->mWidth == 9 ) {
	
		this->SetValue ( buffer, ZLMatrix3x3 ( value ));
	}
	else if ( this->mWidth == 16 ) {

		this->SetValue ( buffer, ZLMatrix4x4 ( value ));
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniformFormatter::SetValue ( void* buffer, const ZLAffine3D& value ) const {

	if ( this->mWidth == 9 ) {
	
		return this->SetValue ( buffer, ZLMatrix3x3 ( value ));
	}
	else if ( this->mWidth == 16 ) {

		return this->SetValue ( buffer, ZLMatrix4x4 ( value ));
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniformFormatter::SetValue ( void* buffer, const ZLMatrix3x3& value ) const {

	if ( this->mWidth == 9 ) {
	
		return this->SetValue ( buffer, value.m );
	}
	else if ( this->mWidth == 16 ) {

		return this->SetValue ( buffer, ZLMatrix4x4 ( value ));
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniformFormatter::SetValue ( void* buffer, const ZLMatrix4x4& value ) const {

	if ( this->mWidth == 9 ) {
	
		return this->SetValue ( buffer, ZLMatrix3x3 ( value ));
	}
	else if ( this->mWidth == 16 ) {

		return this->SetValue ( buffer, value.m );
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniformFormatter::SetValue ( void* buffer, const ZLVec4D& value ) const {

	if ( this->mWidth == 1 ) {
		this->SetValue ( buffer, value.mX );
		return;
	}

	float m [ 4 ];

	m [ 0 ]		= value.mX;
	m [ 1 ]		= value.mY;
	m [ 2 ]		= value.mZ;
	m [ 3 ]		= value.mW;

	this->SetValue ( buffer, m );
}
