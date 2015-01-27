// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxBuffer.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGfxResourceMgr.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	release
	@text	Releases any memory associated with buffer.
	
	@in		MOAIGfxBuffer self
	@out	nil
*/
int	MOAIGfxBuffer::_release ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxBuffer, "U" )
	
	self->Clear ();
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reserve
	@text	Sets capacity of buffer in bytes.
	
	@in		MOAIGfxBuffer self
	@in		number size
	@out	nil
*/
int	MOAIGfxBuffer::_reserve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxBuffer, "UN" )
	
	u32 size = state.GetValue < u32 >( 2, 0 );
	self->Reserve ( size );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIGfxBuffer::_reserveVBOs ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxBuffer, "UN" )

	u32 vbos = state.GetValue < u32 >( 2, 0 );
	self->ReserveVBOs ( vbos );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reset
	@text	Resets the vertex stream writing to the head of the stream.
	
	@in		MOAIGfxBuffer self
	@out	nil
*/
int MOAIGfxBuffer::_reset ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxBuffer, "U" )
	
	//self->SetBuffer ( self->mBuffer, self->mBuffer.Size ());
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	writeColor32
	@text	Write a packed 32-bit color to the vertex buffer.
	
	@in		MOAIGfxBuffer self
	@opt	number r				Default value is 1.
	@opt	number g				Default value is 1.
	@opt	number b				Default value is 1.
	@opt	number a				Default value is 1.
	@out	nil
*/
int MOAIGfxBuffer::_writeColor32 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxBuffer, "U" )
	
	float r = state.GetValue < float >( 2, 1.0f );
	float g = state.GetValue < float >( 3, 1.0f );
	float b = state.GetValue < float >( 4, 1.0f );
	float a = state.GetValue < float >( 5, 1.0f );
	
	u32 color = ZLColor::PackRGBA ( r, g, b, a );
	self->Write < u32 >( color );
	
	return 0;
}

//================================================================//
// MOAIGfxBuffer
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxBuffer::Clear () {

	this->Reserve ( 0 );
	this->ReserveVBOs ( 0 );
	
	this->Destroy ();
}

//----------------------------------------------------------------//
bool MOAIGfxBuffer::IsDirty () {

	this->mHint;
}

//----------------------------------------------------------------//
u32 MOAIGfxBuffer::GetLoadingPolicy () {

	return MOAIGfxResource::LOADING_POLICY_CPU_GPU_BIND;
}

//----------------------------------------------------------------//
MOAIGfxBuffer::MOAIGfxBuffer () :
	mCurrentVBO ( 0 ),
	mTarget ( ZGL_BUFFER_TARGET_ARRAY ),
	mHint ( ZGL_BUFFER_USAGE_STATIC_DRAW ),
	mIsDirty ( false ),
	mIsDynamic ( false ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGfxResource )
		RTTI_EXTEND ( MOAIStream )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxBuffer::~MOAIGfxBuffer () {

	this->Clear ();
}

//----------------------------------------------------------------//
bool MOAIGfxBuffer::OnCPUCreate () {
	return true;
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::OnCPUDestroy () {
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::OnGPUBind () {
		
	if ( this->mIsDirty ) {
		this->mCurrentVBO = ( this->mCurrentVBO + 1 ) % this->mVBOs.Size ();
	}
	
	MOAIVbo& vbo = this->mVBOs [ this->mCurrentVBO ];
	
	if ( this->mIsDirty ) {
		
		zglBindBuffer ( ZGL_BUFFER_TARGET_ARRAY, vbo.mVBO );
		void* buffer = zglMapBuffer ( ZGL_BUFFER_TARGET_ARRAY );
		memcpy ( buffer, this->mBuffer, this->GetLength ());
		zglUnmapBuffer ( ZGL_BUFFER_TARGET_ARRAY );
		zglBindBuffer ( ZGL_BUFFER_TARGET_ARRAY, 0 );
		
		this->mIsDirty = false;
	}
}

//----------------------------------------------------------------//
bool MOAIGfxBuffer::OnGPUCreate () {

	u32 count = 0;

	for ( u32 i = 0; i < this->mVBOs.Size (); ++i ) {
		
		u32 vbo = zglCreateBuffer ();
		if ( vbo ) {
		
			zglBindBuffer ( this->mTarget, vbo );
			zglBufferData ( this->mTarget, this->GetLength (), 0, this->mHint );
			zglBindBuffer ( this->mTarget, 0 ); // OK?
			count++;
		}
		
		this->mVBOs [ i ] = vbo;
	}
	return count == this->mVBOs.Size ();
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::OnGPUDestroy () {

	for ( u32 i = 0; i < this->mVBOs.Size (); ++i ) {
		MOAIGfxResourceMgr::Get ().PushDeleter ( MOAIGfxDeleter::DELETE_BUFFER, this->mVBOs [ i ]);
		this->mVBOs [ i ] = 0;
	}
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::OnGPULost () {

	for ( u32 i = 0; i < this->mVBOs.Size (); ++i ) {
		this->mVBOs [ i ] = 0;
	}
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::OnGPUUnbind () {

	zglBindBuffer ( this->mTarget, 0 ); // OK?
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIGfxResource::RegisterLuaClass ( state );
	MOAIStream::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIGfxResource::RegisterLuaFuncs ( state );
	MOAIStream::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "release",				_release },
		{ "reserve",				_reserve },
		{ "reserveVBOs",			_reserveVBOs },
		{ "reset",					_reset },
		{ "writeColor32",			_writeColor32 },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::Reserve ( u32 size ) {

	this->mBuffer.Init ( size );
	
	MOAIByteStream* byteStream = new MOAIByteStream ();
	byteStream->SetBuffer ( this->mBuffer, size );
	this->SetProxiedStream ( byteStream );
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::ReserveVBOs ( u32 gpuBuffers ) {

	if ( gpuBuffers ) {
		this->mVBOs.Resize ( gpuBuffers, 0 );
		this->mCurrentVBO = gpuBuffers - 1;
	}
	else {
		this->mVBOs.Clear ();
		this->mCurrentVBO = 0;
	}
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( serializer );

//	state.GetField ( -1, "mBuffer" );
//	if ( state.IsType ( -1, LUA_TSTRING )) {
//		
//		MOAIDataBuffer dataBuffer;
//		dataBuffer.Load ( state, -1 );
//		dataBuffer.Inflate ();
//		
//		size_t bufferSize = dataBuffer.Size ();
//		this->Reserve ( bufferSize );
//		dataBuffer.Read ( this->mBuffer.Data (), bufferSize );
//		
//		this->SetLength ( bufferSize );
//		this->Seek ( bufferSize, SEEK_SET );
//	}
//	lua_pop ( state, 1 );
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( serializer );

//	size_t size = this->GetLength ();
//	
//	MOAIDataBuffer dataBuffer;
//	dataBuffer.Load ( this->mBuffer.Data (), size );
//	dataBuffer.Deflate ();
//	dataBuffer.PushString ( state );
//	lua_setfield ( state, -2, "mBuffer" );
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::SetStream ( MOAIStream* stream ) {

	this->SetProxiedStream ( stream );
	this->mStream.Set ( *this, stream );
}
