// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAICANARY_H
#define MOAICANARY_H

#include <moai-core/MOAILua.h>

#include <moai-core/MOAILuaState-impl.h>
#include <moai-core/MOAILuaClass-impl.h>

//================================================================//
// MOAICanary
//================================================================//
class MOAICanary :
	public virtual MOAILuaObject {
private:
	
	STLString		mMessage;
	
	//----------------------------------------------------------------//
	static int		_setMessage			( lua_State* L );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAICanary )

	//----------------------------------------------------------------//
					MOAICanary			( ZLContext& context );
					~MOAICanary			();
};

#endif
