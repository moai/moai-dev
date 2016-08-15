// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIShaderUniform.h>

//================================================================//
// MOAIShaderUniform
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderUniform::AddValue ( MOAIShaderUniformInstance& instance, u32 index, const MOAIAttribute& attr ) {

	switch ( attr.GetTypeID ()) {

		case MOAIAttribute::ATTR_TYPE_FLOAT_32: {

			float value = attr.GetValue ( 0.0f );

			if ( value != 0.0f ) {
				*( float* )this->GetUniformBuffer ( instance, index ) += value;
			}
			break;
		}
		case MOAIAttribute::ATTR_TYPE_INT_32: {

			int value = ( int )attr.GetValue ( 0 );

			if ( value != 0 ) {
				*( s32* )this->GetUniformBuffer ( instance, index ) += value;
			}
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniform::Bind ( const MOAIShaderUniformInstance& instance, u32 index ) {

	if ( this->mGPUBase == ZGL_INVALID_UNIFORM_ADDR ) return;

	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

	if ( this->mType == UNIFORM_TYPE_FLOAT ) {

		gfx.UniformFloat ( this->mGPUBase, this->mWidth, ( float* )this->GetUniformBuffer ( instance, index ));
	}
	else if ( this->mType == UNIFORM_TYPE_INT ) {
		
		gfx.UniformInt ( this->mGPUBase, this->mWidth, ( s32* )this->GetUniformBuffer ( instance, index ));
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniform::Default ( MOAIShaderUniformInstance& instance ) const {
	
	if ( this->mWidth == 9 ) {
	
		for ( u32 i = 0; i < this->mCount; ++i ) {
			this->SetValue ( instance, i, ZLMatrix3x3::IDENT );
		}
	}
	else if ( this->mWidth == 16 ) {
	
		for ( u32 i = 0; i < this->mCount; ++i ) {
			this->SetValue ( instance, i, ZLMatrix4x4::IDENT );
		}
	}
	else {
	
		memset ( instance.mBuffer, 0, this->mWidth * instance.mCount * ELEMENT_SIZE );
	}
}

//----------------------------------------------------------------//
void* MOAIShaderUniform::GetUniformBuffer ( MOAIShaderUniformInstance& instance, u32 index ) const {

	return ( void* )(( size_t )instance.mBuffer + ( this->mWidth * ELEMENT_SIZE * index ));
}

//----------------------------------------------------------------//
const void* MOAIShaderUniform::GetUniformBuffer ( const MOAIShaderUniformInstance& instance, u32 index ) const {

	return ( const void* )(( size_t )instance.mBuffer + ( this->mWidth * ELEMENT_SIZE * index ));
}

//----------------------------------------------------------------//
bool MOAIShaderUniform::Init ( u32 type, u32 width, u32 count ) {
	
	this->mType		= type;
	this->mWidth	= width;
	this->mCount	= count;
}

//----------------------------------------------------------------//
MOAIShaderUniform::MOAIShaderUniform () :
	mType ( UNIFORM_TYPE_FLOAT ),
	mWidth ( 1 ),
	mCount ( 0 ),
	mGPUBase ( ZGL_INVALID_UNIFORM_ADDR ) {
}

//----------------------------------------------------------------//
MOAIShaderUniform::~MOAIShaderUniform () {
}

//----------------------------------------------------------------//
void MOAIShaderUniform::SetRawValue ( MOAIShaderUniformInstance& instance, u32 index, const void* srcBuffer ) const {
	
	memcpy ( this->GetUniformBuffer ( instance, index ), srcBuffer, this->mWidth * ELEMENT_SIZE );
}

//----------------------------------------------------------------//
void MOAIShaderUniform::SetValue ( MOAIShaderUniformInstance& instance, u32 index, const s32* srcBuffer ) const {

	if ( this->mType == UNIFORM_TYPE_FLOAT ) {
	
		float fBuffer [ MAX_BUFFER_WIDTH ];
		for ( size_t i = 0; i < this->mWidth; ++i ) {
		
			fBuffer [ i ] = ( float )srcBuffer [ i ];
		}
		return this->SetRawValue ( instance, index, fBuffer );
	}
	return this->SetRawValue ( instance, index, srcBuffer );
}

//----------------------------------------------------------------//
void MOAIShaderUniform::SetValue ( MOAIShaderUniformInstance& instance, u32 index, const float* srcBuffer ) const {

	if ( this->mType == UNIFORM_TYPE_INT ) {
	
		s32 iBuffer [ MAX_BUFFER_WIDTH ];
		for ( size_t i = 0; i < this->mWidth; ++i ) {
		
			iBuffer [ i ] = ( s32 )srcBuffer [ i ];
		}
		return this->SetRawValue ( instance, index, iBuffer );
	}
	return this->SetRawValue ( instance, index, srcBuffer );
}

//----------------------------------------------------------------//
void MOAIShaderUniform::SetValue ( MOAIShaderUniformInstance& instance, u32 index, float value ) const {

	if ( this->mType == MOAIShaderUniform::UNIFORM_TYPE_INT ) {
		this->SetValue ( instance, index, ( s32 )value );
		return;
	}
	
	*( float* )this->GetUniformBuffer ( instance, index ) = value;
}

//----------------------------------------------------------------//
void MOAIShaderUniform::SetValue ( MOAIShaderUniformInstance& instance, u32 index, s32 value ) const {

	if ( this->mType == MOAIShaderUniform::UNIFORM_TYPE_FLOAT ) {
		this->SetValue ( instance, index, ( float )value );
		return;
	}

	*( s32* )this->GetUniformBuffer ( instance, index ) = value;
}

//----------------------------------------------------------------//
void MOAIShaderUniform::SetValue ( MOAIShaderUniformInstance& instance, u32 index, const MOAIAttribute& attr ) const {

	switch ( attr.GetTypeID ()) {

		case MOAIAttribute::ATTR_TYPE_COLOR_VEC_4: {
			ZLColorVec color = attr.GetValue ( ZLColorVec::WHITE );
			this->SetValue ( instance, index, color );
			break;
		}
		
		case MOAIAttribute::ATTR_TYPE_FLOAT_32: {
			this->SetValue ( instance, index, ( float )attr.GetValue ( 0.0f ));
			break;
		}
		
		case MOAIAttribute::ATTR_TYPE_INT_32: {
			return this->SetValue ( instance, index, ( int )attr.GetValue ( 0 ));
			break;
		}
		
		case MOAIAttribute::ATTR_TYPE_AFFINE_3D: {
			this->SetValue ( instance, index, attr.GetValue ( ZLAffine3D::IDENT ));
			break;
		}
		
		case MOAIAttribute::ATTR_TYPE_MATRIX_3X3: {
			this->SetValue ( instance, index, attr.GetValue ( ZLMatrix3x3::IDENT ));
			break;
		}
		
		case MOAIAttribute::ATTR_TYPE_MATRIX_4X4: {
			this->SetValue ( instance, index, attr.GetValue ( ZLMatrix4x4::IDENT ));
			break;
		}
		
		case MOAIAttribute::ATTR_TYPE_VEC_3: {
			ZLVec3D vec3 = attr.GetValue ( ZLVec3D::ORIGIN );
			ZLVec4D vec4 ( vec3.mX, vec3.mY, vec3.mZ, 0.0f );
			this->SetValue ( instance, index, vec4 );
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniform::SetValue ( MOAIShaderUniformInstance& instance, u32 index, const ZLColorVec& value ) const {

	if ( this->mWidth == 1 ) {
		this->SetValue ( instance, index, value.mR ); // TODO: this right?
		return;
	}

	float m [ 4 ];

	m [ 0 ]		= value.mR;
	m [ 1 ]		= value.mG;
	m [ 2 ]		= value.mB;
	m [ 3 ]		= value.mA;

	this->SetValue ( instance, index, m );
}

//----------------------------------------------------------------//
void MOAIShaderUniform::SetValue ( MOAIShaderUniformInstance& instance, u32 index, const ZLIntVec4D& value ) const {

	if ( this->mWidth == 1 ) {
		this->SetValue ( instance, index, value.mX );
		return;
	}

	s32 m [ 4 ];
	
	m [ 0 ]		= value.mX;
	m [ 1 ]		= value.mY;
	m [ 2 ]		= value.mZ;
	m [ 3 ]		= value.mW;

	this->SetValue ( instance, index, m );
}

//----------------------------------------------------------------//
void MOAIShaderUniform::SetValue ( MOAIShaderUniformInstance& instance, u32 index, const ZLAffine2D& value ) const {

	if ( this->mWidth == 9 ) {
	
		this->SetValue ( instance, index, ZLMatrix3x3 ( value ));
	}
	else if ( this->mWidth == 16 ) {

		this->SetValue ( instance, index, ZLMatrix4x4 ( value ));
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniform::SetValue ( MOAIShaderUniformInstance& instance, u32 index, const ZLAffine3D& value ) const {

	if ( this->mWidth == 9 ) {
	
		return this->SetValue ( instance, index, ZLMatrix3x3 ( value ));
	}
	else if ( this->mWidth == 16 ) {

		return this->SetValue ( instance, index, ZLMatrix4x4 ( value ));
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniform::SetValue ( MOAIShaderUniformInstance& instance, u32 index, const ZLMatrix3x3& value ) const {

	if ( this->mWidth == 9 ) {
	
		return this->SetValue ( instance, index, value.m );
	}
	else if ( this->mWidth == 16 ) {

		return this->SetValue ( instance, index, ZLMatrix4x4 ( value ));
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniform::SetValue ( MOAIShaderUniformInstance& instance, u32 index, const ZLMatrix4x4& value ) const {

	if ( this->mWidth == 9 ) {
	
		return this->SetValue ( instance, index, ZLMatrix3x3 ( value ));
	}
	else if ( this->mWidth == 16 ) {

		return this->SetValue ( instance, index, value.m );
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniform::SetValue ( MOAIShaderUniformInstance& instance, u32 index, const ZLVec4D& value ) const {

	if ( this->mWidth == 1 ) {
		this->SetValue ( instance, index, value.mX );
		return;
	}

	float m [ 4 ];

	m [ 0 ]		= value.mX;
	m [ 1 ]		= value.mY;
	m [ 2 ]		= value.mZ;
	m [ 3 ]		= value.mW;

	this->SetValue ( instance, index, m );
}
