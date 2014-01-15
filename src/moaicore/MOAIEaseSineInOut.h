//
//  MOAIEaseSineInOut.h
//  libmoai
//
//  Created by Aaron Barrett on 1/2/14.
//
//

#ifndef __libmoai__MOAIEaseSineInOut__
#define __libmoai__MOAIEaseSineInOut__

#include <moaicore/MOAIEase.h>

//================================================================//
// MOAIEaseSineInOut
//================================================================//
/**	@name MOAIEaseSineInOut
 @text A sinusoidal ease in and out.
 */
class MOAIEaseSineInOut : public virtual MOAIEase {
public:
	DECL_LUA_FACTORY( MOAIEaseSineInOut );
	
	//----------------------------------------------------------------//
	MOAIEaseSineInOut();
	virtual ~MOAIEaseSineInOut();
	
	//----------------------------------------------------------------//
	float DistortedTime(float inputTime);
	
	//----------------------------------------------------------------//
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif /* defined(__libmoai__MOAIEaseSineInOut__) */
