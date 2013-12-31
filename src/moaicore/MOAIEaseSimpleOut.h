//
//  MOAIEaseSimpleOut.h
//  libmoai
//
//  Created by Aaron Barrett on 12/30/13.
//
//

#ifndef __libmoai__MOAIEaseSimpleOut__
#define __libmoai__MOAIEaseSimpleOut__

#include <moaicore/MOAIEaseRate.h>

class MOAIEaseSimpleOut : public virtual MOAIEaseRate {
private:
	
	
public:
	
	DECL_LUA_FACTORY( MOAIEaseSimpleOut );
	
	
	MOAIEaseSimpleOut();
	~MOAIEaseSimpleOut();
	
	float DistortedTime(float inputTime);
	
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	
};

#endif /* defined(__libmoai__MOAIEaseSimpleOut__) */
