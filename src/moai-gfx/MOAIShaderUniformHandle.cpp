// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIShaderUniformHandle.h>

//================================================================//
// MOAIShaderUniformHandle
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderUniformHandle::AddValue ( const ZLAttribute& attr ) {

	switch ( attr.GetTypeID ()) {

		case ZLAttribute::ATTR_TYPE_FLOAT_32: {

			float value = attr.GetValue ( 0.0f );

			if ( value != 0.0f ) {
				*( float* )this->mBuffer += value;
			}
			break;
		}
		case ZLAttribute::ATTR_TYPE_INT_32: {

			int value = ( int )attr.GetValue ( 0 );

			if ( value != 0 ) {
				*( s32* )this->mBuffer += value;
			}
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniformHandle::Default ( u32 count ) {
	
	if ( this->mWidth == 9 ) {
	
		MOAIShaderUniformHandle cursor = *this;
	
		for ( u32 i = 0; i < count; ++i ) {
			cursor.SetValue ( ZLMatrix3x3::IDENT );
			cursor.Next ();
		}
	}
	else if ( this->mWidth == 16 ) {
	
		MOAIShaderUniformHandle cursor = *this;
	
		for ( u32 i = 0; i < count; ++i ) {
			cursor.SetValue ( ZLMatrix4x4::IDENT );
			cursor.Next ();
		}
	}
	else {
	
		memset ( this->mBuffer, 0, this->mWidth * count * COMPONENT_SIZE );
	}
}

//----------------------------------------------------------------//
bool MOAIShaderUniformHandle::IsValid () {

	return ( this->mBuffer != NULL );
}

//----------------------------------------------------------------//
MOAIShaderUniformHandle::MOAIShaderUniformHandle () {
}

//----------------------------------------------------------------//
MOAIShaderUniformHandle::~MOAIShaderUniformHandle () {
}

//----------------------------------------------------------------//
void MOAIShaderUniformHandle::Next () {

	this->mBuffer = ( void* )(( size_t )this->mBuffer + ( this->mWidth * COMPONENT_SIZE ));
}

//----------------------------------------------------------------//
MOAIShaderUniformHandle MOAIShaderUniformHandle::Offset ( ZLIndex index ) {

	MOAIShaderUniformHandle offset = *this;
	offset.mBuffer = ( void* )(( size_t )this->mBuffer + ( this->mWidth * COMPONENT_SIZE * ( ZLSize )index ));
	return offset;
}

//----------------------------------------------------------------//
void MOAIShaderUniformHandle::SetRawValue ( const void* srcBuffer ) {
	
	memcpy ( this->mBuffer, srcBuffer, this->mWidth * COMPONENT_SIZE );
}

//----------------------------------------------------------------//
void MOAIShaderUniformHandle::SetValue ( const s32* srcBuffer ) {

	if ( this->mType == UNIFORM_TYPE_FLOAT ) {
	
		float fBuffer [ MAX_BUFFER_WIDTH ];
		for ( size_t i = 0; i < this->mWidth; ++i ) {
		
			fBuffer [ i ] = ( float )srcBuffer [ i ];
		}
		return this->SetRawValue ( fBuffer );
	}
	return this->SetRawValue ( srcBuffer );
}

//----------------------------------------------------------------//
void MOAIShaderUniformHandle::SetValue ( const float* srcBuffer ) {

	if ( this->mType == UNIFORM_TYPE_INT ) {
	
		s32 iBuffer [ MAX_BUFFER_WIDTH ];
		for ( size_t i = 0; i < this->mWidth; ++i ) {
		
			iBuffer [ i ] = ( s32 )srcBuffer [ i ];
		}
		return this->SetRawValue ( iBuffer );
	}
	return this->SetRawValue ( srcBuffer );
}

//----------------------------------------------------------------//
void MOAIShaderUniformHandle::SetValue ( float value ) {

	if ( this->mType == MOAIShaderUniformHandle::UNIFORM_TYPE_INT ) {
		this->SetValue (( s32 )value );
		return;
	}
	
	*( float* )this->mBuffer = value;
}

//----------------------------------------------------------------//
void MOAIShaderUniformHandle::SetValue ( s32 value ) {

	if ( this->mType == MOAIShaderUniformHandle::UNIFORM_TYPE_FLOAT ) {
		this->SetValue (( float )value );
		return;
	}

	*( s32* )this->mBuffer = value;
}

//----------------------------------------------------------------//
void MOAIShaderUniformHandle::SetValue ( const ZLAttribute& attr ) {

	switch ( attr.GetTypeID ()) {

		case ZLAttribute::ATTR_TYPE_COLOR_VEC_4: {
			ZLColorVec color = attr.GetValue ( ZLColorVec::WHITE );
			this->SetValue ( color );
			break;
		}
		
		case ZLAttribute::ATTR_TYPE_FLOAT_32: {
			this->SetValue (( float )attr.GetValue ( 0.0f ));
			break;
		}
		
		case ZLAttribute::ATTR_TYPE_INT_32: {
			return this->SetValue (( int )attr.GetValue ( 0 ));
			break;
		}
		
		case ZLAttribute::ATTR_TYPE_AFFINE_3D: {
			this->SetValue ( attr.GetValue ( ZLAffine3D::IDENT ));
			break;
		}
		
		case ZLAttribute::ATTR_TYPE_MATRIX_3X3: {
			this->SetValue ( attr.GetValue ( ZLMatrix3x3::IDENT ));
			break;
		}
		
		case ZLAttribute::ATTR_TYPE_MATRIX_4X4: {
			this->SetValue ( attr.GetValue ( ZLMatrix4x4::IDENT ));
			break;
		}
		
		case ZLAttribute::ATTR_TYPE_VEC_3: {
			ZLVec3D vec3 = attr.GetValue ( ZLVec3D::ORIGIN );
			ZLVec4D vec4 ( vec3.mX, vec3.mY, vec3.mZ, 0.0f );
			this->SetValue ( vec4 );
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniformHandle::SetValue ( const ZLColorVec& value ) {

	if ( this->mWidth == 1 ) {
		this->SetValue ( value.mA );
		return;
	}

	float m [ 4 ];

	m [ 0 ]		= value.mR;
	m [ 1 ]		= value.mG;
	m [ 2 ]		= value.mB;
	m [ 3 ]		= value.mA;

	this->SetValue ( m );
}

//----------------------------------------------------------------//
void MOAIShaderUniformHandle::SetValue ( const ZLIntVec4D& value ) {

	if ( this->mWidth == 1 ) {
		this->SetValue ( value.mX );
		return;
	}

	s32 m [ 4 ];
	
	m [ 0 ]		= value.mX;
	m [ 1 ]		= value.mY;
	m [ 2 ]		= value.mZ;
	m [ 3 ]		= value.mW;

	this->SetValue ( m );
}

//----------------------------------------------------------------//
void MOAIShaderUniformHandle::SetValue ( const ZLAffine2D& value ) {

	if ( this->mWidth == 9 ) {
	
		this->SetValue ( ZLMatrix3x3 ( value ));
	}
	else if ( this->mWidth == 16 ) {

		this->SetValue ( ZLMatrix4x4 ( value ));
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniformHandle::SetValue ( const ZLAffine3D& value ) {

	if ( this->mWidth == 9 ) {
	
		return this->SetValue ( ZLMatrix3x3 ( value ));
	}
	else if ( this->mWidth == 16 ) {

		return this->SetValue ( ZLMatrix4x4 ( value ));
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniformHandle::SetValue ( const ZLMatrix3x3& value ) {

	if ( this->mWidth == 9 ) {
	
		return this->SetValue ( value.m );
	}
	else if ( this->mWidth == 16 ) {

		return this->SetValue ( ZLMatrix4x4 ( value ));
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniformHandle::SetValue ( const ZLMatrix4x4& value ) {

	if ( this->mWidth == 9 ) {
	
		return this->SetValue ( ZLMatrix3x3 ( value ));
	}
	else if ( this->mWidth == 16 ) {

		return this->SetValue ( value.m );
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniformHandle::SetValue ( const ZLVec4D& value ) {

	if ( this->mWidth == 1 ) {
		this->SetValue ( value.mX );
		return;
	}

	float m [ 4 ];

	m [ 0 ]		= value.mX;
	m [ 1 ]		= value.mY;
	m [ 2 ]		= value.mZ;
	m [ 3 ]		= value.mW;

	this->SetValue ( m );
}
