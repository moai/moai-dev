// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITRANSITION_H
#define	MOAITRANSITION_H

//================================================================//
// MOAITransition
//================================================================//
/**	@brief Lua namespace to hold transition modes.
*/
class MOAITransition :
	public USLuaData {
public:
	
	DECL_LUA_SINGLETON ( MOAITransition )
	
	//----------------------------------------------------------------//
	void				RegisterLuaClass		( USLuaState& state );
};

#endif
