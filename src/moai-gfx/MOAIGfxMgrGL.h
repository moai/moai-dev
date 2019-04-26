// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGRGL_H
#define	MOAIGFXMGRGL_H

#include <moai-gfx/MOAIGfxMgr.h>

//================================================================//
// MOAIGfxMgrGL
//================================================================//
// TODO: doxygen
class MOAIGfxMgrGL :
	public ZLContextClass < MOAIGfxMgrGL >,
	public virtual MOAIGfxMgr {
private:
	
	//----------------------------------------------------------------//
	void				OnGlobalsFinalize			();
	void				OnGlobalsInitialize			();
	
public:
		
	//----------------------------------------------------------------//
					MOAIGfxMgrGL			();
					~MOAIGfxMgrGL			();
};

#endif
