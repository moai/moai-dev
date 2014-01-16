//
//  MOAIEaseSineIn.h
//  libmoai
//
//  Created by Aaron Barrett on 1/2/14.
//
//

#ifndef __libmoai__MOAIEaseSineIn__
#define __libmoai__MOAIEaseSineIn__

#include <moaicore/MOAIEase.h>

//================================================================//
// MOAIEaseSineIn
//================================================================//
/**	@name MOAIEaseSineIn
 @text A sinusoidal ease in.
 */
class MOAIEaseSineIn : public virtual MOAIEase {
public:
	DECL_LUA_FACTORY( MOAIEaseSineIn );
	
	//----------------------------------------------------------------//
	MOAIEaseSineIn();
	virtual ~MOAIEaseSineIn();
	
	//----------------------------------------------------------------//
	float DistortedTime(float inputTime);
	
	//----------------------------------------------------------------//
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif /* defined(__libmoai__MOAIEaseSineIn__) */
