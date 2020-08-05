// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIEXAMPLEINSTANCEDSL_H
#define	MOAIEXAMPLEINSTANCEDSL_H

#include <moai-core/MOAIAbstractDSLInterface.h>
#include <moai-core/MOAIExampleDSLInterface.h>

//================================================================//
// MOAIExampleInstanceDSL
//================================================================//
// TODO: doxygen
class MOAIExampleInstanceDSL :
	public virtual MOAIAbstractDSLInterface,
	public virtual MOAIExampleDSLInterface {
protected:

	//----------------------------------------------------------------//
	void					MOAIAbstractDSLHandler_SubmitCommand		( u32 cmd, const void* param, ZLSize paramSize ); // immediate mode

public:

	DECL_LUA_FACTORY ( MOAIExampleInstanceDSL )

	//----------------------------------------------------------------//
							MOAIExampleInstanceDSL					();
							~MOAIExampleInstanceDSL					();
};

#endif
