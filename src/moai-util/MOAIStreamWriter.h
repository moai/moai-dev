// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
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
protected:
	
	//----------------------------------------------------------------//
	void			MOAILuaObject_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );
	
public:
	
	DECL_LUA_FACTORY ( MOAIStreamWriter )

	//----------------------------------------------------------------//
					MOAIStreamWriter		();
					~MOAIStreamWriter		();
};

#endif
