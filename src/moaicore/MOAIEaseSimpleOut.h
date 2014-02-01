//
//  MOAIEaseSimpleOut.h
//  libmoai
//
//  Created by Aaron Barrett on 12/30/13.
//
//

#ifndef __libmoai__MOAIEaseSimpleOut__
#define __libmoai__MOAIEaseSimpleOut__

#include <moaicore/MOAIEaseSimpleBase.h>

//================================================================//
// MOAIEaseSimpleOut
//================================================================//
/**	@name MOAIEaseSimpleOut
 @text A simple exponential ease out.
 */
class MOAIEaseSimpleOut : public virtual MOAIEaseSimpleBase {
private:
	
public:
	
	DECL_LUA_FACTORY( MOAIEaseSimpleOut );
	
	//----------------------------------------------------------------//
	MOAIEaseSimpleOut();
	virtual ~MOAIEaseSimpleOut();
	
	//----------------------------------------------------------------//
	float DistortedTime(float inputTime);
	
	//----------------------------------------------------------------//
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	
};

#endif /* defined(__libmoai__MOAIEaseSimpleOut__) */
