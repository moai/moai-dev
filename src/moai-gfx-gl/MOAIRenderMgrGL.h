// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIRENDERMGRGL_H
#define	MOAIRENDERMGRGL_H

//================================================================//
// MOAIRenderMgrGL
//================================================================//
// TODO: doxygen
class MOAIRenderMgrGL :
	public ZLContextClass < MOAIRenderMgrGL >,
	public virtual MOAIAbstractRenderMgr {
private:

	//----------------------------------------------------------------//
	void			MOAIAbstractRenderMgr_Render	();

public:

	//----------------------------------------------------------------//
					MOAIRenderMgrGL					();
					~MOAIRenderMgrGL				();
	
	//----------------------------------------------------------------//
	// disambiguate
	static MOAIRenderMgrGL& Get () {
		return ZLContextClass < MOAIRenderMgrGL >::Get ();
	}
};

#endif
