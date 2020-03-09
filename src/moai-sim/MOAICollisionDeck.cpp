// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICollisionDeck.h>
#include <moai-sim/MOAICollisionShape.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAICollisionDeck::_reserveShapes ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICollisionDeck, "U" )
	
	self->ReserveShapes ( state.GetValue < u32 >( 2, 0 ));
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAICollisionDeck::_setBox ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICollisionDeck, "U" )
	
	ZLIndex idx = state.GetValue < MOAILuaIndex >( 2, 0 );
	ZLBox box = state.GetBox ( 3 );
	
	self->SetBox ( idx, box );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAICollisionDeck::_setRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICollisionDeck, "U" )
	
	ZLIndex idx = state.GetValue < MOAILuaIndex >( 2, 0 );
	ZLRect rect = state.GetRect < float >( 3 );
	
	self->SetRect ( idx, rect );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAICollisionDeck::_setQuad ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICollisionDeck, "U" )
	
	ZLIndex idx = state.GetValue < MOAILuaIndex >( 2, 0 );
	
	ZLQuad quad;
	
	quad.mV [ 0 ].mX = state.GetValue < float >( 3, 0.0f );
	quad.mV [ 0 ].mY = state.GetValue < float >( 4, 0.0f );
	quad.mV [ 1 ].mX = state.GetValue < float >( 5, 0.0f );
	quad.mV [ 1 ].mY = state.GetValue < float >( 6, 0.0f );
	quad.mV [ 2 ].mX = state.GetValue < float >( 7, 0.0f );
	quad.mV [ 2 ].mY = state.GetValue < float >( 8, 0.0f );
	quad.mV [ 3 ].mX = state.GetValue < float >( 9, 0.0f );
	quad.mV [ 3 ].mY = state.GetValue < float >( 10, 0.0f );
	
	self->SetQuad ( idx, quad );
	
	return 0;
}

//================================================================//
// MOAICollisionDeck
//================================================================//

//----------------------------------------------------------------//
MOAICollisionShape& MOAICollisionDeck::AffirmShape ( ZLIndex idx ) {

	assert ( this->mShapes.CheckIndex ( idx ));

	MOAICollisionShape* shape = this->mShapes [ idx ];

	if ( !shape ) {
		shape = new MOAICollisionShape ();
		this->mShapes [ idx ] = shape;
	}
	
	assert ( shape );
	
	return *shape;
}

//----------------------------------------------------------------//
MOAICollisionDeck::MOAICollisionDeck () {

	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAICollisionDeck )
		RTTI_EXTEND ( MOAIDeck )
	RTTI_END
}

//----------------------------------------------------------------//
MOAICollisionDeck::~MOAICollisionDeck () {
}

//----------------------------------------------------------------//
void MOAICollisionDeck::ReserveShapes ( u32 totalShapes ) {
	
	this->mShapes.Resize ( totalShapes );
}

//----------------------------------------------------------------//
void MOAICollisionDeck::SetBox ( ZLIndex idx, const ZLBox& box ) {

	if ( this->mShapes.CheckIndex ( idx )) {
		this->AffirmShape ( idx ).Set ( 0, box );
	}
}

//----------------------------------------------------------------//
void MOAICollisionDeck::SetRect ( ZLIndex idx, const ZLRect& rect ) {

	if ( this->mShapes.CheckIndex ( idx )) {
		this->AffirmShape ( idx ).Set ( 0, rect );
	}
}

//----------------------------------------------------------------//
void MOAICollisionDeck::SetQuad ( ZLIndex idx, const ZLQuad& quad ) {

	if ( this->mShapes.CheckIndex ( idx )) {
		this->AffirmShape ( idx ).Set ( 0, quad );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
ZLBounds MOAICollisionDeck::MOAIDeck_ComputeMaxAABB () {

	return this->MOAIDeck::GetBounds ( 0 );
}

//----------------------------------------------------------------//
void MOAICollisionDeck::MOAIDeck_Draw ( ZLIndex idx ) {
	UNUSED ( idx );
	
//	u32 size = ( u32 )this->mQuads.Size ();
//	if ( size ) {
//
//		idx = idx - 1;
//		u32 itemIdx = idx % size;
//
//		if ( !this->LoadGfxState ( materials, this->mMaterialIDs [ itemIdx ], idx, MOAIShaderPresetEnum::DECK2D_SHADER )) return;
//
//		MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
//		MOAIQuadBrush::BindVertexFormat ();
//		
//		gfxState.SetVertexTransform ( MOAIGfxMgr::MODEL_TO_CLIP_MTX );
//		gfxState.SetUVTransform ( MOAIGfxMgr::UV_TO_MODEL_MTX );
//		
//		this->mQuads [ itemIdx ].Draw ( offset.mX, offset.mY, offset.mZ, scale.mX, scale.mY  );
//	}
}

//----------------------------------------------------------------//
ZLBounds MOAICollisionDeck::MOAIDeck_GetBounds ( ZLIndex idx ) {
	
	assert ( this->mShapes.CheckIndex ( idx ));

	return this->mShapes [ idx ]->GetBounds ();
}

//----------------------------------------------------------------//
MOAICollisionShape* MOAICollisionDeck::MOAIDeck_GetCollisionShape ( ZLIndex idx ) {

	if ( this->mShapes.CheckIndex ( idx )) {
		return this->mShapes [ idx ];
	}
	return 0;
}

//----------------------------------------------------------------//
bool MOAICollisionDeck::MOAIDeck_Overlap ( ZLIndex idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result ) {
	UNUSED ( idx );
	UNUSED ( vec );
	UNUSED ( granularity );
	UNUSED ( result );

	//	u32 size = ( u32 )this->mQuads.Size ();
	//	if ( size ) {
	//		idx = ( idx - 1 ) % size;
	//		const MOAIQuadBrush& quadBrush = this->mQuads [ idx ];
	//		return this->TestHit ( materials, idx, granularity, quadBrush.mModelQuad, quadBrush.mUVQuad, vec.mX, vec.mY );
	//	}
	//	return false;

	return false;
}

//----------------------------------------------------------------//
bool MOAICollisionDeck::MOAIDeck_Overlap ( ZLIndex idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result ) {
	UNUSED ( idx );
	UNUSED ( vec );
	UNUSED ( granularity );
	UNUSED ( result );

	//return this->TestHit ( materials, idx, granularity, this->mQuad.mModelQuad, this->mQuad.mUVQuad, vec.mX, vec.mY );
	return false;
}

//----------------------------------------------------------------//
void MOAICollisionDeck::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAIDeck, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAICollisionDeck::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAIDeck, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	
	luaL_Reg regTable [] = {
		{ "reserveShapes",		_reserveShapes },
		{ "setBox",				_setBox },
		{ "setRect",			_setRect },
		{ "setQuad",			_setQuad },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
