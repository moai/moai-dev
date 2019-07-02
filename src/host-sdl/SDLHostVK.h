// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	SDLHOSTVK_H
#define	SDLHOSTVK_H

#include <vulkan/vulkan.h>
#include "SDLAbstractHost.h"

//================================================================//
// SDLHostVK
//================================================================//
class SDLHostVK :
	public SDLAbstractHost {
protected:
	
	//----------------------------------------------------------------//
	static VkSurfaceKHR		_AKUGfxVKFunc_CreateSurface				( VkInstance instance, void* userdata );
	static const char**		_AKUGfxVKFunc_GetInstanceExtensions		( void* userdata );
	
	//----------------------------------------------------------------//
	void		SDLAbstractHost_MainLoopDidFinish		();
	void		SDLAbstractHost_MainLoopDidStart		();
	void		SDLAbstractHost_OpenWindow				( const char* title, int width, int height );
	void		SDLAbstractHost_Render					();

public:

	//----------------------------------------------------------------//
				SDLHostVK			();
				~SDLHostVK			();
};

#endif
