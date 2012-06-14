// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIVertexFormat.h>
#include <moaicore/MOAIVertexBuffer.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	bless
	@text	Call this after initializing the buffer and settings it vertices
			to prepare it for use.
	
	@in		MOAIVertexBuffer self
	@out	nil
*/
int MOAIVertexBuffer::_bless ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexBuffer, "U" )
	
	self->mBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	
	if ( self->mFormat ) {
		self->mFormat->ComputeBounds ( self->mBuffer, self->mStream.GetLength (), self->mBounds );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	release
	@text	Releases any memory associated with buffer.
	
	@in		MOAIVertexBuffer self
	@out	nil
*/
int	MOAIVertexBuffer::_release ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexBuffer, "U" )
	
	self->Clear ();
	return 0;
}

//----------------------------------------------------------------//
/**	@name	reserve
	@text	Sets capacity of buffer in bytes.
	
	@in		MOAIVertexBuffer self
	@in		number size
	@out	nil
*/
int	MOAIVertexBuffer::_reserve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexBuffer, "UN" )
	
	u32 size = state.GetValue < u32 >( 2, 0 );
	self->Reserve ( size );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	reserveVerts
	@text	Sets capacity of buffer in vertices. This function should
			only be used after attaching a valid MOAIVertexFormat
			to the buffer.
	
	@in		MOAIVertexBuffer self
	@in		number size
	@out	nil
*/
int	MOAIVertexBuffer::_reserveVerts ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexBuffer, "UN" )
	
	u32 total = state.GetValue < u32 >( 2, 0 );
	
	if ( self->mFormat ) {
		self->Reserve ( total * self->mFormat->GetVertexSize ());
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	reset
	@text	Resets the vertex stream writing to the head of the stream.
	
	@in		MOAIVertexBuffer self
	@out	nil
*/
int MOAIVertexBuffer::_reset ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexBuffer, "U" )
	
	self->mStream.SetBuffer ( self->mBuffer, self->mBuffer.Size ());
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setFormat
	@text	Sets the vertex format for the buffer.
	
	@in		MOAIVertexBuffer self
	@in		MOAIVertexFormat format
	@out	nil
*/
int MOAIVertexBuffer::_setFormat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexBuffer, "U" )
	
	self->mFormat.Set ( *self, state.GetLuaObject < MOAIVertexFormat >( 2, true ));

	return 0;
}

//----------------------------------------------------------------//
/**	@name	writeColor32
	@text	Write a packed 32-bit color to the vertex buffer.
	
	@in		MOAIVertexBuffer self
	@opt	number r				Default value is 1.
	@opt	number g				Default value is 1.
	@opt	number b				Default value is 1.
	@opt	number a				Default value is 1.
	@out	nil
*/
int MOAIVertexBuffer::_writeColor32 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexBuffer, "U" )
	
	float r = state.GetValue < float >( 2, 1.0f );
	float g = state.GetValue < float >( 3, 1.0f );
	float b = state.GetValue < float >( 4, 1.0f );
	float a = state.GetValue < float >( 5, 1.0f );
	
	u32 color = USColor::PackRGBA ( r, g, b, a );
	self->mStream.Write < u32 >( color );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	writeFloat
	@text	Write a 32-bit float to the vertex buffer.
	
	@in		MOAIVertexBuffer self
	@opt	number f				Default value is 0.
	@out	nil
*/
int MOAIVertexBuffer::_writeFloat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexBuffer, "U" )
	
	u32 top = state.GetTop ();
	for ( u32 i = 2; i <= top; ++i ) {
		float param = state.GetValue < float >( i, 0.0f );
		self->mStream.Write < float >( param );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	writeInt8
	@text	Write an 8-bit integer to the vertex buffer.
	
	@in		MOAIVertexBuffer self
	@opt	number i				Default value is 0.
	@out	nil
*/
int MOAIVertexBuffer::_writeInt8 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexBuffer, "U" )
	
	u32 top = state.GetTop ();
	for ( u32 i = 2; i <= top; ++i ) {
		int param = state.GetValue < int >( i, 0 );
		self->mStream.Write < s8 >(( s8 )param );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	writeInt16
	@text	Write an 16-bit integer to the vertex buffer.
	
	@in		MOAIVertexBuffer self
	@opt	number i				Default value is 0.
	@out	nil
*/
int MOAIVertexBuffer::_writeInt16 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexBuffer, "U" )
	
	u32 top = state.GetTop ();
	for ( u32 i = 2; i <= top; ++i ) {
		int param = state.GetValue < int >( i, 0 );
		self->mStream.Write < s16 >(( s16 )param );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	writeInt32
	@text	Write an 32-bit integer to the vertex buffer.
	
	@in		MOAIVertexBuffer self
	@opt	number i				Default value is 0.
	@out	nil
*/
int MOAIVertexBuffer::_writeInt32 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexBuffer, "U" )
	
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
bool MOAIVertexBuffer::Bind () {

	if ( this->mFormat && this->mBuffer ) {
		MOAIGfxDevice::Get ().SetVertexFormat ( *this->mFormat, this->mBuffer );
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::Clear () {

	this->Reserve ( 0 );
	this->mFormat.Set ( *this, 0 );
}

//----------------------------------------------------------------//
u32 MOAIVertexBuffer::GetVertexCount () {

	if ( this->mFormat ) {
		return ( u32 )( this->mStream.GetLength () / this->mFormat->GetVertexSize ());
	}
	return 0;
}

//----------------------------------------------------------------//
bool MOAIVertexBuffer::IsValid () {

	return ( this->mFormat && this->mStream.GetLength ());
}

//----------------------------------------------------------------//
MOAIVertexBuffer::MOAIVertexBuffer () {
	
	RTTI_SINGLE ( MOAIVertexBuffer )
	
	this->mBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIVertexBuffer::~MOAIVertexBuffer () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "bless",					_bless },
		{ "release",				_release },
		{ "reserve",				_reserve },
		{ "reserveVerts",			_reserveVerts },
		{ "reset",					_reset },
		{ "setFormat",				_setFormat },
		{ "writeColor32",			_writeColor32 },
		{ "writeFloat",				_writeFloat },
		{ "writeInt8",				_writeInt8 },
		{ "writeInt16",				_writeInt16 },
		{ "writeInt32",				_writeInt32 },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::Reserve ( u32 size ) {

	this->mBuffer.Init ( size );
	this->mStream.SetBuffer ( this->mBuffer, size );
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::Unbind () {

	if ( this->mFormat ) {
		MOAIGfxDevice::Get ().SetVertexFormat ();
	}
}
