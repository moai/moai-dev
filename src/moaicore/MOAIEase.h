//
//  MOAIEase.h
//  libmoai
//
//  Created by Aaron Barrett on 12/30/13.
//
//

#ifndef __libmoai__MOAIEase__
#define __libmoai__MOAIEase__

#include <moaicore/MOAILua.h>

// Abstract superclass of all eases.

class MOAIEase : public virtual MOAILuaObject {
private:
	int mEaseType;  // default to linear
	
	//----------------------------------------------------------------//
	
	static int _setEaseType( lua_State* L );
	
	
public:
	
	DECL_LUA_FACTORY(MOAIEase);
	
	MOAIEase();
	~MOAIEase();
	
	virtual float DistortedTime(float inputTime);
	
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	
	
};

#endif /* defined(__libmoai__MOAIEase__) */
