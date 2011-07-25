// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIEaseDriver.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIGlslProgram.h>
#include <moaicore/MOAITransformBase.h>

//================================================================//
// MOAIShaderUniform
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderUniform::Bind ( const float* attributes ) {

	switch ( this->mType ) {
		
		case UNIFORM_FLOAT: {
			
			glUniform1fv ( this->mAddr, this->mSize, &attributes [ this->mSrc ]);
			break;
		}
		case UNIFORM_MODEL: {
			
			const USAffine2D& affine = MOAIGfxDevice::Get ().GetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM );
			this->BindAffine ( affine );
			break;
		}
		case UNIFORM_TRANSFORM: {
			
			if ( this->mTransform ) {
				const USAffine2D& affine = this->mTransform->GetLocalToWorldMtx ();
				this->BindAffine ( affine );
			}
			break;
		}
		case UNIFORM_VIEW_PROJ: {
		
			// TODO
			//const USAffine2D& affine = MOAIGfxDevice::Get ().GetCameraTransform ();
			//this->BindAffine ( affine );
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniform::BindAffine ( const USAffine2D& affine ) {

	float mtx [ 16 ];
	const float* m = affine.m;
	
	mtx [ 0x00 ] = m [ USAffine2D::C0_R0 ];
	mtx [ 0x01 ] = m [ USAffine2D::C0_R1 ];
	mtx [ 0x02 ] = 0.0f;
	mtx [ 0x03 ] = 0.0f;
	
	mtx [ 0x04 ] = m [ USAffine2D::C1_R0 ];
	mtx [ 0x05 ] = m [ USAffine2D::C1_R1 ];
	mtx [ 0x06 ] = 0.0f;
	mtx [ 0x07 ] = 0.0f;
	
	mtx [ 0x08 ] = 0.0f;
	mtx [ 0x09 ] = 0.0f;
	mtx [ 0x0a ] = 1.0f;
	mtx [ 0x0b ] = 0.0f;
	
	mtx [ 0x0c ] = m [ USAffine2D::C2_R0 ];
	mtx [ 0x0d ] = m [ USAffine2D::C2_R1 ];
	mtx [ 0x0e ] = 0.0f;
	mtx [ 0x0f ] = 1.0f;
	
	glUniformMatrix4fv ( this->mAddr, 1, true, mtx );
}

//----------------------------------------------------------------//
MOAIShaderUniform::MOAIShaderUniform () :
	mType ( UNIFORM_NONE ) {
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGlslProgram::_clearUniform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGlslProgram, "UN" )
	
	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	
	self->ClearUniform ( idx );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGlslProgram::_declareUniform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGlslProgram, "UNS" )
	
	u32 idx				= state.GetValue < u32 >( 2, 1 ) - 1;
	STLString name		= state.GetValue < cc8* >( 3, "" );
	u32  type			= state.GetValue < u32 >( 4, MOAIShaderUniform::UNIFORM_NONE );
	
	if ( idx < self->mUniforms.Size ()) {
		
		self->ClearUniform ( idx );
		
		MOAIShaderUniform& uniform = self->mUniforms [ idx ];
		
		uniform.mName	= name;
		uniform.mType	= type;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGlslProgram::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGlslProgram, "USS" )
	
	cc8* vtxSource	= state.GetValue < cc8* >( 2, 0 );
	cc8* frgSource	= state.GetValue < cc8* >( 3, 0 );
	
	self->SetSource ( vtxSource, frgSource );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGlslProgram::_reserveAttributes ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGlslProgram, "U" )
	
	u32 total = state.GetValue < u32 >( 2, 0 );
	self->mAttributes.Init ( total );
	
	memset ( self->mAttributes, 0, total * sizeof ( float ));
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGlslProgram::_reserveUniforms ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGlslProgram, "U" )
	
	u32 total = state.GetValue < u32 >( 2, 0 );
	self->mUniforms.Init ( total );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGlslProgram::_setUniform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGlslProgram, "U" )
	
	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	
	if ( idx < self->mUniforms.Size ()) {
		
		MOAIShaderUniform& uniform = self->mUniforms [ idx ];
		
		switch ( uniform.mType ) {
			case MOAIShaderUniform::UNIFORM_FLOAT:
			case MOAIShaderUniform::UNIFORM_INT:
				uniform.mSrc	= state.GetValue < u32 >( 3, 0 );
				uniform.mSize	= state.GetValue < u32 >( 4, 0 );
				break;
			case MOAIShaderUniform::UNIFORM_TRANSFORM:
				uniform.mTransform	= state.GetLuaObject < MOAITransformBase >( 3 );
				break;
		}
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGlslProgram::_setVertexAttribute ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGlslProgram, "UNS" )
	
	GLuint index			= state.GetValue < GLuint >( 2, 0 );
	STLString attribute		= state.GetValue < cc8* >( 3, "" );
	
	if ( attribute.length ()) {
		self->mAttributeMap [ index ] = attribute;
	}
	return 0;
}

//================================================================//
// MOAIGlslProgram
//================================================================//

//----------------------------------------------------------------//
void MOAIGlslProgram::Affirm () {

	if ( this->mState != STATE_PENDING_LOAD ) return;

	this->mVertexShader = this->CompileShader ( GL_VERTEX_SHADER, this->mVertexShaderSource );
	this->mFragmentShader = this->CompileShader ( GL_FRAGMENT_SHADER, this->mFragmentShaderSource );
	this->mProgram = glCreateProgram ();
	
	if ( !( this->mVertexShader && this->mFragmentShader && this->mProgram )) {
		this->mState = STATE_ERROR;
		this->Clear ();
		return;
	}
    
	glAttachShader ( this->mProgram, this->mVertexShader );
	glAttachShader ( this->mProgram, this->mFragmentShader );
    
	// bind attribute locations.
	// this needs to be done prior to linking.
	AttributeMapIt attrMapIt = this->mAttributeMap.begin ();
	for ( ; attrMapIt != this->mAttributeMap.end (); ++attrMapIt ) {
		glBindAttribLocation ( this->mProgram, attrMapIt->first, attrMapIt->second.str ());
	}
    
    // link program.
	glLinkProgram ( this->mProgram );
	
	GLint status;
	glGetProgramiv ( this->mProgram, GL_LINK_STATUS, &status );
	
	if ( status == 0 ) {
		this->mState = STATE_ERROR;
		this->Clear ();
		return;
	}
	
	// get the uniform locations and clear out the names (no longer needed)
	for ( u32 i = 0; i < this->mUniforms.Size (); ++i ) {
		MOAIShaderUniform& uniform = this->mUniforms [ i ];
		
		if ( uniform.mType != MOAIShaderUniform::UNIFORM_NONE ) {
			uniform.mAddr = glGetUniformLocation ( this->mProgram, uniform.mName );
			uniform.mName.clear ();
		}
	}

	glDeleteShader ( this->mVertexShader );
	this->mVertexShader = 0;
	
	glDeleteShader ( this->mFragmentShader );
	this->mFragmentShader = 0;
	
	this->mAttributeMap.clear ();
	
	this->mState = STATE_VALID;
}

//----------------------------------------------------------------//
void MOAIGlslProgram::Bind () {

	this->Affirm ();
	
	if ( this->mState == STATE_VALID ) {
		
		// use shader program.
		glUseProgram ( this->mProgram );

		// reload the uniform values
		for ( u32 i = 0; i < this->mUniforms.Size (); ++i ) {
			this->mUniforms [ i ].Bind ( this->mAttributes );
		}
	}
}

//----------------------------------------------------------------//
void MOAIGlslProgram::Clear () {

	if ( this->mVertexShader ) {
		glDeleteShader ( this->mVertexShader );
		this->mVertexShader = 0;
	}
	
	if ( this->mFragmentShader ) {
		glDeleteShader ( this->mFragmentShader );
		this->mFragmentShader = 0;
	}
	
	if ( this->mProgram ) {
		glDeleteProgram ( this->mProgram );
		this->mProgram = 0;
	}
	
	this->mAttributeMap.clear ();
	this->mUniforms.Clear ();
}

//----------------------------------------------------------------//
void MOAIGlslProgram::ClearUniform ( u32 idx ) {

	if ( idx < this->mUniforms.Size ()) {
		
		MOAIShaderUniform& uniform = this->mUniforms [ idx ];
		
		uniform.mName.clear ();
		uniform.mType = MOAIShaderUniform::UNIFORM_NONE;
		uniform.mTransform = 0;
	}
}

//----------------------------------------------------------------//
GLuint MOAIGlslProgram::CompileShader ( GLuint type,  cc8* source ) {

	GLuint shader = glCreateShader ( type );
	glShaderSource ( shader, 1, &source, NULL );
	glCompileShader ( shader );

	GLint status;
	glGetShaderiv ( shader, GL_COMPILE_STATUS, &status );
	
	if ( status == 0 ) {
		glDeleteShader ( shader );
		return 0;
	}

	return shader;
}

//----------------------------------------------------------------//
MOAIGlslProgram::MOAIGlslProgram () :
	mState ( STATE_UNINITIALIZED ),
	mProgram ( 0 ),
	mVertexShader ( 0 ),
	mFragmentShader ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIShader )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGlslProgram::~MOAIGlslProgram () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIGlslProgram::RegisterLuaClass ( USLuaState& state ) {
	
	MOAIShader::RegisterLuaClass ( state );
	
	state.SetField ( -1, "UNIFORM_INT",			( u32 )MOAIShaderUniform::UNIFORM_INT );
	state.SetField ( -1, "UNIFORM_FLOAT",		( u32 )MOAIShaderUniform::UNIFORM_FLOAT );
	state.SetField ( -1, "UNIFORM_MODEL",		( u32 )MOAIShaderUniform::UNIFORM_MODEL );
	state.SetField ( -1, "UNIFORM_TRANSFORM",	( u32 )MOAIShaderUniform::UNIFORM_TRANSFORM );
	state.SetField ( -1, "UNIFORM_VIEW_PROJ",	( u32 )MOAIShaderUniform::UNIFORM_VIEW_PROJ );
}

//----------------------------------------------------------------//
void MOAIGlslProgram::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAIShader::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "clearUniform",				_clearUniform },
		{ "declareUniform",				_declareUniform },
		{ "load",						_load },
		{ "reserveAttributes",			_reserveAttributes },
		{ "reserveUniforms",			_reserveUniforms },
		{ "setUniform",					_setUniform },
		{ "setVertexAttribute",			_setVertexAttribute },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGlslProgram::SetSource ( cc8* vshSource, cc8* fshSource ) {

	if ( this->mState != STATE_UNINITIALIZED ) {
		this->Clear ();
	}

	this->mState = STATE_UNINITIALIZED;

	if ( vshSource && fshSource ) {

		this->mVertexShaderSource = vshSource;
		this->mFragmentShaderSource = fshSource;
		
		this->mState = STATE_PENDING_LOAD;
	}
}

//----------------------------------------------------------------//
STLString MOAIGlslProgram::ToString () {

	STLString repr;

	return repr;
}

//----------------------------------------------------------------//
void MOAIGlslProgram::Validate () {

    GLint logLength;
    
    glValidateProgram ( this->mProgram );
    glGetProgramiv ( this->mProgram, GL_INFO_LOG_LENGTH, &logLength );
	
    if ( logLength > 0 ) {
        GLchar *log = ( GLchar* )malloc ( logLength );
        glGetProgramInfoLog ( this->mProgram, logLength, &logLength, log );
        printf ( "%s", log );
        free ( log );
    }
    
	GLint status;
    glGetProgramiv ( this->mProgram, GL_VALIDATE_STATUS, &status );
    if ( status == 0 ) {
		this->mState = STATE_ERROR;
		this->Clear ();
	}
}
