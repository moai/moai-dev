// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIEASETYPE_H
#define	MOAIEASETYPE_H

#include <moaicore/MOAILua.h>

//================================================================//
// MOAIEaseType
//================================================================//
/**	@name	MOAIEaseType
	@text	Namespace to hold ease modes. Moai ease in/out has opposite meaning of Flash ease in/out.
	
	@const	EASE_IN			Quartic ease in - Fast start then slow when approaching value; ease into position. 
	@const	EASE_OUT		Quartic ease out - Slow start then fast when approaching value; ease out of position.
	@const	FLAT			Stepped change - Maintain original value until end of ease.
	@const	LINEAR			Linear interpolation.
	@const	SHARP_EASE_IN	Octic ease in.
	@const	SHARP_EASE_OUT	Octic ease out.
	@const	SHARP_SMOOTH	Octic smooth.
	@const	SMOOTH			Quartic ease out then ease in.
	@const	SOFT_EASE_IN	Quadratic ease in.
	@const	SOFT_EASE_OUT	Quadratic ease out.
	@const	SOFT_SMOOTH		Quadratic smooth.
*/
class MOAIEaseType :
	public MOAILuaObject {
public:
	
	DECL_LUA_SINGLETON ( MOAIEaseType )
	
	//----------------------------------------------------------------//
	void				RegisterLuaClass		( MOAILuaState& state );
};

#endif
