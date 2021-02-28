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
	UNUSED ( gfx );
	return 0;
//	return this->mUseVBOs ? 0 : this->ZLCopyOnWrite::GetSharedConstBuffer ();

//	if ( this->mUseVBOs ) return 0;
//
//	ZLSharedConstBuffer* buffer = this->ZLCopyOnWrite::GetSharedConstBuffer ();
//	return this->mCopyOnBind ? gfx.CopyBuffer ( buffer ) : buffer;
}

//----------------------------------------------------------------//
MOAIGfxBufferGL::MOAIGfxBufferGL ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	MOAIGfxResource ( context ),
	MOAIStream ( context ),
	MOAIGfxBuffer ( context ),
	MOAIEventSource ( context ),
	MOAIInstanceEventSource ( context ),
	MOAIGfxResourceGL ( context ),
	mCurrentVBO ( 0 ),
	mTarget ( ZLGfxEnum::BUFFER_TARGET_ARRAY ),
	mCopyOnUpdate ( false ) {
		
	RTTI_BEGIN ( MOAIGfxBufferGL )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIGfxBufferGL >)
		RTTI_VISITOR ( MOAIAbstractLuaSerializationVisitor, MOAILuaSerializationVisitor < MOAIGfxBufferGL >)
		RTTI_EXTEND ( MOAIGfxResourceGL )
		RTTI_EXTEND ( MOAIGfxBuffer )
	RTTI_END
	
	this->ReserveVBOs ( 1 );
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

//----------------------------------------------------------------//
void MOAIGfxBufferGL::Update () {

	this->ScheduleForGPUUpdate ();
	this->Bind ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxBufferGL::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
	
	state.SetField ( -1, "INDEX_BUFFER",			( u32 )ZLGfxEnum::BUFFER_TARGET_ELEMENT_ARRAY );
	state.SetField ( -1, "VERTEX_BUFFER",			( u32 )ZLGfxEnum::BUFFER_TARGET_ARRAY );
}

//----------------------------------------------------------------//
void MOAIGfxBufferGL::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "reserveVBOs",			_reserveVBOs },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxBufferGL::_SerializeIn ( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer ) {
	if ( history.Visit ( *this )) return;

	u32 totalVBOs = state.GetFieldValue < cc8*, u32 >( -1, "mTotalVBOs", 0 );
	this->ReserveVBOs ( totalVBOs );
}

//----------------------------------------------------------------//
void MOAIGfxBufferGL::_SerializeOut ( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer ) {
	if ( history.Visit ( *this )) return;
	
	state.SetField < cc8*, MOAILuaSize >( -1, "mTotalVBOs", this->CountVBOs ());
}

//----------------------------------------------------------------//
void MOAIGfxBufferGL::MOAIGfxResourceGL_OnGPUBind () {
		
	const ZLGfxHandle& vbo = this->mVBOs [ this->mCurrentVBO ];
	
	if ( vbo.CanBind ()) {
		this->mGfxMgr->GetDrawingAPI ().BindBuffer ( this->mTarget, vbo );
	}
}

//----------------------------------------------------------------//
bool MOAIGfxBufferGL::MOAIGfxResourceGL_OnGPUCreate () {

	u32 count = 0;

	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
	ZLGfxEnum::_ hint = this->mVBOs.Size () > 1 ? ZLGfxEnum::BUFFER_USAGE_STREAM_DRAW : ZLGfxEnum::BUFFER_USAGE_STATIC_DRAW;

	for ( ZLIndex i = 0; i < this->mVBOs.Size (); ++i ) {
		
		ZLGfxHandle vbo = gfx.CreateBuffer ();
		assert ( vbo ); // TODO: error handling
		
		ZLSharedConstBuffer* buffer = this->GetCursor () ? this->GetSharedConstBuffer () : NULL;
	
		gfx.BindBuffer ( this->mTarget, vbo );
		gfx.BufferData ( this->mTarget, this->GetLength (), buffer, 0, hint );
		gfx.BindBuffer ( this->mTarget, ZLGfxResource::UNBIND );
	
		count++;

		this->mVBOs [ i ] = vbo;
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
		
		gfx.BindBuffer ( this->mTarget, vbo );
		gfx.BufferSubData ( this->mTarget, 0, this->GetCursor (), buffer, 0 );
		gfx.BindBuffer ( this->mTarget, ZLGfxResource::UNBIND );
	
		//u32 hint = this->mVBOs.Size () > 1 ? ZLGfxEnum::BUFFER_USAGE_DYNAMIC_DRAW : ZLGfxEnum::BUFFER_USAGE_STATIC_DRAW;
		//zglBufferData ( this->mTarget, this->GetLength (), 0, hint );
	}
	
	return true;
}
