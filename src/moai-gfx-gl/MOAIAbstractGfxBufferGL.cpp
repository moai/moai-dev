// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIAbstractGfxBufferGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	copyFromStream
	@text	Copies buffer contents from a stream.
 
	@in		MOAIAbstractGfxBufferGL self
	@in		MOAIStream stream
	@opt	number length
	@out	nil
*/
int MOAIAbstractGfxBufferGL::_copyFromStream ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxBufferGL, "U" )
	
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
 
	@in		MOAIAbstractGfxBufferGL self
	@out	nil
*/
int	MOAIAbstractGfxBufferGL::_release ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxBufferGL, "U" )
	
	self->Clear ();
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reserve
	@text	Sets capacity of buffer in bytes.
 
	@in		MOAIAbstractGfxBufferGL self
	@in		number size
	@out	nil
*/
int	MOAIAbstractGfxBufferGL::_reserve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxBufferGL, "UN" )
	
	u32 size = state.GetValue < u32 >( 2, 0 );
	self->Reserve ( size );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reserveVBOs
	@text	Reserves one or more VBO objects. Multi-buffering is
			supported via multiple VBOs.
 
	@in		MOAIAbstractGfxBufferGL self
	@in		number count
	@out	nil
*/
int	MOAIAbstractGfxBufferGL::_reserveVBOs ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxBufferGL, "UN" )

	u32 vbos = state.GetValue < u32 >( 2, 0 );
	self->ReserveVBOs ( vbos );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	scheduleFlush
	@text	Trigger an update of the GPU-side buffer. Call this when
			the backing buffer has been altered.
 
	@in		MOAIAbstractGfxBufferGL self
	@out	nil
*/
int MOAIAbstractGfxBufferGL::_scheduleFlush ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxBufferGL, "U" )
	
	self->ScheduleForGPUUpdate ();
	return 0;
}

//================================================================//
// MOAIAbstractGfxBufferGL
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractGfxBufferGL::Clear () {

	this->ZLCopyOnWrite::Free ();

	this->mVBOs.Clear ();
	this->mCurrentVBO = ZLIndexOp::ZERO;
	
	this->Destroy ();
}

//----------------------------------------------------------------//
void MOAIAbstractGfxBufferGL::CopyFromStream ( ZLStream& stream, size_t size ) {

	this->Reserve (( u32 )size );
	this->WriteStream ( stream );
	
	this->ScheduleForGPUUpdate ();
}

//----------------------------------------------------------------//
size_t MOAIAbstractGfxBufferGL::CountVBOs () {

	return this->mVBOs.Size ();
}

//----------------------------------------------------------------//
ZLSharedConstBuffer* MOAIAbstractGfxBufferGL::GetBufferForBind ( ZLGfx& gfx ) {
	UNUSED(gfx);
	return this->mUseVBOs ? 0 : this->ZLCopyOnWrite::GetSharedConstBuffer ();

//	if ( this->mUseVBOs ) return 0;
//
//	ZLSharedConstBuffer* buffer = this->ZLCopyOnWrite::GetSharedConstBuffer ();
//	return this->mCopyOnBind ? gfx.CopyBuffer ( buffer ) : buffer;
}

//----------------------------------------------------------------//
MOAIAbstractGfxBufferGL::MOAIAbstractGfxBufferGL () :
	mCurrentVBO ( ZLIndexOp::ZERO ),
	mTarget ( ZGL_BUFFER_TARGET_ARRAY ),
	mUseVBOs ( false ),
	mCopyOnUpdate ( false ) {
		
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIAbstractGfxResourceGL )
		RTTI_EXTEND ( MOAIStream )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractGfxBufferGL::~MOAIAbstractGfxBufferGL () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIAbstractGfxBufferGL::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIAbstractGfxResourceGL::RegisterLuaClass ( state );
	MOAIStream::RegisterLuaClass ( state );
	
	state.SetField ( -1, "INDEX_BUFFER",			( u32 )ZGL_BUFFER_TARGET_ELEMENT_ARRAY );
	state.SetField ( -1, "VERTEX_BUFFER",			( u32 )ZGL_BUFFER_TARGET_ARRAY );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxBufferGL::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIAbstractGfxResourceGL::RegisterLuaFuncs ( state );
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
void MOAIAbstractGfxBufferGL::Reserve ( ZLSize size ) {
	
	this->ZLCopyOnWrite::Free ();
	
	if ( size ) {
		this->ZLCopyOnWrite::Reserve ( size );
		this->FinishInit ();
	}
}

//----------------------------------------------------------------//
void MOAIAbstractGfxBufferGL::ReserveVBOs ( ZLSize gpuBuffers ) {

	if ( gpuBuffers < this->mVBOs.Size ()) {
		this->mVBOs.Clear ();
	}

	if ( gpuBuffers ) {
		this->mVBOs.Resize ( gpuBuffers );
		this->mCurrentVBO = ZLIndexCast ( gpuBuffers - 1 );
	}

	this->FinishInit ();
}

//----------------------------------------------------------------//
void MOAIAbstractGfxBufferGL::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( serializer );

	u32 totalVBOs		= state.GetFieldValue < cc8*, u32 >( -1, "mTotalVBOs", 0 );
	u32 size			= state.GetFieldValue < cc8*, u32 >( -1, "mSize", 0 );

	this->Reserve ( size );
	this->ReserveVBOs ( totalVBOs );
	
	state.PushField ( -1, "mData" );
	if ( state.IsType ( -1, LUA_TSTRING )) {
		
		STLString zipString = lua_tostring ( state, -1 );
		size_t unzipLen = zipString.zip_inflate ( this->Invalidate (), size );
		assert ( unzipLen == size ); // TODO: fail gracefully
		UNUSED ( unzipLen ); // TODO: this *should* be handled by the zl assert redefine
		
		this->Seek ( size, SEEK_SET );
	}
	lua_pop ( state, 1 );
	
	this->ScheduleForGPUUpdate ();
	this->FinishInit ();
}

//----------------------------------------------------------------//
void MOAIAbstractGfxBufferGL::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( serializer );

	ZLSize size = this->GetLength ();

	state.SetField < cc8*, MOAILuaSize >( -1, "mTotalVBOs", this->CountVBOs ());
	state.SetField < cc8*, MOAILuaSize >( -1, "mSize", size );
	
	STLString zipString;
	zipString.zip_deflate ( this->GetBuffer (), size );
	
	lua_pushstring ( state, zipString.str ());
	lua_setfield ( state, -2, "mData" );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool MOAIAbstractGfxBufferGL::ZLAbstractGfxResource_OnCPUCreate () {
	return true;
}

//----------------------------------------------------------------//
void MOAIAbstractGfxBufferGL::ZLAbstractGfxResource_OnCPUDestroy () {
}

//----------------------------------------------------------------//
void MOAIAbstractGfxBufferGL::ZLAbstractGfxResource_OnGPUBind () {
	
	if ( !this->mUseVBOs ) return;
	
	const ZLGfxHandle& vbo = this->mVBOs [ this->mCurrentVBO ];
	
	if ( vbo.CanBind ()) {
		this->mGfxMgr->GetDrawingAPI ().BindBuffer ( this->mTarget, vbo );
	}
}

//----------------------------------------------------------------//
bool MOAIAbstractGfxBufferGL::ZLAbstractGfxResource_OnGPUCreate () {

	u32 count = 0;

	this->mUseVBOs = ( this->mVBOs.Size () > 0 );
	
	if ( this->mUseVBOs ) {

		ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
		u32 hint = this->mVBOs.Size () > 1 ? ZGL_BUFFER_USAGE_STREAM_DRAW : ZGL_BUFFER_USAGE_STATIC_DRAW;

		for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mVBOs.Size (); ++i ) {
			
			ZLGfxHandle vbo = gfx.CreateBuffer ();
			
			// TODO: error handling
			//if ( vbo ) {
			
				ZLSharedConstBuffer* buffer = this->GetCursor () ? this->GetSharedConstBuffer () : 0;
			
//				if ( this->mCopyOnUpdate ) {
//					buffer = gfx.CopyBuffer ( buffer );
//				}
			
				gfx.BindBuffer ( this->mTarget, vbo );
				gfx.BufferData ( this->mTarget, this->GetLength (), buffer, 0, hint );
				gfx.BindBuffer ( this->mTarget, ZLGfxResource::UNBIND );
			
				count++;
			//}
			this->mVBOs [ i ] = vbo;
		}
	}
	
	return count == this->mVBOs.Size ();
}

//----------------------------------------------------------------//
void MOAIAbstractGfxBufferGL::ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( bool shouldDelete ) {

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mVBOs.Size (); ++i ) {
		this->mGfxMgr->DeleteOrDiscard ( this->mVBOs [ i ], shouldDelete );
	}
}

//----------------------------------------------------------------//
void MOAIAbstractGfxBufferGL::ZLAbstractGfxResource_OnGPUUnbind () {

	this->mGfxMgr->GetDrawingAPI ().BindBuffer ( this->mTarget, ZLGfxResource::UNBIND ); // OK?
}

//----------------------------------------------------------------//
bool MOAIAbstractGfxBufferGL::ZLAbstractGfxResource_OnGPUUpdate () {

	if ( !this->mUseVBOs ) return true;
	
	bool dirty = this->GetCursor () > 0;
	
	if ( dirty ) {
		this->mCurrentVBO =  ZLIndexOp::AddAndWrap ( this->mCurrentVBO, 1, this->mVBOs.Size ());
	}
	
	const ZLGfxHandle& vbo = this->mVBOs [ this->mCurrentVBO ];
	
	if ( dirty && vbo.CanBind ()) {
		
		ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
		
		// TODO: There are a few different ways to approach updating buffers with varying performance
		// on different platforms. The approach here is just to multi-buffer the VBO and replace its
		// contents via zglBufferSubData when they change. The TODO here is to do performance tests
		// on multiple devices, evaluate other approaches and possible expose the configuration of
		// those to the end user via Lua.
			
		ZLSharedConstBuffer* buffer = this->GetSharedConstBuffer ();
		
//		if ( this->mCopyOnUpdate ) {
//			buffer = gfx.CopyBuffer ( buffer );
//		}
		
		ZLGfxHandle vbo = gfx.CreateBuffer ();
		gfx.BindBuffer ( this->mTarget, vbo );
		gfx.BufferSubData ( this->mTarget, 0, this->GetCursor (), buffer, 0 );
		gfx.BindBuffer ( this->mTarget, ZLGfxResource::UNBIND );
	
		//u32 hint = this->mVBOs.Size () > 1 ? ZGL_BUFFER_USAGE_DYNAMIC_DRAW : ZGL_BUFFER_USAGE_STATIC_DRAW;
		//zglBufferData ( this->mTarget, this->GetLength (), 0, hint );
	}
	
	return true;
}
