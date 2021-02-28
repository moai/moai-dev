// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISTREAMREADER_H
#define MOAISTREAMREADER_H

#include <moai-core/MOAIStreamAdapter.h>

//================================================================//
// MOAIStreamReader
//================================================================//
/**	@lua	MOAIStreamReader
	@text	This class has been deprecated. Use MOAIStreamAdapter instead.
*/
class MOAIStreamReader :
	public virtual MOAIStreamAdapter {
protected:

	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAIStreamReader )

	//----------------------------------------------------------------//
					MOAIStreamReader		( ZLContext& context );
					~MOAIStreamReader		();
};

#endif
