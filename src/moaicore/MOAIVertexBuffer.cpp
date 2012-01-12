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
	
	self->mBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	
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
	
	self->mFormat.Set ( *self, state.GetLuaObject < MOAIVertexFormat >( 2 ));

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setPenWidth
	@text	Sets the pen with for drawing prims in this vertex buffer.
			Only valid with prim types GL_LINES, GL_LINE_LOOP, GL_LINE_STRIP.
	
	@in		MOAIVertexBuffer self
	@in		number penWidth
	@out	nil
*/
int MOAIVertexBuffer::_setPenWidth ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexBuffer, "UN" )
	
	self->mPenWidth = state.GetValue < float >( 2, 1.0f );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setPointSize
	@text	Sets the point size for drawing prims in this vertex buffer.
			Only valid with prim types GL_POINTS.
	
	@in		MOAIVertexBuffer self
	@in		number pointSize
	@out	nil
*/
int MOAIVertexBuffer::_setPointSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexBuffer, "UN" )
	
	self->mPointSize = state.GetValue < float >( 2, 1.0f );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setPrimType
	@text	Sets the prim type the buffer represents.
	
	@in		MOAIVertexBuffer self
	@in		number primType		One of MOAIVertexBuffer GL_POINTS, GL_LINES, GL_TRIANGLES, GL_LINE_LOOP,
								GL_LINE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLE_STRIP
	@out	nil
*/
int MOAIVertexBuffer::_setPrimType ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexBuffer, "UN" )
	
	u32 primType = state.GetValue < u32 >( 2, 0 );
	self->SetPrimType ( primType );

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
void MOAIVertexBuffer::Clear () {

	this->Reserve ( 0 );
	this->mFormat.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::Draw () {

	if ( this->mFormat ) {
		
		MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
		
		gfxDevice.SetPenWidth ( this->mPenWidth );
		gfxDevice.SetPointSize ( this->mPointSize );
		
		gfxDevice.DrawPrims ( *this->mFormat, this->mPrimType, this->mBuffer, this->mStream.GetLength ());
	}
}

//----------------------------------------------------------------//
bool MOAIVertexBuffer::IsValid () {

	return ( this->mFormat && this->mStream.GetLength ());
}

//----------------------------------------------------------------//
MOAIVertexBuffer::MOAIVertexBuffer () :
	mPenWidth ( 1.0f ),
	mPointSize ( 1.0f ),
	mPrimSize ( 0 ) {
	
	RTTI_SINGLE ( MOAIVertexBuffer )
	
	this->mBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIVertexBuffer::~MOAIVertexBuffer () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "GL_POINTS", ( u32 )GL_POINTS );
	state.SetField ( -1, "GL_LINES", ( u32 )GL_LINES );
	state.SetField ( -1, "GL_TRIANGLES", ( u32 )GL_TRIANGLES );
	state.SetField ( -1, "GL_LINE_LOOP", ( u32 )GL_LINE_LOOP );
	state.SetField ( -1, "GL_LINE_STRIP", ( u32 )GL_LINE_STRIP );
	state.SetField ( -1, "GL_TRIANGLE_FAN", ( u32 )GL_TRIANGLE_FAN );
	state.SetField ( -1, "GL_TRIANGLE_STRIP", ( u32 )GL_TRIANGLE_STRIP );
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
		{ "setPenWidth",			_setPenWidth },
		{ "setPointSize",			_setPointSize },
		{ "setPrimType",			_setPrimType },
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

