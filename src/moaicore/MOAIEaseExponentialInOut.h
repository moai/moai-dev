//
//  MOAIEaseExponentialInOut.h
//  libmoai
//
//  Created by Aaron Barrett on 1/2/14.
//
//

#ifndef __libmoai__MOAIEaseExponentialInOut__
#define __libmoai__MOAIEaseExponentialInOut__

#include <moaicore/MOAIEase.h>

//================================================================//
// MOAIEaseExponentialInOut
//================================================================//
/**	@name MOAIEaseExponentialInOut
 @text An exponentially increasing, then decreasing ease.
 */
class MOAIEaseExponentialInOut : public virtual MOAIEase {
public:
	DECL_LUA_FACTORY( MOAIEaseExponentialInOut );
	
	//----------------------------------------------------------------//
	MOAIEaseExponentialInOut();
	virtual ~MOAIEaseExponentialInOut();
	
	//----------------------------------------------------------------//
	float DistortedTime(float inputTime);
	
	//----------------------------------------------------------------//
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif /* defined(__libmoai__MOAIEaseExponentialInOut__) */
