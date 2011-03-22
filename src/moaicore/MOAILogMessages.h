// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILOGMESSAGES_H
#define	MOAILOGMESSAGES_H

#define MOAI_LUA_VALID_INDEX(idx,size)													\
	if ( !( idx < size )) {																\
		MOAI_ERROR ( state, MOAILogMessages::MOAI_IndexOutOfRange_DDD, idx, 0, size );	\
		return 0;																		\
	}

//================================================================//
// MOAILogMessages
//================================================================//
namespace MOAILogMessages {

	enum {
		MOAI_IndexOutOfRange_DDD,
	};
	
	//----------------------------------------------------------------//
	void	RegisterLogMessages		( USLuaState& state );
};

#endif
