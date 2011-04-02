// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIEASETYPE_H
#define	MOAIEASETYPE_H

//================================================================//
// MOAIEaseType
//================================================================//
/**	@brief	Namespace to hold ease modes.
	@const EASE_IN
	@const EASE_OUT
	@const FLAT
	@const LINEAR
	@const SMOOTH
	@const SOFT_EASE_IN
	@const SOFT_EASE_OUT
	@const SOFT_SMOOTH
*/
class MOAIEaseType :
	public USLuaObject {
public:
	
	DECL_LUA_SINGLETON ( MOAIEaseType )
	
	//----------------------------------------------------------------//
	void				RegisterLuaClass		( USLuaState& state );
};

#endif
