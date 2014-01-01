//
//  MOAIEaseBackInOut.h
//  libmoai
//
//  Created by Aaron Barrett on 1/1/14.
//
//

#ifndef __libmoai__MOAIEaseBackInOut__
#define __libmoai__MOAIEaseBackInOut__

#include <moaicore/MOAIEaseBackBase.h>

class MOAIEaseBackInOut : public virtual MOAIEaseBackBase {
	
public:
	
	DECL_LUA_FACTORY( MOAIEaseBackInOut );
	
	
	MOAIEaseBackInOut();
	~MOAIEaseBackInOut();
	
	float DistortedTime(float inputTime);
	
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	
};

#endif /* defined(__libmoai__MOAIEaseBackInOut__) */
