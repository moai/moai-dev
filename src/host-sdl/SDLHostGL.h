// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	SDLHOSTGL_H
#define	SDLHOSTGL_H

#include "SDLAbstractHost.h"

//================================================================//
// SDLHostGL
//================================================================//
class SDLHostGL :
	public SDLAbstractHost {
protected:
	
	SDL_GLContext	mGLContext;
	
	//----------------------------------------------------------------//
	void		SDLAbstractHost_MainLoopDidFinish		();
	void		SDLAbstractHost_MainLoopDidStart		();
	void		SDLAbstractHost_OpenWindow				( const char* title, int width, int height );
	void		SDLAbstractHost_Render					();

public:

	//----------------------------------------------------------------//
				SDLHostGL			();
				~SDLHostGL			();
};

#endif
