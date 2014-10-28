// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISTREAMREADER_H
#define MOAISTREAMREADER_H

#include <moai-util/MOAIStreamAdapter.h>

//================================================================//
// MOAIStreamReader
//================================================================//
/**	@lua	MOAIStreamReader
	@text	This class has been deprecated. Use MOAIStreamAdapter instead.
*/
class MOAIStreamReader :
	public virtual MOAIStreamAdapter {
public:
	
	DECL_LUA_FACTORY ( MOAIStreamReader )

	//----------------------------------------------------------------//
					MOAIStreamReader		();
					~MOAIStreamReader		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
