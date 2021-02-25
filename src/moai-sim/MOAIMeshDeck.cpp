// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <tesselator.h>

#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIMeshDeck.h>

//================================================================//
// MOAIMeshCallable
//================================================================//
class MOAIMeshCallable :
	public MOAIAbstractGfxScriptCallback {
public:

	MOAIMesh*					mMesh;
	MOAIMeshSpan*				mSpan;
	u32							mTotalElements;

	//----------------------------------------------------------------//
	void MOAIAbstractGfxScriptCallback_Call () {
	
		MOAIMeshSpan* span = this->mSpan;
	
		for ( ; span; span = span->mNext ) {
			if ( span->mBase == span->mTop ) continue;
			assert (( span->mBase < span->mTop ) && ( span->mTop <= this->mTotalElements ));
			this->mMesh->DrawPrims ( span->mBase, ( u32 )( span->mTop - span->mBase ));
		}
	}
};

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIMeshDeck::_setMesh ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMeshDeck, "U" )
	
	self->mMesh = state.GetLuaObject < MOAIMesh >( 2, false );
		
	MOAI_LUA_RETURN_SELF
}

//================================================================//
// MOAIMeshDeck
//================================================================//

//----------------------------------------------------------------//
void MOAIMeshDeck::DrawIndex ( ZLIndex idx, MOAIMeshSpan* span ) {
	UNUSED ( idx );

	if ( !this->mMesh ) return;
	u32 totalElements = this->mMesh->GetTotalElements ();

	MOAIGfxScript* gfxScript = this->GetGfxScript ();
	if ( !gfxScript ) return;

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	gfxMgr.SetShader ( MOAIShaderPresetEnum::MESH_SHADER );
	gfxMgr.SetMesh ( this->mMesh );

	MOAIMeshSpan defaultSpan;
	if ( !span ) {
		defaultSpan.mBase 		= 0;
		defaultSpan.mTop 		= totalElements;
		defaultSpan.mNext 		= 0;
		span = &defaultSpan;
	}
	
	MOAIMeshCallable callable;
	callable.mMesh				= this->mMesh;
	callable.mSpan	 			= span;
	callable.mTotalElements		= totalElements;
	gfxScript->ExecuteBytecode ( &callable );
	
	callable.Flush ();
}

//----------------------------------------------------------------//
MOAIMeshDeck::MOAIMeshDeck () {

	RTTI_BEGIN ( MOAIMeshDeck )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIMeshDeck >)
		RTTI_EXTEND ( MOAIDeck )
		RTTI_EXTEND ( MOAIHasGfxScript )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIMeshDeck::~MOAIMeshDeck () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIMeshDeck::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIMeshDeck::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "setMesh",					_setMesh },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
ZLBounds MOAIMeshDeck::MOAIDeck_GetBounds () {

	return this->mMesh ? this->mMesh->GetBounds () : ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
ZLBounds MOAIMeshDeck::MOAIDeck_GetBounds ( ZLIndex idx ) {
	UNUSED ( idx );

	return this->mMesh ? this->mMesh->GetBounds () : ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
void MOAIMeshDeck::MOAIDeck_Render ( ZLIndex idx, MOAIRenderPhaseEnum::_ renderPhase ) {

	this->DrawIndex ( idx, 0 );
}
