// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractGfxScriptCallback.h>
#include <moai-gfx/MOAIDrawingAPI.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIGfxScriptRetained.h>
#include <moai-gfx/MOAIShader.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxScriptRetained::_call ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxScriptRetained, "U" )
	self->Call ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxScriptRetained::_callFromShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxScriptRetained, "U" )
	self->CallFromShader ();
	return 0;
}

//================================================================//
// MOAIGfxScriptRetained
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxScriptRetained::Call () {

	this->SubmitCommand (( MOAIDrawingAPIEnum::_ )CALL );
}

//----------------------------------------------------------------//
void MOAIGfxScriptRetained::CallFromShader() {

	this->SubmitCommand (( MOAIDrawingAPIEnum::_ )CALL_FROM_SHADER );
}

//----------------------------------------------------------------//
void MOAIGfxScriptRetained::Execute ( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ cmd, const void* rawParam ) const {

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
				MOAIAbstractGfxScript* gfxScript = shader ? shader->GetGfxScript () : NULL;
			
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
			this->MOAIGfxScriptRetained_Execute ( callable, cmd, rawParam );
	}
}

//----------------------------------------------------------------//
void MOAIGfxScriptRetained::ExecuteBytecode ( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ callCommand ) {

	bool didCall = false;

	const void* bytecode = this->mBytecode.GetBuffer ();
	const void* top = ( const void* )(( uintptr )bytecode + this->mBytecode.Size ());

	while ( bytecode < top ) {
		
		const MOAIGfxScriptCommand& command = *( const MOAIGfxScriptCommand* )bytecode;
		bytecode = ( const void* )(( uintptr )bytecode + sizeof ( MOAIGfxScriptCommand ));
		
		this->Execute ( callable, command.mType, bytecode );
		bytecode = ( const void* )(( uintptr )bytecode + command.mParamSize );
		
		didCall = didCall || ( command.mType == CALL ) || ( command.mType == CALL_FROM_SHADER );
	}

	if ( !didCall ) {
		this->Execute ( callable, callCommand, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIGfxScriptRetained::ExecuteMemStream ( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ callCommand ) {

	bool didCall = false;

	this->mCommandStream.Seek ( 0 );
	while ( !this->mCommandStream.IsAtEnd ()) {
	
		MOAIGfxScriptCommand command = this->mCommandStream.Read < MOAIGfxScriptCommand >();
		void* buffer = command.mParamSize > 0 ? alloca ( command.mParamSize ) : NULL;
		this->mCommandStream.ReadBytes ( buffer, command.mParamSize );
		this->Execute ( callable, command.mType, buffer );
		
		didCall = didCall || ( command.mType == CALL ) || ( command.mType == CALL_FROM_SHADER );
	}
	
	if ( callable && ( !didCall )) {
		this->Execute ( callable, callCommand, 0 );
	}
}

//----------------------------------------------------------------//
bool MOAIGfxScriptRetained::HasContent () {

	return (( this->mCommandStream.GetCursor () > 0 ) || ( this->mBytecode.Size () > 0 ));
}

//----------------------------------------------------------------//
MOAIGfxScriptRetained::MOAIGfxScriptRetained () {

	RTTI_BEGIN ( MOAIGfxScriptRetained )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIGfxScriptRetained >)
		RTTI_EXTEND ( MOAIAbstractGfxScript )
		RTTI_EXTEND ( MOAIAbstractDrawingLuaAPI )
	RTTI_END

	this->mCommandStream.SetChunkSize ( 256 );
}

//----------------------------------------------------------------//
MOAIGfxScriptRetained::~MOAIGfxScriptRetained () {
}

//----------------------------------------------------------------//
void MOAIGfxScriptRetained::Optimize () {

	ZLSize size = this->mCommandStream.GetCursor ();
	if ( size > 0 ) {
	
		this->mBytecode.Init ( size );
		this->mCommandStream.Seek ( 0 );
		this->mCommandStream.ReadBytes ( this->mBytecode.GetBuffer (), size );
		this->mCommandStream.Clear ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxScriptRetained::Reset () {

	this->mCommandStream.Seek ( 0 );
	this->mBytecode.Clear ();
	this->mRetainedObjects.Clear ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxScriptRetained::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIGfxScriptRetained::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "call",						_call },
		{ "callFromShader",				_callFromShader },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxScriptRetained::MOAIAbstractDrawingAPI_RetainObject ( ZLRefCountedObject* object ) {

	if ( object ) {
		this->mRetainedObjects.Push ( object );
	}
}

//----------------------------------------------------------------//
void MOAIGfxScriptRetained::MOAIAbstractDrawingAPI_SubmitCommand ( MOAIDrawingAPIEnum::_ cmd, const void* param, ZLSize size ) {

	MOAIDrawingAPI command;
	command.mType = cmd;
	command.mParamSize = size;
	
	this->mCommandStream.Write < MOAIDrawingAPI >( command );
	this->mCommandStream.WriteBytes ( param, size );
}

//----------------------------------------------------------------//
void MOAIGfxScriptRetained::MOAIAbstractGfxScript_RunScript ( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ callCommand ) {

	if ( this->mBytecode.Size () > 0 ) {
		this->ExecuteBytecode ( callable, callCommand );
	}
	else {
		this->ExecuteMemStream ( callable, callCommand );
	}
}

//----------------------------------------------------------------//
void MOAIGfxScriptRetained::MOAIGfxScriptRetained_Execute ( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ cmd, const void* rawParam ) const {

	MOAIDrawingAPI::Execute ( callable, cmd, rawParam );
}
