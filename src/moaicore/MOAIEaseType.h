// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIEASETYPE_H
#define	MOAIEASETYPE_H

//================================================================//
// MOAIEaseType
//================================================================//
/**	@brief Lua namespace to hold transition modes.
*/
class MOAIEaseType :
	public USLuaObject {
public:
	
	DECL_LUA_SINGLETON ( MOAIEaseType )
	
	//----------------------------------------------------------------//
	void				RegisterLuaClass		( USLuaState& state );
};

#endif
