// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIVertexFormat.h>
#include <moaicore/MOAIVertexBuffer.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexBuffer::_bless ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexBuffer, "U" )
	
	self->mBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	
	if ( self->mFormat ) {
		self->mFormat->ComputeBounds ( self->mBuffer, self->mStream.GetLength (), self->mBounds );
	}
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIVertexBuffer::_release ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexBuffer, "U" )
	
	self->Release ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIVertexBuffer::_reserve ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexBuffer, "UN" )
	
	u32 size = state.GetValue < u32 >( 2, 0 );
	self->Reserve ( size );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIVertexBuffer::_reserveVerts ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexBuffer, "UN" )
	
	u32 total = state.GetValue < u32 >( 2, 0 );
	
	if ( self->mFormat ) {
		self->Reserve ( total * self->mFormat->GetVertexSize ());
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIVertexBuffer::_reset ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexBuffer, "U" )
	
	self->mStream.SetBuffer ( self->mBuffer, self->mBuffer.Size ());
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIVertexBuffer::_seek ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexBuffer, "UN" )
	
	u32 pos = state.GetValue < u32 >( 2, 0 );
	self->mStream.Seek ( pos, SEEK_SET );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexBuffer::_setFormat ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexBuffer, "U" )
	
	self->mFormat = state.GetLuaObject < MOAIVertexFormat >( 2 );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexBuffer::_setPrimType ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexBuffer, "UN" )
	
	u32 primType = state.GetValue < u32 >( 2, 0 );
	self->SetPrimType ( primType );

	return 0;
}

//----------------------------------------------------------------//
int MOAIVertexBuffer::_writeColor ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexBuffer, "U" )
	
	float r = state.GetValue < float >( 2, 1.0f );
	float g = state.GetValue < float >( 3, 1.0f );
	float b = state.GetValue < float >( 4, 1.0f );
	float a = state.GetValue < float >( 5, 1.0f );
	
	u32 color = USColor::PackRGBA ( r, g, b, a );
	self->mStream.Write < u32 >( color );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIVertexBuffer::_writeFloat ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexBuffer, "U" )
	
	u32 top = state.GetTop ();
	for ( u32 i = 2; i <= top; ++i ) {
		float param = state.GetValue < float >( i, 0.0f );
		self->mStream.Write < float >( param );
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIVertexBuffer::_writeInt8 ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexBuffer, "U" )
	
	u32 top = state.GetTop ();
	for ( u32 i = 2; i <= top; ++i ) {
		int param = state.GetValue < int >( i, 0 );
		self->mStream.Write < s8 >(( s8 )param );
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIVertexBuffer::_writeInt16 ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexBuffer, "U" )
	
	u32 top = state.GetTop ();
	for ( u32 i = 2; i <= top; ++i ) {
		int param = state.GetValue < int >( i, 0 );
		self->mStream.Write < s16 >(( s16 )param );
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIVertexBuffer::_writeInt32 ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexBuffer, "U" )
	
	u32 top = state.GetTop ();
	for ( u32 i = 2; i <= top; ++i ) {
		int param = state.GetValue < int >( i, 0 );
		self->mStream.Write < s32 >(( s32 )param );
	}
	return 0;
}

//================================================================//
// MOAIGfxQuadListDeck2D
//================================================================//

//----------------------------------------------------------------//
void MOAIVertexBuffer::Draw () {

	if ( !this->mFormat ) return;
	
	u32 nVerts = ( u32 )( this->mStream.GetLength () / this->mFormat->GetVertexSize ());
	if ( nVerts ) {
	
		this->mFormat->Bind ( this->mBuffer );
		glDrawArrays ( this->mPrimType, 0, nVerts );
	}
}

//----------------------------------------------------------------//
bool MOAIVertexBuffer::IsValid () {

	return ( this->mFormat && this->mStream.GetLength ());
}

//----------------------------------------------------------------//
MOAIVertexBuffer::MOAIVertexBuffer () :
	mPrimSize ( 0 ) {
	
	RTTI_SINGLE ( MOAIVertexBuffer )
	
	this->mBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIVertexBuffer::~MOAIVertexBuffer () {

	this->Release ();
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::RegisterLuaClass ( USLuaState& state ) {
	
	state.SetField ( -1, "GL_POINTS", ( u32 )GL_POINTS );
	state.SetField ( -1, "GL_LINES", ( u32 )GL_LINES );
	state.SetField ( -1, "GL_TRIANGLES", ( u32 )GL_TRIANGLES );
	state.SetField ( -1, "GL_LINE_LOOP", ( u32 )GL_LINE_LOOP );
	state.SetField ( -1, "GL_LINE_STRIP", ( u32 )GL_LINE_STRIP );
	state.SetField ( -1, "GL_TRIANGLE_FAN", ( u32 )GL_TRIANGLE_FAN );
	state.SetField ( -1, "GL_TRIANGLE_STRIP", ( u32 )GL_TRIANGLE_STRIP );
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "bless",					_bless },
		{ "release",				_release },
		{ "reserve",				_reserve },
		{ "reserveVerts",			_reserveVerts },
		{ "reset",					_reset },
		{ "seek",					_seek },
		{ "setFormat",				_setFormat },
		{ "setPrimType",			_setPrimType },
		{ "writeColor",				_writeColor },
		{ "writeFloat",				_writeFloat },
		{ "writeInt8",				_writeInt8 },
		{ "writeInt16",				_writeInt16 },
		{ "writeInt32",				_writeInt32 },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::Release () {

	this->Reserve ( 0 );
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::Reserve ( u32 size ) {

	this->mBuffer.Init ( size );
	this->mStream.SetBuffer ( this->mBuffer, size );
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::SetPrimType ( u32 primType ) {

	if ( this->mPrimType != primType ) {
		
		this->mPrimType = primType;

		switch ( primType ) {
		
			case GL_POINTS:
				this->mPrimSize = 1;
				break;
			
			case GL_LINES:
				this->mPrimSize = 2;
				break;
			
			case GL_TRIANGLES:
				this->mPrimSize = 3;
				break;
			
			case GL_LINE_LOOP:
			case GL_LINE_STRIP:
			case GL_TRIANGLE_FAN:
			case GL_TRIANGLE_STRIP:
			default:
				this->mPrimSize = 0;
				break;
		}
	}
}

//----------------------------------------------------------------//
STLString MOAIVertexBuffer::ToString () {

	STLString repr;
	return repr;
}
