// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIGfxConstsGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL.h>
#include <moai-gfx-gl/MOAIIndexBufferGL.h>
#include <moai-gfx-gl/MOAIMeshGL.h>
#include <moai-gfx-gl/MOAIVertexBufferGL.h>
#include <moai-gfx-gl/MOAIVertexFormatGL.h>

//================================================================//
// MOAIVertexBufferWithFormatGL
//================================================================//

//----------------------------------------------------------------//
MOAIVertexBufferWithFormatGL::MOAIVertexBufferWithFormatGL () {
}


//----------------------------------------------------------------//
MOAIVertexBufferWithFormatGL::~MOAIVertexBufferWithFormatGL () {
}

//----------------------------------------------------------------//
void MOAIVertexBufferWithFormatGL::SetBufferAndFormat ( MOAIVertexBufferGL* buffer, MOAIVertexFormatGL* format ) {

	this->mBuffer = buffer;
	this->mFormat = format;
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMeshGL::_reserveVertexBuffers ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMeshGL, "U" )
	
	self->ReserveVertexBuffers ( state.GetValue < u32 >( 2, 0 ));
	
	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMeshGL::_setIndexBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMeshGL, "U" )
	
	MOAIIndexBufferGL* buffer = state.GetLuaObject < MOAIIndexBufferGL >( 2, false );
	
	self->SetIndexBuffer ( buffer );

	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMeshGL::_setVertexBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMeshGL, "U" )
	
	u32 baseParam = 2;
	ZLIndex idx = 0;
	
	if ( state.IsType ( baseParam, LUA_TNUMBER )) {
		idx = state.GetValue < u32 >( baseParam++, 1 ) - 1;
	}
	
	MOAIVertexBufferGL* buffer	= state.GetLuaObject < MOAIVertexBufferGL >( baseParam++, false );
	MOAIVertexFormatGL* format	= state.GetLuaObject < MOAIVertexFormatGL >( baseParam++, false );
	
	self->SetVertexBuffer ( idx, buffer, format );

	MOAI_LUA_RETURN_SELF
}

//================================================================//
// MOAIMeshGL
//================================================================//

//----------------------------------------------------------------//
bool MOAIMeshGL::AffirmVertexBuffers ( u32 idx ) {

	if (( this->mVertexBuffers.Size () == 0 ) && ( idx == 0 )) {
		this->ReserveVertexBuffers ( 1 );
	}
	return ( idx < this->mVertexBuffers.Size ());
}

//----------------------------------------------------------------//
void MOAIMeshGL::BindVertexArrayItems () {

	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();

	ZLSize totalVBOs = this->mVertexBuffers.Size ();
	for ( ZLIndex i = 0; i < totalVBOs; ++i ) {
	
		MOAIVertexBufferGL* buffer = this->mVertexBuffers [ i ].mBuffer;
		MOAIVertexFormatGL* format = this->mVertexBuffers [ i ].mFormat;
		
		assert ( buffer && format );
		
		buffer->Bind ();
		format->Bind ( gfx, NULL );
		buffer->Unbind ();
	}
	
	if ( this->mIndexBuffer ) {
		this->mIndexBuffer->Bind ();
	}
}

//----------------------------------------------------------------//
MOAIMeshGL::MOAIMeshGL () {
	
	RTTI_BEGIN ( MOAIMeshGL )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIMeshGL >)
		RTTI_VISITOR ( MOAIAbstractLuaSerializationVisitor, MOAILuaSerializationVisitor < MOAIMeshGL >)
		RTTI_EXTEND ( MOAIVertexArray )
		RTTI_EXTEND ( MOAIGfxResourceGL )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIMeshGL::~MOAIMeshGL () {

	this->ScheduleForGPUDestroy ();
}

//----------------------------------------------------------------//
void MOAIMeshGL::UnbindVertexArrayItems () {

	size_t totalVBOs = this->mVertexBuffers.Size ();
	for ( ZLIndex i = 0; i < totalVBOs; ++i ) {
	
		MOAIVertexFormatGL* format = this->mVertexBuffers [ i ].mFormat;
		assert ( format );
		format->Unbind ( this->mGfxMgr->GetDrawingAPI ());
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIMeshGL::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIMeshGL::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "reserveVertexBuffers",		_reserveVertexBuffers },
		{ "setIndexBuffer",				_setIndexBuffer },
		{ "setVertexBuffer",			_setVertexBuffer },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIMeshGL::_SerializeIn ( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer ) {
	if ( history.Visit ( *this )) return;
	
	this->mIndexBuffer = serializer.MemberIDToObject < MOAIIndexBufferGL >( state.GetFieldValue < cc8*, MOAISerializer::ObjID >( -1, "mIndexBuffer", 0 ));
	
	ZLSize totalVertexBuffers = state.GetFieldValue < cc8*, MOAILuaSize >( -1, "mTotalVertexBuffers", 0 );
	this->ReserveVertexBuffers ( totalVertexBuffers );
	
	if ( state.PushFieldWithType ( -1, "mVertexBuffers", LUA_TTABLE )) {
		int itr = state.PushTableItr ( -1 );
		for ( ZLIndex i = 0; state.TableItrNext ( itr ); ++i ) {
			if ( state.IsType ( -1, LUA_TTABLE )) {
				MOAIVertexBufferGL* buffer = serializer.MemberIDToObject < MOAIVertexBufferGL >( state.GetFieldValue < cc8*, MOAISerializer::ObjID >( -1, "mBuffer", 0 ));
				MOAIVertexFormatGL* format = serializer.MemberIDToObject < MOAIVertexFormatGL >( state.GetFieldValue < cc8*, MOAISerializer::ObjID >( -1, "mFormat", 0 ));
				this->SetVertexBuffer ( i, buffer, format );
			}
		}
		state.Pop ();
	}
	
	this->ScheduleForGPUUpdate ();
}

//----------------------------------------------------------------//
void MOAIMeshGL::_SerializeOut ( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer ) {
	if ( history.Visit ( *this )) return;

	state.SetField < cc8*, MOAISerializer::ObjID >( -1, "mIndexBuffer", serializer.AffirmMemberID ( this->mIndexBuffer ));

	state.SetField < cc8*, MOAILuaSize >( -1, "mTotalVertexBuffers", this->mVertexBuffers.Size ());
	
	lua_newtable ( state );
	for ( ZLIndex i = 0; i < this->mVertexBuffers.Size (); ++i ) {
		state.Push ( MOAILuaIndex ( i ));
		lua_newtable ( state );
		
		MOAIVertexBufferGL* buffer = this->mVertexBuffers [ i ].mBuffer;
		MOAIVertexFormatGL* format = this->mVertexBuffers [ i ].mFormat;
		
		assert ( buffer && format );
		
		state.SetField < cc8*, MOAISerializer::ObjID >( -1, "mBuffer", serializer.AffirmMemberID ( buffer ));
		state.SetField < cc8*, MOAISerializer::ObjID >( -1, "mFormat", serializer.AffirmMemberID ( format ));
		lua_settable ( state, -3 );
	}
	lua_setfield ( state, -2, "mVertexBuffers" );
}

//----------------------------------------------------------------//
void MOAIMeshGL::MOAIAbstractMesh_DrawPrims ( MOAIGfxTopologyEnum::_ primType, u32 base, u32 count ) {
	
	this->mGfxMgr->ApplyStateChanges ();

	ZLGfxEnum::_ primTypeZGL = MOAIGfxConstsGL::Remap ( primType );

	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();

	if ( this->mIndexBuffer ) {

		size_t indexSize = this->mIndexBuffer->GetIndexSize ();
		ZLGfxEnum::_ indexType = indexSize == 2 ? ZLGfxEnum::TYPE_UNSIGNED_SHORT : ZLGfxEnum::TYPE_UNSIGNED_INT;
		gfx.DrawElements ( primTypeZGL, count, indexType, NULL, base * indexSize );
	}
	else {
		gfx.DrawArrays ( primTypeZGL, base, count );
	}
}

//----------------------------------------------------------------//
MOAIIndexBuffer* MOAIMeshGL::MOAIAbstractMesh_GetIndexBuffer () {

	return this->mIndexBuffer;
}

//----------------------------------------------------------------//
MOAIVertexBuffer* MOAIMeshGL::MOAIAbstractMesh_GetVertexBuffer ( ZLIndex idx ) {

	return idx < this->mVertexBuffers.Size () ? ( MOAIVertexBufferGL* )this->mVertexBuffers [ idx ].mBuffer : ( MOAIVertexBufferGL* )0;
}

//----------------------------------------------------------------//
MOAIVertexFormat* MOAIMeshGL::MOAIAbstractMesh_GetVertexFormat ( ZLIndex idx ) {

	return idx < this->mVertexBuffers.Size () ? ( MOAIVertexFormatGL* )this->mVertexBuffers [ idx ].mFormat : ( MOAIVertexFormatGL* )0;
}

//----------------------------------------------------------------//
void MOAIMeshGL::MOAIAbstractMesh_ReserveVertexBuffers ( ZLSize size ) {

	for ( ZLIndex i = 0; i < this->mVertexBuffers.Size (); ++i ) {
		this->mVertexBuffers [ i ].SetBufferAndFormat ( 0, 0 );
	}
	this->mVertexBuffers.Init ( size );
	
	this->ScheduleForGPUUpdate ();
}

//----------------------------------------------------------------//
void MOAIMeshGL::MOAIAbstractMesh_SetIndexBuffer ( MOAIIndexBuffer* idxBuffer ) {

	this->mIndexBuffer = MOAICastAssert < MOAIIndexBufferGL >( idxBuffer );
	this->ScheduleForGPUUpdate ();
}

//----------------------------------------------------------------//
void MOAIMeshGL::MOAIAbstractMesh_SetVertexBuffer ( ZLIndex idx, MOAIVertexBuffer* vtxBuffer, MOAIVertexFormat* vtxFormat ) {

	MOAIVertexBufferGL* vertexBufferGL = MOAICastAssert < MOAIVertexBufferGL >( vtxBuffer );
	MOAIVertexFormatGL* vertexFormatGL = MOAICastAssert < MOAIVertexFormatGL >( vtxFormat );

	if ( this->AffirmVertexBuffers ( idx )) {
	
		MOAIVertexBufferWithFormatGL& bufferWithFormat = this->mVertexBuffers [ idx ];
		
		if (( bufferWithFormat.mBuffer != vtxBuffer ) || ( bufferWithFormat.mFormat != vtxFormat )) {
			bufferWithFormat.SetBufferAndFormat ( vertexBufferGL, vertexFormatGL );
			this->ScheduleForGPUUpdate ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIMeshGL::MOAIGfxResourceGL_OnGPUBind () {

	this->mGfxMgr->GetDrawingAPI ().BindVertexArray ( this->mVAO );
	this->BindVertexArrayItems ();
}

//----------------------------------------------------------------//
bool MOAIMeshGL::MOAIGfxResourceGL_OnGPUCreate () {

	this->mVAO = this->mGfxMgr->GetDrawingAPI ().CreateVertexArray ();
	this->MOAIGfxResourceGL_OnGPUUpdate ();
	
	return true;
}

//----------------------------------------------------------------//
void MOAIMeshGL::MOAIGfxResourceGL_OnGPUDeleteOrDiscard ( bool shouldDelete ) {

	this->mGfxMgr->DeleteOrDiscard ( this->mVAO, shouldDelete );
}

//----------------------------------------------------------------//
void MOAIMeshGL::MOAIGfxResourceGL_OnGPUUnbind () {

	this->mGfxMgr->GetDrawingAPI ().BindVertexArray ( ZLGfxResource::UNBIND );
}

//----------------------------------------------------------------//
bool MOAIMeshGL::MOAIGfxResourceGL_OnGPUUpdate () {

	const ZLGfxHandle& vao = this->mVAO;
	
	if ( vao.CanBind ()) {
		ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
		gfx.BindVertexArray ( vao );
		this->BindVertexArrayItems ();
		gfx.BindVertexArray ( ZLGfxResource::UNBIND );
		return true;
	}
	return false;
}
