//
//  MOAIEaseBackBase.h
//  libmoai
//
//  Created by Aaron Barrett on 1/1/14.
//
//

#ifndef __libmoai__MOAIEaseBackBase__
#define __libmoai__MOAIEaseBackBase__

#include <moaicore/MOAIEase.h>

//================================================================//
// MOAIEaseBackBase
//================================================================//
/**	@name MOAIEaseBackBase
 @text The supercalss for all back eases.
 */
class MOAIEaseBackBase : public virtual MOAIEase {
protected:
	float mOvershoot;
private:
	static int _setOvershoot( lua_State *L );
public:
	
	//----------------------------------------------------------------//
	MOAIEaseBackBase();
	virtual ~MOAIEaseBackBase();
	
	//----------------------------------------------------------------//
	void SetOvershoot(float overshoot );
	
	//----------------------------------------------------------------//
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	
};

#endif /* defined(__libmoai__MOAIEaseBackBase__) */