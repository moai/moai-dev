// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxBufferVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>

//================================================================//
// lua
//================================================================//

////----------------------------------------------------------------//
///**	@lua	reserveVBOs
//	@text	Reserves one or more VBO objects. Multi-buffering is
//			supported via multiple VBOs.
//
//	@in		MOAIGfxBufferVK self
//	@in		number count
//	@out	nil
//*/
//int	MOAIGfxBufferVK::_reserveVBOs ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIGfxBufferVK, "UN" )
//
//	u32 vbos = state.GetValue < u32 >( 2, 0 );
//	self->ReserveVBOs ( vbos );
//	return 0;
//}

//================================================================//
// MOAIGfxBufferVK
//================================================================//

////----------------------------------------------------------------//
//size_t MOAIGfxBufferVK::CountVBOs () {
//
//	return this->mVBOs.Size ();
//}
//
////----------------------------------------------------------------//
//ZLSharedConstBuffer* MOAIGfxBufferVK::GetBufferForBind ( ZLGfx& gfx ) {
//	UNUSED(gfx);
//	return this->mUseVBOs ? 0 : this->ZLCopyOnWrite::GetSharedConstBuffer ();
//
////	if ( this->mUseVBOs ) return 0;
////
////	ZLSharedConstBuffer* buffer = this->ZLCopyOnWrite::GetSharedConstBuffer ();
////	return this->mCopyOnBind ? gfx.CopyBuffer ( buffer ) : buffer;
//}
//
////----------------------------------------------------------------//
//MOAIGfxBufferVK::MOAIGfxBufferVK () :
//	mCurrentVBO ( ZLIndexOp::ZERO ),
//	mTarget ( ZGL_BUFFER_TARGET_ARRAY ),
//	mUseVBOs ( false ),
//	mCopyOnUpdate ( false ) {
//
//	RTTI_BEGIN
//		RTTI_EXTEND ( MOAIGfxResourceVK )
//		RTTI_EXTEND ( MOAIGfxBuffer )
//	RTTI_END
//}

//----------------------------------------------------------------//
MOAIGfxBufferVK::MOAIGfxBufferVK () {
		
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGfxResourceVK )
		RTTI_EXTEND ( MOAIGfxBuffer )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxBufferVK::~MOAIGfxBufferVK () {
}

////----------------------------------------------------------------//
//void MOAIGfxBufferVK::ReserveVBOs ( ZLSize gpuBuffers ) {
//
//	if ( gpuBuffers < this->mVBOs.Size ()) {
//		this->mVBOs.Clear ();
//	}
//
//	if ( gpuBuffers ) {
//		this->mVBOs.Resize ( gpuBuffers );
//		this->mCurrentVBO = ZLIndexCast ( gpuBuffers - 1 );
//	}
//
//	this->MOAIGfxResourceVK::ScheduleForGPUUpdate ();
//}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool MOAIGfxBufferVK::MOAIGfxResource_OnCPUCreate () {
	return true;
}

//----------------------------------------------------------------//
void MOAIGfxBufferVK::MOAIGfxResource_OnCPUDestroy () {
	MOAIGfxBuffer::MOAIGfxResource_OnCPUDestroy ();

//	this->mVBOs.Clear ();
//	this->mCurrentVBO = ZLIndexOp::ZERO;
}

//----------------------------------------------------------------//
void MOAIGfxBufferVK::MOAIGfxResource_OnCPUPurgeRecoverable () {
}

//----------------------------------------------------------------//
void MOAIGfxBufferVK::MOAIGfxResourceVK_OnGPUBind () {
	
//	if ( !this->mUseVBOs ) return;
//
//	const ZLGfxHandle& vbo = this->mVBOs [ this->mCurrentVBO ];
//
//	if ( vbo.CanBind ()) {
//		this->mGfxMgr->GetDrawingAPI ().BindBuffer ( this->mTarget, vbo );
//	}
}

//----------------------------------------------------------------//
bool MOAIGfxBufferVK::MOAIGfxResourceVK_OnGPUCreate () {

//	u32 count = 0;
//
//	this->mUseVBOs = ( this->mVBOs.Size () > 0 );
//
//	if ( this->mUseVBOs ) {
//
//		ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
//		u32 hint = this->mVBOs.Size () > 1 ? ZGL_BUFFER_USAGE_STREAM_DRAW : ZGL_BUFFER_USAGE_STATIC_DRAW;
//
//		for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mVBOs.Size (); ++i ) {
//
//			ZLGfxHandle vbo = gfx.CreateBuffer ();
//
//			// TODO: error handling
//			//if ( vbo ) {
//
//				ZLSharedConstBuffer* buffer = this->GetCursor () ? this->GetSharedConstBuffer () : 0;
//
////				if ( this->mCopyOnUpdate ) {
////					buffer = gfx.CopyBuffer ( buffer );
////				}
//
//				gfx.BindBuffer ( this->mTarget, vbo );
//				gfx.BufferData ( this->mTarget, this->GetLength (), buffer, 0, hint );
//				gfx.BindBuffer ( this->mTarget, ZLGfxResource::UNBIND );
//
//				count++;
//			//}
//			this->mVBOs [ i ] = vbo;
//		}
//	}
//
//	return count == this->mVBOs.Size ();
	return true;
}

//----------------------------------------------------------------//
void MOAIGfxBufferVK::MOAIGfxResourceVK_OnGPUDeleteOrDiscard ( bool shouldDelete ) {

//	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mVBOs.Size (); ++i ) {
//		this->mGfxMgr->DeleteOrDiscard ( this->mVBOs [ i ], shouldDelete );
//	}
}

//----------------------------------------------------------------//
void MOAIGfxBufferVK::MOAIGfxResourceVK_OnGPUUnbind () {

//	this->mGfxMgr->GetDrawingAPI ().BindBuffer ( this->mTarget, ZLGfxResource::UNBIND ); // OK?
}

//----------------------------------------------------------------//
bool MOAIGfxBufferVK::MOAIGfxResourceVK_OnGPUUpdate () {

//	if ( !this->mUseVBOs ) return true;
//
//	bool dirty = this->GetCursor () > 0;
//
//	if ( dirty ) {
//		this->mCurrentVBO =  ZLIndexOp::AddAndWrap ( this->mCurrentVBO, 1, this->mVBOs.Size ());
//	}
//
//	const ZLGfxHandle& vbo = this->mVBOs [ this->mCurrentVBO ];
//
//	if ( dirty && vbo.CanBind ()) {
//
//		ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
//
//		// TODO: There are a few different ways to approach updating buffers with varying performance
//		// on different platforms. The approach here is just to multi-buffer the VBO and replace its
//		// contents via zglBufferSubData when they change. The TODO here is to do performance tests
//		// on multiple devices, evaluate other approaches and possible expose the configuration of
//		// those to the end user via Lua.
//
//		ZLSharedConstBuffer* buffer = this->GetSharedConstBuffer ();
//
////		if ( this->mCopyOnUpdate ) {
////			buffer = gfx.CopyBuffer ( buffer );
////		}
//
//		ZLGfxHandle vbo = gfx.CreateBuffer ();
//		gfx.BindBuffer ( this->mTarget, vbo );
//		gfx.BufferSubData ( this->mTarget, 0, this->GetCursor (), buffer, 0 );
//		gfx.BindBuffer ( this->mTarget, ZLGfxResource::UNBIND );
//
//		//u32 hint = this->mVBOs.Size () > 1 ? ZGL_BUFFER_USAGE_DYNAMIC_DRAW : ZGL_BUFFER_USAGE_STATIC_DRAW;
//		//zglBufferData ( this->mTarget, this->GetLength (), 0, hint );
//	}
	
	return true;
}

//----------------------------------------------------------------//
void MOAIGfxBufferVK::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxResourceVK, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxBuffer, MOAILuaObject_RegisterLuaClass ( composer, state ));
	
//	state.SetField ( -1, "INDEX_BUFFER",			( u32 )ZGL_BUFFER_TARGET_ELEMENT_ARRAY );
//	state.SetField ( -1, "VERTEX_BUFFER",			( u32 )ZGL_BUFFER_TARGET_ARRAY );
}

//----------------------------------------------------------------//
void MOAIGfxBufferVK::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxResourceVK, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxBuffer, MOAILuaObject_RegisterLuaFuncs ( composer, state ));

//	luaL_Reg regTable [] = {
//		{ "reserveVBOs",			_reserveVBOs },
//		{ NULL, NULL }
//	};
//
//	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxBufferVK::MOAILuaObject_SerializeIn ( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer ) {

//	u32 totalVBOs = state.GetFieldValue < cc8*, u32 >( -1, "mTotalVBOs", 0 );
//	this->ReserveVBOs ( totalVBOs );

	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxBuffer, MOAILuaObject_SerializeIn ( composer, state, serializer ));
}

//----------------------------------------------------------------//
void MOAIGfxBufferVK::MOAILuaObject_SerializeOut ( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer ) {
	
//	state.SetField < cc8*, MOAILuaSize >( -1, "mTotalVBOs", this->CountVBOs ());
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxBuffer, MOAILuaObject_SerializeOut ( composer, state, serializer ));
}
