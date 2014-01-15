//
//  MOAIEaseLinear.h
//  libmoai
//
//  Created by Aaron Barrett on 12/30/13.
//
//

#ifndef __libmoai__MOAIEaseLinear__
#define __libmoai__MOAIEaseLinear__

#include <moaicore/MOAIEase.h>

//================================================================//
// MOAIEaseLinear
//================================================================//
/**	@name MOAIEaseLinear
 @text The default ease; undistorted.
 */
class MOAIEaseLinear : public virtual MOAIEase {
public:
	DECL_LUA_FACTORY( MOAIEaseLinear );
	
	//----------------------------------------------------------------//
	MOAIEaseLinear();
	virtual ~MOAIEaseLinear();
	
	//----------------------------------------------------------------//
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif /* defined(__libmoai__MOAIEaseLinear__) */
