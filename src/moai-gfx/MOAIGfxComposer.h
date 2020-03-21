// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXCOMPOSER_H
#define	MOAIGFXCOMPOSER_H

#include <moai-gfx/MOAIGfxComposerCmd.h>
#include <moai-gfx/MOAIGfxComposerInterface.h>
#include <moai-gfx/MOAITexture.h>

class MOAIAbstractGfxComposerCallable;
class MOAIAbstractUniformBuffer;
class MOAIUniformSchema;

//================================================================//
// MOAIGfxComposer
//================================================================//
// TODO: doxygen
class MOAIGfxComposer :
	public virtual MOAIGfxComposerInterface {
protected:

	friend class MOAIGfxComposerInterface;

	ZLLeanStack < ZLStrongPtr < MOAIGfxComposerCmd > > 		mCommands;

	//----------------------------------------------------------------//
	MOAIGfxComposer&		MOAIGfxComposerInterface_AffirmComposer		();
	MOAIGfxComposer*		MOAIGfxComposerInterface_GetComposer		();

public:

	DECL_LUA_FACTORY ( MOAIGfxComposer )

	//----------------------------------------------------------------//
	void 			Execute 					( MOAIAbstractGfxComposerCallable& callable, MOAIGfxComposerCmdEnum::_ callCommand );
					MOAIGfxComposer				();
					~MOAIGfxComposer			();
};

#endif
