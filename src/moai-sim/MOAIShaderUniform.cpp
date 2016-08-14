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
void MOAIShaderUniform::AddValue ( void* buffer, u32 index, const MOAIAttribute& attr ) {

	switch ( attr.GetTypeID ()) {

		case MOAIAttribute::ATTR_TYPE_FLOAT_32: {

			float value = attr.GetValue ( 0.0f );

			if ( value != 0.0f ) {
				*( float* )this->GetUniformBuffer ( buffer, index ) += value;
			}
			break;
		}
		case MOAIAttribute::ATTR_TYPE_INT_32: {

			int value = ( int )attr.GetValue ( 0 );

			if ( value != 0 ) {
				*( s32* )this->GetUniformBuffer ( buffer, index ) += value;
			}
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniform::Bind ( const void* buffer, u32 index ) {

	if ( this->mGPUBase == ZGL_INVALID_UNIFORM_ADDR ) return;

	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

	if ( this->mType == UNIFORM_TYPE_FLOAT ) {

		gfx.UniformFloat ( this->mGPUBase, this->mWidth, ( float* )this->GetUniformBuffer ( buffer, index ));
	}
	else if ( this->mType == UNIFORM_TYPE_INT ) {
		
		gfx.UniformInt ( this->mGPUBase, this->mWidth, ( s32* )this->GetUniformBuffer ( buffer, index ));
	}
}

//----------------------------------------------------------------//
u32 MOAIShaderUniform::CopyValue ( void* buffer, const void* srcBuffer, u32 index ) {

	return this->SetRawValue ( buffer, index, this->GetUniformBuffer ( srcBuffer, index ));
}

//----------------------------------------------------------------//
void MOAIShaderUniform::Default ( void* buffer ) {
	
	if ( this->mWidth == 9 ) {
	
		for ( u32 i = 0; i < this->mCount; ++i ) {
			this->SetValue ( buffer, i, ZLMatrix3x3::IDENT );
		}
	}
	else if ( this->mWidth == 16 ) {
	
		for ( u32 i = 0; i < this->mCount; ++i ) {
			this->SetValue ( buffer, i, ZLMatrix4x4::IDENT );
		}
	}
	else {
	
		memset ( buffer, 0, this->mSize );
	}
}

//----------------------------------------------------------------//
void* MOAIShaderUniform::GetUniformBuffer ( void* buffer, u32 index ) {

	return ( void* )(( size_t )buffer + this->mCPUBase + ( this->mWidth * ELEMENT_SIZE * index ));
}

//----------------------------------------------------------------//
const void* MOAIShaderUniform::GetUniformBuffer ( const void* buffer, u32 index ) {

	return ( const void* )(( size_t )buffer + this->mCPUBase + ( this->mWidth * ELEMENT_SIZE * index ));
}

//----------------------------------------------------------------//
bool MOAIShaderUniform::Init ( u32 type, u32 width, u32 count ) {
	
	this->mType		= type;
	this->mWidth	= width;
	this->mCount	= count;
	this->mSize		= count * width * ELEMENT_SIZE;
}

//----------------------------------------------------------------//
MOAIShaderUniform::MOAIShaderUniform () :
	mType ( UNIFORM_TYPE_FLOAT ),
	mWidth ( 1 ),
	mCount ( 0 ),
	mSize ( 0 ),
	mCPUBase ( ZGL_INVALID_UNIFORM_ADDR ),
	mGPUBase ( ZGL_INVALID_UNIFORM_ADDR ) {
}

//----------------------------------------------------------------//
MOAIShaderUniform::~MOAIShaderUniform () {
}

//----------------------------------------------------------------//
u32 MOAIShaderUniform::SetRawValue ( void* buffer, u32 index, const void* srcBuffer ) {
	
	buffer = this->GetUniformBuffer ( buffer, index );
	
	if ( memcmp ( buffer, srcBuffer, this->mSize ) == 0 ) return 0;
	memcpy ( buffer, srcBuffer, this->mSize );
	return MOAIShaderUniform::UNIFORM_FLAG_DIRTY;
}

//----------------------------------------------------------------//
u32 MOAIShaderUniform::SetValue ( void* buffer, u32 index, const s32* srcBuffer ) {

	if ( this->mType == UNIFORM_TYPE_FLOAT ) {
	
		float fBuffer [ MAX_BUFFER_WIDTH ];
		for ( size_t i = 0; i < this->mWidth; ++i ) {
		
			fBuffer [ i ] = ( float )srcBuffer [ i ];
		}
		return this->SetRawValue ( buffer, index, fBuffer );
	}
	return this->SetRawValue ( buffer, index, srcBuffer );
}

//----------------------------------------------------------------//
u32 MOAIShaderUniform::SetValue ( void* buffer, u32 index, const float* srcBuffer ) {

	if ( this->mType == UNIFORM_TYPE_INT ) {
	
		s32 iBuffer [ MAX_BUFFER_WIDTH ];
		for ( size_t i = 0; i < this->mWidth; ++i ) {
		
			iBuffer [ i ] = ( s32 )srcBuffer [ i ];
		}
		return this->SetRawValue ( buffer, index, iBuffer );
	}
	return this->SetRawValue ( buffer, index, srcBuffer );
}

//----------------------------------------------------------------//
u32 MOAIShaderUniform::SetValue ( void* buffer, u32 index, float value ) {

	if ( this->mType == MOAIShaderUniform::UNIFORM_TYPE_INT ) {
		return this->SetValue ( buffer, index, ( s32 )value );
	}
	
	float* i = ( float* )(( size_t )buffer + this->mCPUBase );
	u32 flags = ( *i ) != value ? MOAIShaderUniform::UNIFORM_FLAG_DIRTY : 0;
	( *i ) = value;
	return flags;
}

//----------------------------------------------------------------//
u32 MOAIShaderUniform::SetValue ( void* buffer, u32 index, s32 value ) {

	if ( this->mType == MOAIShaderUniform::UNIFORM_TYPE_FLOAT ) {
		return this->SetValue ( buffer, index, ( float )value );
	}

	s32* i = ( s32* )(( size_t )buffer + this->mCPUBase );
	u32 flags = ( *i ) != value ? MOAIShaderUniform::UNIFORM_FLAG_DIRTY : 0;
	( *i ) = value;
	return flags;
}

//----------------------------------------------------------------//
u32 MOAIShaderUniform::SetValue ( void* buffer, u32 index, const MOAIAttribute& attr ) {

	switch ( attr.GetTypeID ()) {

		case MOAIAttribute::ATTR_TYPE_COLOR_VEC_4: {
			ZLColorVec color = attr.GetValue ( ZLColorVec::WHITE );
			return this->SetValue ( buffer, index, color );
		}
		
		case MOAIAttribute::ATTR_TYPE_FLOAT_32: {
			return this->SetValue ( buffer, index, ( float )attr.GetValue ( 0.0f ));
			break;
		}
		
		case MOAIAttribute::ATTR_TYPE_INT_32: {
			return this->SetValue ( buffer, index, ( int )attr.GetValue ( 0 ));
			break;
		}
		
		case MOAIAttribute::ATTR_TYPE_AFFINE_3D: {
			return this->SetValue ( buffer, index, attr.GetValue ( ZLAffine3D::IDENT ));
		}
		
		case MOAIAttribute::ATTR_TYPE_MATRIX_3X3: {
			return this->SetValue ( buffer, index, attr.GetValue ( ZLMatrix3x3::IDENT ));
		}
		
		case MOAIAttribute::ATTR_TYPE_MATRIX_4X4: {
			return this->SetValue ( buffer, index, attr.GetValue ( ZLMatrix4x4::IDENT ));
		}
		
		case MOAIAttribute::ATTR_TYPE_VEC_3: {
			ZLVec3D vec3 = attr.GetValue ( ZLVec3D::ORIGIN );
			ZLVec4D vec4 ( vec3.mX, vec3.mY, vec3.mZ, 0.0f );
			return this->SetValue ( buffer, index, vec4 );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
u32 MOAIShaderUniform::SetValue ( void* buffer, u32 index, const ZLColorVec& value ) {

	if ( this->mWidth == 1 ) return this->SetValue ( buffer, index, value.mR ); // TODO: this right?

	float m [ 4 ];

	m [ 0 ]		= value.mR;
	m [ 1 ]		= value.mG;
	m [ 2 ]		= value.mB;
	m [ 3 ]		= value.mA;

	return this->SetValue ( buffer, index, m );
}

//----------------------------------------------------------------//
u32 MOAIShaderUniform::SetValue ( void* buffer, u32 index, const ZLIntVec4D& value ) {

	if ( this->mWidth == 1 ) return this->SetValue ( buffer, index, value.mX );

	s32 m [ 4 ];
	
	m [ 0 ]		= value.mX;
	m [ 1 ]		= value.mY;
	m [ 2 ]		= value.mZ;
	m [ 3 ]		= value.mW;

	return this->SetValue ( buffer, index, m );
}

//----------------------------------------------------------------//
u32 MOAIShaderUniform::SetValue ( void* buffer, u32 index, const ZLAffine2D& value ) {

	if ( this->mWidth == 9 ) {
	
		return this->SetValue ( buffer, index, ZLMatrix3x3 ( value ));
	}
	else if ( this->mWidth == 16 ) {

		return this->SetValue ( buffer, index, ZLMatrix4x4 ( value ));
	}
	return 0;
}

//----------------------------------------------------------------//
u32 MOAIShaderUniform::SetValue ( void* buffer, u32 index, const ZLAffine3D& value ) {

	if ( this->mWidth == 9 ) {
	
		return this->SetValue ( buffer, index, ZLMatrix3x3 ( value ));
	}
	else if ( this->mWidth == 16 ) {

		return this->SetValue ( buffer, index, ZLMatrix4x4 ( value ));
	}
	return 0;
}

//----------------------------------------------------------------//
u32 MOAIShaderUniform::SetValue ( void* buffer, u32 index, const ZLMatrix3x3& value ) {

	if ( this->mWidth == 9 ) {
	
		return this->SetValue ( buffer, index, value.m );
	}
	else if ( this->mWidth == 16 ) {

		return this->SetValue ( buffer, index, ZLMatrix4x4 ( value ));
	}
	return 0;
}

//----------------------------------------------------------------//
u32 MOAIShaderUniform::SetValue ( void* buffer, u32 index, const ZLMatrix4x4& value ) {

	if ( this->mWidth == 9 ) {
	
		return this->SetValue ( buffer, index, ZLMatrix3x3 ( value ));
	}
	else if ( this->mWidth == 16 ) {

		return this->SetValue ( buffer, index, value.m );
	}
	return 0;
}

//----------------------------------------------------------------//
u32 MOAIShaderUniform::SetValue ( void* buffer, u32 index, const ZLVec4D& value ) {

	if ( this->mWidth == 1 ) return this->SetValue ( buffer, index, value.mX );

	float m [ 4 ];

	m [ 0 ]		= value.mX;
	m [ 1 ]		= value.mY;
	m [ 2 ]		= value.mZ;
	m [ 3 ]		= value.mW;

	return this->SetValue ( buffer, index, m );
}
