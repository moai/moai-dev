//
//  MOAIEaseExponentialOut.h
//  libmoai
//
//  Created by Aaron Barrett on 1/2/14.
//
//

#ifndef __libmoai__MOAIEaseExponentialOut__
#define __libmoai__MOAIEaseExponentialOut__

#include <moaicore/MOAIEase.h>

class MOAIEaseExponentialOut : public virtual MOAIEase {
public:
	DECL_LUA_FACTORY( MOAIEaseExponentialOut );
	
	
	MOAIEaseExponentialOut();
	~MOAIEaseExponentialOut();
	
	float DistortedTime(float inputTime);
	
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif /* defined(__libmoai__MOAIEaseExponentialOut__) */
