// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractGfxBufferGL.h>
#include <moai-gfx/MOAIGfxMgrGL.h>
#include <moai-gfx/MOAIShaderGL.h>
#include <moai-gfx/MOAIShaderMgrGL.h>
#include <moai-gfx/MOAITextureBaseGL.h>
#include <moai-gfx/MOAIVertexArrayGL.h>
#include <moai-gfx/MOAIVertexBufferGL.h>
#include <moai-gfx/MOAIVertexFormatGL.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexArrayGL::_reserveVAOs ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexArrayGL, "U" )
	
	self->ReserveVAOs ( state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexArrayGL::_reserveVertexBuffers ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexArrayGL, "U" )
	
	self->ReserveVertexBuffers ( state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexArrayGL::_setVertexBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexArrayGL, "U" )
	
	u32 baseParam = 2;
	ZLIndex idx = ZLIndexOp::ZERO;
	
	if ( state.IsType ( baseParam, LUA_TNUMBER )) {
		idx = ZLIndexCast ( state.GetValue < u32 >( baseParam++, 1 ) - 1 );
	}
	
	MOAIVertexBufferGL* buffer	= state.GetLuaObject < MOAIVertexBufferGL >( baseParam++, false );
	MOAIVertexFormatGL* format	= state.GetLuaObject < MOAIVertexFormatGL >( baseParam++, false );
	
	self->SetVertexBuffer ( idx, buffer, format );

	return 0;
}

//================================================================//
// MOAIVertexArrayGL
//================================================================//

//----------------------------------------------------------------//
MOAIVertexArrayGL::MOAIVertexArrayGL () {

	RTTI_BEGIN
		RTTI_EXTEND ( ZLVertexArrayGL )
		RTTI_EXTEND ( MOAIAbstractGfxResourceGL )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIVertexArrayGL::~MOAIVertexArrayGL () {

	this->ReserveVAOs ( 0 );
	this->ReserveVertexBuffers ( 0 );
}

//----------------------------------------------------------------//
void MOAIVertexArrayGL::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIGfxResourceGL < ZLVertexArrayGL >::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIVertexArrayGL::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIGfxResourceGL < ZLVertexArrayGL >::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "reserveVAOs",				_reserveVAOs },
		{ "reserveVertexBuffers",		_reserveVertexBuffers },
		{ "setVertexBuffer",			_setVertexBuffer },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIVertexArrayGL::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {

	ZLSize totalVAOs = state.GetFieldValue < cc8*, MOAILuaSize >( -1, "mTotalVAOs", 0 );
	this->ReserveVAOs ( totalVAOs );
	
	ZLSize totalVertexBuffers = state.GetFieldValue < cc8*, MOAILuaSize >( -1, "mTotalVertexBuffers", 0 );
	this->ReserveVertexBuffers ( totalVertexBuffers );
	
	if ( state.PushFieldWithType ( -1, "mVertexBuffers", LUA_TTABLE )) {
		int itr = state.PushTableItr ( -1 );
		for ( ZLIndex i = ZLIndexOp::ZERO; state.TableItrNext ( itr ); ++i ) {
			if ( state.IsType ( -1, LUA_TTABLE )) {
				MOAIVertexBufferGL* buffer = serializer.MemberIDToObject < MOAIVertexBufferGL >( state.GetFieldValue < cc8*, MOAISerializer::ObjID >( -1, "mBuffer", 0 ));
				MOAIVertexFormatGL* format = serializer.MemberIDToObject < MOAIVertexFormatGL >( state.GetFieldValue < cc8*, MOAISerializer::ObjID >( -1, "mFormat", 0 ));
				this->SetVertexBuffer ( i, buffer, format );
			}
		}
		state.Pop ();
	}
	this->FinishInit ();
}

//----------------------------------------------------------------//
void MOAIVertexArrayGL::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

	state.SetField < cc8*, MOAILuaSize >( -1, "mTotalVAOs", this->mVAOs.Size ());
	state.SetField < cc8*, MOAILuaSize >( -1, "mTotalVertexBuffers", this->mVertexBuffers.Size ());
	
	lua_newtable ( state );
	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mVertexBuffers.Size (); ++i ) {
		state.Push ( MOAILuaIndex ( i ));
		lua_newtable ( state );
		
		MOAIVertexBufferGL* buffer = this->mVertexBuffers [ i ].mBuffer->AsType < MOAIVertexBufferGL >();
		MOAIVertexFormatGL* format = this->mVertexBuffers [ i ].mFormat->AsType < MOAIVertexFormatGL >();
		
		assert ( buffer && format );
		
		state.SetField < cc8*, MOAISerializer::ObjID >( -1, "mBuffer", serializer.AffirmMemberID ( buffer ));
		state.SetField < cc8*, MOAISerializer::ObjID >( -1, "mFormat", serializer.AffirmMemberID ( format ));
		lua_settable ( state, -3 );
	}
	lua_setfield ( state, -2, "mVertexBuffers" );
}
