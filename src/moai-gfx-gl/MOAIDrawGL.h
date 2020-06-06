// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAWGL_H
#define	MOAIDRAWGL_H

#include <moai-gfx-gl/MOAIAbstractDrawingLuaAPIGL.h>

//================================================================//
// MOAIDrawGL
//================================================================//
class MOAIDrawGL :
	public virtual MOAIAbstractDrawingLuaAPIGL {
protected:

	//----------------------------------------------------------------//
	void				MOAIAbstractDrawingAPI_RetainObject		( ZLRefCountedObject* object );
	void				MOAIAbstractDrawingAPI_SubmitCommand	( MOAIDrawingAPIEnum::_ cmd, const void* param, ZLSize size );

public:

	DECL_LUA_FACTORY ( MOAIDrawGL )

	//----------------------------------------------------------------//
						MOAIDrawGL				();
	virtual				~MOAIDrawGL				();
};

#endif
