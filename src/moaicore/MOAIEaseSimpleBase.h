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

class MOAIEaseSimpleBase : public virtual MOAIEase {
protected:
	float mRate;
	
private:
	static int _setRate( lua_State *L );
public:
	
	MOAIEaseSimpleBase();
	~MOAIEaseSimpleBase();
	
	void SetRate(float inputRate );
	
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	
};

#endif /* defined(__libmoai__MOAIEaseSimpleBase__) */
