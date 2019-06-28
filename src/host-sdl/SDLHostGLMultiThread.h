// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	SDLHOSTGLMULTITHREAD_H
#define	SDLHOSTGLMULTITHREAD_H

#include "SDLHostGL.h"

//================================================================//
// SDLHostGLMultiThread
//================================================================//
class SDLHostGLMultiThread :
	public SDLAbstractHost {
protected:
	
	size_t		mSimUpdateCounter;
	
	//----------------------------------------------------------------//
	void		SDLAbstractHost_MainLoopDidFinish		();
	void		SDLAbstractHost_MainLoopDidStart		();
	void		SDLAbstractHost_Render					();

public:

	//----------------------------------------------------------------//
				SDLHostGLMultiThread		();
				~SDLHostGLMultiThread		();
};

#endif
