// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSCRIPTGL_H
#define	MOAIGFXSCRIPTGL_H

#include <moai-gfx-gl/MOAIAbstractDrawingObjectGL.h>

//================================================================//
// MOAIGfxScriptGL
//================================================================//
// TODO: doxygen
class MOAIGfxScriptGL :
	public virtual MOAIGfxScript,
	public virtual MOAIAbstractDrawingObjectGL {
protected:
	
	//----------------------------------------------------------------//
	void			MOAIGfxScript_Execute		( MOAIAbstractGfxScriptCallback* callable, MOAIDrawingAPIEnum::_ cmd, const void* rawParam ) const;

public:

	DECL_LUA_FACTORY ( MOAIGfxScriptGL )

	//----------------------------------------------------------------//
					MOAIGfxScriptGL				();
					~MOAIGfxScriptGL			();
};

#endif
