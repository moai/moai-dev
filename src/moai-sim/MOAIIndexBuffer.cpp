// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIIndexBuffer.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	release
	@text	Release any memory held by this index buffer.
	
	@in		MOAIIndexBuffer self
	@out	nil
*/
int	MOAIIndexBuffer::_release ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIIndexBuffer, "U" )
	
	self->Clear ();
	return 0;
}

//----------------------------------------------------------------//
/**	@name	reserve
	@text	Set capacity of buffer.
	
	@in		MOAIIndexBuffer self
	@in		number nIndices
	@out	nil
*/
int	MOAIIndexBuffer::_reserve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIIndexBuffer, "UN" )
	
	u32 indexCount = state.GetValue < u32 >( 2, 0 );
	self->ReserveIndices ( indexCount );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setIndex
	@text	Initialize an index.
	
	@in		MOAIIndexBuffer self
	@in		number idx
	@in		number value
	@out	nil
*/
int	MOAIIndexBuffer::_setIndex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIIndexBuffer, "UNN" )
	
	u32 idx		= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 value	= state.GetValue < u32 >( 3, 1 ) - 1;
	
	self->SetIndex ( idx, value );
	
	return 0;
}

//================================================================//
// MOAIGfxQuadListDeck2D
//================================================================//

//----------------------------------------------------------------//
bool MOAIIndexBuffer::IsRenewable () {

	return true;
}

//----------------------------------------------------------------//
bool MOAIIndexBuffer::IsValid () {

	return ( this->mGLBufferID != 0 );
}

//----------------------------------------------------------------//
MOAIIndexBuffer::MOAIIndexBuffer () :
	mBuffer ( 0 ),
	mIndexCount ( 0 ),
	mGLBufferID ( 0 ),
	mHint ( ZGL_BUFFER_USAGE_STATIC_DRAW ) {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIIndexBuffer::~MOAIIndexBuffer () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::OnBind () {

	if ( this->mGLBufferID ) {
		zglBindBuffer ( ZGL_BUFFER_TARGET_ELEMENT_ARRAY, this->mGLBufferID );
	}
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::OnClear () {
	
	if ( this->mBuffer ) {
		free ( this->mBuffer );
		this->mBuffer = 0;
	}
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::OnCreate () {

	if ( this->mBuffer ) {
		
		this->mGLBufferID = zglCreateBuffer ();
		if ( this->mGLBufferID ) {
		
			zglBindBuffer ( ZGL_BUFFER_TARGET_ELEMENT_ARRAY, this->mGLBufferID );
			zglBufferData ( ZGL_BUFFER_TARGET_ELEMENT_ARRAY, this->mIndexCount * sizeof ( u32 ), this->mBuffer, this->mHint );
		}
	}
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::OnDestroy () {

	if ( this->mGLBufferID ) {
		zglDeleteBuffer ( this->mGLBufferID );
		this->mGLBufferID = 0;
	}
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::OnInvalidate () {

	this->mGLBufferID = 0;
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::OnLoad () {
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::OnUnbind () {

	zglBindBuffer ( ZGL_BUFFER_TARGET_ELEMENT_ARRAY, 0 );
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "release",			_release },
		{ "reserve",			_reserve },
		{ "setIndex",			_setIndex },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::ReserveIndices ( u32 indexCount ) {

	this->Clear ();
	
	this->mIndexCount = indexCount;
	
	size_t size = indexCount * sizeof ( u32 );
	this->mBuffer = ( u32* )malloc ( size );
	this->mStream.SetBuffer ( this->mBuffer, size );
	
	this->Load ();
}

//----------------------------------------------------------------//
void MOAIIndexBuffer::SetIndex ( u32 idx, u32 value ) {

	if ( idx < this->mIndexCount ) {
		this->mBuffer [ idx ] = value;
	}
}
