// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSCRIPTRETAINEDGL_H
#define	MOAIGFXSCRIPTRETAINEDGL_H

#include <moai-gfx-gl/MOAIAbstractDrawingObjectGL.h>

//================================================================//
// MOAIGfxScriptRetainedGL
//================================================================//
// TODO: doxygen
class MOAIGfxScriptRetainedGL :
	public virtual MOAIGfxScriptRetained,
	public virtual MOAIAbstractDrawingObjectGL {
protected:
	
	//----------------------------------------------------------------//
	void			MOAIGfxScriptRetained_Execute		( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ cmd, const void* rawParam ) const;

public:

	DECL_LUA_FACTORY ( MOAIGfxScriptRetainedGL )

	//----------------------------------------------------------------//
					MOAIGfxScriptRetainedGL				();
					~MOAIGfxScriptRetainedGL			();
};

#endif
