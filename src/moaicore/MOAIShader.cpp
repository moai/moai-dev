// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIEaseDriver.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIShader.h>
#include <moaicore/MOAITransformBase.h>

//================================================================//
// MOAIShaderUniform
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderUniform::BindAttributes ( const float* attributes ) {

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
		case UNIFORM_TRANSFORM: {
			
			if ( this->mTransform ) {
				const USAffine2D& affine = this->mTransform->GetLocalToWorldMtx ();
				USMatrix4x4 matrix;
				matrix.Init ( affine );
				this->BindMatrix ( matrix );
			}
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniform::BindMatrix ( const USMatrix4x4& matrix ) {
	
	float m [ 16 ];
	
	m [ 0 ]		= matrix.m [ USMatrix4x4::C0_R0 ];
	m [ 1 ]		= matrix.m [ USMatrix4x4::C1_R0 ];
	m [ 2 ]		= matrix.m [ USMatrix4x4::C2_R0 ];
	m [ 3 ]		= matrix.m [ USMatrix4x4::C3_R0 ];
	
	m [ 4 ]		= matrix.m [ USMatrix4x4::C0_R1 ];
	m [ 5 ]		= matrix.m [ USMatrix4x4::C1_R1 ];
	m [ 6 ]		= matrix.m [ USMatrix4x4::C2_R1 ];
	m [ 7 ]		= matrix.m [ USMatrix4x4::C3_R1 ];
	
	m [ 8 ]		= matrix.m [ USMatrix4x4::C0_R2 ];
	m [ 9 ]		= matrix.m [ USMatrix4x4::C1_R2 ];
	m [ 10 ]	= matrix.m [ USMatrix4x4::C2_R2 ];
	m [ 11 ]	= matrix.m [ USMatrix4x4::C3_R2 ];
	
	m [ 12 ]	= matrix.m [ USMatrix4x4::C0_R3 ];
	m [ 13 ]	= matrix.m [ USMatrix4x4::C1_R3 ];
	m [ 14 ]	= matrix.m [ USMatrix4x4::C2_R3 ];
	m [ 15 ]	= matrix.m [ USMatrix4x4::C3_R3 ];
	
	glUniformMatrix4fv ( this->mAddr, 1, false, m );
}

//----------------------------------------------------------------//
void MOAIShaderUniform::BindPipelineTransforms ( const USMatrix4x4& world, const USMatrix4x4& view, const USMatrix4x4& proj ) {

	switch ( this->mType ) {
		
		case UNIFORM_VIEW_PROJ: {
			
			USMatrix4x4 mtx = view;
			mtx.Append ( proj );
			this->BindMatrix ( mtx );
			break;
		}
		case UNIFORM_WORLD: {
			
			this->BindMatrix ( world );
			break;
		}
		case UNIFORM_WORLD_VIEW_PROJ: {
			
			USMatrix4x4 mtx = world;
			mtx.Append ( view );
			mtx.Append ( proj );
			this->BindMatrix ( mtx );
			break;
		}
	}
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
	
	GLuint idx				= state.GetValue < GLuint >( 2, 1 ) - 1;
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
			this->mUniforms [ i ].BindAttributes ( this->mAttributes );
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

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	GLuint shader = glCreateShader ( type );
	cc8* sources [ 2 ];
	
	sources [ 0 ] = gfxDevice.IsOpenGLES () ? OPENGL_ES_PREPROC : OPENGL_PREPROC;
	sources [ 1 ] = source;
	
	glShaderSource ( shader, 2, sources, NULL );
	glCompileShader ( shader );

	GLint status;
	glGetShaderiv ( shader, GL_COMPILE_STATUS, &status );
	
	if ( status == 0 ) {
		
		int logLength;
		glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &logLength );
 
		char* log = ( char* )malloc ( logLength );
 
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
	
	state.SetField ( -1, "UNIFORM_INT",					( u32 )MOAIShaderUniform::UNIFORM_INT );
	state.SetField ( -1, "UNIFORM_FLOAT",				( u32 )MOAIShaderUniform::UNIFORM_FLOAT );
	state.SetField ( -1, "UNIFORM_TRANSFORM",			( u32 )MOAIShaderUniform::UNIFORM_TRANSFORM );
	state.SetField ( -1, "UNIFORM_VIEW_PROJ",			( u32 )MOAIShaderUniform::UNIFORM_VIEW_PROJ );
	state.SetField ( -1, "UNIFORM_WORLD",				( u32 )MOAIShaderUniform::UNIFORM_WORLD );
	state.SetField ( -1, "UNIFORM_WORLD_VIEW_PROJ",		( u32 )MOAIShaderUniform::UNIFORM_WORLD_VIEW_PROJ );
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
void MOAIShader::UpdatePipelineTransforms ( const USMatrix4x4& world, const USMatrix4x4& view, const USMatrix4x4& proj ) {

	// reload the uniform values
	for ( u32 i = 0; i < this->mUniforms.Size (); ++i ) {
		this->mUniforms [ i ].BindPipelineTransforms ( world, view, proj );
	}
}

//----------------------------------------------------------------//
void MOAIShader::Validate () {

    GLint logLength;
    
    glValidateProgram ( this->mProgram );
    glGetProgramiv ( this->mProgram, GL_INFO_LOG_LENGTH, &logLength );
	
    if ( logLength > 0 ) {
        char* log = ( char* )malloc ( logLength );
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
