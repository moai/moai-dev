// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractGfxScriptCallback.h>
#include <moai-gfx/MOAIDrawingAPI.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIGfxScript.h>
#include <moai-gfx/MOAIShader.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxScript::_affirmBytecode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxScript, "U" )
	self->AffirmBytecode ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxScript::_call ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxScript, "U" )
	self->Call ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxScript::_callFromShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxScript, "U" )
	self->CallFromShader ();
	return 0;
}

//================================================================//
// MOAIGfxScript
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxScript::AffirmBytecode () {

	ZLSize size = this->mCommandStream.GetCursor ();
	if ( size > 0 ) {
		this->mBytecode.Init ( size );
		this->mCommandStream.Seek ( 0 );
		this->mCommandStream.ReadBytes ( this->mBytecode.GetBuffer (), size );
		this->mCommandStream.Clear ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxScript::Call () {

	this->SubmitCommand (( MOAIDrawingAPIEnum::_ )CALL );
}

//----------------------------------------------------------------//
void MOAIGfxScript::CallFromShader() {

	this->SubmitCommand (( MOAIDrawingAPIEnum::_ )CALL_FROM_SHADER );
}

//----------------------------------------------------------------//
void MOAIGfxScript::Execute ( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ cmd, const void* rawParam ) const {

	switch ( cmd ) {

		case CALL: {
			if ( callable ) {
				callable->Call ();
			}
			return;
		}
		
		case CALL_FROM_SHADER: {
		
			if ( callable ) {
				
				MOAIShader* shader = MOAIGfxMgr::Get ().GetShader ();
				MOAIGfxScript* gfxScript = shader ? shader->GetGfxScript () : NULL;
		
				if ( gfxScript ) {
					gfxScript->RunScript ( callable, CALL );
				}
				else {
					callable->Call ();
				}
			}
			return;
		}
		
		default:
			MOAIDrawingAPI::Execute ( callable, cmd, rawParam );
	}
}

//----------------------------------------------------------------//
void MOAIGfxScript::ExecuteBytecode ( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ callCommand ) {

	bool didCall = false;

	const void* bytecode = this->mBytecode.GetBuffer ();
	const void* top = ( const void* )(( uintptr )bytecode + this->mBytecode.Size ());

	while ( bytecode < top ) {
		
		const MOAIGfxScriptCommand& command = *( const MOAIGfxScriptCommand* )bytecode;
		bytecode = ( const void* )(( uintptr )bytecode + sizeof ( MOAIGfxScriptCommand ));
		
		MOAIDrawingAPI::Execute ( callable, command.mType, bytecode );
		bytecode = ( const void* )(( uintptr )bytecode + command.mParamSize );
		
		didCall = didCall || ( command.mType == CALL ) || ( command.mType == CALL_FROM_SHADER );
	}

	if ( !didCall ) {
		MOAIDrawingAPI::Execute ( callable, callCommand, NULL );
		this->Execute ( callable, callCommand, 0 );
	}
}

//----------------------------------------------------------------//
bool MOAIGfxScript::HasContent () {

	return (( this->mCommandStream.GetCursor () > 0 ) || ( this->mBytecode.Size () > 0 ));
}

//----------------------------------------------------------------//
MOAIGfxScript::MOAIGfxScript () {

	RTTI_BEGIN ( MOAIGfxScript )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIGfxScript >)
		RTTI_EXTEND ( MOAIAbstractHasGfxScript )
	RTTI_END

	this->mCommandStream.SetChunkSize ( 256 );
}

//----------------------------------------------------------------//
MOAIGfxScript::~MOAIGfxScript () {
}

//----------------------------------------------------------------//
void MOAIGfxScript::Reset () {

	this->mCommandStream.Seek ( 0 );
	this->mBytecode.Clear ();
	this->mRetainedObjects.Clear ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxScript::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIGfxScript::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "affirmBytecode",				_affirmBytecode },
		{ "call",						_call },
		{ "callFromShader",				_callFromShader },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxScript::MOAIAbstractGfxScript_RetainObject ( ZLRefCountedObject* object ) {

	if ( object ) {
		this->mRetainedObjects.Push ( object );
	}
}

//----------------------------------------------------------------//
void MOAIGfxScript::MOAIAbstractGfxScript_RunScript ( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ callCommand ) {

	this->AffirmBytecode ();
	this->ExecuteBytecode ( callable, callCommand );
}

//----------------------------------------------------------------//
void MOAIGfxScript::MOAIAbstractGfxScript_SubmitCommand ( MOAIDrawingAPIEnum::_ cmd, const void* param, ZLSize size ) {

	MOAIDrawingAPI command;
	command.mType = cmd;
	command.mParamSize = size;
	
	this->mCommandStream.Write < MOAIDrawingAPI >( command );
	this->mCommandStream.WriteBytes ( param, size );
}

//----------------------------------------------------------------//
MOAIAbstractGfxScript& MOAIGfxScript::MOAIAbstractHasGfxScript_AffirmGfxScript () {

	return *this;
}
