// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSCRIPTRETAINEDVK_H
#define	MOAIGFXSCRIPTRETAINEDVK_H

#include <moai-gfx-vk/MOAIAbstractDrawingAPIObjectVK.h>

//================================================================//
// MOAIGfxScriptRetainedVK
//================================================================//
// TODO: doxygen
class MOAIGfxScriptRetainedVK :
	public virtual MOAIGfxScriptRetained,
	public virtual MOAIAbstractDrawingAPIObjectVK {
protected:
	
	//----------------------------------------------------------------//
	void			MOAIGfxScriptRetained_Execute		( MOAIAbstractDrawingAPICallback* callable, MOAIDrawingCmdEnum::_ cmd, const void* rawParam ) const;

public:

	DECL_LUA_FACTORY ( MOAIGfxScriptRetainedVK )

	//----------------------------------------------------------------//
					MOAIGfxScriptRetainedVK				();
					~MOAIGfxScriptRetainedVK			();
};

#endif
