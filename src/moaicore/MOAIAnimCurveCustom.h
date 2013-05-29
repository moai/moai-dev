//
//  MOAIAnimCurveCustom.h
//  libmoai
//
//  Created by Aaron Barrett on 5/29/13.
//
//

#ifndef	MOAIANIMCURVECUSTOM_H
#define	MOAIANIMCURVECUSTOM_H

#include <moaicore/MOAIAnimCurve.h>

class MOAIAnimCurveCustom :
	public MOAIAnimCurve {
private:
	
	MOAILuaRef mCallback; 
	
	//----------------------------------------------------------------//
	static int      _setCallback			( lua_State* L );
	
	//----------------------------------------------------------------//
	float			GetValue			( const MOAIAnimKeySpan& span ) const;
	float			InterpolateCustom	( float x0, float x1, float t, float weight) const;
	
public:
	
	DECL_LUA_FACTORY ( MOAIAnimCurveCustom )
	
	//----------------------------------------------------------------//
	MOAIAnimCurveCustom					();
	~MOAIAnimCurveCustom				();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};


#endif