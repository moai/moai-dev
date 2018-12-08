// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxBuffer.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGfxResourceClerk.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAIVertexArray.h>
#include <moai-sim/MOAIVertexBuffer.h>
#include <moai-sim/MOAIVertexFormat.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexArray::_reserveVAOs ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexArray, "U" )
	
	self->ReserveVAOs ( state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexArray::_reserveVertexBuffers ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexArray, "U" )
	
	self->ReserveVertexBuffers ( state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexArray::_setVertexBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexArray, "U" )
	
	u32 baseParam = 2;
	ZLIndex idx = ZLIndexOp::ZERO;
	
	if ( state.IsType ( baseParam, LUA_TNUMBER )) {
		idx = ZLIndexCast ( state.GetValue < u32 >( baseParam++, 1 ) - 1 );
	}
	
	MOAIVertexBuffer* buffer	= state.GetLuaObject < MOAIVertexBuffer >( baseParam++, false );
	MOAIVertexFormat* format	= state.GetLuaObject < MOAIVertexFormat >( baseParam++, false );
	
	self->SetVertexBuffer ( idx, buffer, format );

	return 0;
}

//================================================================//
// MOAIVertexArray
//================================================================//

//----------------------------------------------------------------//
bool MOAIVertexArray::AffirmVertexBuffers ( u32 idx ) {

	if (( this->mVertexBuffers.Size () == 0 ) && ( idx == 0 )) {
		this->ReserveVertexBuffers ( 1 );
	}
	return ( idx < this->mVertexBuffers.Size ());
}

//----------------------------------------------------------------//
void MOAIVertexArray::BindVertexArrayItems () {

	MOAIGfxState& gfxState = MOAIGfxMgr::Get ().mGfxState;

	ZLSize totalVBOs = this->mVertexBuffers.Size ();
	for ( ZLIndex i = ZLIndexOp::ZERO; i < totalVBOs; ++i ) {
		gfxState.BindVertexBufferWithFormat ( this->mVertexBuffers [ i ], this->mUseVAOs );
	}
}

//----------------------------------------------------------------//
MOAIVertexBuffer* MOAIVertexArray::GetVertexBuffer ( ZLIndex idx ) {

	return idx < this->mVertexBuffers.Size () ? this->mVertexBuffers [ idx ].mBuffer : ( MOAIVertexBuffer* )0;
}

//----------------------------------------------------------------//
MOAIVertexFormat* MOAIVertexArray::GetVertexFormat ( ZLIndex idx ) {

	return idx < this->mVertexBuffers.Size () ? this->mVertexBuffers [ idx ].mFormat : ( MOAIVertexFormat* )0;
}

//----------------------------------------------------------------//
MOAIVertexArray::MOAIVertexArray () :
	mCurrentVAO ( ZLIndexOp::INVALID ),
	mUseVAOs ( false ) {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGfxResource )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIVertexArray::~MOAIVertexArray () {

	this->ReserveVAOs ( 0 );
	this->ReserveVertexBuffers ( 0 );
}

//----------------------------------------------------------------//
bool MOAIVertexArray::OnCPUCreate () {

	return true;
}

//----------------------------------------------------------------//
void MOAIVertexArray::OnCPUDestroy () {
}

//----------------------------------------------------------------//
void MOAIVertexArray::OnGPUBind () {

	if ( this->mUseVAOs && this->mVAOs.Size ()) {
		MOAIGfxMgr::GetDrawingAPI ().BindVertexArray ( this->mVAOs [ this->mCurrentVAO ]);
	}
	else {
		this->BindVertexArrayItems ();
	}
}

//----------------------------------------------------------------//
bool MOAIVertexArray::OnGPUCreate () {

	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

	this->mUseVAOs = false;
	
	size_t totalVAOs = this->mVAOs.Size ();

	if ( totalVAOs ) {
		
		for ( ZLIndex i = ZLIndexOp::ZERO; i < totalVAOs; ++i ) {
			ZLGfxHandle vao = gfx.CreateVertexArray (); // OK for this to return 0
			if ( vao.GetType () != ZLGfxResource::NONE ) {
				this->mVAOs [ i ] = vao;
				this->mUseVAOs = true;
			}
		}
	}
	
	this->mCurrentVAO = ZLIndexCast ( 0 );
	this->OnGPUUpdate ();
	
	return true;
}

//----------------------------------------------------------------//
void MOAIVertexArray::OnGPUDeleteOrDiscard ( bool shouldDelete ) {

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mVAOs.Size (); ++i ) {
		MOAIGfxResourceClerk::DeleteOrDiscard ( this->mVAOs [ i ], shouldDelete );
	}
}

//----------------------------------------------------------------//
void MOAIVertexArray::OnGPUUnbind () {

	if ( this->mUseVAOs ) {
		MOAIGfxMgr::GetDrawingAPI ().BindVertexArray ( ZLGfxResource::UNBIND );
	}
	else {
		this->UnbindVertexArrayItems ();
	}
}

//----------------------------------------------------------------//
bool MOAIVertexArray::OnGPUUpdate () {

	if ( !this->mUseVAOs ) return true;
	if ( !this->mVAOs.Size ()) return false;

	this->mCurrentVAO =  ZLIndexOp::AddAndWrap ( this->mCurrentVAO, 1, this->mVAOs.Size ());
	const ZLGfxHandle& vao = this->mVAOs [ this->mCurrentVAO ];
	
	if ( vao.CanBind ()) {
		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
		gfx.BindVertexArray ( vao );
		this->BindVertexArrayItems ();
		gfx.BindVertexArray ( ZLGfxResource::UNBIND );
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIVertexArray::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIGfxResource::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIVertexArray::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIGfxResource::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "reserveVAOs",				_reserveVAOs },
		{ "reserveVertexBuffers",		_reserveVertexBuffers },
		{ "setVertexBuffer",			_setVertexBuffer },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIVertexArray::ReserveVAOs ( u32 total ) {

	if ( MOAIGfxMgr::IsValid ()) {
		for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mVAOs.Size (); ++i ) {
			MOAIGfxResourceClerk::DeleteOrDiscard ( this->mVAOs [ i ], false );
		}
	}
	this->mVAOs.Init ( total );
	
	this->FinishInit ();
}

//----------------------------------------------------------------//
void MOAIVertexArray::ReserveVertexBuffers ( u32 total ) {

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mVertexBuffers.Size (); ++i ) {
		this->mVertexBuffers [ i ].SetBufferAndFormat ( *this, 0, 0 );
	}
	this->mVertexBuffers.Init ( total );
	
	this->FinishInit ();
}

//----------------------------------------------------------------//
void MOAIVertexArray::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {

	ZLSize totalVAOs = state.GetFieldValue < cc8*, MOAILuaSize >( -1, "mTotalVAOs", 0 );
	this->ReserveVAOs ( totalVAOs );
	
	ZLSize totalVertexBuffers = state.GetFieldValue < cc8*, MOAILuaSize >( -1, "mTotalVertexBuffers", 0 );
	this->ReserveVertexBuffers ( totalVertexBuffers );
	
	if ( state.PushFieldWithType ( -1, "mVertexBuffers", LUA_TTABLE )) {
		int itr = state.PushTableItr ( -1 );
		for ( ZLIndex i = ZLIndexOp::ZERO; state.TableItrNext ( itr ); ++i ) {
			if ( state.IsType ( -1, LUA_TTABLE )) {
				MOAIVertexBuffer* buffer = serializer.MemberIDToObject < MOAIVertexBuffer >( state.GetFieldValue < cc8*, MOAISerializer::ObjID >( -1, "mBuffer", 0 ));
				MOAIVertexFormat* format = serializer.MemberIDToObject < MOAIVertexFormat >( state.GetFieldValue < cc8*, MOAISerializer::ObjID >( -1, "mFormat", 0 ));
				this->SetVertexBuffer ( i, buffer, format );
			}
		}
		state.Pop ();
	}
	this->FinishInit ();
}

//----------------------------------------------------------------//
void MOAIVertexArray::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

	state.SetField < cc8*, MOAILuaSize >( -1, "mTotalVAOs", this->mVAOs.Size ());
	state.SetField < cc8*, MOAILuaSize >( -1, "mTotalVertexBuffers", this->mVertexBuffers.Size ());
	
	lua_newtable ( state );
	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mVertexBuffers.Size (); ++i ) {
		state.Push ( MOAILuaIndex ( i ));
		lua_newtable ( state );
		state.SetField < cc8*, MOAISerializer::ObjID >( -1, "mBuffer", serializer.AffirmMemberID ( this->mVertexBuffers [ i ].mBuffer ));
		state.SetField < cc8*, MOAISerializer::ObjID >( -1, "mFormat", serializer.AffirmMemberID ( this->mVertexBuffers [ i ].mFormat ));
		lua_settable ( state, -3 );
	}
	lua_setfield ( state, -2, "mVertexBuffers" );
}

//----------------------------------------------------------------//
void MOAIVertexArray::SetVertexBuffer ( ZLIndex idx, MOAIVertexBuffer* vtxBuffer, MOAIVertexFormat* vtxFormat ) {

	if ( this->AffirmVertexBuffers ( idx )) {
		this->mVertexBuffers [ idx ].SetBufferAndFormat ( *this, vtxBuffer, vtxFormat );
		this->ScheduleForGPUUpdate ();
	}
}

//----------------------------------------------------------------//
void MOAIVertexArray::UnbindVertexArrayItems () {

	MOAIGfxState& gfxState = MOAIGfxMgr::Get ().mGfxState;

	size_t totalVBOs = this->mVertexBuffers.Size ();
	for ( ZLIndex i = ZLIndexOp::ZERO; i < totalVBOs; ++i ) {
		gfxState.UnbindVertexBufferWithFormat ( this->mVertexBuffers [ i ]);
	}
}
