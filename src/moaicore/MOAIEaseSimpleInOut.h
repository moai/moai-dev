//
//  MOAIEaseSimpleInOut.h
//  libmoai
//
//  Created by Aaron Barrett on 12/30/13.
//
//

#ifndef __libmoai__MOAIEaseSimpleInOut__
#define __libmoai__MOAIEaseSimpleInOut__

#include <moaicore/MOAIEaseRate.h>

class MOAIEaseSimpleInOut : public virtual MOAIEaseRate {
public:
	DECL_LUA_FACTORY( MOAIEaseSimpleInOut );
	
	MOAIEaseSimpleInOut();
	~MOAIEaseSimpleInOut();
	
	float DistortedTime( float inputTime );
	
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif /* defined(__libmoai__MOAIEaseSimpleInOut__) */
