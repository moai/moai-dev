// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIVertexBuffer.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int	MOAIVertexBuffer::_release ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexBuffer, "U" )
	
	self->Release ();
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int	MOAIVertexBuffer::_reserve ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexBuffer, "UN" )
	
	u32 vertexCount = state.GetValue < u32 >( 2, 0 );
	self->Reserve ( vertexCount );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int	MOAIVertexBuffer::_setFormat ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexBuffer, "UU" )
	
	self->mFormat = state.GetLuaObject < MOAIVertexFormat >( 2 );
	
	return 0;
}

//================================================================//
// MOAIGfxQuadListDeck2D
//================================================================//

//----------------------------------------------------------------//
bool MOAIVertexBuffer::Bind () {

	//if ( this->mBuffer ) {
	//
	//	this->Release ();
	//	
	//	glGenBuffers ( 1, &this->mGLBufferID );
	//	if ( !this->mGLBufferID ) return false;
	//	
	//	glBindBuffer ( GL_ARRAY_BUFFER, this->mGLBufferID );
	//	glBufferData ( GL_ARRAY_BUFFER, this->mVertexSize * this->mVertexCount, this->mBuffer, this->mHint );
	//	
	//	this->ClearBuffer ();
	//}
	//
	//glBindBuffer ( GL_ARRAY_BUFFER, this->mGLBufferID );
	return true;
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::ClearBuffer () {

	if ( this->mBuffer ) {
		free ( this->mBuffer );
		this->mBuffer = 0;
	}
}

//----------------------------------------------------------------//
MOAIVertexBuffer::MOAIVertexBuffer () :
	mBuffer ( 0 ),
	mBufferSize ( 0 ),
	mGLBufferID ( 0 ),
	mHint ( GL_STATIC_DRAW ) {
	
	RTTI_SINGLE ( MOAIVertexBuffer )
}

//----------------------------------------------------------------//
MOAIVertexBuffer::~MOAIVertexBuffer () {

	this->Release ();
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "release",				_release },
		{ "reserve",				_reserve },
		{ "setFormat",				_setFormat },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::Release () {

	this->ClearBuffer ();
	if ( this->mGLBufferID ) {
		glDeleteBuffers ( 1, &this->mGLBufferID );
		this->mGLBufferID = 0;
	}
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::Reserve ( u32 vertexCount ) {

	this->ClearBuffer ();
	
	if ( this->mFormat ) {
	
		u32 vertexSize = this->mFormat->GetVertexSize ();
	
		if ( vertexSize ) {
			this->mBufferSize = vertexCount * vertexSize;
			this->mBuffer = malloc ( this->mBufferSize );
		}
	}
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::SetVertexElement ( u32 vtx, u32 elemIdx, float* params ) {
	
	if ( !this->mBuffer ) return;
	if ( !this->mFormat ) return;
	
	this->mFormat->SetVertexElement ( this->mBuffer, this->mBufferSize, vtx, elemIdx, params );
}

//----------------------------------------------------------------//
STLString MOAIVertexBuffer::ToString () {

	STLString repr;
	return repr;
}
