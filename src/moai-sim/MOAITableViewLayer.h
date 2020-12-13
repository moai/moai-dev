// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITABLEVIEWLAYER_H
#define	MOAITABLEVIEWLAYER_H

#include <moai-sim/MOAIAbstractViewLayer.h>

//================================================================//
// MOAITableViewLayer
//================================================================//
// TODO: doxygen
class MOAITableViewLayer :
	public virtual MOAIAbstractViewLayer,
	public virtual MOAIRenderNode {
protected:

	//----------------------------------------------------------------//
	bool			MOAIAbstractRenderNode_LoadGfxState		( u32 renderPhase );
	void			MOAIAbstractRenderNode_Render			( u32 renderPhase );

public:
	
	DECL_LUA_FACTORY ( MOAITableViewLayer )
	
	//----------------------------------------------------------------//
					MOAITableViewLayer			();
					~MOAITableViewLayer			();
};

#endif
