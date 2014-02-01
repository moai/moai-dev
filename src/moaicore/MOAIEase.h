//
//  MOAIEase.h
//  libmoai
//
//  Created by Aaron Barrett on 12/30/13.
//
//

#ifndef __libmoai__MOAIEase__
#define __libmoai__MOAIEase__

#include <moaicore/MOAILua.h>

//================================================================//
// MOAIEase
//================================================================//
/**	@name	MOAIEase
 @text	Abstract superclass of all eases.
 */
class MOAIEase : public virtual MOAILuaObject {
private:
	
	//----------------------------------------------------------------//
	static int		_getValueAtTime		( lua_State* L );
	
public:
	
	DECL_LUA_FACTORY(MOAIEase);
	
	//----------------------------------------------------------------//
	MOAIEase();
	virtual ~MOAIEase();

	//----------------------------------------------------------------//
	virtual float DistortedTime(float inputTime);

	//----------------------------------------------------------------//
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	
	
};

#endif /* defined(__libmoai__MOAIEase__) */
