// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICMDMEDIUMADAPTER_H
#define	MOAICMDMEDIUMADAPTER_H

#include <moai-core/MOAILua.h>

class MOAIAbstractCmdAPI;
class MOAIAbstractCmdHandler;
class MOAIAbstractCmdStream;
class MOAICmdMediumPolymorphic;

//================================================================//
// MOAICmdMediumAdapter
//================================================================//
class MOAICmdMediumAdapter {
protected:

	friend class MOAIAbstractCmdMedium;
	friend class MOAICmdMedium;

	MOAILuaStrongRef				mLuaInterface;
	
	MOAIAbstractCmdStream*			mStream;
	ZLTypeID						mAPITypeID;
	MOAIAbstractCmdHandler*			mHandler;
	MOAIAbstractCmdAPI*				mAPI;
	
	//----------------------------------------------------------------//
	virtual MOAICmdMediumPolymorphic*		MOAICmdMediumAdapter_GetPolymorphic			();
	
	//----------------------------------------------------------------//
	MOAICmdMediumPolymorphic&		GetPolymorphic					();
									MOAICmdMediumAdapter			();
									MOAICmdMediumAdapter			( MOAIAbstractCmdStream& stream );
	virtual							~MOAICmdMediumAdapter			();
	void							RetainObject					( MOAILuaObject* object );
	void							SubmitCommand					( u32 cmd, const void* param, ZLSize paramSize );
};

#endif
