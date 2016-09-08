// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxBuffer.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGfxResourceClerk.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>

//================================================================//
// MOAIGfxBufferBase
//================================================================//

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	copyFromStream
	@text	Copies buffer contents from a stream.
	
	@in		MOAIGfxBuffer self
	@in		MOAIStream stream
	@opt	number length
	@out	nil
*/
int MOAIGfxBuffer::_copyFromStream ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxBuffer, "U" )
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	if ( stream ) {
	
		size_t size = state.GetValue < u32 >( 3, ( u32 )( stream->GetLength () - stream->GetCursor () ));
		self->CopyFromStream ( *stream, size );
	}
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
/**	@lua	reserveVBOs
	@text	Reserves one or more VBO objects. Multi-buffering is
			supported via multiple VBOs.
	
	@in		MOAIGfxBuffer self
	@in		number count
	@out	nil
*/
int	MOAIGfxBuffer::_reserveVBOs ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxBuffer, "UN" )

	u32 vbos = state.GetValue < u32 >( 2, 0 );
	self->ReserveVBOs ( vbos );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	scheduleFlush
	@text	Trigger an update of the GPU-side buffer. Call this when
			the backing buffer has been altered.
	
	@in		MOAIGfxBuffer self
	@out	nil
*/
int MOAIGfxBuffer::_scheduleFlush ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxBuffer, "U" )
	
	self->ScheduleForGPUUpdate ();
	return 0;
}

//================================================================//
// MOAIGfxBuffer
//================================================================//

//----------------------------------------------------------------//
//void MOAIGfxBuffer::BindVertexFormat ( MOAIVertexFormat* format ) {
//}

//----------------------------------------------------------------//
void MOAIGfxBuffer::Clear () {

	this->ZLCopyOnWrite::Free ();

	this->mVBOs.Clear ();
	this->mCurrentVBO = 0;
	
	this->Destroy ();
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::CopyFromStream ( ZLStream& stream, size_t size ) {

	this->Reserve (( u32 )size );
	this->WriteStream ( stream );
	
	this->ScheduleForGPUUpdate ();
}

//----------------------------------------------------------------//
ZLSharedConstBuffer* MOAIGfxBuffer::GetBuffer () {

	return this->mUseVBOs ? 0 : this->ZLCopyOnWrite::GetSharedConstBuffer ();
}

//----------------------------------------------------------------//
MOAIGfxBuffer::MOAIGfxBuffer () :
	mCurrentVBO ( 0 ),
	mTarget ( ZGL_BUFFER_TARGET_ARRAY ),
	mLoader ( 0 ),
	mUseVBOs ( false ),
	mCopyOnUpdate ( false ) {
	
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
	
	if ( !this->mUseVBOs ) return;
	
	ZLGfxHandle* vbo = this->mVBOs [ this->mCurrentVBO ];
	
	if ( vbo ) {
		MOAIGfxMgr::GetDrawingAPI ().BindBuffer ( this->mTarget, vbo );
	}
}

//----------------------------------------------------------------//
bool MOAIGfxBuffer::OnGPUCreate () {

	u32 count = 0;
	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

	this->mUseVBOs = ( this->mVBOs.Size () > 0 );
	
	if ( this->mUseVBOs ) {

		u32 hint = this->mVBOs.Size () > 1 ? ZGL_BUFFER_USAGE_STREAM_DRAW : ZGL_BUFFER_USAGE_STATIC_DRAW;

		for ( u32 i = 0; i < this->mVBOs.Size (); ++i ) {
			
			ZLGfxHandle* vbo = gfx.CreateBuffer ();
			if ( vbo ) {
			
				ZLSharedConstBuffer* buffer = this->GetCursor () ? this->GetSharedConstBuffer () : 0;
				
				if ( this->mCopyOnUpdate ) {
					buffer = gfx.CopyBuffer ( buffer );
				}
			
				gfx.BindBuffer ( this->mTarget, vbo );
				gfx.BufferData ( this->mTarget, this->GetLength (), buffer, 0, hint );
				gfx.BindBuffer ( this->mTarget, 0 );
				
				count++;
			}
			this->mVBOs [ i ] = vbo;
		}
	}
	
	return count == this->mVBOs.Size ();
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::OnGPUDeleteOrDiscard ( bool shouldDelete ) {

	for ( u32 i = 0; i < this->mVBOs.Size (); ++i ) {
		MOAIGfxResourceClerk::DeleteOrDiscardHandle ( this->mVBOs [ i ], shouldDelete );
	}
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::OnGPUUnbind () {

	MOAIGfxMgr::GetDrawingAPI ().BindBuffer ( this->mTarget, 0 ); // OK?
}

//----------------------------------------------------------------//
bool MOAIGfxBuffer::OnGPUUpdate () {

	if ( !this->mUseVBOs ) return true;
	
	bool dirty = this->GetCursor () > 0;
	
	if ( dirty ) {
		this->mCurrentVBO = ( this->mCurrentVBO + 1 ) % this->mVBOs.Size ();
	}
	
	ZLGfxHandle* vbo = this->mVBOs [ this->mCurrentVBO ];
	
	if ( dirty && vbo ) {
		
		// TODO: There are a few different ways to approach updating buffers with varying performance
		// on different platforms. The approach here is just to multi-buffer the VBO and replace its
		// contents via zglBufferSubData when they change. The TODO here is to do performance tests
		// on multiple devices, evaluate other approaches and possible expose the configuration of
		// those to the end user via Lua.
	
		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
		
		ZLSharedConstBuffer* buffer = this->GetSharedConstBuffer ();
		
		if ( this->mCopyOnUpdate ) {
			buffer = gfx.CopyBuffer ( buffer );
		}
		
		gfx.BindBuffer ( this->mTarget, vbo );
		gfx.BufferSubData ( this->mTarget, 0, this->GetCursor (), buffer, 0 );
		gfx.BindBuffer ( this->mTarget, 0 );
	
		//u32 hint = this->mVBOs.Size () > 1 ? ZGL_BUFFER_USAGE_DYNAMIC_DRAW : ZGL_BUFFER_USAGE_STATIC_DRAW;
		//zglBufferData ( this->mTarget, this->GetLength (), 0, hint );
	}
	
	return true;
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
		{ "release",				_release },
		{ "reserve",				_reserve },
		{ "reserveVBOs",			_reserveVBOs },
		{ "scheduleFlush",			_scheduleFlush },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::Reserve ( u32 size ) {
	
	this->ZLCopyOnWrite::Free ();
	
	if ( size ) {
		this->ZLCopyOnWrite::Reserve ( size );
		this->FinishInit ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::ReserveVBOs ( u32 gpuBuffers ) {

	if ( gpuBuffers < this->mVBOs.Size ()) {
		this->mVBOs.Clear ();
	}

	if ( gpuBuffers ) {
		this->mVBOs.Resize ( gpuBuffers, 0 );
		this->mCurrentVBO = gpuBuffers - 1;
	}

	this->FinishInit ();
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( serializer );

	u32 totalVBOs		= state.GetField < u32 >( -1, "mTotalVBOs", 0 );
	u32 size			= state.GetField < u32 >( -1, "mSize", 0 );

	this->Reserve ( size );
	this->ReserveVBOs ( totalVBOs );
	
	state.GetField ( -1, "mData" );
	if ( state.IsType ( -1, LUA_TSTRING )) {
		
		STLString zipString = lua_tostring ( state, -1 );
		size_t unzipLen = zipString.zip_inflate ( this->ZLCopyOnWrite::Invalidate (), size );
		assert ( unzipLen == size ); // TODO: fail gracefully
		UNUSED ( unzipLen ); // TODO: this *should* be handled by the zl assert redefine
		
		this->Seek ( size, SEEK_SET );
	}
	lua_pop ( state, 1 );
	
	this->ScheduleForGPUUpdate ();
	this->FinishInit ();
}

//----------------------------------------------------------------//
void MOAIGfxBuffer::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( serializer );

	size_t size = this->GetLength ();

	state.SetField < u32 >( -1, "mTotalVBOs", ( u32 )this->mVBOs.Size ());
	state.SetField < u32 >( -1, "mSize", ( u32 )size );
	
	STLString zipString;
	zipString.zip_deflate ( this->ZLCopyOnWrite::GetBuffer (), size );
	
	lua_pushstring ( state, zipString.str ());
	lua_setfield ( state, -2, "mData" );
}
