//
//  MOAIEaseBackOut.h
//  libmoai
//
//  Created by Aaron Barrett on 1/1/14.
//
//

#ifndef __libmoai__MOAIEaseBackOut__
#define __libmoai__MOAIEaseBackOut__

#include <moaicore/MOAIEaseBackBase.h>

//================================================================//
// MOAIEaseBackOut
//================================================================//
/**	@name MOAIEaseBackOut
 @text A back ease out.
 */
class MOAIEaseBackOut : public virtual MOAIEaseBackBase {
public:
	
	DECL_LUA_FACTORY( MOAIEaseBackOut );
	
	//----------------------------------------------------------------//
	MOAIEaseBackOut();
	virtual ~MOAIEaseBackOut();
	
	//----------------------------------------------------------------//
	float DistortedTime(float inputTime);
	
	//----------------------------------------------------------------//
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	
};

#endif /* defined(__libmoai__MOAIEaseBackOut__) */
