// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIShaderUniform.h>

//================================================================//
// MOAIShaderUniformBuffer
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderUniformBuffer::AddValue ( const MOAIAttrOp& attrOp ) {

	switch ( attrOp.GetTypeHint ()) {

		case MOAIAttrOp::ATTR_TYPE_FLOAT: {

			float value = attrOp.GetValue ( 0.0f );

			if ( value != 0.0f ) {
				this->mFloat += value;
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
void MOAIShaderUniformBuffer::ClearValue () {

	this->SetType ( this->mType );
}

//----------------------------------------------------------------//
void MOAIShaderUniformBuffer::Default () {

	switch ( this->mType ) {
	
		case MOAIShaderUniformBuffer::UNIFORM_INDEX: {
			this->SetValue ( 1 );
			break;
		}
		case MOAIShaderUniformBuffer::UNIFORM_MATRIX_F3: {
			ZLMatrix3x3 mtx;
			mtx.Ident ();
			this->SetValue ( mtx );
			break;
		}
		case MOAIShaderUniformBuffer::UNIFORM_MATRIX_F4: {
			ZLMatrix4x4 mtx;
			mtx.Ident ();
			this->SetValue ( mtx );
			break;
		}
		default: {
			this->mBuffer.Fill ( 0 );
		}
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniformBuffer::GetFlags ( MOAIAttrOp& attrOp ) {
    
	switch ( this->mType ) {
			
		case UNIFORM_FLOAT:
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
			
		case UNIFORM_FLOAT: {
			
			attrOp.Apply ( this->mFloat, MOAIAttrOp::GET, MOAIAttrOp::ATTR_READ_WRITE, MOAIAttrOp::ATTR_TYPE_FLOAT );
			break;
		}
		case UNIFORM_INDEX:
		case UNIFORM_INT: {
			
			attrOp.Apply ( this->mInt, MOAIAttrOp::GET, MOAIAttrOp::ATTR_READ_WRITE, MOAIAttrOp::ATTR_TYPE_INT );
			break;
		}
		case UNIFORM_MATRIX_F3:
		case UNIFORM_MATRIX_F4: {
			// TODO:
		}
		case UNIFORM_VECTOR_F4: {
			// TODO:
		}
	}
}

//----------------------------------------------------------------//
u32 MOAIShaderUniformBuffer::SetBuffer ( void* buffer, size_t size ) {

	if ( memcmp ( this->mBuffer, buffer, size ) == 0 ) return 0;
	memcpy ( this->mBuffer, buffer, size );
	return MOAIShaderUniform::UNIFORM_FLAG_DIRTY;
}

//----------------------------------------------------------------//
void MOAIShaderUniformBuffer::SetType ( u32 type ) {

	this->mBuffer.Clear ();

	this->mType = type;
	this->mInt = 0;

	switch ( type ) {
		
		case UNIFORM_MATRIX_F3: {
		
			this->mBuffer.Resize ( 12 * sizeof ( float ), 0 );
			break;
		}
		
		case UNIFORM_MATRIX_F4: {
		
			this->mBuffer.Resize ( 16 * sizeof ( float ), 0 );
			break;
		}
		
		case UNIFORM_VECTOR_F4: {
		
			this->mBuffer.Resize ( 4 * sizeof ( float ), 0 );
			break;
		}
	};
}

//----------------------------------------------------------------//
u32 MOAIShaderUniformBuffer::SetValue ( float value ) {

	u32 flags = this->mFloat != value ? MOAIShaderUniform::UNIFORM_FLAG_DIRTY : 0;
	this->mFloat = value;
	return flags;
}

//----------------------------------------------------------------//
u32 MOAIShaderUniformBuffer::SetValue ( int value ) {

	u32 flags = this->mInt != value ? MOAIShaderUniform::UNIFORM_FLAG_DIRTY : 0;
	this->mInt = value;
	return flags;
}

//----------------------------------------------------------------//
u32 MOAIShaderUniformBuffer::SetValue ( const MOAIAttrOp& attrOp ) {

	switch ( attrOp.GetTypeHint ()) {

		case MOAIAttrOp::ATTR_TYPE_COLOR: {
			ZLColorVec* color = attrOp.GetValue < ZLColorVec* >( 0 );
			if ( color ) {
				return this->SetValue ( *color );
			}
			break;
		}
		case MOAIAttrOp::ATTR_TYPE_FLOAT: {
			return this->SetValue (( float )attrOp.GetValue ( 0.0f ));
			break;
		}
		case MOAIAttrOp::ATTR_TYPE_INT: {
			return this->SetValue (( int )attrOp.GetValue ( 0 ));
			break;
		}
		case MOAIAttrOp::ATTR_TYPE_TRANSFORM: {
			ZLAffine3D* affine = attrOp.GetValue < ZLAffine3D* >( 0 );
			if ( affine ) {
				return this->SetValue ( *affine );
			}
			break;
		}
		case MOAIAttrOp::ATTR_TYPE_VECTOR: {
			// TODO:
		}
	}
	return 0;
}

//----------------------------------------------------------------//
u32 MOAIShaderUniformBuffer::SetValue ( const ZLColorVec& value ) {

	float m [ 4 ];

	m [ 0 ]		= value.mR;
	m [ 1 ]		= value.mG;
	m [ 2 ]		= value.mB;
	m [ 3 ]		= value.mA;

	return this->SetBuffer ( m, sizeof ( m ));
}

//----------------------------------------------------------------//
u32 MOAIShaderUniformBuffer::SetValue ( const ZLAffine3D& value ) {

	assert (( this->mType == UNIFORM_MATRIX_F3 ) || ( this->mType == UNIFORM_MATRIX_F4 ));

	if ( this->mType == UNIFORM_MATRIX_F3 ) {
	
		float m [ 9 ];

		m [ 0 ]    = value.m [ AffineElem3D::C0_R0 ];
		m [ 1 ]    = value.m [ AffineElem3D::C1_R0 ];
		m [ 2 ]    = value.m [ AffineElem3D::C2_R0 ];

		m [ 3 ]    = value.m [ AffineElem3D::C0_R1 ];
		m [ 4 ]    = value.m [ AffineElem3D::C1_R1 ];
		m [ 5 ]    = value.m [ AffineElem3D::C2_R1 ];

		m [ 6 ]    = value.m [ AffineElem3D::C0_R2 ];
		m [ 7 ]    = value.m [ AffineElem3D::C1_R2 ];
		m [ 8 ]    = value.m [ AffineElem3D::C2_R2 ];

		return this->SetBuffer ( m, sizeof ( m ));
	}

	float m [ 16 ];
	
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

	return this->SetBuffer ( m, sizeof ( m ));
}

//----------------------------------------------------------------//
u32 MOAIShaderUniformBuffer::SetValue ( const ZLMatrix4x4& value ) {

	assert (( this->mType == UNIFORM_MATRIX_F3 ) || ( this->mType == UNIFORM_MATRIX_F4 ));

	if ( this->mType == UNIFORM_MATRIX_F3 ) {
	
		float m [ 9 ];

		m [ 0 ]    = value.m [ ZLMatrix4x4::C0_R0 ];
		m [ 1 ]    = value.m [ ZLMatrix4x4::C1_R0 ];
		m [ 2 ]    = value.m [ ZLMatrix4x4::C2_R0 ];

		m [ 3 ]    = value.m [ ZLMatrix4x4::C0_R1 ];
		m [ 4 ]    = value.m [ ZLMatrix4x4::C1_R1 ];
		m [ 5 ]    = value.m [ ZLMatrix4x4::C2_R1 ];

		m [ 6 ]    = value.m [ ZLMatrix4x4::C0_R2 ];
		m [ 7 ]    = value.m [ ZLMatrix4x4::C1_R2 ];
		m [ 8 ]    = value.m [ ZLMatrix4x4::C2_R2 ];

		return this->SetBuffer ( m, sizeof ( m ));
	}

	float m [ 16 ];

	m [ 0 ]		= value.m [ ZLMatrix4x4::C0_R0 ];
	m [ 1 ]		= value.m [ ZLMatrix4x4::C1_R0 ];
	m [ 2 ]		= value.m [ ZLMatrix4x4::C2_R0 ];
	m [ 3 ]		= value.m [ ZLMatrix4x4::C3_R0 ];

	m [ 4 ]		= value.m [ ZLMatrix4x4::C0_R1 ];
	m [ 5 ]		= value.m [ ZLMatrix4x4::C1_R1 ];
	m [ 6 ]		= value.m [ ZLMatrix4x4::C2_R1 ];
	m [ 7 ]		= value.m [ ZLMatrix4x4::C3_R1 ];

	m [ 8 ]		= value.m [ ZLMatrix4x4::C0_R2 ];
	m [ 9 ]		= value.m [ ZLMatrix4x4::C1_R2 ];
	m [ 10 ]	= value.m [ ZLMatrix4x4::C2_R2 ];
	m [ 11 ]	= value.m [ ZLMatrix4x4::C3_R2 ];

	m [ 12 ]	= value.m [ ZLMatrix4x4::C0_R3 ];
	m [ 13 ]	= value.m [ ZLMatrix4x4::C1_R3 ];
	m [ 14 ]	= value.m [ ZLMatrix4x4::C2_R3 ];
	m [ 15 ]	= value.m [ ZLMatrix4x4::C3_R3 ];

	return this->SetBuffer ( m, sizeof ( m ));
}

//----------------------------------------------------------------//
u32 MOAIShaderUniformBuffer::SetValue ( const ZLMatrix3x3& value ) {

	assert (( this->mType == UNIFORM_MATRIX_F3 ) || ( this->mType == UNIFORM_MATRIX_F4 ));

	if ( this->mType == UNIFORM_MATRIX_F3 ) {

		float m [ 9 ];

		m [ 0 ]    = value.m [ ZLMatrix3x3::C0_R0 ];
		m [ 1 ]    = value.m [ ZLMatrix3x3::C1_R0 ];
		m [ 2 ]    = value.m [ ZLMatrix3x3::C2_R0 ];

		m [ 3 ]    = value.m [ ZLMatrix3x3::C0_R1 ];
		m [ 4 ]    = value.m [ ZLMatrix3x3::C1_R1 ];
		m [ 5 ]    = value.m [ ZLMatrix3x3::C2_R1 ];

		m [ 6 ]    = value.m [ ZLMatrix3x3::C0_R2 ];
		m [ 7 ]    = value.m [ ZLMatrix3x3::C1_R2 ];
		m [ 8 ]    = value.m [ ZLMatrix3x3::C2_R2 ];

		return this->SetBuffer ( m, sizeof ( m ));
	}
	
	float m [ 16 ];

	m [ 0 ]		= value.m [ ZLMatrix3x3::C0_R0 ];
	m [ 1 ]		= value.m [ ZLMatrix3x3::C1_R0 ];
	m [ 2 ]		= value.m [ ZLMatrix3x3::C2_R0 ];
	m [ 3 ]		= 0.0f;

	m [ 4 ]		= value.m [ ZLMatrix3x3::C0_R1 ];
	m [ 5 ]		= value.m [ ZLMatrix3x3::C1_R1 ];
	m [ 6 ]		= value.m [ ZLMatrix3x3::C2_R1 ];
	m [ 7 ]		= 0.0f;

	m [ 8 ]		= value.m [ ZLMatrix3x3::C0_R2 ];
	m [ 9 ]		= value.m [ ZLMatrix3x3::C1_R2 ];
	m [ 10 ]	= value.m [ ZLMatrix3x3::C2_R2 ];
	m [ 11 ]	= 0.0f;

	m [ 12 ]	= 0.0f;
	m [ 13 ]	= 0.0f;
	m [ 14 ]	= 0.0f;
	m [ 15 ]	= 1.0f;

	return this->SetBuffer ( m, sizeof ( m ));
}

//----------------------------------------------------------------//
u32 MOAIShaderUniformBuffer::SetValue ( const MOAIShaderUniformBuffer& uniformBuffer ) {

	if ( this->mType == UNIFORM_FLOAT ) return this->SetValue ( uniformBuffer.mFloat );
	if (( this->mType == UNIFORM_INDEX ) || ( this->mType == UNIFORM_INT )) return this->SetValue ( uniformBuffer.mInt );
	return this->SetBuffer ( uniformBuffer.mBuffer, this->mBuffer.Size ());
}

//================================================================//
// MOAIShaderUniform
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderUniform::Bind () {

	if ( this->mAddr == ZGL_INVALID_UNIFORM_ADDR ) return;

	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

	switch ( this->mType ) {

		case UNIFORM_FLOAT:
			gfx.Uniform1f ( this->mAddr, this->mFloat );
			break;

		case UNIFORM_INDEX:
			gfx.Uniform1i ( this->mAddr, this->mInt - 1 );
			break;

		case UNIFORM_INT:
			gfx.Uniform1i ( this->mAddr, this->mInt );
			break;

		case UNIFORM_MATRIX_F3:
    		gfx.UniformMatrix3fv ( this->mAddr, 1, false, ( float* )this->mBuffer.Data ());
    		break;

		case UNIFORM_MATRIX_F4:
			gfx.UniformMatrix4fv ( this->mAddr, 1, false, ( float* )this->mBuffer.Data ());
			break;

		case UNIFORM_VECTOR_F4:
			gfx.Uniform4fv ( this->mAddr, 1, ( float* )this->mBuffer.Data ());
			break;
	}
}

//----------------------------------------------------------------//
MOAIShaderUniform::MOAIShaderUniform () :
	mAddr ( ZGL_INVALID_UNIFORM_ADDR ),
	mFlags ( UNIFORM_FLAG_DIRTY ) {
}

//----------------------------------------------------------------//
MOAIShaderUniform::~MOAIShaderUniform () {
}
