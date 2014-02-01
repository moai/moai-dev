//
//  MOAIEaseSimpleBase.h
//  libmoai
//
//  Created by Aaron Barrett on 12/30/13.
//
//

#ifndef __libmoai__MOAIEaseSimpleBase__
#define __libmoai__MOAIEaseSimpleBase__

#include <moaicore/MOAIEase.h>

//================================================================//
// MOAIEaseSimpleBase
//================================================================//
/**	@name MOAIEaseSimpleBase
 @text The superclass for all Simple eases.
 */
class MOAIEaseSimpleBase : public virtual MOAIEase {
protected:
	float mRate;
	
private:
	static int _setRate( lua_State *L );
public:
	
	MOAIEaseSimpleBase();
	virtual ~MOAIEaseSimpleBase();
	
	//----------------------------------------------------------------//
	void SetRate(float inputRate );
	
	//----------------------------------------------------------------//
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	
};

#endif /* defined(__libmoai__MOAIEaseSimpleBase__) */
