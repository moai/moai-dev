// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIRENDERROOTGL_H
#define	MOAIRENDERROOTGL_H

//================================================================//
// MOAIRenderRootGL
//================================================================//
class MOAIRenderRootGL :
	public virtual MOAIRenderNode {
protected:

	//----------------------------------------------------------------//
	void			MOAIAbstractRenderNode_Render 		( u32 renderPhase );

public:

	//----------------------------------------------------------------//
					MOAIRenderRootGL			();
					~MOAIRenderRootGL			();
};

#endif
