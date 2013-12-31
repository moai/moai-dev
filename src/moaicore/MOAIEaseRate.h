//
//  MOAIEaseRate.h
//  libmoai
//
//  Created by Aaron Barrett on 12/30/13.
//
//

#ifndef __libmoai__MOAIEaseRate__
#define __libmoai__MOAIEaseRate__

#include <moaicore/MOAIEase.h>

class MOAIEaseRate : public virtual MOAIEase {
protected:
	float mRate;
	
private:
	static int _setRate( lua_State *L );
public:
	
	MOAIEaseRate();
	~MOAIEaseRate();
	
	void SetRate(float inputRate );
	
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	
};

#endif /* defined(__libmoai__MOAIEaseRate__) */
