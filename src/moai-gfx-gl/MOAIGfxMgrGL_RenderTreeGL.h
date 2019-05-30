// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGRGL_RENDERTREEGL_H
#define	MOAIGFXMGRGL_RENDERTREEGL_H

#include <moai-gfx-gl/MOAIGfxMgrGLComponents.h>

//================================================================//
// MOAIGfxMgrGL_RenderTreeGL
//================================================================//
// TODO: doxygen
class MOAIGfxMgrGL_RenderTreeGL :
	virtual public MOAIGfxMgrGLComponents,
	public virtual MOAIGfxMgr_RenderTree {
private:

	//----------------------------------------------------------------//
	void			MOAIRenderTree_Render			();

public:

	//----------------------------------------------------------------//
					MOAIGfxMgrGL_RenderTreeGL		();
					~MOAIGfxMgrGL_RenderTreeGL		();
	
	//----------------------------------------------------------------//
	// disambiguate
	static MOAIGfxMgrGL_RenderTreeGL& Get () {
		return ZLContextClass < MOAIGfxMgrGL_RenderTreeGL >::Get ();
	}
};

#endif
