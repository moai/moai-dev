//
//  MOAIEaseExponentialIn.h
//  libmoai
//
//  Created by Aaron Barrett on 1/2/14.
//
//

#ifndef __libmoai__MOAIEaseExponentialIn__
#define __libmoai__MOAIEaseExponentialIn__

#include <moaicore/MOAIEase.h>

class MOAIEaseExponentialIn : public virtual MOAIEase {
public:
	DECL_LUA_FACTORY( MOAIEaseExponentialIn );
	
	
	MOAIEaseExponentialIn();
	~MOAIEaseExponentialIn();
	
	float DistortedTime(float inputTime);
	
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif /* defined(__libmoai__MOAIEaseExponentialIn__) */
