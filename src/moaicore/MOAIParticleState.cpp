// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIContentLibrary2D.h>
#include <moaicore/MOAIParticleEngine.h>
#include <moaicore/MOAIParticleForce.h>
#include <moaicore/MOAIParticleScript.h>
#include <moaicore/MOAIParticleState.h>
#include <moaicore/MOAITexture.h>

class MOAIData;

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleState::_clearForces ( lua_State* L ) {
	LUA_SETUP ( MOAIParticleState, "U" )
	
	self->ClearForces ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleState::_init ( lua_State* L ) {
	LUA_SETUP ( MOAIParticleState, "UUUU" )
	
	MOAIParticleScript* init		= state.GetLuaData < MOAIParticleScript >( 2 );
	MOAIParticleScript* update		= state.GetLuaData < MOAIParticleScript >( 3 );
	MOAIParticleScript* render		= state.GetLuaData < MOAIParticleScript >( 4 );

	if ( init ) {
		init->Compile ();
	}

	if ( update ) {
		update->Compile ();
	}
	
	if ( render ) {
		render->Compile ();
	}

	self->mInit		= init;
	self->mUpdate	= update;
	self->mRender	= render;

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleState::_pushForce ( lua_State* L ) {
	LUA_SETUP ( MOAIParticleState, "UU" )
	
	MOAIParticleForce* force = state.GetLuaData < MOAIParticleForce >( 2 );
	if ( force ) {
		self->PushForce ( *force );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleState::_setNext ( lua_State* L ) {
	LUA_SETUP ( MOAIParticleState, "UU" )
	
	self->mNext = state.GetLuaData < MOAIParticleState >( 2 );
	
	return 0;
}

//================================================================//
// MOAIParticleState
//================================================================//

//----------------------------------------------------------------//
void MOAIParticleState::ClearForces () {

	while ( this->mForces.Count ()) {
	
		ForceNode* forceNode = this->mForces.Head ();
		this->mForces.PopFront ();
		
		forceNode->Data ()->Release ();
		delete forceNode;
	}
}

//----------------------------------------------------------------//
USVec2D MOAIParticleState::GetAcceleration ( const USVec2D& loc ) {

	USVec2D acceleration;
	USVec2D forceAcceleration;
	
	acceleration.Init ( 0.0f, 0.0f );
	
	ForceNode* forceNode = this->mForces.Head ();
	for ( ; forceNode; forceNode = forceNode->Next ()) {
		MOAIParticleForce* force = forceNode->Data ();
		
		forceAcceleration = force->GetAcceleration ( loc );
		acceleration.Add ( forceAcceleration );
	}
	return acceleration;
}

//----------------------------------------------------------------//
MOAIParticleState::MOAIParticleState () {

	RTTI_BEGIN
		RTTI_EXTEND ( USLuaData )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIParticleState::~MOAIParticleState () {

	this->ClearForces ();
}

//----------------------------------------------------------------//
void MOAIParticleState::PushForce ( MOAIParticleForce& force ) {

	force.Retain ();

	ForceNode* forceNode = new ForceNode ();
	forceNode->Data ( &force );
	
	this->mForces.PushBack ( *forceNode );
}

//----------------------------------------------------------------//
void MOAIParticleState::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIParticleState::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "clearForces",		_clearForces },
		{ "init",				_init },
		{ "pushForce",			_pushForce },
		{ "setNext",			_setNext },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
