// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIGfxBufferGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	reserveVBOs
	@text	Reserves one or more VBO objects. Multi-buffering is
			supported via multiple VBOs.
 
	@in		MOAIGfxBufferGL self
	@in		number count
	@out	nil
*/
int	MOAIGfxBufferGL::_reserveVBOs ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxBufferGL, "UN" )

	u32 vbos = state.GetValue < u32 >( 2, 0 );
	self->ReserveVBOs ( vbos );
	return 0;
}

//================================================================//
// MOAIGfxBufferGL
//================================================================//

//----------------------------------------------------------------//
size_t MOAIGfxBufferGL::CountVBOs () {

	return this->mVBOs.Size ();
}

//----------------------------------------------------------------//
ZLSharedConstBuffer* MOAIGfxBufferGL::GetBufferForBind ( ZLGfx& gfx ) {
	UNUSED(gfx);
	return this->mUseVBOs ? 0 : this->ZLCopyOnWrite::GetSharedConstBuffer ();

//	if ( this->mUseVBOs ) return 0;
//
//	ZLSharedConstBuffer* buffer = this->ZLCopyOnWrite::GetSharedConstBuffer ();
//	return this->mCopyOnBind ? gfx.CopyBuffer ( buffer ) : buffer;
}

//----------------------------------------------------------------//
MOAIGfxBufferGL::MOAIGfxBufferGL () :
	mCurrentVBO ( 0 ),
	mTarget ( ZLGfxEnum::BUFFER_TARGET_ARRAY ),
	mUseVBOs ( false ),
	mCopyOnUpdate ( false ) {
		
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGfxResourceGL )
		RTTI_EXTEND ( MOAIGfxBuffer )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxBufferGL::~MOAIGfxBufferGL () {

	this->ScheduleForGPUDestroy ();
}

//----------------------------------------------------------------//
void MOAIGfxBufferGL::ReserveVBOs ( ZLSize gpuBuffers ) {

	if ( gpuBuffers < this->mVBOs.Size ()) {
		this->mVBOs.Clear ();
	}

	if ( gpuBuffers ) {
		this->mVBOs.Resize ( gpuBuffers );
		this->mCurrentVBO = gpuBuffers - 1;
	}

	this->MOAIGfxResourceGL::ScheduleForGPUUpdate ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxBufferGL::MOAIGfxResourceGL_OnGPUBind () {
	
	if ( !this->mUseVBOs ) return;
	
	const ZLGfxHandle& vbo = this->mVBOs [ this->mCurrentVBO ];
	
	if ( vbo.CanBind ()) {
		this->mGfxMgr->GetDrawingAPI ().BindBuffer ( this->mTarget, vbo );
	}
}

//----------------------------------------------------------------//
bool MOAIGfxBufferGL::MOAIGfxResourceGL_OnGPUCreate () {

	u32 count = 0;

	this->mUseVBOs = ( this->mVBOs.Size () > 0 );
	
	if ( this->mUseVBOs ) {

		ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
		ZLGfxEnum::_ hint = this->mVBOs.Size () > 1 ? ZLGfxEnum::BUFFER_USAGE_STREAM_DRAW : ZLGfxEnum::BUFFER_USAGE_STATIC_DRAW;

		for ( ZLIndex i = 0; i < this->mVBOs.Size (); ++i ) {
			
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
void MOAIGfxBufferGL::MOAIGfxResourceGL_OnGPUDeleteOrDiscard ( bool shouldDelete ) {

	for ( ZLIndex i = 0; i < this->mVBOs.Size (); ++i ) {
		this->mGfxMgr->DeleteOrDiscard ( this->mVBOs [ i ], shouldDelete );
	}
}

//----------------------------------------------------------------//
void MOAIGfxBufferGL::MOAIGfxResourceGL_OnGPUUnbind () {

	this->mGfxMgr->GetDrawingAPI ().BindBuffer ( this->mTarget, ZLGfxResource::UNBIND ); // OK?
}

//----------------------------------------------------------------//
bool MOAIGfxBufferGL::MOAIGfxResourceGL_OnGPUUpdate () {

	if ( !this->mUseVBOs ) return true;
	
	bool dirty = this->GetCursor () > 0;
	
	if ( dirty ) {
		this->mCurrentVBO = ZLIndexOp::AddAndWrap ( this->mCurrentVBO, 1, this->mVBOs.Size ());
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
	
		//u32 hint = this->mVBOs.Size () > 1 ? ZLGfxEnum::BUFFER_USAGE_DYNAMIC_DRAW : ZLGfxEnum::BUFFER_USAGE_STATIC_DRAW;
		//zglBufferData ( this->mTarget, this->GetLength (), 0, hint );
	}
	
	return true;
}

//----------------------------------------------------------------//
void MOAIGfxBufferGL::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxResourceGL, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxBuffer, MOAILuaObject_RegisterLuaClass ( composer, state ));
	
	state.SetField ( -1, "INDEX_BUFFER",			( u32 )ZLGfxEnum::BUFFER_TARGET_ELEMENT_ARRAY );
	state.SetField ( -1, "VERTEX_BUFFER",			( u32 )ZLGfxEnum::BUFFER_TARGET_ARRAY );
}

//----------------------------------------------------------------//
void MOAIGfxBufferGL::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxResourceGL, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxBuffer, MOAILuaObject_RegisterLuaFuncs ( composer, state ));

	luaL_Reg regTable [] = {
		{ "reserveVBOs",			_reserveVBOs },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxBufferGL::MOAILuaObject_SerializeIn ( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer ) {

	u32 totalVBOs = state.GetFieldValue < cc8*, u32 >( -1, "mTotalVBOs", 0 );
	this->ReserveVBOs ( totalVBOs );

	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxBuffer, MOAILuaObject_SerializeIn ( composer, state, serializer ));
}

//----------------------------------------------------------------//
void MOAIGfxBufferGL::MOAILuaObject_SerializeOut ( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer ) {
	
	state.SetField < cc8*, MOAILuaSize >( -1, "mTotalVBOs", this->CountVBOs ());
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxBuffer, MOAILuaObject_SerializeOut ( composer, state, serializer ));
}
