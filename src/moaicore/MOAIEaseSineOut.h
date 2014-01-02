//
//  MOAIEaseSineOut.h
//  libmoai
//
//  Created by Aaron Barrett on 1/2/14.
//
//

#ifndef __libmoai__MOAIEaseSineOut__
#define __libmoai__MOAIEaseSineOut__

#include <moaicore/MOAIEase.h>

class MOAIEaseSineOut : public virtual MOAIEase {
public:
	DECL_LUA_FACTORY( MOAIEaseSineOut );
	
	
	MOAIEaseSineOut();
	~MOAIEaseSineOut();
	
	float DistortedTime(float inputTime);
	
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif /* defined(__libmoai__MOAIEaseSineOut__) */
