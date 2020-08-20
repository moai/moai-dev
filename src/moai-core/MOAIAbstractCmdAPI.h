// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTCMDAPI_H
#define	MOAIABSTRACTCMDAPI_H

#include <moai-core/MOAICmdMedium.h>
#include <moai-core/MOAILua.h>

//================================================================//
// MOAIAbstractCmdAPI
//================================================================//
class MOAIAbstractCmdAPI :
	public virtual MOAIAbstractCmdMedium {
protected:

	friend class MOAICmdMedium;

	//----------------------------------------------------------------//
	virtual void			MOAIAbstractCmdAPI_RegisterLuaAPI		( MOAILuaState& state ) = 0;

public:

	//----------------------------------------------------------------//
							MOAIAbstractCmdAPI			();
	virtual					~MOAIAbstractCmdAPI			();
};

#endif
