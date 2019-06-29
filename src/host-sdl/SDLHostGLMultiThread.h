// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	SDLHOSTGLMULTITHREAD_H
#define	SDLHOSTGLMULTITHREAD_H

#include "SDLHostGL.h"

class WorkerThreadInfo;

//================================================================//
// SDLHostGLMultiThread
//================================================================//
class SDLHostGLMultiThread :
	public SDLAbstractHost {
protected:
	
	size_t		mSimUpdateCounter;
	
	WorkerThreadInfo*	mLoadingThread;
	WorkerThreadInfo*	mRenderThread;
	
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
