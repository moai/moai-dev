// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIRENDERBATCH_H
#define	MOAIRENDERBATCH_H

#include <moai-gfx/MOAISurfaceClearColor.h>

class MOAIColor;

//================================================================//
// MOAIRenderBatch
//================================================================//
class MOAIRenderBatch :
	public virtual MOAILuaObject,
	public virtual MOAISurfaceClearColor {
protected:

	friend class MOAIGfxMgr_RenderTree;

	MOAILuaStrongRef	mRenderRoot;

	//----------------------------------------------------------------//
	void			DrawRenderRoot				();

	//----------------------------------------------------------------//
	virtual void	MOAIRenderBatch_Render		() = 0;

public:

	//----------------------------------------------------------------//
					MOAIRenderBatch				();
					~MOAIRenderBatch			();
	void			Render						();
};

#endif
