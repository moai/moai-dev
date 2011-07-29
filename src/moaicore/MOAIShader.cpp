// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIEaseDriver.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIShader.h>
#include <moaicore/MOAIShader.h>
#include <moaicore/MOAITransformBase.h>

//================================================================//
// MOAIShaderUniform
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderUniform::Bind ( const float* attributes ) {

	switch ( this->mType ) {
		
		case UNIFORM_INT: {
			
			// TODO: convert from floats to ints
			//glUniform1iv ( this->mAddr, this->mSize, &attributes [ this->mSrc ]);
			break;
		}
		case UNIFORM_FLOAT: {
			
			glUniform1fv ( this->mAddr, this->mSize, &attributes [ this->mSrc ]);
			break;
		}
		case UNIFORM_MODEL: {
			
			USAffine2D affine = MOAIGfxDevice::Get ().GetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM );
			this->BindAffine ( affine );
			break;
		}
		case UNIFORM_TRANSFORM: {
			
			if ( this->mTransform ) {
				USAffine2D affine = this->mTransform->GetLocalToWorldMtx ();
				this->BindAffine ( affine );
			}
			break;
		}
		case UNIFORM_VIEW_PROJ: {
		
			USAffine2D affine = MOAIGfxDevice::Get ().GetViewProjMtx ();
			this->BindAffine ( affine );
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
int MOAIShader::_clearUniform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShader, "UN" )
	
	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
	
	self->ClearUniform ( idx );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShader::_declareUniform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShader, "UNS" )
	
	u32 idx				= state.GetValue < u32 >( 2, 1 ) - 1;
	STLString name		= state.GetValue < cc8* >( 3, "" );
	u32  type			= state.GetValue < u32 >( 4, MOAIShaderUniform::UNIFORM_NONE );
	
	self->DeclareUniform ( idx, name, type );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShader::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShader, "USS" )
	
	cc8* vtxSource	= state.GetValue < cc8* >( 2, 0 );
	cc8* frgSource	= state.GetValue < cc8* >( 3, 0 );
	
	self->SetSource ( vtxSource, frgSource );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShader::_reserveAttributes ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShader, "U" )
	
	u32 nAttributes = state.GetValue < u32 >( 2, 0 );
	self->ReserveAttributes ( nAttributes );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShader::_reserveUniforms ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShader, "U" )
	
	u32 nUniforms = state.GetValue < u32 >( 2, 0 );
	self->ReserveUniforms ( nUniforms );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShader::_setUniform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShader, "U" )
	
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
int MOAIShader::_setVertexAttribute ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShader, "UNS" )
	
	GLuint idx				= state.GetValue < GLuint >( 2, 0 );
	STLString attribute		= state.GetValue < cc8* >( 3, "" );
	
	self->SetVertexAttribute ( idx, attribute );

	return 0;
}

//================================================================//
// MOAIShader
//================================================================//

//----------------------------------------------------------------//
void MOAIShader::Affirm () {

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
void MOAIShader::Bind () {

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
void MOAIShader::Clear () {

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
void MOAIShader::ClearUniform ( u32 idx ) {

	if ( idx < this->mUniforms.Size ()) {
		
		MOAIShaderUniform& uniform = this->mUniforms [ idx ];
		
		uniform.mName.clear ();
		uniform.mType = MOAIShaderUniform::UNIFORM_NONE;
		uniform.mTransform = 0;
	}
}

//----------------------------------------------------------------//
GLuint MOAIShader::CompileShader ( GLuint type, cc8* source ) {

	GLuint shader = glCreateShader ( type );
	glShaderSource ( shader, 1, &source, NULL );
	glCompileShader ( shader );

	GLint status;
	glGetShaderiv ( shader, GL_COMPILE_STATUS, &status );
	
	if ( status == 0 ) {
	
		int logLength;
		glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &logLength );
 
		/* The maxLength includes the NULL character */
		GLchar* log = ( GLchar* )malloc ( logLength );
 
		glGetShaderInfoLog ( shader, logLength, &logLength, log );
		printf ( "%s", log );
		
		free ( log );
	
		glDeleteShader ( shader );
		return 0;
	}

	return shader;
}

//----------------------------------------------------------------//
void MOAIShader::DeclareUniform ( u32 idx, cc8* name, u32 type ) {
	
	if ( idx < this->mUniforms.Size ()) {
		
		this->ClearUniform ( idx );
		
		MOAIShaderUniform& uniform = this->mUniforms [ idx ];
		
		uniform.mName	= name;
		uniform.mType	= type;
	}
}

//----------------------------------------------------------------//
MOAIShader::MOAIShader () :
	mState ( STATE_UNINITIALIZED ),
	mProgram ( 0 ),
	mVertexShader ( 0 ),
	mFragmentShader ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAINode )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIShader::~MOAIShader () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIShader::RegisterLuaClass ( USLuaState& state ) {
	
	MOAINode::RegisterLuaClass ( state );
	
	state.SetField ( -1, "UNIFORM_INT",			( u32 )MOAIShaderUniform::UNIFORM_INT );
	state.SetField ( -1, "UNIFORM_FLOAT",		( u32 )MOAIShaderUniform::UNIFORM_FLOAT );
	state.SetField ( -1, "UNIFORM_MODEL",		( u32 )MOAIShaderUniform::UNIFORM_MODEL );
	state.SetField ( -1, "UNIFORM_TRANSFORM",	( u32 )MOAIShaderUniform::UNIFORM_TRANSFORM );
	state.SetField ( -1, "UNIFORM_VIEW_PROJ",	( u32 )MOAIShaderUniform::UNIFORM_VIEW_PROJ );
}

//----------------------------------------------------------------//
void MOAIShader::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAINode::RegisterLuaFuncs ( state );
	
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
void MOAIShader::ReserveAttributes ( u32 nAttributes ) {

	this->mAttributes.Init ( nAttributes );
	memset ( this->mAttributes, 0, nAttributes * sizeof ( float ));
}

//----------------------------------------------------------------//
void MOAIShader::ReserveUniforms ( u32 nUniforms ) {

	this->mUniforms.Init ( nUniforms );
}

//----------------------------------------------------------------//
void MOAIShader::SetSource ( cc8* vshSource, cc8* fshSource ) {

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
void MOAIShader::SetUniform ( u32 idx, u32 src, u32 size ) {
	
	if ( idx < this->mUniforms.Size ()) {
		
		MOAIShaderUniform& uniform = this->mUniforms [ idx ];
		if ( uniform.mType == MOAIShaderUniform::UNIFORM_TRANSFORM ) return;
		
		uniform.mSrc	= src;
		uniform.mSize	= size;
	}
}

//----------------------------------------------------------------//
void MOAIShader::SetUniform ( u32 idx, MOAITransformBase* transform ) {
	
	if ( idx < this->mUniforms.Size ()) {
		
		MOAIShaderUniform& uniform = this->mUniforms [ idx ];
		if ( uniform.mType != MOAIShaderUniform::UNIFORM_TRANSFORM ) return;
		
		uniform.mTransform	= transform;
	}
}

//----------------------------------------------------------------//
void MOAIShader::SetVertexAttribute ( u32 idx, cc8* attribute ) {

	if ( attribute ) {
		this->mAttributeMap [ idx ] = attribute;
	}
}

//----------------------------------------------------------------//
STLString MOAIShader::ToString () {

	STLString repr;

	return repr;
}

//----------------------------------------------------------------//
void MOAIShader::Validate () {

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
