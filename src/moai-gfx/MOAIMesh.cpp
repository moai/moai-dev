// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIMesh.h>
#include <moai-gfx/MOAIIndexBuffer.h>
#include <moai-gfx/MOAIVertexBuffer.h>
#include <moai-gfx/MOAIVertexFormat.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_getIndexBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	state.Push ( self->MOAIMesh_GetIndexBuffer ());
	
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_getVertexBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	ZLIndex index = state.GetValue < MOAILuaIndex >( 2, 0 );
	
	state.Push ( self->MOAIMesh_GetVertexBuffer ( index ));
	state.Push ( self->MOAIMesh_GetVertexFormat ( index ));
	
	return 2;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_setBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	self->ClearBounds ();
	
	if ( state.CheckParams ( 2, "NNNNNN-" )) {
		self->SetBounds ( state.GetValue < ZLBox >( 2, ZLBox::EMPTY ));
	}
	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_setIndexBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	MOAIIndexBuffer* indexBuffer = state.GetLuaObject < MOAIIndexBuffer >( 2, false );
	
	self->SetIndexBuffer ( indexBuffer );
	
	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
/**	@lua	setPenWidth
	@text	Sets the pen with for drawing prims in this vertex buffer.
			Only valid with prim types GL_LINES, GL_LINE_LOOP, GL_LINE_STRIP.
	
	@in		MOAIMesh self
	@in		number penWidth
	@out	nil
*/
int MOAIMesh::_setPenWidth ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "UN" )
	
	self->mPenWidth = state.GetValue < float >( 2, 1.0f );
	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
/**	@lua	setPrimType
	@text	Sets the prim type the buffer represents.
	
	@in		MOAIMesh self
	@in		number primType		One of MOAIMesh GL_POINTS, GL_LINES, GL_TRIANGLES, GL_LINE_LOOP,
								GL_LINE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLE_STRIP
	@out	nil
*/
int MOAIMesh::_setPrimType ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "UN" )
	
	self->SetPrimType (( MOAIGfxTopologyEnum::_ )state.GetValue < u32 >( 2, 0 ));
	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_setTotalElements ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )

	self->SetTotalElements ( state.GetValue < u32 >( 2, 0 ));
	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_setVertexBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	ZLIndex index = 0;
	
	MOAIVertexBuffer* vertexBuffer = state.GetLuaObject < MOAIVertexBuffer >( 2, false );
	MOAIVertexFormat* vertexFormat = state.GetLuaObject < MOAIVertexFormat >( 3, false );
	
	self->SetVertexBuffer ( 0, vertexBuffer, vertexFormat );
	
	MOAI_LUA_RETURN_SELF
}

//================================================================//
// MOAIMesh
//================================================================//

//----------------------------------------------------------------//
void MOAIMesh::ClearBounds () {

	this->mBounds = ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
u32 MOAIMesh::CountPrims () const {

	return 0;
}

//----------------------------------------------------------------//
void MOAIMesh::DrawPrims ( u32 base, u32 count ) {

	this->MOAIMesh_DrawPrims ( this->mPrimType, base, count );
}

//----------------------------------------------------------------//
MOAIIndexBuffer* MOAIMesh::GetIndexBuffer () {

	return this->MOAIMesh_GetIndexBuffer ();
}

//----------------------------------------------------------------//
MOAIVertexBuffer* MOAIMesh::GetVertexBuffer ( ZLIndex idx ) {

	return this->MOAIMesh_GetVertexBuffer ( idx );
}

//----------------------------------------------------------------//
MOAIVertexFormat* MOAIMesh::GetVertexFormat ( ZLIndex idx ) {

	return this->MOAIMesh_GetVertexFormat ( idx );
}

//----------------------------------------------------------------//
MOAIMesh::MOAIMesh () :
	mTotalElements ( 0 ),
	mPrimType ( MOAIGfxTopologyEnum::TRIANGLE_LIST ),
	mPenWidth ( 1.0f ) {

	RTTI_BEGIN ( MOAIMesh )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIMesh >)
		RTTI_VISITOR ( MOAIAbstractLuaSerializationVisitor, MOAILuaSerializationVisitor < MOAIMesh >)
		RTTI_EXTEND ( MOAIGfxResource )
	RTTI_END

	this->ClearBounds ();
}

//----------------------------------------------------------------//
MOAIMesh::~MOAIMesh () {
}

//----------------------------------------------------------------//
void MOAIMesh::ReserveVertexBuffers ( ZLSize size ) {

	this->MOAIMesh_ReserveVertexBuffers ( size );
}

//----------------------------------------------------------------//
void MOAIMesh::SetIndexBuffer ( MOAIIndexBuffer* indexBuffer ) {

	this->MOAIMesh_SetIndexBuffer ( indexBuffer );
}

//----------------------------------------------------------------//
void MOAIMesh::SetVertexBuffer ( ZLIndex idx, MOAIVertexBuffer* vtxBuffer, MOAIVertexFormat* vtxFormat ) {

	this->MOAIMesh_SetVertexBuffer ( idx, vtxBuffer, vtxFormat );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIMesh::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
	
	state.SetField ( -1, "GL_LINES",			( u32 )MOAIGfxTopologyEnum::LINE_LIST );
	state.SetField ( -1, "GL_LINE_LOOP",		( u32 )MOAIGfxTopologyEnum::LINE_LOOP );
	state.SetField ( -1, "GL_LINE_STRIP",		( u32 )MOAIGfxTopologyEnum::LINE_STRIP );
	state.SetField ( -1, "GL_POINTS",			( u32 )MOAIGfxTopologyEnum::POINT_LIST );
	state.SetField ( -1, "GL_TRIANGLE_FAN",		( u32 )MOAIGfxTopologyEnum::TRIANGLE_FAN );
	state.SetField ( -1, "GL_TRIANGLES",		( u32 )MOAIGfxTopologyEnum::TRIANGLE_LIST );
	state.SetField ( -1, "GL_TRIANGLE_STRIP",	( u32 )MOAIGfxTopologyEnum::TRIANGLE_STRIP );
}

//----------------------------------------------------------------//
void MOAIMesh::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getIndexBuffer",				_getIndexBuffer },
		{ "getVertexBuffer",			_getVertexBuffer },
		{ "setBounds",					_setBounds },
		{ "setIndexBuffer",				_setIndexBuffer },
		{ "setPenWidth",				_setPenWidth },
		{ "setPrimType",				_setPrimType },
		{ "setTotalElements",			_setTotalElements },
		{ "setVertexBuffer",			_setVertexBuffer },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIMesh::_SerializeIn ( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer ) {
	if ( history.Visit ( *this )) return;
	
	this->mTotalElements = state.GetFieldValue < cc8*, u32 >( -1, "mTotalElements", 0 );
	
	if ( state.PushFieldWithType ( -1, "mBounds", LUA_TTABLE )) {
		
		ZLBox aabb;
		
		aabb.mMin.mX	= state.GetFieldValue < cc8*, float >( -1, "mMinX", 0 );
		aabb.mMin.mY	= state.GetFieldValue < cc8*, float >( -1, "mMinY", 0 );
		aabb.mMin.mZ	= state.GetFieldValue < cc8*, float >( -1, "mMinZ", 0 );
		
		aabb.mMax.mX	= state.GetFieldValue < cc8*, float >( -1, "mMaxX", 0 );
		aabb.mMax.mY	= state.GetFieldValue < cc8*, float >( -1, "mMaxY", 0 );
		aabb.mMax.mZ	= state.GetFieldValue < cc8*, float >( -1, "mMaxZ", 0 );
		
		this->mBounds.Init ( aabb );
		
		state.Pop ();
	}
	
	this->mPenWidth = state.GetFieldValue < cc8*, float >( -1, "mPenWidth", 0 );
}

//----------------------------------------------------------------//
void MOAIMesh::_SerializeOut ( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer ) {
	if ( history.Visit ( *this )) return;
	
	state.SetField < cc8*, u32 >( -1, "mTotalElements", this->mTotalElements );
	
	lua_newtable ( state );
	
		state.SetField < cc8*, float >( -1, "mMinX", this->mBounds.mAABB.mMin.mX );
		state.SetField < cc8*, float >( -1, "mMinY", this->mBounds.mAABB.mMin.mY );
		state.SetField < cc8*, float >( -1, "mMinZ", this->mBounds.mAABB.mMin.mZ );
	
		state.SetField < cc8*, float >( -1, "mMaxX", this->mBounds.mAABB.mMax.mX );
		state.SetField < cc8*, float >( -1, "mMaxY", this->mBounds.mAABB.mMax.mY );
		state.SetField < cc8*, float >( -1, "mMaxZ", this->mBounds.mAABB.mMax.mZ );
	
	lua_setfield ( state, -2, "mBounds" );
	
	state.SetField < cc8*, u32 >( -1, "mPenWidth", ( u32 )this->mPenWidth );
}
