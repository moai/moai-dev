// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSCRIPTRETAINEDVK_H
#define	MOAIGFXSCRIPTRETAINEDVK_H

#include <moai-gfx-vk/MOAIAbstractDrawingObjectVK.h>

//================================================================//
// MOAIGfxScriptRetainedVK
//================================================================//
// TODO: doxygen
class MOAIGfxScriptRetainedVK :
	public virtual MOAIGfxScriptRetained,
	public virtual MOAIAbstractDrawingObjectVK {
protected:
	
	//----------------------------------------------------------------//
	void			MOAIGfxScriptRetained_Execute		( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ cmd, const void* rawParam ) const;

public:

	DECL_LUA_FACTORY ( MOAIGfxScriptRetainedVK )

	//----------------------------------------------------------------//
					MOAIGfxScriptRetainedVK				();
					~MOAIGfxScriptRetainedVK			();
};

#endif
