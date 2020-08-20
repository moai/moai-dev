// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTGFXSCRIPTCALLBACK_H
#define	MOAIABSTRACTGFXSCRIPTCALLBACK_H

#include <moai-gfx/MOAIDrawAPI.h>

//================================================================//
// MOAIAbstractGfxScriptCallback
//================================================================//
class MOAIAbstractGfxScriptCallback :
	public MOAIAbstractCmdStreamFilter {
private:

	ZLSize			mPhase;
	bool			mDidDraw;

	//----------------------------------------------------------------//
	void			Invoke								();

	//----------------------------------------------------------------//
	bool				MOAIAbstractCmdStreamFilter_FilterCommand		( u32 cmd, const void* param, ZLSize paramSize );
	virtual void		MOAIAbstractGfxScriptCallback_Call				() = 0;

public:

	//----------------------------------------------------------------//
	void			Flush								();
					MOAIAbstractGfxScriptCallback		();
					~MOAIAbstractGfxScriptCallback		();
	void			Reset								();
};

#endif
