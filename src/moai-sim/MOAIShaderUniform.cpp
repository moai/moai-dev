// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIShaderUniform.h>

//================================================================//
// MOAIShaderUniformBuffer
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderUniformBuffer::AddValue ( const MOAIAttrOp& attrOp ) {

	switch ( attrOp.GetTypeHint ()) {

		case MOAIAttrOp::ATTR_TYPE_REAL: {

			real value = ( real )attrOp.GetValue ( 0.0 );

			if ( value != 0.0 ) {
				this->mReal += value;
			}
			break;
		}
		case MOAIAttrOp::ATTR_TYPE_INT: {

			int value = ( int )attrOp.GetValue ( 0 );

			if ( value != 0 ) {
				this->mInt += value;
			}
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniformBuffer::Clear () {

	this->mType = MOAIShaderUniformBuffer::UNIFORM_NONE;
	this->mBuffer.Clear ();
}

//----------------------------------------------------------------//
void MOAIShaderUniformBuffer::GetFlags ( MOAIAttrOp& attrOp ) {
    
	switch ( this->mType ) {
			
		case UNIFORM_REAL:
		case UNIFORM_INT:
			
			attrOp.SetFlags ( MOAIAttrOp::ATTR_READ_WRITE );
            break;

        default:

            attrOp.SetFlags ( MOAIAttrOp::ATTR_WRITE );
            break;
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniformBuffer::GetValue ( MOAIAttrOp& attrOp ) {

	// TODO: support color type hint

	switch ( this->mType ) {
			
		case UNIFORM_REAL: {
			
			attrOp.Apply ( this->mReal, MOAIAttrOp::GET, MOAIAttrOp::ATTR_READ_WRITE, MOAIAttrOp::ATTR_TYPE_REAL );
			break;
		}
		case UNIFORM_INDEX:
		case UNIFORM_INT: {
			
			attrOp.Apply ( this->mInt, MOAIAttrOp::GET, MOAIAttrOp::ATTR_READ_WRITE, MOAIAttrOp::ATTR_TYPE_INT );
			break;
		}
		case UNIFORM_MATRIX_R3:
		case UNIFORM_MATRIX_R4: {
			// TODO:
		}
		case UNIFORM_VECTOR_R4: {
			// TODO:
		}
	}
}

//----------------------------------------------------------------//
bool MOAIShaderUniformBuffer::SetBuffer ( void* buffer, size_t size, bool check ) {

	if ( check ) {
		if ( memcmp ( this->mBuffer, buffer, size ) == 0 ) return false;
	}

	memcpy ( this->mBuffer, buffer, size );
	return true;
}

//----------------------------------------------------------------//
void MOAIShaderUniformBuffer::SetType ( u32 type ) {

	this->mBuffer.Clear ();

	this->mType = type;
	this->mInt = 0;

	switch ( type ) {
		
		case UNIFORM_MATRIX_R3: {
		
			this->mBuffer.Resize ( 12 * sizeof ( real ), 0 );
			break;
		}
		
		case UNIFORM_MATRIX_R4: {
		
			this->mBuffer.Resize ( 16 * sizeof ( real ), 0 );
			break;
		}
		
		case UNIFORM_VECTOR_R4: {
		
			this->mBuffer.Resize ( 4 * sizeof ( real ), 0 );
			break;
		}
	};
}

//----------------------------------------------------------------//
bool MOAIShaderUniformBuffer::SetValue ( real value ) {

	bool check = this->mReal != value;
	this->mReal = value;
	return check;
}

//----------------------------------------------------------------//
bool MOAIShaderUniformBuffer::SetValue ( int value ) {

	bool check = this->mInt != value;
	this->mInt = value;
	return check;
}

//----------------------------------------------------------------//
bool MOAIShaderUniformBuffer::SetValue ( const MOAIAttrOp& attrOp, bool check ) {

	switch ( attrOp.GetTypeHint ()) {

		case MOAIAttrOp::ATTR_TYPE_COLOR: {
			ZLColorVec* color = attrOp.GetValue < ZLColorVec* >( 0 );
			if ( color ) {
				return this->SetValue ( *color, check );
			}
			break;
		}
		case MOAIAttrOp::ATTR_TYPE_REAL: {
			return this->SetValue (( real )attrOp.GetValue ( 0.0 ));
			break;
		}
		case MOAIAttrOp::ATTR_TYPE_INT: {
			return this->SetValue (( int )attrOp.GetValue ( 0 ));
			break;
		}
		case MOAIAttrOp::ATTR_TYPE_TRANSFORM: {
			ZLAffine3D* affine = attrOp.GetValue < ZLAffine3D* >( 0 );
			if ( affine ) {
				return this->SetValue ( *affine, check );
			}
			break;
		}
		case MOAIAttrOp::ATTR_TYPE_VECTOR: {
			// TODO:
		}
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIShaderUniformBuffer::SetValue ( const ZLColorVec& value, bool check ) {

	real m [ 4 ];

	m [ 0 ]		= value.mR;
	m [ 1 ]		= value.mG;
	m [ 2 ]		= value.mB;
	m [ 3 ]		= value.mA;

	return this->SetBuffer ( m, sizeof ( m ), check );
}

//----------------------------------------------------------------//
bool MOAIShaderUniformBuffer::SetValue ( const ZLAffine3D& value, bool check ) {

	real m [ 16 ];
	
	m [ 0 ]		= value.m [ AffineElem3D::C0_R0 ];
	m [ 1 ]		= value.m [ AffineElem3D::C1_R0 ];
	m [ 2 ]		= value.m [ AffineElem3D::C2_R0 ];
	m [ 3 ]		= value.m [ AffineElem3D::C3_R0 ];

	m [ 4 ]		= value.m [ AffineElem3D::C0_R1 ];
	m [ 5 ]		= value.m [ AffineElem3D::C1_R1 ];
	m [ 6 ]		= value.m [ AffineElem3D::C2_R1 ];
	m [ 7 ]		= value.m [ AffineElem3D::C3_R1 ];

	m [ 8 ]		= value.m [ AffineElem3D::C0_R2 ];
	m [ 9 ]		= value.m [ AffineElem3D::C1_R2 ];
	m [ 10 ]	= value.m [ AffineElem3D::C2_R2 ];
	m [ 11 ]	= value.m [ AffineElem3D::C3_R2 ];

	m [ 12 ]	= 0.0f;
	m [ 13 ]	= 0.0f;
	m [ 14 ]	= 0.0f;
	m [ 15 ]	= 1.0f;

	return this->SetBuffer ( m, sizeof ( m ), check );
}

//----------------------------------------------------------------//
bool MOAIShaderUniformBuffer::SetValue ( const ZLMatrix4x4& value, bool check ) {

	real m [ 16 ];

	m [ 0 ]		= ( real )value.m [ ZLMatrix4x4::C0_R0 ];
	m [ 1 ]		= ( real )value.m [ ZLMatrix4x4::C1_R0 ];
	m [ 2 ]		= ( real )value.m [ ZLMatrix4x4::C2_R0 ];
	m [ 3 ]		= ( real )value.m [ ZLMatrix4x4::C3_R0 ];

	m [ 4 ]		= ( real )value.m [ ZLMatrix4x4::C0_R1 ];
	m [ 5 ]		= ( real )value.m [ ZLMatrix4x4::C1_R1 ];
	m [ 6 ]		= ( real )value.m [ ZLMatrix4x4::C2_R1 ];
	m [ 7 ]		= ( real )value.m [ ZLMatrix4x4::C3_R1 ];

	m [ 8 ]		= ( real )value.m [ ZLMatrix4x4::C0_R2 ];
	m [ 9 ]		= ( real )value.m [ ZLMatrix4x4::C1_R2 ];
	m [ 10 ]	= ( real )value.m [ ZLMatrix4x4::C2_R2 ];
	m [ 11 ]	= ( real )value.m [ ZLMatrix4x4::C3_R2 ];

	m [ 12 ]	= ( real )value.m [ ZLMatrix4x4::C0_R3 ];
	m [ 13 ]	= ( real )value.m [ ZLMatrix4x4::C1_R3 ];
	m [ 14 ]	= ( real )value.m [ ZLMatrix4x4::C2_R3 ];
	m [ 15 ]	= ( real )value.m [ ZLMatrix4x4::C3_R3 ];

	return this->SetBuffer ( m, sizeof ( m ), check );
}

//----------------------------------------------------------------//
bool MOAIShaderUniformBuffer::SetValue ( const ZLMatrix3x3& value, bool check ) {

	real m [ 9 ];

	m [ 0 ]    = ( real )value.m [ ZLMatrix3x3::C0_R0 ];
	m [ 1 ]    = ( real )value.m [ ZLMatrix3x3::C1_R0 ];
	m [ 2 ]    = ( real )value.m [ ZLMatrix3x3::C2_R0 ];

	m [ 3 ]    = ( real )value.m [ ZLMatrix3x3::C0_R1 ];
	m [ 4 ]    = ( real )value.m [ ZLMatrix3x3::C1_R1 ];
	m [ 5 ]    = ( real )value.m [ ZLMatrix3x3::C2_R1 ];

	m [ 6 ]    = ( real )value.m [ ZLMatrix3x3::C0_R2 ];
	m [ 7 ]    = ( real )value.m [ ZLMatrix3x3::C1_R2 ];
	m [ 8 ]    = ( real )value.m [ ZLMatrix3x3::C2_R2 ];

	return this->SetBuffer ( m, sizeof ( m ), check );
}

//----------------------------------------------------------------//
bool MOAIShaderUniformBuffer::SetValue ( const MOAIShaderUniformBuffer& uniformBuffer, bool check ) {

	if ( this->mType == UNIFORM_REAL ) return this->SetValue ( uniformBuffer.mReal );
	if (( this->mType == UNIFORM_INDEX ) || ( this->mType == UNIFORM_INT )) return this->SetValue ( uniformBuffer.mInt );
	return this->SetBuffer ( uniformBuffer.mBuffer, this->mBuffer.Size (), check );
}

//================================================================//
// MOAIShaderUniform
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderUniform::Bind () {

	switch ( this->mType ) {

		case UNIFORM_REAL:
			zglUniform1f ( this->mAddr, this->mReal );
			break;

		case UNIFORM_INDEX:
			zglUniform1i ( this->mAddr, this->mInt - 1 );
			break;

		case UNIFORM_INT:
			zglUniform1i ( this->mAddr, this->mInt );
			break;

		case UNIFORM_MATRIX_R3:
    		zglUniformMatrix3fv ( this->mAddr, 1, false, ( real* )this->mBuffer.Data ());
    		break;

		case UNIFORM_MATRIX_R4:
			zglUniformMatrix4fv ( this->mAddr, 1, false, ( real* )this->mBuffer.Data ());
			break;

		case UNIFORM_VECTOR_R4:
			zglUniform4fv ( this->mAddr, 1, ( real* )this->mBuffer.Data ());
			break;
	}
}
