//
//  MOAIEaseElasticBase.h
//  libmoai
//
//  Created by Aaron Barrett on 1/1/14.
//
//

#ifndef __libmoai__MOAIEaseElasticBase__
#define __libmoai__MOAIEaseElasticBase__

#include <moaicore/MOAIEase.h>

//================================================================//
// MOAIEaseElasticBase
//================================================================//
/**	@name MOAIEaseElasticBase
 @text The superclass for all elastic eases.
 */
class MOAIEaseElasticBase : public virtual MOAIEase {
protected:
	float mPeriod;
private:
	static int _setPeriod( lua_State *L );
public:
	
	//----------------------------------------------------------------//
	MOAIEaseElasticBase();
	virtual ~MOAIEaseElasticBase();
	
	//----------------------------------------------------------------//
	void SetPeriod(float period );
	
	//----------------------------------------------------------------//
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif /* defined(__libmoai__MOAIEaseElasticBase__) */
