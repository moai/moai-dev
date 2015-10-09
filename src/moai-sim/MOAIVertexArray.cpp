// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxBuffer.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGfxResourceMgr.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIProp.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAIVertexArray.h>
#include <moai-sim/MOAIVertexFormat.h>

//================================================================//
// MOAIVertexArrayItem
//================================================================//

//----------------------------------------------------------------//
void MOAIVertexArrayItem::Bind ( bool useVAOs ) {

	if ( this->mBuffer && this->mFormat ) {
		
		MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
		gfxDevice.BindVertexBuffer ( this->mBuffer );
		gfxDevice.BindVertexFormat ( this->mFormat );
		
		assert (( useVAOs && this->mBuffer->IsUsingVBOs ()) || ( !useVAOs )); // buffer objects must use VBOs to work with VAOs
	}
}

//----------------------------------------------------------------//
MOAIVertexArrayItem::MOAIVertexArrayItem () {
}

//----------------------------------------------------------------//
MOAIVertexArrayItem::~MOAIVertexArrayItem () {
}

//----------------------------------------------------------------//
void MOAIVertexArrayItem::SetBufferAndFormat ( MOAIVertexArray& owner, MOAIVertexBuffer* buffer, MOAIVertexFormat* format ) {

	this->mBuffer.Set ( owner, buffer );
	this->mFormat.Set ( owner, format );
}

//----------------------------------------------------------------//
void MOAIVertexArrayItem::Unbind () {

	if ( this->mBuffer && this->mFormat ) {
	
		MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
		gfxDevice.BindVertexBuffer ();
		gfxDevice.BindVertexFormat ();
	}
}

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
/**	@lua	setVertexBuffer
	@text	Set the vertex buffer to render.
	
	@in		MOAIVertexArray self
	@in		MOAIGfxBuffer vertexBuffer
	@out	nil
*/
int MOAIVertexArray::_setVertexBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexArray, "U" )
	
	u32 baseParam = 2;
	u32 idx = 0;
	
	if ( state.IsType ( baseParam, LUA_TNUMBER )) {
		idx = state.GetValue < u32 >( baseParam++, 1 ) - 1;
	}
	
	MOAIVertexBuffer* buffer	= state.GetLuaObject < MOAIVertexBuffer >( baseParam++, false );
	MOAIVertexFormat* format	= state.GetLuaObject < MOAIVertexFormat >( baseParam++, false );
	
	self->SetVertexBuffer ( idx, buffer, format );
	self->FinishInit ();

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

	size_t totalVBOs = this->mVertexBuffers.Size ();
	for ( size_t i = 0; i < totalVBOs; ++i ) {
		this->mVertexBuffers [ i ].Bind ( this->mUseVAOs );
	}
}

//----------------------------------------------------------------//
MOAIVertexArray::MOAIVertexArray () :
	mUseVAOs ( false ),
	mNeedsFlush ( false ) {

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

	u32 vao = 0;

	if ( this->mUseVAOs && this->mNeedsFlush ) {
	
		if ( !this->mVAOs.Size ()) return;
	
		if ( this->mNeedsFlush ) {
			this->mCurrentVAO = ( this->mCurrentVAO + 1 ) % this->mVAOs.Size ();
		}
		vao = this->mVAOs [ this->mCurrentVAO ];
	}
	
	if ( vao ) {

		zglBindVertexArray ( vao );

		if ( this->mNeedsFlush ) {
			this->BindVertexArrayItems ();
			this->mNeedsFlush = false;
		}
	}
	else {
	
		this->BindVertexArrayItems ();
	}
}

//----------------------------------------------------------------//
bool MOAIVertexArray::OnGPUCreate () {

	this->mUseVAOs = false;
	
	size_t totalVAOs = this->mVAOs.Size ();

	if ( totalVAOs ) {
		
		for ( size_t i = 0; i < totalVAOs; ++i ) {
			u32 vao = zglCreateVertexArray (); // OK for this to return 0
			if ( !vao ) return true;
			this->mVAOs [ i ] = vao;
		}
		this->mUseVAOs = true;
	}
	return true;
}

//----------------------------------------------------------------//
void MOAIVertexArray::OnGPUDestroy () {
}

//----------------------------------------------------------------//
void MOAIVertexArray::OnGPULost () {

	this->mVAOs.Fill ( 0 );
}

//----------------------------------------------------------------//
void MOAIVertexArray::OnGPUUnbind () {

	if ( this->mUseVAOs ) {
		zglBindVertexArray ( 0 );
	}
	this->UnbindVertexArrayItems ();
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

	if ( MOAIGfxResourceMgr::IsValid ()) {
		for ( size_t i = 0; i < this->mVAOs.Size (); ++i ) {
			MOAIGfxResourceMgr::Get ().PushDeleter ( MOAIGfxDeleter::DELETE_BUFFER, this->mVAOs [ i ]);
		}
	}
	this->mVAOs.Init ( total );
	this->mVAOs.Fill ( 0 );
}

//----------------------------------------------------------------//
void MOAIVertexArray::ReserveVertexBuffers ( u32 total ) {

	for ( size_t i = 0; i < this->mVertexBuffers.Size (); ++i ) {
		this->mVertexBuffers [ i ].SetBufferAndFormat ( *this, 0, 0 );
	}
	this->mVertexBuffers.Init ( total );
}

//----------------------------------------------------------------//
void MOAIVertexArray::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {

	u32 totalVAOs = state.GetField < u32 >( -1, "mTotalVAOs", 0 );
	this->ReserveVAOs ( totalVAOs );
	
	u32 totalVertexBuffers = state.GetField < u32 >( -1, "mTotalVertexBuffers", 0 );
	this->ReserveVertexBuffers ( totalVertexBuffers );
	
	if ( state.GetFieldWithType ( -1, "mVertexBuffers", LUA_TTABLE )) {
		int itr = state.PushTableItr ( -1 );
		for ( u32 i = 0; state.TableItrNext ( itr ); ++i ) {
			if ( state.IsType ( -1, LUA_TTABLE )) {
				MOAIVertexBuffer* buffer = serializer.MemberIDToObject < MOAIVertexBuffer >( state.GetField < MOAISerializer::ObjID >( -1, "mBuffer", 0 ));
				MOAIVertexFormat* format = serializer.MemberIDToObject < MOAIVertexFormat >( state.GetField < MOAISerializer::ObjID >( -1, "mFormat", 0 ));
				this->SetVertexBuffer ( i, buffer, format );
			}
		}
		state.Pop ();
	}
	
	this->mNeedsFlush = true;
	this->FinishInit ();
}

//----------------------------------------------------------------//
void MOAIVertexArray::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

	state.SetField < u32 >( -1, "mTotalVAOs", this->mVAOs.Size ());
	state.SetField < u32 >( -1, "mTotalVertexBuffers", this->mVertexBuffers.Size ());
	
	lua_newtable ( state );
	for ( u32 i = 0; i < this->mVertexBuffers.Size (); ++i ) {
		state.Push ( i + 1 );
		lua_newtable ( state );
		state.SetField < MOAISerializer::ObjID >( -1, "mBuffer", serializer.AffirmMemberID ( this->mVertexBuffers [ i ].mBuffer ));
		state.SetField < MOAISerializer::ObjID >( -1, "mFormat", serializer.AffirmMemberID ( this->mVertexBuffers [ i ].mFormat ));
		lua_settable ( state, -3 );
	}
	lua_setfield ( state, -2, "mVertexBuffers" );
}

//----------------------------------------------------------------//
void MOAIVertexArray::SetVertexBuffer ( u32 idx, MOAIVertexBuffer* vtxBuffer, MOAIVertexFormat* vtxFormat ) {

	if ( this->AffirmVertexBuffers ( idx )) {
		this->mVertexBuffers [ idx ].SetBufferAndFormat ( *this, vtxBuffer, vtxFormat );
		this->mNeedsFlush = true;
	}
}

//----------------------------------------------------------------//
void MOAIVertexArray::UnbindVertexArrayItems () {

	size_t totalVBOs = this->mVertexBuffers.Size ();
	for ( size_t i = 0; i < totalVBOs; ++i ) {
		this->mVertexBuffers [ i ].Unbind ();
	}
}
