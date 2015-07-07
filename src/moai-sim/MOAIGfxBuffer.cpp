// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxBuffer.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGfxResourceMgr.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>

//================================================================//
// MOAIGfxBufferBase
//================================================================//

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxBuffer::_copyFromStream ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxBuffer, "U" )
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	if ( stream ) {
		self->CopyFromStream ( *stream );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxBuffer::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxBuffer, "U" )
	return 0;
}

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
// TODO: doxygen
int MOAIGfxBuffer::_scheduleFlush ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxBuffer, "U" )
	
	self->mNeedsFlush = true;
	return 0;
}

//================================================================//
// MOAIGfxBuffer
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxBuffer::BindVertexFormat ( MOAIVertexFormat* format ) {
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::Clear () {

	this->ZLRevBufferStream::Clear ();

	this->mVBOs.Clear ();
	this->mCurrentVBO = 0;
	
	this->Destroy ();
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::CopyFromStream ( ZLStream& stream ) {

	size_t size = stream.GetLength () - stream.GetCursor ();
	this->Reserve (( u32 )size );
	this->WriteStream ( stream );
	
	this->mNeedsFlush = true;
}

//----------------------------------------------------------------//
ZLRevBufferEdition* MOAIGfxBuffer::GetBuffer () {

	return this->mUseVBOs ? 0 : this->GetEdition ();
}

//----------------------------------------------------------------//
size_t MOAIGfxBuffer::GetSize () {

	return this->GetLength ();
}

//----------------------------------------------------------------//
u32 MOAIGfxBuffer::GetLoadingPolicy () {

	return MOAIGfxResource::LOADING_POLICY_CPU_GPU_BIND;
}

//----------------------------------------------------------------//
MOAIGfxBuffer::MOAIGfxBuffer () :
	mCurrentVBO ( 0 ),
	mTarget ( ZGL_BUFFER_TARGET_ARRAY ),
	mNeedsFlush ( false ),
	mLoader ( 0 ),
	mUseVBOs ( false ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGfxResource )
		RTTI_EXTEND ( MOAIStream )
	RTTI_END
}

//----------------------------------------------------------------//
bool MOAIGfxBuffer::NeedsFlush () {

	return this->mNeedsFlush;
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
	
	if ( !this->mUseVBOs ) return;
	
	bool dirty = this->mNeedsFlush && this->GetCursor ();
	
	if ( dirty ) {
		this->mCurrentVBO = ( this->mCurrentVBO + 1 ) % this->mVBOs.Size ();
	}
	
	ZLGfxHandle* vbo = this->mVBOs [ this->mCurrentVBO ];
	
	if ( vbo ) {
		
		ZLGfx& gfx = MOAIGfxDevice::GetAPI ();
		
		gfx.BindBuffer ( this->mTarget, vbo );
		
		if ( dirty ) {
			
			// TODO: There are a few different ways to approach updating buffers with varying performance
			// on different platforms. The approach here is just to multi-buffer the VBO and replace its
			// contents via zglBufferSubData when they change. The TODO here is to do performance tests
			// on multiple devices, evaluate other approaches and possible expose the configuration of
			// those to the end user via Lua.
		
			//u32 hint = this->mVBOs.Size () > 1 ? ZGL_BUFFER_USAGE_DYNAMIC_DRAW : ZGL_BUFFER_USAGE_STATIC_DRAW;
			//zglBufferData ( this->mTarget, this->GetLength (), 0, hint );
			
			gfx.BufferSubData ( this->mTarget, 0, this->GetCursor (), this->GetEdition (), 0 );
			
			this->mNeedsFlush = false;
		}
	}
}

//----------------------------------------------------------------//
bool MOAIGfxBuffer::OnGPUCreate () {

	this->mUseVBOs = ( this->mVBOs.Size () > 0 );
	if ( !this->mUseVBOs ) return true;

	u32 count = 0;
	u32 hint = this->mVBOs.Size () > 1 ? ZGL_BUFFER_USAGE_STREAM_DRAW : ZGL_BUFFER_USAGE_STATIC_DRAW;

	ZLGfx& gfx = MOAIGfxDevice::GetAPI ();

	for ( u32 i = 0; i < this->mVBOs.Size (); ++i ) {
		
		ZLGfxHandle* vbo = gfx.CreateBuffer ();
		if ( vbo ) {
		
			gfx.BindBuffer ( this->mTarget, vbo );
			gfx.BufferData ( this->mTarget, this->GetLength (), this->mNeedsFlush ? this->GetBuffer () : 0, 0, hint );
			gfx.BindBuffer ( this->mTarget, 0 );
			
			count++;
		}
		this->mVBOs [ i ] = vbo;
	}
	this->mNeedsFlush = false;
	return count == this->mVBOs.Size ();
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::OnGPUDestroy () {

	for ( u32 i = 0; i < this->mVBOs.Size (); ++i ) {
		MOAIGfxResourceMgr::Get ().PushDeleter ( this->mVBOs [ i ]);
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

	MOAIGfxDevice::GetAPI ().BindBuffer ( this->mTarget, 0 ); // OK?
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIGfxResource::RegisterLuaClass ( state );
	MOAIStream::RegisterLuaClass ( state );
	
	state.SetField ( -1, "INDEX_BUFFER",			( u32 )ZGL_BUFFER_TARGET_ELEMENT_ARRAY );
	state.SetField ( -1, "VERTEX_BUFFER",			( u32 )ZGL_BUFFER_TARGET_ARRAY );
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIGfxResource::RegisterLuaFuncs ( state );
	MOAIStream::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "copyFromStream",			_copyFromStream },
		{ "load",					_load },
		{ "release",				_release },
		{ "reserve",				_reserve },
		{ "reserveVBOs",			_reserveVBOs },
		{ "reset",					_reset },
		{ "scheduleFlush",			_scheduleFlush },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::Reserve ( u32 size ) {
	
	this->Clear ();
	
	if ( size ) {
		this->ZLRevBufferStream::Reserve ( size );
		this->FinishInit ();
	}
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
void MOAIGfxBuffer::ScheduleFlush () {

	this->mNeedsFlush = true;
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {

//	u32 totalVBOs		= state.GetField < u32 >( -1, "mTotalVBOs", 0 );
//	u32 size			= state.GetField < u32 >( -1, "mSize", 0 );
//
//	this->Reserve ( size );
//	this->ReserveVBOs ( totalVBOs );
//	
//	state.GetField ( -1, "mData" );
//	if ( state.IsType ( -1, LUA_TSTRING )) {
//		
//		STLString zipString = lua_tostring ( state, -1 );
//		size_t unzipLen = zipString.zip_inflate ( this->mData, size );
//		assert ( unzipLen == size ); // TODO: fail gracefully
//		
//		this->Seek ( size, SEEK_SET );
//	}
//	lua_pop ( state, 1 );
//	
//	this->ScheduleFlush ();
//	this->FinishInit ();
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

//	u32 size = ( u32 )this->GetLength ();
//
//	state.SetField < u32 >( -1, "mTotalVBOs", this->mVBOs.Size ());
//	state.SetField < u32 >( -1, "mSize", size );
//	
//	STLString zipString;
//	zipString.zip_deflate ( this->mData, size );
//	
//	lua_pushstring ( state, zipString.str ());
//	lua_setfield ( state, -2, "mData" );
}

