// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIVertexBuffer.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>

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
	
	self->Bless ();
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
	self->Reserve ( size, 0 );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexBuffer::_reserveVBOs ( lua_State* L ) {

	return;
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
		self->Reserve ( total * self->mFormat->GetVertexSize (), 0 );
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
	
	u32 color = ZLColor::PackRGBA ( r, g, b, a );
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
void MOAIVertexBuffer::Bless () {

	this->mBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	this->mVertexCount = 0;
	
	const MOAIVertexFormat* format = this->GetFormat ();
	if ( format ) {
		format->ComputeBounds ( this->mBuffer, this->mStream.GetLength (), this->mBounds );
		this->mVertexCount = ( u32 )( this->mStream.GetLength () / format->GetVertexSize ());
	}
	
	this->mIsDirty = this->mUseVBOs;
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::Clear () {

	this->Reserve ( 0, 0 );
	this->mFormat.Set ( *this, 0 );
}

//----------------------------------------------------------------//
const MOAIVertexFormat* MOAIVertexBuffer::GetFormat () {

	if ( this->mFormat ) return this->mFormat;
	if ( this->mDefaultFormat != NULL_FORMAT ) return &MOAIVertexFormatMgr::Get ().GetPreset ( this->mDefaultFormat );
	return 0;
}

//----------------------------------------------------------------//
bool MOAIVertexBuffer::IsRenewable () {

	return true;
}

//----------------------------------------------------------------//
bool MOAIVertexBuffer::IsValid () {

	return this->mIsValid;
}

//----------------------------------------------------------------//
MOAIVertexBuffer::MOAIVertexBuffer () :
	mDefaultFormat ( NULL_FORMAT ),
	mVertexCount ( 0 ),
	mCurrentVBO ( 0 ),
	mHint ( ZGL_BUFFER_USAGE_STATIC_DRAW ),
	mIsDirty ( false ),
	mIsValid ( false ),
	mUseVBOs ( false ) {
	
	RTTI_SINGLE ( MOAILuaObject )
	
	this->mBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIVertexBuffer::~MOAIVertexBuffer () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::OnBind () {

	if ( this->mUseVBOs ) {
		
		MOAIGfxDevice::Get ().SetVertexFormat ();
		
		if ( this->mIsDirty ) {
			this->mCurrentVBO = ( this->mCurrentVBO + 1 ) % this->mVBOs.Size ();
		}
		
		MOAIVbo& vbo = this->mVBOs [ this->mCurrentVBO ];
		
		if ( this->mIsDirty ) {
			
			zglBindBuffer ( ZGL_BUFFER_TARGET_ARRAY, vbo.mVBO );
			void* buffer = zglMapBuffer ( ZGL_BUFFER_TARGET_ARRAY );
			memcpy ( buffer, this->mBuffer, this->mStream.GetLength ());
			zglUnmapBuffer ( ZGL_BUFFER_TARGET_ARRAY );
			zglBindBuffer ( ZGL_BUFFER_TARGET_ARRAY, 0 );
			
			this->mIsDirty = false;
		}
		
		zglBindVertexArray ( vbo.mVAO );
	}
	else {
	
		const MOAIVertexFormat* format = this->GetFormat ();
		if ( format && this->mBuffer ) {
			MOAIGfxDevice::Get ().SetVertexFormat ( *format, this->mBuffer );
			return true;
		}
	}
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::OnClear () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::OnCreate () {

	if ( this->mUseVBOs ) {

		const MOAIVertexFormat* format = this->GetFormat ();
		u32 count = 0;

		for ( u32 i = 0; i < this->mVBOs.Size (); ++i ) {
			MOAIVbo& vbo = this->mVBOs [ i ];
			
			vbo.mVAO = zglCreateVertexArray ();
			if ( vbo.mVAO ) {
				zglBindVertexArray ( vbo.mVAO );
				
				vbo.mVBO = zglCreateBuffer ();
				if ( vbo.mVBO ) {
				
					zglBindBuffer ( ZGL_BUFFER_TARGET_ARRAY, vbo.mVBO );
					zglBufferData ( ZGL_BUFFER_TARGET_ARRAY, this->mStream.GetLength (), 0, this->mHint );
					format->Bind ( 0 );
					count++;
				}
				
				zglBindVertexArray ( 0 );
				zglBindBuffer ( ZGL_BUFFER_TARGET_ARRAY, 0 );
				format->Unbind ();
			}
		}
		
		this->mIsValid = count == this->mVBOs.Size ();
	}
	else {
		const MOAIVertexFormat* format = this->GetFormat ();
		this->mIsValid = ( format && this->mStream.GetLength ());
	}
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::OnDestroy () {

	for ( u32 i = 0; i < this->mVBOs.Size (); ++i ) {
		MOAIVbo& vbo = this->mVBOs [ i ];

		if ( vbo.mVBO ) {
			zglDeleteBuffer ( vbo.mVBO );
			vbo.mVBO = 0;
		}
	}
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::OnInvalidate () {

	for ( u32 i = 0; i < this->mVBOs.Size (); ++i ) {
		MOAIVbo& vbo = this->mVBOs [ i ];

		if ( vbo.mVBO ) {
			vbo.mVBO = 0;
		}
	}
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::OnLoad () {
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::OnUnbind () {

	if ( this->mUseVBOs ) {
		zglBindVertexArray ( 0 );
	}
	else if ( this->GetFormat ()) {
		MOAIGfxDevice::Get ().SetVertexFormat ();
	}
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
void MOAIVertexBuffer::Reserve ( u32 size, u32 gpuBuffers ) {

	this->mBuffer.Init ( size );
	this->mStream.SetBuffer ( this->mBuffer, size );
	
	if ( gpuBuffers ) {
	
		MOAIVbo blank;
	
		blank.mVAO = 0;
		blank.mVBO = 0;
		
		this->mVBOs.Resize ( gpuBuffers, blank );
		this->mCurrentVBO = gpuBuffers - 1;
		this->mUseVBOs = true;
	}
	else {
		this->mVBOs.Clear ();
		this->mUseVBOs = false;
	}
	
	this->Load ();
}
