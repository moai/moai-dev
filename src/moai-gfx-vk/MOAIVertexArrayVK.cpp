// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIVertexArrayVK.h>
#include <moai-gfx-vk/MOAIVertexFormatVK.h>

//================================================================//
// lua
//================================================================//

////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAIVertexArrayVK::_reserveVAOs ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIVertexArrayVK, "U" )
//
//	self->ReserveVAOs ( state.GetValue < u32 >( 2, 0 ));
//	return 0;
//}
//
////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAIVertexArrayVK::_reserveVertexBuffers ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIVertexArrayVK, "U" )
//
//	self->ReserveVertexBuffers ( state.GetValue < u32 >( 2, 0 ));
//	return 0;
//}
//
////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAIVertexArrayVK::_setVertexBuffer ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIVertexArrayVK, "U" )
//
//	u32 baseParam = 2;
//	ZLIndex idx = ZLIndexOp::ZERO;
//
//	if ( state.IsType ( baseParam, LUA_TNUMBER )) {
//		idx = ZLIndexCast ( state.GetValue < u32 >( baseParam++, 1 ) - 1 );
//	}
//
//	MOAIVertexBufferVK* buffer	= state.GetLuaObject < MOAIVertexBufferVK >( baseParam++, false );
//	MOAIVertexFormatVK* format	= state.GetLuaObject < MOAIVertexFormatVK >( baseParam++, false );
//
//	self->SetVertexBuffer ( idx, buffer, format );
//
//	return 0;
//}

////================================================================//
//// MOAIVertexBufferWithFormatVK
////================================================================//
//
////----------------------------------------------------------------//
//MOAIVertexBufferWithFormatVK::MOAIVertexBufferWithFormatVK () {
//}
//
//
////----------------------------------------------------------------//
//MOAIVertexBufferWithFormatVK::~MOAIVertexBufferWithFormatVK () {
//}
//
////----------------------------------------------------------------//
//void MOAIVertexBufferWithFormatVK::SetBufferAndFormat ( MOAIVertexBufferVK* buffer, MOAIVertexFormatVK* format ) {
//
//	this->mBuffer = buffer;
//	this->mFormat = format;
//}

//================================================================//
// MOAIVertexArrayVK
//================================================================//

////----------------------------------------------------------------//
//bool MOAIVertexArrayVK::AffirmVertexBuffers ( u32 idx ) {
//
//	if (( this->mVertexBuffers.Size () == 0 ) && ( idx == 0 )) {
//		this->ReserveVertexBuffers ( 1 );
//	}
//	return ( idx < this->mVertexBuffers.Size ());
//}
//
////----------------------------------------------------------------//
//void MOAIVertexArrayVK::BindVertexArrayItems () {
//
//	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
//
//	ZLSize totalVBOs = this->mVertexBuffers.Size ();
//	for ( ZLIndex i = ZLIndexOp::ZERO; i < totalVBOs; ++i ) {
//
//		MOAIVertexBufferVK* buffer = this->mVertexBuffers [ i ].mBuffer;
//		MOAIVertexFormatVK* format = this->mVertexBuffers [ i ].mFormat;
//
//		assert ( buffer && format );
//
//		assert (( this->mUseVAOs && buffer->IsUsingVBOs ()) || ( !this->mUseVAOs )); // buffer objects must use VBOs to work with VAOs
//
//		ZLSharedConstBuffer* bufferForBind = buffer->GetBufferForBind ( gfx );
//
//		buffer->Bind ();
//		format->Bind ( gfx, bufferForBind );
//		buffer->Unbind ();
//	}
//}
//
////----------------------------------------------------------------//
//MOAIVertexArrayVK::MOAIVertexArrayVK () :
//	mCurrentVAO ( ZLIndexOp::INVALID ),
//	mUseVAOs ( false ) {
//
//	RTTI_BEGIN
//		RTTI_EXTEND ( MOAIVertexArray )
//		RTTI_EXTEND ( MOAIGfxResourceVK )
//	RTTI_END
//}

//----------------------------------------------------------------//
MOAIVertexArrayVK::MOAIVertexArrayVK () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIVertexArray )
		RTTI_EXTEND ( MOAIGfxResourceVK )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIVertexArrayVK::~MOAIVertexArrayVK () {

//	this->ReserveVAOs ( 0 );
//	this->ReserveVertexBuffers ( 0 );
}

////----------------------------------------------------------------//
//void MOAIVertexArrayVK::ReserveVAOs ( u32 total ) {
//
//	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mVAOs.Size (); ++i ) {
//		this->mGfxMgr->DeleteOrDiscard ( this->mVAOs [ i ], false );
//	}
//	this->mVAOs.Init ( total );
//	this->ScheduleForGPUUpdate ();
//}
//
////----------------------------------------------------------------//
//void MOAIVertexArrayVK::ReserveVertexBuffers ( u32 total ) {
//
//	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mVertexBuffers.Size (); ++i ) {
//		this->mVertexBuffers [ i ].SetBufferAndFormat ( 0, 0 );
//	}
//	this->mVertexBuffers.Init ( total );
//
//	this->ScheduleForGPUUpdate ();
//}
//
////----------------------------------------------------------------//
//void MOAIVertexArrayVK::UnbindVertexArrayItems () {
//
//	size_t totalVBOs = this->mVertexBuffers.Size ();
//	for ( ZLIndex i = ZLIndexOp::ZERO; i < totalVBOs; ++i ) {
//
//		MOAIVertexFormatVK* format = this->mVertexBuffers [ i ].mFormat;
//		assert ( format );
//		format->Unbind ( this->mGfxMgr->GetDrawingAPI ());
//	}
//}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool MOAIVertexArrayVK::MOAIGfxResource_OnCPUCreate () {

	return true;
}

//----------------------------------------------------------------//
void MOAIVertexArrayVK::MOAIGfxResource_OnCPUPurgeRecoverable () {
}

//----------------------------------------------------------------//
void MOAIVertexArrayVK::MOAIGfxResourceVK_OnGPUBind () {

//	if ( this->mUseVAOs && this->mVAOs.Size ()) {
//		this->mGfxMgr->GetDrawingAPI ().BindVertexArray ( this->mVAOs [ this->mCurrentVAO ]);
//	}
//	else {
//		this->BindVertexArrayItems ();
//	}
}

//----------------------------------------------------------------//
bool MOAIVertexArrayVK::MOAIGfxResourceVK_OnGPUCreate () {

//	this->mUseVAOs = false;
//
//	size_t totalVAOs = this->mVAOs.Size ();
//
//	if ( totalVAOs ) {
//
//		for ( ZLIndex i = ZLIndexOp::ZERO; i < totalVAOs; ++i ) {
//			ZLGfxHandle vao = this->mGfxMgr->GetDrawingAPI ().CreateVertexArray (); // OK for this to return 0
//			if ( vao.GetType () != ZLGfxResource::NONE ) {
//				this->mVAOs [ i ] = vao;
//				this->mUseVAOs = true;
//			}
//		}
//	}
//
//	this->mCurrentVAO = ZLIndexCast ( 0 );
//	this->MOAIGfxResourceVK_OnGPUUpdate ();
	
	return true;
}

//----------------------------------------------------------------//
void MOAIVertexArrayVK::MOAIGfxResourceVK_OnGPUDeleteOrDiscard ( bool shouldDelete ) {

//	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mVAOs.Size (); ++i ) {
//		this->mGfxMgr->DeleteOrDiscard ( this->mVAOs [ i ], shouldDelete );
//	}
}

//----------------------------------------------------------------//
void MOAIVertexArrayVK::MOAIGfxResourceVK_OnGPUUnbind () {

//	if ( this->mUseVAOs ) {
//		this->mGfxMgr->GetDrawingAPI ().BindVertexArray ( ZLGfxResource::UNBIND );
//	}
//	else {
//		this->UnbindVertexArrayItems ();
//	}
}

//----------------------------------------------------------------//
bool MOAIVertexArrayVK::MOAIGfxResourceVK_OnGPUUpdate () {

//	if ( !this->mUseVAOs ) return true;
//	if ( !this->mVAOs.Size ()) return false;
//
//	this->mCurrentVAO = ZLIndexOp::AddAndWrap ( this->mCurrentVAO, 1, this->mVAOs.Size ());
//	const ZLGfxHandle& vao = this->mVAOs [ this->mCurrentVAO ];
//
//	if ( vao.CanBind ()) {
//		ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
//		gfx.BindVertexArray ( vao );
//		this->BindVertexArrayItems ();
//		gfx.BindVertexArray ( ZLGfxResource::UNBIND );
//		return true;
//	}
//	return false;
	return true;
}

//----------------------------------------------------------------//
void MOAIVertexArrayVK::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIVertexArray, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxResourceVK, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIVertexArrayVK::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIVertexArray, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxResourceVK, MOAILuaObject_RegisterLuaFuncs ( composer, state ));

//	luaL_Reg regTable [] = {
//		{ "reserveVAOs",				_reserveVAOs },
//		{ "reserveVertexBuffers",		_reserveVertexBuffers },
//		{ "setVertexBuffer",			_setVertexBuffer },
//		{ NULL, NULL }
//	};
//
//	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIVertexArrayVK::MOAILuaObject_SerializeIn ( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer ) {

//	ZLSize totalVAOs = state.GetFieldValue < cc8*, MOAILuaSize >( -1, "mTotalVAOs", 0 );
//	this->ReserveVAOs ( totalVAOs );
//
//	ZLSize totalVertexBuffers = state.GetFieldValue < cc8*, MOAILuaSize >( -1, "mTotalVertexBuffers", 0 );
//	this->ReserveVertexBuffers ( totalVertexBuffers );
//
//	if ( state.PushFieldWithType ( -1, "mVertexBuffers", LUA_TTABLE )) {
//		int itr = state.PushTableItr ( -1 );
//		for ( ZLIndex i = ZLIndexOp::ZERO; state.TableItrNext ( itr ); ++i ) {
//			if ( state.IsType ( -1, LUA_TTABLE )) {
//				MOAIVertexBufferVK* buffer = serializer.MemberIDToObject < MOAIVertexBufferVK >( state.GetFieldValue < cc8*, MOAISerializer::ObjID >( -1, "mBuffer", 0 ));
//				MOAIVertexFormatVK* format = serializer.MemberIDToObject < MOAIVertexFormatVK >( state.GetFieldValue < cc8*, MOAISerializer::ObjID >( -1, "mFormat", 0 ));
//				this->SetVertexBuffer ( i, buffer, format );
//			}
//		}
//		state.Pop ();
//	}
//	this->ScheduleForGPUUpdate ();
}

//----------------------------------------------------------------//
void MOAIVertexArrayVK::MOAILuaObject_SerializeOut ( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer ) {

//	state.SetField < cc8*, MOAILuaSize >( -1, "mTotalVAOs", this->mVAOs.Size ());
//	state.SetField < cc8*, MOAILuaSize >( -1, "mTotalVertexBuffers", this->mVertexBuffers.Size ());
//
//	lua_newtable ( state );
//	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mVertexBuffers.Size (); ++i ) {
//		state.Push ( MOAILuaIndex ( i ));
//		lua_newtable ( state );
//
//		MOAIVertexBufferVK* buffer = this->mVertexBuffers [ i ].mBuffer->AsType < MOAIVertexBufferVK >();
//		MOAIVertexFormatVK* format = this->mVertexBuffers [ i ].mFormat->AsType < MOAIVertexFormatVK >();
//
//		assert ( buffer && format );
//
//		state.SetField < cc8*, MOAISerializer::ObjID >( -1, "mBuffer", serializer.AffirmMemberID ( buffer ));
//		state.SetField < cc8*, MOAISerializer::ObjID >( -1, "mFormat", serializer.AffirmMemberID ( format ));
//		lua_settable ( state, -3 );
//	}
//	lua_setfield ( state, -2, "mVertexBuffers" );
}

//----------------------------------------------------------------//
MOAIVertexBuffer* MOAIVertexArrayVK::MOAIVertexArray_GetVertexBuffer ( ZLIndex idx ) {

//	return idx < this->mVertexBuffers.Size () ? ( MOAIVertexBufferVK* )this->mVertexBuffers [ idx ].mBuffer : ( MOAIVertexBufferVK* )0;
	return 0;
}

//----------------------------------------------------------------//
MOAIVertexFormat* MOAIVertexArrayVK::MOAIVertexArray_GetVertexFormat ( ZLIndex idx ) {

//	return idx < this->mVertexBuffers.Size () ? ( MOAIVertexFormatVK* )this->mVertexBuffers [ idx ].mFormat : ( MOAIVertexFormatVK* )0;
	return 0;
}

//----------------------------------------------------------------//
void MOAIVertexArrayVK::MOAIVertexArray_SetVertexBuffer ( ZLIndex idx, MOAIVertexBuffer* vtxBuffer, MOAIVertexFormat* vtxFormat ) {

//	MOAIVertexBufferVK* vertexBufferVK = MOAICastAssert < MOAIVertexBufferVK >( vtxBuffer );
//	MOAIVertexFormatVK* vertexFormatVK = MOAICastAssert < MOAIVertexFormatVK >( vtxFormat );
//
//	if ( this->AffirmVertexBuffers ( idx )) {
//		this->mVertexBuffers [ idx ].SetBufferAndFormat ( vertexBufferVK, vertexFormatVK );
//		this->ScheduleForGPUUpdate ();
//	}
}
