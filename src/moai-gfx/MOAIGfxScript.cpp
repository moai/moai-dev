// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractGfxScriptCallable.h>
#include <moai-gfx/MOAIAbstractUniformBuffer.h>
#include <moai-gfx/MOAIGfxScript.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIMaterialMgr.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>
#include <moai-gfx/MOAIUniformSchema.h>

//================================================================//
// MOAIGfxScript
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxScript::ExecuteBytecode ( MOAIAbstractGfxScriptCallable& callable, MOAIGfxScriptCmdEnum::_ callCommand ) {

	bool didCall = false;

	const void* bytecode = this->mBytecode.GetBuffer ();
	const void* top = ( const void* )(( uintptr )bytecode + this->mBytecode.Size ());

	while ( bytecode < top ) {
		
		const MOAIGfxScriptCommand& command = *( const MOAIGfxScriptCommand* )bytecode;
		bytecode = ( const void* )(( uintptr )bytecode + sizeof ( MOAIGfxScriptCommand ));
		
		MOAIGfxScriptCommand::Execute ( callable, command.mType, bytecode );
		bytecode = ( const void* )(( uintptr )bytecode + sizeof ( command.mParamSize ));
		
		didCall = didCall || ( command.mType >= MOAIGfxScriptCmdEnum::CALL );
	}

	if ( !didCall ) {
		MOAIGfxScriptCommand::Execute ( callable, callCommand, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIGfxScript::ExecuteMemStream ( MOAIAbstractGfxScriptCallable& callable, MOAIGfxScriptCmdEnum::_ callCommand ) {

	bool didCall = false;

	this->mCommandStream.Seek ( 0 );
	while ( !this->mCommandStream.IsAtEnd ()) {
	
		MOAIGfxScriptCommand command = this->mCommandStream.Read < MOAIGfxScriptCommand >();
		void* buffer = command.mParamSize > 0 ? alloca ( command.mParamSize ) : NULL;
		this->mCommandStream.ReadBytes ( buffer, command.mParamSize );
		MOAIGfxScriptCommand::Execute ( callable, command.mType, buffer );
		
		didCall = didCall || ( command.mType >= MOAIGfxScriptCmdEnum::CALL );
	}
	
	if ( !didCall ) {
		MOAIGfxScriptCommand::Execute ( callable, callCommand, 0 );
	}
}

//----------------------------------------------------------------//
MOAIGfxScript::MOAIGfxScript () {

	RTTI_BEGIN ( MOAIGfxScript )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIGfxScript >)
		RTTI_EXTEND ( MOAIAbstractGfxScriptInterface )
	RTTI_END

	this->mCommandStream.SetChunkSize ( 256 );
}

//----------------------------------------------------------------//
MOAIGfxScript::~MOAIGfxScript () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxScript::MOAIAbstractGfxScript_Execute ( MOAIAbstractGfxScriptCallable& callable, MOAIGfxScriptCmdEnum::_ callCommand ) {

	if ( this->mBytecode.Size () > 0 ) {
		this->ExecuteBytecode ( callable, callCommand );
	}
	else {
		this->ExecuteMemStream ( callable, callCommand );
	}
}

//----------------------------------------------------------------//
void MOAIGfxScript::MOAIAbstractGfxScript_Optimize () {

	ZLSize size = this->mCommandStream.GetCursor ();
	if ( size > 0 ) {
	
		this->mBytecode.Init ( size );
		this->mCommandStream.Seek ( 0 );
		this->mCommandStream.ReadBytes ( this->mBytecode.GetBuffer (), size );
		this->mCommandStream.Clear ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxScript::MOAIAbstractGfxScript_RetainObject ( ZLRefCountedObject* object ) {

	if ( object ) {
		this->mRetainedObjects.Push ( object );
	}
}

//----------------------------------------------------------------//
void MOAIGfxScript::MOAIAbstractGfxScript_SubmitCommand ( MOAIGfxScriptCmdEnum::_ cmd, const void* param, ZLSize size ) {

	MOAIGfxScriptCommand command;
	command.mType = cmd;
	command.mParamSize = size;
	
	this->mCommandStream.Write < MOAIGfxScriptCommand >( command );
	this->mCommandStream.WriteBytes ( param, size );
}

//----------------------------------------------------------------//
MOAIAbstractGfxScript& MOAIGfxScript::MOAIAbstractGfxScriptInterface_AffirmComposer () {

	return *this;
}

//----------------------------------------------------------------//
MOAIAbstractGfxScript* MOAIGfxScript::MOAIAbstractGfxScriptInterface_GetComposer () {

	return this;
}
