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
void MOAIShaderUniformBuffer::AddValue ( const MOAIAttribute& attr ) {

	switch ( attr.GetTypeID ()) {

		case MOAIAttribute::ATTR_TYPE_FLOAT_32: {

			float value = attr.GetValue ( 0.0f );

			if ( value != 0.0f ) {
				this->mFloat += value;
			}
			break;
		}
		case MOAIAttribute::ATTR_TYPE_INT_32: {

			int value = ( int )attr.GetValue ( 0 );

			if ( value != 0 ) {
				this->mInt += value;
			}
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniformBuffer::Clear () {

	this->mType = MOAIShaderUniformBuffer::UNIFORM_TYPE_FLOAT;
	this->mBuffer.Clear ();
	this->mFloat = 0.0f;
}

//----------------------------------------------------------------//
void MOAIShaderUniformBuffer::ClearValue () {

	this->mFloat = 0.0f;
	this->mBuffer.Fill ( 0 );
}

//----------------------------------------------------------------//
void MOAIShaderUniformBuffer::Default () {

	this->mInt = 0;
	this->mBuffer.Fill ( 0 );
	
	if ( this->mWidth == 9 ) {
	
		this->SetValue ( ZLMatrix3x3::IDENT );
	}
	else if ( this->mWidth == 16 ) {
	
		this->SetValue ( ZLMatrix4x4::IDENT );
	}
}

//----------------------------------------------------------------//
bool MOAIShaderUniformBuffer::Init ( u32 type, u32 width ) {

	this->Clear ();
	
	if ( !width ) return false;
	
	this->mType = type;
	this->mWidth = width;

	if ( width > 1 ) {
	
		if ( type == UNIFORM_TYPE_FLOAT ) {
		
			this->mBuffer.Resize ( width * sizeof ( float ), 0 );
		}
		else if ( type == UNIFORM_TYPE_INT ) {
		
			this->mBuffer.Resize ( width * sizeof ( s32 ), 0 );
		}
	}
}

//----------------------------------------------------------------//
MOAIShaderUniformBuffer::MOAIShaderUniformBuffer () :
	mType ( UNIFORM_TYPE_FLOAT ),
	mWidth ( 1 ) {
	
	this->mFloat = 0.0f;
}

//----------------------------------------------------------------//
MOAIShaderUniformBuffer::~MOAIShaderUniformBuffer () {
}

//----------------------------------------------------------------//
u32 MOAIShaderUniformBuffer::SetBuffer ( const s32* buffer, u32 width ) {

	if ( this->mType == UNIFORM_TYPE_FLOAT ) {
	
		float fBuffer [ MAX_BUFFER_WIDTH ];
		for ( size_t i = 0; i < width; ++i ) {
		
			fBuffer [ i ] = ( float )buffer [ i ];
		}
		return this->SetBuffer ( fBuffer, width );
	}
	if ( memcmp ( this->mBuffer, buffer, width * sizeof ( s32 )) == 0 ) return 0;
	memcpy ( this->mBuffer, buffer, width * sizeof ( s32 ));
	return MOAIShaderUniform::UNIFORM_FLAG_DIRTY;
}

//----------------------------------------------------------------//
u32 MOAIShaderUniformBuffer::SetBuffer ( const float* buffer, u32 width ) {

	if ( this->mType == UNIFORM_TYPE_INT ) {
	
		s32 iBuffer [ MAX_BUFFER_WIDTH ];
		for ( size_t i = 0; i < width; ++i ) {
		
			iBuffer [ i ] = ( s32 )buffer [ i ];
		}
		return this->SetBuffer ( iBuffer, width );
	}
	if ( memcmp ( this->mBuffer, buffer, width * sizeof ( float )) == 0 ) return 0;
	memcpy ( this->mBuffer, buffer, width * sizeof ( float ));
	return MOAIShaderUniform::UNIFORM_FLAG_DIRTY;
}

//----------------------------------------------------------------//
u32 MOAIShaderUniformBuffer::SetValue ( float value ) {

	if ( this->mType == MOAIShaderUniformBuffer::UNIFORM_TYPE_INT ) {
		return this->SetValue (( s32 )value );
	}

	u32 flags = this->mFloat != value ? MOAIShaderUniform::UNIFORM_FLAG_DIRTY : 0;
	this->mFloat = value;
	return flags;
}

//----------------------------------------------------------------//
u32 MOAIShaderUniformBuffer::SetValue ( s32 value ) {

	if ( this->mType == MOAIShaderUniformBuffer::UNIFORM_TYPE_FLOAT ) {
		return this->SetValue (( float )value );
	}

	u32 flags = this->mInt != value ? MOAIShaderUniform::UNIFORM_FLAG_DIRTY : 0;
	this->mInt = value;
	return flags;
}

//----------------------------------------------------------------//
u32 MOAIShaderUniformBuffer::SetValue ( const MOAIAttribute& attr ) {

	switch ( attr.GetTypeID ()) {

		case MOAIAttribute::ATTR_TYPE_COLOR_VEC_4: {
			ZLColorVec color = attr.GetValue ( ZLColorVec::WHITE );
			return this->SetValue ( color );
		}
		
		case MOAIAttribute::ATTR_TYPE_FLOAT_32: {
			return this->SetValue (( float )attr.GetValue ( 0.0f ));
			break;
		}
		
		case MOAIAttribute::ATTR_TYPE_INT_32: {
			return this->SetValue (( int )attr.GetValue ( 0 ));
			break;
		}
		
		case MOAIAttribute::ATTR_TYPE_AFFINE_3D: {
			return this->SetValue ( attr.GetValue ( ZLAffine3D::IDENT ));
		}
		
		case MOAIAttribute::ATTR_TYPE_MATRIX_3X3: {
			return this->SetValue ( attr.GetValue ( ZLMatrix3x3::IDENT ));
		}
		
		case MOAIAttribute::ATTR_TYPE_MATRIX_4X4: {
			return this->SetValue ( attr.GetValue ( ZLMatrix4x4::IDENT ));
		}
		
		case MOAIAttribute::ATTR_TYPE_VEC_3: {
			ZLVec3D vec3 = attr.GetValue ( ZLVec3D::ORIGIN );
			ZLVec4D vec4 ( vec3.mX, vec3.mY, vec3.mZ, 0.0f );
			return this->SetValue ( vec4 );

		}
	}
	return 0;
}

//----------------------------------------------------------------//
u32 MOAIShaderUniformBuffer::SetValue ( const MOAIShaderUniformBuffer& uniformBuffer ) {

	assert (( this->mType == uniformBuffer.mType ) && ( this->mWidth == uniformBuffer.mWidth ));

	if ( this->mType == UNIFORM_TYPE_FLOAT ) {
	
		return this->mWidth == 1 ? this->SetValue ( uniformBuffer.mFloat ) : this->SetBuffer (( float* )uniformBuffer.mBuffer.Data (), this->mWidth );
	}
	else if ( this->mType == UNIFORM_TYPE_INT ) {
	
		return this->mWidth == 1 ? this->SetValue ( uniformBuffer.mInt ) : this->SetBuffer (( s32* )uniformBuffer.mBuffer.Data (), this->mWidth );
	}
	return 0;
}

//----------------------------------------------------------------//
u32 MOAIShaderUniformBuffer::SetValue ( const ZLColorVec& value ) {

	if ( this->mWidth == 1 ) return this->SetValue ( value.mR );

	float m [ 4 ];

	m [ 0 ]		= value.mR;
	m [ 1 ]		= value.mG;
	m [ 2 ]		= value.mB;
	m [ 3 ]		= value.mA;

	return this->SetBuffer ( m, this->mWidth );
}

//----------------------------------------------------------------//
u32 MOAIShaderUniformBuffer::SetValue ( const ZLIntVec4D& value ) {

	if ( this->mWidth == 1 ) return this->SetValue ( value.mX );

	s32 m [ 4 ];
	
	m [ 0 ]		= value.mX;
	m [ 1 ]		= value.mY;
	m [ 2 ]		= value.mZ;
	m [ 3 ]		= value.mW;

	return this->SetBuffer ( m, this->mWidth );
}

//----------------------------------------------------------------//
u32 MOAIShaderUniformBuffer::SetValue ( const ZLAffine2D& value ) {

	if ( this->mWidth == 9 ) {
	
		return this->SetValue ( ZLMatrix3x3 ( value ));
	}
	else if ( this->mWidth == 16 ) {

		return this->SetValue ( ZLMatrix4x4 ( value ));
	}
	return 0;
}

//----------------------------------------------------------------//
u32 MOAIShaderUniformBuffer::SetValue ( const ZLAffine3D& value ) {

	if ( this->mWidth == 9 ) {
	
		return this->SetValue ( ZLMatrix3x3 ( value ));
	}
	else if ( this->mWidth == 16 ) {

		return this->SetValue ( ZLMatrix4x4 ( value ));
	}
	return 0;
}

//----------------------------------------------------------------//
u32 MOAIShaderUniformBuffer::SetValue ( const ZLMatrix3x3& value ) {

	if ( this->mWidth == 9 ) {
	
		return this->SetBuffer ( value.m, 9 );
	}
	else if ( this->mWidth == 16 ) {

		return this->SetValue ( ZLMatrix4x4 ( value ));
	}
	return 0;
}

//----------------------------------------------------------------//
u32 MOAIShaderUniformBuffer::SetValue ( const ZLMatrix4x4& value ) {

	if ( this->mWidth == 9 ) {
	
		return this->SetValue ( ZLMatrix3x3 ( value ));
	}
	else if ( this->mWidth == 16 ) {

		return this->SetBuffer ( value.m, 16 );
	}
	return 0;
}

//----------------------------------------------------------------//
u32 MOAIShaderUniformBuffer::SetValue ( const ZLVec4D& value ) {

	if ( this->mWidth == 1 ) return this->SetValue ( value.mX );

	float m [ 4 ];

	m [ 0 ]		= value.mX;
	m [ 1 ]		= value.mY;
	m [ 2 ]		= value.mZ;
	m [ 3 ]		= value.mW;

	return this->SetBuffer ( m, this->mWidth );
}

//================================================================//
// MOAIShaderUniform
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderUniform::Bind () {

	if ( this->mAddr == ZGL_INVALID_UNIFORM_ADDR ) return;

	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

	if ( this->mType == UNIFORM_TYPE_FLOAT ) {

		gfx.UniformFloat ( this->mAddr, this->mWidth, this->mWidth == 1 ? &this->mFloat : ( float* )this->mBuffer.Data ());
	}
	else if ( this->mType == UNIFORM_TYPE_INT ) {
		
		gfx.UniformInt ( this->mAddr, this->mWidth, this->mWidth == 1 ? &this->mInt : ( s32* )this->mBuffer.Data ());
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
