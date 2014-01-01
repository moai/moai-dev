//
//  MOAIEaseBackOut.h
//  libmoai
//
//  Created by Aaron Barrett on 1/1/14.
//
//

#ifndef __libmoai__MOAIEaseBackOut__
#define __libmoai__MOAIEaseBackOut__

#include <moaicore/MOAIEaseBackBase.h>

class MOAIEaseBackOut : public virtual MOAIEaseBackBase {
	
public:
	
	DECL_LUA_FACTORY( MOAIEaseBackOut );
	
	
	MOAIEaseBackOut();
	~MOAIEaseBackOut();
	
	float DistortedTime(float inputTime);
	
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	
};

#endif /* defined(__libmoai__MOAIEaseBackOut__) */
