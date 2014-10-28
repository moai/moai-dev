// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISTREAMWRITER_H
#define MOAISTREAMWRITER_H

#include <moai-util/MOAIStreamAdapter.h>

//================================================================//
// MOAIStreamWriter
//================================================================//
/**	@lua	MOAIStreamWriter
	@text	This class has been deprecated. Use MOAIStreamAdapter instead.
*/
class MOAIStreamWriter :
	public virtual MOAIStreamAdapter {
public:
	
	DECL_LUA_FACTORY ( MOAIStreamWriter )

	//----------------------------------------------------------------//
					MOAIStreamWriter		();
					~MOAIStreamWriter		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
