// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDRAWGL_H
#define	MOAIDRAWGL_H

#include <moai-gfx-gl/MOAIDrawAPIGL.h>

//================================================================//
// MOAIDrawGL
//================================================================//
// TODO: doxygen
class MOAIDrawGL :
	public virtual MOAIAbstractCmdHandlerWithAPI < MOAIDrawAPIGL >,
	public virtual ZLContextClass {
protected:

	//----------------------------------------------------------------//
	void			MOAIAbstractCmdHandler_HandleCommand		( u32 cmd, const void* param );

public:

	DECL_LUA_SINGLETON ( MOAIDrawGL )

	//----------------------------------------------------------------//
					MOAIDrawGL					( ZLContext& context );
					~MOAIDrawGL					();
};

#endif
