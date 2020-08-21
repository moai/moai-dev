// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTCMDHANDLER_H
#define	MOAIABSTRACTCMDHANDLER_H

#include <moai-core/MOAILua.h>

class MOAIAbstractCmdAPI;
class MOAIAbstractCmdHandler;
class MOAIAbstractCmdStream;
class MOAIAbstractCmdMedium;
class MOAICmdMediumPolymorphic;
template < typename TYPE > class MOAICmdMediumWithAPI;

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
	template < typename API_TYPE >
	void InitializePolymorphic ( MOAICmdMediumPolymorphic& polymorphic ); // implemented in MOAICmdMediumPolymorphic.h

	//----------------------------------------------------------------//
	void			HandleCommand					( u32 cmd, const void* param );
	void			InitializePolymorphic			( MOAICmdMediumPolymorphic& polymorphic );

	//----------------------------------------------------------------//
	virtual void	MOAIAbstractCmdHandler_HandleCommand			( u32 cmd, const void* param ) = 0;
	virtual void	MOAIAbstractCmdHandler_InitializePolymorphic	( MOAICmdMediumPolymorphic& polymorphic ) = 0;

public:

	//----------------------------------------------------------------//
					MOAIAbstractCmdHandler			();
					~MOAIAbstractCmdHandler			();
};

#endif
