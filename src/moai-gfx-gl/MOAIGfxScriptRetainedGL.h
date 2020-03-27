// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSCRIPTRETAINEDGL_H
#define	MOAIGFXSCRIPTRETAINEDGL_H

#include <moai-gfx-gl/MOAIAbstractDrawingAPIObjectGL.h>

//================================================================//
// MOAIGfxScriptRetainedGL
//================================================================//
// TODO: doxygen
class MOAIGfxScriptRetainedGL :
	public virtual MOAIGfxScriptRetained,
	public virtual MOAIAbstractDrawingAPIObjectGL {
protected:
	
	//----------------------------------------------------------------//
	void			MOAIGfxScriptRetained_Execute		( MOAIAbstractDrawingAPICallback* callable, MOAIDrawingCmdEnum::_ cmd, const void* rawParam ) const;

public:

	DECL_LUA_FACTORY ( MOAIGfxScriptRetainedGL )

	//----------------------------------------------------------------//
					MOAIGfxScriptRetainedGL				();
					~MOAIGfxScriptRetainedGL			();
};

#endif
