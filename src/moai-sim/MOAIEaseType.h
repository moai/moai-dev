// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIEASETYPE_H
#define	MOAIEASETYPE_H

//================================================================//
// MOAIEaseType
//================================================================//
/**	@lua	MOAIEaseType
	@text	Namespace to hold ease modes. Moai ease in/out has opposite meaning of Flash ease in/out.
	
	@const	EASE_IN							t ^ 4 - Fast start then slow when approaching value; ease into position.
	@const	EASE_OUT						t ^ 4 - Slow start then fast when approaching value; ease out of position.
	@const	EXTRA_SHARP_EASE_IN				t ^ 16
	@const	EXTRA_SHARP_EASE_OUT			t ^ 16
	@const	EXTRA_SHARP_SMOOTH				t ^ 16
	@const	EXTRA_SHARP_SMOOTH_EASE_OUT		t ^ 16
	@const	FLAT							Stepped change - Maintain original value until end of ease.
	@const	LINEAR							Linear interpolation.
	@const	SHARP_EASE_IN					t ^ 8
	@const	SHARP_EASE_OUT					t ^ 8
	@const	SHARP_SMOOTH					t ^ 8
	@const	SHARP_SMOOTH_EASE_OUT			t ^ 8
	@const	SMOOTH							t ^ 4 - Ease out then ease in.
	@const	SMOOTH_EASE_OUT					t ^ 4 - Ease out then ease in scaled by ease out.
	@const	SOFT_EASE_IN					t ^ 2
	@const	SOFT_EASE_OUT					t ^ 2
	@const	SOFT_SMOOTH						t ^ 2
	@const	SOFT_SMOOTH_EASE_OUT			t ^ 2
	@const	SINE_EASE_IN	
	@const	SINE_EASE_OUT		
	@const	SINE_SMOOTH		
	@const	CIRC_EASE_IN		
	@const	CIRC_EASE_OUT		
	@const	CIRC_SMOOTH		
	@const	BOUNCE_IN		
	@const	BOUNCE_OUT		
	@const	BOUNCE_SMOOTH		
	@const	ELASTIC_IN		
	@const	ELASTIC_OUT		
	@const	ELASTIC_SMOOTH		
	@const	BACK_EASE_IN		
	@const	BACK_EASE_OUT		
	@const	BACK_SMOOTH		
*/
class MOAIEaseType :
	public MOAIGlobalClass < MOAIEaseType, MOAILuaObject > {
public:
	
	DECL_LUA_SINGLETON ( MOAIEaseType )
	
	//----------------------------------------------------------------//
	void				RegisterLuaClass		( MOAILuaState& state );
};

#endif
