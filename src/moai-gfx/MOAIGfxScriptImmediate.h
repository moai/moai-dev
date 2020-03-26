// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSCRIPTIMMEDIATE_H
#define	MOAIGFXSCRIPTIMMEDIATE_H

#include <moai-gfx/MOAIAbstractGfxScript.h>
#include <moai-gfx/MOAIDrawingCommand.h>

//================================================================//
// MOAIGfxScriptImmediate
//================================================================//
// TODO: doxygen
class MOAIGfxScriptImmediate :
	public virtual MOAILuaObject,
	public virtual MOAIAbstractGfxScript {
protected:
	
	// TODO: lua ref
	
	//----------------------------------------------------------------//
	void			_RegisterLuaClass							( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs							( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAIAbstractGfxScript_Optimize				();
	void			MOAIAbstractGfxScript_RunScript				( MOAIAbstractDrawingAPICallback* callable, MOAIDrawingCmdEnum::_ callCommand );

public:

	DECL_LUA_FACTORY ( MOAIGfxScriptImmediate )

	//----------------------------------------------------------------//
					MOAIGfxScriptImmediate		();
					~MOAIGfxScriptImmediate		();
};

#endif
