// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGR_RENDERTREE_H
#define	MOAIGFXMGR_RENDERTREE_H

#include <moai-gfx/MOAIGfxMgrComponents.h>

class MOAIAbstractRenderable;

//================================================================//
// MOAIGfxMgr_RenderTree
//================================================================//
// TODO: doxygen
class MOAIGfxMgr_RenderTree :
	public virtual MOAIGfxMgrComponents {
protected:

	u32					mRenderCounter;	// increments every render
	double				mRenderDuration;
	double				mRenderTime;
	
	MOAILuaStrongRef	mRenderRoot;

	//----------------------------------------------------------------//
	void				AffirmRenderRoot			();

public:
	
	GET ( u32, RenderCounter, mRenderCounter )
	GET ( double, RenderDuration, mRenderDuration )
	GET ( double, RenderTime, mRenderTime )
	
	//----------------------------------------------------------------//
						MOAIGfxMgr_RenderTree		( ZLContext& context );
	virtual				~MOAIGfxMgr_RenderTree		();
	void				PushRenderable				( MOAIAbstractRenderable& node ); // goes into the default batch, if it exists
	void				PushRenderable				( MOAILuaState& state, int idx );
	void				Render						();
};

#endif
