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
	
public:
	
	DECL_LUA_FACTORY ( MOAIAnimCurveCustom )
	
	//----------------------------------------------------------------//
	MOAIAnimCurveCustom				();
	~MOAIAnimCurveCustom			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};


#endif