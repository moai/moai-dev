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
int MOAIGfxBuffer::_computeBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxBuffer, "U" )
	
	bool hasBounds = false;
	ZLBox bounds;
	bounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );

	// offset, stride, components
	if ( state.CheckParams ( 2, "NNN", false )) {
	
		u32 offset			= state.GetValue < u32 >( 2, 0 );
		u32 stride			= state.GetValue < u32 >( 3, 0 );
		u32 components		= state.GetValue < u32 >( 4, 0 );
		u32 length			= state.GetValue < u32 >( 5, ( u32 )self->GetLength ());
		
		hasBounds = MOAIVertexFormat::ComputeBounds ( bounds, *self, length, offset, stride, components );
	}
	
	// vertex format
	else if ( state.CheckParams ( 2, "U", false )) {
	
		MOAIVertexFormat* format = state.GetLuaObject < MOAIVertexFormat >( 2, 0 );
		if ( format ) {
			u32 length = state.GetValue < u32 >( 3, ( u32 )self->GetCursor ());
			self->Seek ( 0, SEEK_SET );
			hasBounds = format->ComputeBounds ( bounds, *self, length );
		}
	}
	
	if ( hasBounds ) {
		state.Push ( bounds );
		return 6;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxBuffer::_countElements ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxBuffer, "U" )

	u32 totalElements = 0;

	// offset, stride, components
	if ( state.CheckParams ( 2, "NNN", false )) {
	
		u32 offset			= state.GetValue < u32 >( 2, 0 );
		u32 stride			= state.GetValue < u32 >( 3, 0 );
		u32 length			= state.GetValue < u32 >( 4, ( u32 )self->GetLength ());
		
		totalElements =  ( u32 )MOAIVertexFormat::CountElements ( length, offset, stride );
	}
	
	// vertex format
	else if ( state.CheckParams ( 2, "U", false )) {
	
		MOAIVertexFormat* format = state.GetLuaObject < MOAIVertexFormat >( 2, 0 );
		if ( format ) {
			u32 length = state.GetValue < u32 >( 3, ( u32 )self->GetLength ());
			totalElements = ( u32 )format->CountElements ( length );
		}
	}
	
	state.Push ( totalElements );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxBuffer::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxBuffer, "U" )
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxBuffer::_makeDirty ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxBuffer, "U" )
	
	self->mIsDirty = true;
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
int MOAIGfxBuffer::_setTarget ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxBuffer, "U" )

	self->SetTarget ( state.GetValue < u32 >( 2, ZGL_BUFFER_TARGET_ARRAY ));
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
size_t MOAIGfxBuffer::GetSize () {

	return this->GetLength ();
}

//----------------------------------------------------------------//
u32 MOAIGfxBuffer::GetLoadingPolicy () {

	return MOAIGfxResource::LOADING_POLICY_CPU_GPU_BIND;
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::MakeDirty () {

	this->mIsDirty = true;
}

//----------------------------------------------------------------//
MOAIGfxBuffer::MOAIGfxBuffer () :
	mCurrentVBO ( 0 ),
	mTarget ( ZGL_BUFFER_TARGET_ARRAY ),
	mIsDirty ( true ),
	mLoader ( 0 ),
	mData ( 0 ) {
	
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
	
	bool renew = this->mIsDirty && this->GetLength ();
	
	if ( renew ) {
		this->mCurrentVBO = ( this->mCurrentVBO + 1 ) % this->mVBOs.Size ();
	}
	
	u32 vbo = this->mVBOs [ this->mCurrentVBO ];
	
	if ( vbo ) {
	
		zglBindBuffer ( this->mTarget, vbo );
		
		if ( renew ) {
		
			void* buffer = zglMapBuffer ( this->mTarget );
			if ( buffer ) {
				this->Seek ( 0, SEEK_SET );
				this->ReadBytes ( buffer, this->GetLength ());
			}
			zglUnmapBuffer ( this->mTarget );
			this->mIsDirty = false;
		}
	}
}

//----------------------------------------------------------------//
bool MOAIGfxBuffer::OnGPUCreate () {

	if ( !this->mVBOs.Size ()) {
		this->mVBOs.Init ( 1 );
	}

	u32 count = 0;
	u32 hint = this->mVBOs.Size () > 1 ? ZGL_BUFFER_USAGE_DYNAMIC_DRAW : ZGL_BUFFER_USAGE_STATIC_DRAW;

	for ( u32 i = 0; i < this->mVBOs.Size (); ++i ) {
		
		u32 vbo = zglCreateBuffer ();
		if ( vbo ) {
		
			zglBindBuffer ( this->mTarget, vbo );
			zglBufferData ( this->mTarget, this->GetLength (), 0, hint );
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
	
	state.SetField ( -1, "INDEX_BUFFER",			( u32 )ZGL_BUFFER_TARGET_ELEMENT_ARRAY );
	state.SetField ( -1, "VERTEX_BUFFER",			( u32 )ZGL_BUFFER_TARGET_ARRAY );
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIGfxResource::RegisterLuaFuncs ( state );
	MOAIStream::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "computeBounds",			_computeBounds },
		{ "countElements",			_countElements },
		{ "load",					_load },
		{ "makeDirty",				_makeDirty },
		{ "release",				_release },
		{ "reserve",				_reserve },
		{ "reserveVBOs",			_reserveVBOs },
		{ "reset",					_reset },
		{ "setTarget",				_setTarget },
		{ "writeColor32",			_writeColor32 },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::Reserve ( u32 size ) {
	
	if ( this->mData ) {
		free ( this->mData );
		this->mData = 0;
	}
	
	if ( size ) {
		this->mData = calloc ( size, 1 );
		this->SetBuffer ( this->mData, size, size );
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
void MOAIGfxBuffer::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {

	u32 totalVBOs		= state.GetField < u32 >( -1, "mTotalVBOs", 0 );
	u32 target			= state.GetField < u32 >( -1, "mTarget", ZGL_BUFFER_TARGET_ARRAY );
	u32 size			= state.GetField < u32 >( -1, "mSize", 0 );

	this->Reserve ( size );
	this->ReserveVBOs ( totalVBOs );
	this->mTarget = target;
	
	state.GetField ( -1, "mData" );
	if ( state.IsType ( -1, LUA_TSTRING )) {
		
		STLString zipString = lua_tostring ( state, -1 );
		size_t unzipLen = zipString.zip_inflate ( this->mData, size );
		assert ( unzipLen == size ); // TODO: fail gracefully
		
		this->Seek ( size, SEEK_SET );
	}
	lua_pop ( state, 1 );
	
	this->MakeDirty ();
	this->FinishInit ();
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

	u32 size = ( u32 )this->GetLength ();

	state.SetField < u32 >( -1, "mTotalVBOs", this->mVBOs.Size ());
	state.SetField < u32 >( -1, "mTarget", this->mTarget );
	state.SetField < u32 >( -1, "mSize", size );
	
	STLString zipString;
	zipString.zip_deflate ( this->mData, size );
	
	lua_pushstring ( state, zipString.str ());
	lua_setfield ( state, -2, "mData" );
}
