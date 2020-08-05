// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIEXAMPLESHAREDDSL_H
#define	MOAIEXAMPLESHAREDDSL_H

#include <moai-core/MOAIAbstractDSLInterface.h>
#include <moai-core/MOAIExampleDSLInterface.h>

//================================================================//
// MOAIExampleSharedDSL
//================================================================//
// TODO: doxygen
class MOAIExampleSharedDSL :
	public ZLContextClass < MOAIExampleSharedDSL >,
	public virtual MOAIAbstractDSLInterface,
	public virtual MOAIExampleDSLInterface {
protected:

	//----------------------------------------------------------------//
	void					MOAIAbstractDSLHandler_SubmitCommand		( u32 cmd, const void* param, ZLSize paramSize ); // immediate mode

public:

	DECL_LUA_SINGLETON ( MOAIExampleSharedDSL )

	//----------------------------------------------------------------//
							MOAIExampleSharedDSL					();
							~MOAIExampleSharedDSL					();
};

#endif
