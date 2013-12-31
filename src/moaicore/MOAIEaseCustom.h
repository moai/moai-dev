//
//  MOAIEaseCustom.h
//  libmoai
//
//  Created by Aaron Barrett on 12/30/13.
//
//

#ifndef __libmoai__MOAIEaseCustom__
#define __libmoai__MOAIEaseCustom__

#include <MOAIEase.h>

class MOAIEaseCustom : public virtual MOAIEase {
	
private:
	
	MOAILuaRef mFunction;
	
	static int _setFunction( lua_State *L );
	
	
public:
	
	DECL_LUA_FACTORY(MOAIEaseCustom);
	
	float			DistortedTime( float inputTime );
	
	MOAIEaseCustom();
	~MOAIEaseCustom();
	
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	
	
};

#endif /* defined(__libmoai__MOAIEaseCustom__) */
