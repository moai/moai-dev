//
//  MOAIEaseBackIn.h
//  libmoai
//
//  Created by Aaron Barrett on 1/1/14.
//
//

#ifndef __libmoai__MOAIEaseBackIn__
#define __libmoai__MOAIEaseBackIn__

#include <moaicore/MOAIEaseBackBase.h>

class MOAIEaseBackIn : public virtual MOAIEaseBackBase {
	
public:
	
	DECL_LUA_FACTORY( MOAIEaseBackIn );
	
	
	MOAIEaseBackIn();
	~MOAIEaseBackIn();
	
	float DistortedTime(float inputTime);
	
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	
};

#endif /* defined(__libmoai__MOAIEaseBackIn__) */
