// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractGfxComposerCallable.h>
#include <moai-gfx/MOAIAbstractUniformBuffer.h>
#include <moai-gfx/MOAIGfxComposerRetained.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIMaterialMgr.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>
#include <moai-gfx/MOAIUniformSchema.h>

//================================================================//
// MOAIGfxComposerRetained
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxComposerRetained::ExecuteBytecode ( MOAIAbstractGfxComposerCallable& callable, MOAIGfxComposerCmdEnum::_ callCommand ) {

	bool didCall = false;

	const void* bytecode = this->mBytecode.GetBuffer ();
	const void* top = ( const void* )(( uintptr )bytecode + this->mBytecode.Size ());

	while ( bytecode < top ) {
		
		const MOAIGfxComposerCommand& command = *( const MOAIGfxComposerCommand* )bytecode;
		bytecode = ( const void* )(( uintptr )bytecode + sizeof ( MOAIGfxComposerCommand ));
		
		MOAIGfxComposerCommand::Execute ( callable, command.mType, bytecode );
		bytecode = ( const void* )(( uintptr )bytecode + sizeof ( command.mParamSize ));
		
		didCall = didCall || ( command.mType >= MOAIGfxComposerCmdEnum::CALL );
	}

	if ( !didCall ) {
		MOAIGfxComposerCommand::Execute ( callable, callCommand, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIGfxComposerRetained::ExecuteMemStream ( MOAIAbstractGfxComposerCallable& callable, MOAIGfxComposerCmdEnum::_ callCommand ) {

	bool didCall = false;

	this->mCommandStream.Seek ( 0 );
	while ( !this->mCommandStream.IsAtEnd ()) {
	
		MOAIGfxComposerCommand command = this->mCommandStream.Read < MOAIGfxComposerCommand >();
		void* buffer = command.mParamSize > 0 ? alloca ( command.mParamSize ) : NULL;
		this->mCommandStream.ReadBytes ( buffer, command.mParamSize );
		MOAIGfxComposerCommand::Execute ( callable, command.mType, buffer );
		
		didCall = didCall || ( command.mType >= MOAIGfxComposerCmdEnum::CALL );
	}
	
	if ( !didCall ) {
		MOAIGfxComposerCommand::Execute ( callable, callCommand, 0 );
	}
}

//----------------------------------------------------------------//
MOAIGfxComposerRetained::MOAIGfxComposerRetained () {

	RTTI_BEGIN ( MOAIGfxComposerRetained )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIGfxComposerRetained >)
		RTTI_EXTEND ( MOAIAbstractGfxComposerInterface )
	RTTI_END

	this->mCommandStream.SetChunkSize ( 256 );
}

//----------------------------------------------------------------//
MOAIGfxComposerRetained::~MOAIGfxComposerRetained () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxComposerRetained::MOAIAbstractGfxComposer_Execute ( MOAIAbstractGfxComposerCallable& callable, MOAIGfxComposerCmdEnum::_ callCommand ) {

	if ( this->mBytecode.Size () > 0 ) {
		this->ExecuteBytecode ( callable, callCommand );
	}
	else {
		this->ExecuteMemStream ( callable, callCommand );
	}
}

//----------------------------------------------------------------//
void MOAIGfxComposerRetained::MOAIAbstractGfxComposer_Optimize () {

	ZLSize size = this->mCommandStream.GetCursor ();
	if ( size > 0 ) {
	
		this->mBytecode.Init ( size );
		this->mCommandStream.Seek ( 0 );
		this->mCommandStream.ReadBytes ( this->mBytecode.GetBuffer (), size );
		this->mCommandStream.Clear ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxComposerRetained::MOAIAbstractGfxComposer_RetainObject ( ZLRefCountedObject* object ) {

	if ( object ) {
		this->mRetainedObjects.Push ( object );
	}
}

//----------------------------------------------------------------//
void MOAIGfxComposerRetained::MOAIAbstractGfxComposer_SubmitCommand ( MOAIGfxComposerCmdEnum::_ cmd, const void* param, ZLSize size ) {

	MOAIGfxComposerCommand command;
	command.mType = cmd;
	command.mParamSize = size;
	
	this->mCommandStream.Write < MOAIGfxComposerCommand >( command );
	this->mCommandStream.WriteBytes ( param, size );
}

//----------------------------------------------------------------//
MOAIAbstractGfxComposer& MOAIGfxComposerRetained::MOAIAbstractGfxComposerInterface_AffirmComposer () {

	return *this;
}

//----------------------------------------------------------------//
MOAIAbstractGfxComposer* MOAIGfxComposerRetained::MOAIAbstractGfxComposerInterface_GetComposer () {

	return this;
}
