// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractGfxBuffer.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIVertexFormat.h>
#include <moai-gfx/MOAIVertexFormatMgr.h>

//================================================================//
// MOAIGfxBufferBase
//================================================================//

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	copyFromStream
	@text	Copies buffer contents from a stream.
	
	@in		MOAIAbstractGfxBuffer self
	@in		MOAIStream stream
	@opt	number length
	@out	nil
*/
int MOAIAbstractGfxBuffer::_copyFromStream ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxBuffer, "U" )
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	if ( stream ) {
	
		size_t size = state.GetValue < u32 >( 3, ( u32 )( stream->GetLength () - stream->GetCursor () ));
		self->MOAIGfxBuffer_ZLGfxBuffer ().CopyFromStream ( *stream, size );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	release
	@text	Releases any memory associated with buffer.
	
	@in		MOAIAbstractGfxBuffer self
	@out	nil
*/
int	MOAIAbstractGfxBuffer::_release ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxBuffer, "U" )
	
	self->MOAIGfxBuffer_ZLGfxBuffer ().Clear ();
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reserve
	@text	Sets capacity of buffer in bytes.
	
	@in		MOAIAbstractGfxBuffer self
	@in		number size
	@out	nil
*/
int	MOAIAbstractGfxBuffer::_reserve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxBuffer, "UN" )
	
	u32 size = state.GetValue < u32 >( 2, 0 );
	self->MOAIGfxBuffer_ZLGfxBuffer ().Reserve ( size );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reserveVBOs
	@text	Reserves one or more VBO objects. Multi-buffering is
			supported via multiple VBOs.
	
	@in		MOAIAbstractGfxBuffer self
	@in		number count
	@out	nil
*/
int	MOAIAbstractGfxBuffer::_reserveVBOs ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxBuffer, "UN" )

	u32 vbos = state.GetValue < u32 >( 2, 0 );
	self->MOAIGfxBuffer_ZLGfxBuffer ().ReserveVBOs ( vbos );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	scheduleFlush
	@text	Trigger an update of the GPU-side buffer. Call this when
			the backing buffer has been altered.
	
	@in		MOAIAbstractGfxBuffer self
	@out	nil
*/
int MOAIAbstractGfxBuffer::_scheduleFlush ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxBuffer, "U" )
	
	self->MOAIGfxBuffer_ZLGfxBuffer ().ScheduleForGPUUpdate ();
	return 0;
}

//================================================================//
// MOAIAbstractGfxBuffer
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractGfxBuffer::MOAIAbstractGfxBuffer () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIStream )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractGfxBuffer::~MOAIAbstractGfxBuffer () {
}

//----------------------------------------------------------------//
void MOAIAbstractGfxBuffer::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIStream::RegisterLuaClass ( state );
	
	state.SetField ( -1, "INDEX_BUFFER",			( u32 )ZGL_BUFFER_TARGET_ELEMENT_ARRAY );
	state.SetField ( -1, "VERTEX_BUFFER",			( u32 )ZGL_BUFFER_TARGET_ARRAY );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxBuffer::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIStream::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "copyFromStream",			_copyFromStream },
		{ "release",				_release },
		{ "reserve",				_reserve },
		{ "reserveVBOs",			_reserveVBOs },
		{ "scheduleFlush",			_scheduleFlush },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxBuffer::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( serializer );

	ZLGfxBuffer& buffer = this->MOAIGfxBuffer_ZLGfxBuffer ();

	u32 totalVBOs		= state.GetFieldValue < cc8*, u32 >( -1, "mTotalVBOs", 0 );
	u32 size			= state.GetFieldValue < cc8*, u32 >( -1, "mSize", 0 );

	buffer.Reserve ( size );
	buffer.ReserveVBOs ( totalVBOs );
	
	state.PushField ( -1, "mData" );
	if ( state.IsType ( -1, LUA_TSTRING )) {
		
		STLString zipString = lua_tostring ( state, -1 );
		size_t unzipLen = zipString.zip_inflate ( buffer.Invalidate (), size );
		assert ( unzipLen == size ); // TODO: fail gracefully
		UNUSED ( unzipLen ); // TODO: this *should* be handled by the zl assert redefine
		
		this->Seek ( size, SEEK_SET );
	}
	lua_pop ( state, 1 );
	
	buffer.ScheduleForGPUUpdate ();
	buffer.FinishInit ();
}

//----------------------------------------------------------------//
void MOAIAbstractGfxBuffer::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( serializer );

	ZLGfxBuffer& buffer = this->MOAIGfxBuffer_ZLGfxBuffer ();

	ZLSize size = this->GetLength ();

	state.SetField < cc8*, MOAILuaSize >( -1, "mTotalVBOs", buffer.CountVBOs ());
	state.SetField < cc8*, MOAILuaSize >( -1, "mSize", size );
	
	STLString zipString;
	zipString.zip_deflate ( buffer.GetBuffer (), size );
	
	lua_pushstring ( state, zipString.str ());
	lua_setfield ( state, -2, "mData" );
}
