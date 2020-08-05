// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMMEDIATEDSLSTREAM_H
#define	MOAIIMMEDIATEDSLSTREAM_H

#include <moai-core/MOAIAbstractDSLStream.h>

//================================================================//
// MOAIImmediateDSLStream
//================================================================//
// TODO: doxygen
class MOAIImmediateDSLStream :
	public virtual MOAIAbstractDSLStream {
protected:

	//----------------------------------------------------------------//
	void			MOAIAbstractDSLHandler_SubmitCommand	( u32 cmd, const void* param, ZLSize paramSize );

public:

	DECL_LUA_FACTORY ( MOAIImmediateDSLStream )

	//----------------------------------------------------------------//
					MOAIImmediateDSLStream					();
					~MOAIImmediateDSLStream					();
};

#endif
