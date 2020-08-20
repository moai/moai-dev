// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTCMDHANDLERWITHAPI_H
#define	MOAIABSTRACTCMDHANDLERWITHAPI_H

#include <moai-core/MOAIAbstractCmdAPI.h>
#include <moai-core/MOAIAbstractCmdHandler.h>
#include <moai-core/MOAICmdMedium.h>

//================================================================//
// MOAIAbstractCmdHandlerWithAPI
//================================================================//
template < typename API_TYPE >
class MOAIAbstractCmdHandlerWithAPI :
	public virtual MOAIConcreteCmdMedium < API_TYPE >,
	public virtual MOAIAbstractCmdHandler {
protected:

	//----------------------------------------------------------------//
	void MOAIAbstractCmdHandler_InitializeMedium ( MOAIAbstractCmdMedium& medium ) {
	
		this->template InitializeMedium < API_TYPE >( medium );
	}

	//----------------------------------------------------------------//
	void MOAIAbstractCmdMedium_RetainObject ( MOAILuaObject* object ) {
		UNUSED ( object );
	}
	
	//----------------------------------------------------------------//
	void MOAIAbstractCmdMedium_SubmitCommand ( u32 cmd, const void* param, ZLSize paramSize ) {
		UNUSED ( paramSize );
		this->HandleCommand ( cmd, param );
	}

public:

	//----------------------------------------------------------------//
	MOAIAbstractCmdHandlerWithAPI () {
	}
	
	//----------------------------------------------------------------//
	~MOAIAbstractCmdHandlerWithAPI () {
	}
};

#endif
