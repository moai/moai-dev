// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractDrawingAPICallback.h>
#include <moai-gfx/MOAIGfxScriptRetained.h>

//================================================================//
// MOAIGfxScriptRetained
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxScriptRetained::ExecuteBytecode ( MOAIAbstractDrawingAPICallback* callable, MOAIDrawingCmdEnum::_ callCommand ) {

	bool didCall = false;

	const void* bytecode = this->mBytecode.GetBuffer ();
	const void* top = ( const void* )(( uintptr )bytecode + this->mBytecode.Size ());

	while ( bytecode < top ) {
		
		const MOAIDrawingCommand& command = *( const MOAIDrawingCommand* )bytecode;
		bytecode = ( const void* )(( uintptr )bytecode + sizeof ( MOAIDrawingCommand ));
		
		MOAIDrawingCommand::Execute ( callable, command.mType, bytecode );
		bytecode = ( const void* )(( uintptr )bytecode + sizeof ( command.mParamSize ));
		
		didCall = didCall || ( command.mType >= MOAIDrawingCmdEnum::CALL );
	}

	if ( !didCall ) {
		MOAIDrawingCommand::Execute ( callable, callCommand, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIGfxScriptRetained::ExecuteMemStream ( MOAIAbstractDrawingAPICallback* callable, MOAIDrawingCmdEnum::_ callCommand ) {

	bool didCall = false;

	this->mCommandStream.Seek ( 0 );
	while ( !this->mCommandStream.IsAtEnd ()) {
	
		MOAIDrawingCommand command = this->mCommandStream.Read < MOAIDrawingCommand >();
		void* buffer = command.mParamSize > 0 ? alloca ( command.mParamSize ) : NULL;
		this->mCommandStream.ReadBytes ( buffer, command.mParamSize );
		MOAIDrawingCommand::Execute ( callable, command.mType, buffer );
		
		didCall = didCall || ( command.mType >= MOAIDrawingCmdEnum::CALL );
	}
	
	if ( callable && ( !didCall )) {
		MOAIDrawingCommand::Execute ( callable, callCommand, 0 );
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
void MOAIGfxScriptRetained::MOAIAbstractDrawingAPI_RetainObject ( ZLRefCountedObject* object ) {

	if ( object ) {
		this->mRetainedObjects.Push ( object );
	}
}

//----------------------------------------------------------------//
void MOAIGfxScriptRetained::MOAIAbstractDrawingAPI_SubmitCommand ( MOAIDrawingCmdEnum::_ cmd, const void* param, ZLSize size ) {

	MOAIDrawingCommand command;
	command.mType = cmd;
	command.mParamSize = size;
	
	this->mCommandStream.Write < MOAIDrawingCommand >( command );
	this->mCommandStream.WriteBytes ( param, size );
}

//----------------------------------------------------------------//
void MOAIGfxScriptRetained::MOAIAbstractGfxScript_RunScript ( MOAIAbstractDrawingAPICallback* callable, MOAIDrawingCmdEnum::_ callCommand ) {

	if ( this->mBytecode.Size () > 0 ) {
		this->ExecuteBytecode ( callable, callCommand );
	}
	else {
		this->ExecuteMemStream ( callable, callCommand );
	}
}
