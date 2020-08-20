// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTCMDHANDLER_H
#define	MOAIABSTRACTCMDHANDLER_H

#include <moai-core/MOAILua.h>

class MOAIAbstractCmdHandler;
class MOAIAbstractCmdStream;
class MOAIAbstractCmdMedium;
class MOAIAbstractCmdAPI;
class MOAIAbstractCmdMedium;
template < typename TYPE > class MOAIConcreteCmdMedium;

//================================================================//
// MOAIAbstractCmdHandler
//================================================================//
class MOAIAbstractCmdHandler :
	public virtual MOAILuaObject {
protected:

	friend class MOAIAbstractCmdMedium;
	friend class MOAIImmediateCmdStream;
	friend class MOAIAbstractCmdMedium;
	friend class MOAIRetainedCmdStream;

	//----------------------------------------------------------------//
	// implemented in MOAICmdMedium.h
	template < typename API_TYPE >
	void InitializeMedium ( MOAIAbstractCmdMedium& medium );

	//----------------------------------------------------------------//
	void			HandleCommand					( u32 cmd, const void* param );
	void			InitializeMedium				( MOAIAbstractCmdMedium& medium );

	//----------------------------------------------------------------//
	virtual void		MOAIAbstractCmdHandler_HandleCommand		( u32 cmd, const void* param ) = 0;
	virtual void		MOAIAbstractCmdHandler_InitializeMedium		( MOAIAbstractCmdMedium& medium ) = 0;

public:

	//----------------------------------------------------------------//
					MOAIAbstractCmdHandler			();
					~MOAIAbstractCmdHandler			();
};

#endif
