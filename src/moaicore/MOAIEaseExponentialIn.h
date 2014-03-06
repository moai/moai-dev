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

//================================================================//
// MOAIEaseExponentialIn
//================================================================//
/**	@name MOAIEaseExponentialIn
 @text An exponentially increasing ease.
 */
class MOAIEaseExponentialIn : public virtual MOAIEase {
public:
	DECL_LUA_FACTORY( MOAIEaseExponentialIn );
	
	//----------------------------------------------------------------//
	MOAIEaseExponentialIn();
	virtual ~MOAIEaseExponentialIn();
	
	//----------------------------------------------------------------//
	float DistortedTime(float inputTime);
	
	//----------------------------------------------------------------//
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif /* defined(__libmoai__MOAIEaseExponentialIn__) */
