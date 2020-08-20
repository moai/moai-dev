// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMMEDIATECMDSTREAM_H
#define	MOAIIMMEDIATECMDSTREAM_H

#include <moai-core/MOAIAbstractCmdStream.h>
#include <moai-core/MOAILua.h>

class MOAIAbstractCmdHandler;
class MOAIAbstractCmdStream;

//================================================================//
// MOAIImmediateCmdStream
//================================================================//
class MOAIImmediateCmdStream :
	public virtual MOAIAbstractCmdStream {
protected:

	//----------------------------------------------------------------//
	void		MOAIAbstractCmdStream_RetainObject		( MOAILuaObject* object );
	void		MOAIAbstractCmdStream_SubmitCommand		( MOAIAbstractCmdHandler& handler, u32 cmd, const void* param, ZLSize paramSize );

public:

	DECL_LUA_FACTORY ( MOAIImmediateCmdStream )

	//----------------------------------------------------------------//
				MOAIImmediateCmdStream					();
				~MOAIImmediateCmdStream					();
};

#endif
