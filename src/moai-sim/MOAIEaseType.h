// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIEASETYPE_H
#define	MOAIEASETYPE_H

//================================================================//
// MOAIEaseType
//================================================================//
/**	@name	MOAIEaseType
	@text	Namespace to hold ease modes. Moai ease in/out has opposite meaning of Flash ease in/out.
	
	@const	EASE_IN					Quartic ease in - Fast start then slow when approaching value; ease into position.
	@const	EASE_OUT				Quartic ease out - Slow start then fast when approaching value; ease out of position.
	@const	FLAT					Stepped change - Maintain original value until end of ease.
	@const	LINEAR					Linear interpolation.
	@const	SHARP_EASE_IN			t ^ 8
	@const	SHARP_EASE_OUT			t ^ 8
	@const	SHARP_SMOOTH			t ^ 8
	@const	EXTRA_SHARP_EASE_IN		t ^ 12
	@const	EXTRA_SHARP_EASE_OUT	t ^ 12
	@const	EXTRA_SHARP_SMOOTH		t ^ 12
	@const	SMOOTH					Quartic ease out then ease in.
	@const	SOFT_EASE_IN			t ^ 2
	@const	SOFT_EASE_OUT			t ^ 2
	@const	SOFT_SMOOTH				t ^ 2
*/
class MOAIEaseType :
	public MOAIGlobalClass < MOAIEaseType, MOAILuaObject > {
public:
	
	DECL_LUA_SINGLETON ( MOAIEaseType )
	
	//----------------------------------------------------------------//
	void				RegisterLuaClass		( MOAILuaState& state );
};

#endif
