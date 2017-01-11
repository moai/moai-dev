// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIFOO_H
#define MOAIFOO_H

#include <moai-core/MOAILua.h>

#include <moai-core/MOAILuaState-impl.h>
#include <moai-core/MOAILuaClass-impl.h>

//================================================================//
// MOAILuaObjectWrapper
//================================================================//
// TODO: doxygen
class MOAILuaObjectWrapper :
	public virtual MOAILuaObject {
private:
	
	MOAILuaStrongRef	mWrap;

public:
	
	DECL_LUA_FACTORY ( MOAILuaObjectWrapper )

	//----------------------------------------------------------------//
					MOAILuaObjectWrapper			();
					~MOAILuaObjectWrapper			();
	void			RegisterLuaClass				( MOAILuaState& state );
	void			RegisterLuaFuncs				( MOAILuaState& state );
};

#endif
