// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSCRIPTRETAINEDVK_H
#define	MOAIGFXSCRIPTRETAINEDVK_H

#include <moai-gfx-vk/MOAIAbstractDrawingObjectVK.h>

//================================================================//
// MOAIGfxScriptVK
//================================================================//
// TODO: doxygen
class MOAIGfxScriptVK :
	public virtual MOAIGfxScript,
	public virtual MOAIAbstractDrawingObjectVK {
protected:
	
	//----------------------------------------------------------------//
	void			MOAIGfxScript_Execute		( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ cmd, const void* rawParam ) const;

public:

	DECL_LUA_FACTORY ( MOAIGfxScriptVK )

	//----------------------------------------------------------------//
					MOAIGfxScriptVK				();
					~MOAIGfxScriptVK			();
};

#endif
