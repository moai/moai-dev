//
//  MOAIEaseBackInOut.h
//  libmoai
//
//  Created by Aaron Barrett on 1/1/14.
//
//

#ifndef __libmoai__MOAIEaseBackInOut__
#define __libmoai__MOAIEaseBackInOut__

#include <moaicore/MOAIEaseBackBase.h>

//================================================================//
// MOAIEaseBackInOut
//================================================================//
/**	@name MOAIEaseBackInOut
 @text A back ease in and out.
 */
class MOAIEaseBackInOut : public virtual MOAIEaseBackBase {
public:
	
	DECL_LUA_FACTORY( MOAIEaseBackInOut );
	
	//----------------------------------------------------------------//
	MOAIEaseBackInOut();
	virtual ~MOAIEaseBackInOut();
	
	//----------------------------------------------------------------//
	float DistortedTime(float inputTime);
	
	//----------------------------------------------------------------//
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif /* defined(__libmoai__MOAIEaseBackInOut__) */
