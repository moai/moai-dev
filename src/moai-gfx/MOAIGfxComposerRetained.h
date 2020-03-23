// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXCOMPOSER_H
#define	MOAIGFXCOMPOSER_H

#include <moai-gfx/MOAIAbstractGfxComposer.h>
#include <moai-gfx/MOAIGfxComposerCommand.h>
#include <moai-gfx/MOAITexture.h>

class MOAIAbstractUniformBuffer;
class MOAIUniformSchema;

// NOTE: this will is a draw script. it will eventually encompass and replace MOAIDraw.
// MOAIMaterialMgr, MOAILight, etc. are also going bye-bye. all of those are a (bad) subset of
// what draw scripts can do.

//================================================================//
// MOAIGfxComposerRetained
//================================================================//
// TODO: doxygen
class MOAIGfxComposerRetained :
	public virtual MOAIAbstractGfxComposer {
protected:

	friend class MOAIAbstractGfxComposerInterface;

	ZLMemStream											mCommandStream;
	ZLLeanArray < u8 > 									mBytecode;
	ZLLeanStack < ZLStrongPtr < ZLRefCountedObject > >	mRetainedObjects;
	
	//----------------------------------------------------------------//
	void 				ExecuteBytecode				( MOAIAbstractGfxComposerCallable& callable, MOAIGfxComposerCmdEnum::_ callCommand );
	void				ExecuteMemStream			( MOAIAbstractGfxComposerCallable& callable, MOAIGfxComposerCmdEnum::_ callCommand );
	
	//----------------------------------------------------------------//
	void							MOAIAbstractGfxComposer_Execute						( MOAIAbstractGfxComposerCallable& callable, MOAIGfxComposerCmdEnum::_ callCommand );
	void							MOAIAbstractGfxComposer_Optimize					();
	void							MOAIAbstractGfxComposer_RetainObject 				( ZLRefCountedObject* object );
	void							MOAIAbstractGfxComposer_SubmitCommand				( MOAIGfxComposerCmdEnum::_ cmd, const void* param, ZLSize size );
	MOAIAbstractGfxComposer&		MOAIAbstractGfxComposerInterface_AffirmComposer		();
	MOAIAbstractGfxComposer*		MOAIAbstractGfxComposerInterface_GetComposer		();

public:

	DECL_LUA_FACTORY ( MOAIGfxComposerRetained )

	//----------------------------------------------------------------//
	void 				Execute 					( MOAIAbstractGfxComposerCallable& callable, MOAIGfxComposerCmdEnum::_ callCommand );
						MOAIGfxComposerRetained		();
						~MOAIGfxComposerRetained	();
};

#endif
