//
//  MOAIEaseSimpleIn.h
//  libmoai
//
//  Created by Aaron Barrett on 12/30/13.
//
//

#ifndef __libmoai__MOAIEaseSimpleIn__
#define __libmoai__MOAIEaseSimpleIn__

#include <moaicore/MOAIEaseRate.h>

class MOAIEaseSimpleIn : public virtual MOAIEaseRate {

public:
	
	DECL_LUA_FACTORY( MOAIEaseSimpleIn );
	
	
	MOAIEaseSimpleIn();
	~MOAIEaseSimpleIn();
	
	float DistortedTime(float inputTime);
	
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	
};

#endif /* defined(__libmoai__MOAIEaseSimpleIn__) */
