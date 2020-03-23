// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSCRIPT_H
#define	MOAIGFXSCRIPT_H

#include <moai-gfx/MOAIAbstractGfxScript.h>
#include <moai-gfx/MOAIGfxScriptCommand.h>
#include <moai-gfx/MOAITexture.h>

class MOAIAbstractUniformBuffer;
class MOAIUniformSchema;

// NOTE: this will is a draw script. it will eventually encompass and replace MOAIDraw.
// MOAIMaterialMgr, MOAILight, etc. are also going bye-bye. all of those are a (bad) subset of
// what draw scripts can do.

//================================================================//
// MOAIGfxScript
//================================================================//
// TODO: doxygen
class MOAIGfxScript :
	public virtual MOAIAbstractGfxScript {
protected:

	friend class MOAIAbstractGfxScriptInterface;

	ZLMemStream											mCommandStream;
	ZLLeanArray < u8 > 									mBytecode;
	ZLLeanStack < ZLStrongPtr < ZLRefCountedObject > >	mRetainedObjects;
	
	//----------------------------------------------------------------//
	void 				ExecuteBytecode				( MOAIAbstractGfxScriptCallable& callable, MOAIGfxScriptCmdEnum::_ callCommand );
	void				ExecuteMemStream			( MOAIAbstractGfxScriptCallable& callable, MOAIGfxScriptCmdEnum::_ callCommand );
	
	//----------------------------------------------------------------//
	void						MOAIAbstractGfxScript_Execute					( MOAIAbstractGfxScriptCallable& callable, MOAIGfxScriptCmdEnum::_ callCommand );
	void						MOAIAbstractGfxScript_Optimize					();
	void						MOAIAbstractGfxScript_RetainObject 				( ZLRefCountedObject* object );
	void						MOAIAbstractGfxScript_SubmitCommand				( MOAIGfxScriptCmdEnum::_ cmd, const void* param, ZLSize size );
	MOAIAbstractGfxScript&		MOAIAbstractGfxScriptInterface_AffirmComposer	();
	MOAIAbstractGfxScript*		MOAIAbstractGfxScriptInterface_GetComposer		();

public:

	DECL_LUA_FACTORY ( MOAIGfxScript )

	//----------------------------------------------------------------//
	void 				Execute 					( MOAIAbstractGfxScriptCallable& callable, MOAIGfxScriptCmdEnum::_ callCommand );
						MOAIGfxScript				();
						~MOAIGfxScript				();
};

#endif
