// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILUALOCAL_H
#define	MOAILUALOCAL_H

#include <moai-core/MOAILuaRuntime.h>

//================================================================//
// MOAILuaLocal
//================================================================//
// Use this class to safely compose Lua-bound objects and to allocate
// them on the heap. Only needed if objects will be exposed to Lua and/or
// used to pin other objects via LuaRetain ().
template < typename TYPE >
class MOAILuaLocal :
	public TYPE {
private:

	int				mRef;

public:

	//----------------------------------------------------------------//
	MOAILuaLocal () {
		this->Retain ();
		if ( MOAILuaRuntime::IsValid ()) {
			MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
			this->BindToLua ( state );
			this->mRef = MOAILuaRuntime::Get ().GetRef ( state, -1, false );
		}
	}
	
	//----------------------------------------------------------------//
	~MOAILuaLocal () {
		if ( MOAILuaRuntime::IsValid ()) {
			MOAILuaRuntime::Get ().ClearRef ( this->mRef );
		}
	}
};

#endif
