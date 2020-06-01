// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIGfxScriptImmediate.h>

//================================================================//
// MOAIGfxScriptImmediate
//================================================================//

//----------------------------------------------------------------//
MOAIGfxScriptImmediate::MOAIGfxScriptImmediate () {

	RTTI_BEGIN ( MOAIGfxScriptImmediate )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIGfxScriptImmediate >)
		RTTI_EXTEND ( MOAILuaObject )
		RTTI_EXTEND ( MOAIAbstractGfxScript )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxScriptImmediate::~MOAIGfxScriptImmediate () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxScriptImmediate::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( history );
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIGfxScriptImmediate::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( history );
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIGfxScriptImmediate::MOAIAbstractGfxScript_Optimize () {
}

//----------------------------------------------------------------//
void MOAIGfxScriptImmediate::MOAIAbstractGfxScript_RunScript ( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ callCommand ) {
	UNUSED ( callable );
	UNUSED ( callCommand );
}
