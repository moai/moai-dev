// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGRVK_RENDERTREEVK_H
#define	MOAIGFXMGRVK_RENDERTREEVK_H

#include <moai-gfx-vk/MOAIGfxMgrVKComponents.h>

class MOAIOneTriVK;

//================================================================//
// MOAIGfxMgrVK_RenderTreeVK
//================================================================//
// TODO: doxygen
class MOAIGfxMgrVK_RenderTreeVK :
	public virtual MOAIGfxMgrVKComponents,
	public virtual MOAIGfxMgr_RenderTree {
private:

	ZLStrongPtr < MOAIOneTriVK >	mOneTri;

	//----------------------------------------------------------------//
	MOAIOneTriVK&		AffirmOneTri						();
	void				DrawOneTri							( VkCommandBuffer& commandBuffer, u32 width, u32 height );

	//----------------------------------------------------------------//
	void				MOAIGfxMgr_RenderTree_Render		();

public:

	//----------------------------------------------------------------//
						MOAIGfxMgrVK_RenderTreeVK			();
						~MOAIGfxMgrVK_RenderTreeVK			();
	
	//----------------------------------------------------------------//
	// disambiguate
	static MOAIGfxMgrVK_RenderTreeVK& Get () {
		return ZLContextClass < MOAIGfxMgrVK_RenderTreeVK >::Get ();
	}
};

#endif
