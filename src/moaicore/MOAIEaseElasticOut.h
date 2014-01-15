//
//  MOAIEaseElasticOut.h
//  libmoai
//
//  Created by Aaron Barrett on 1/2/14.
//
//

#ifndef __libmoai__MOAIEaseElasticOut__
#define __libmoai__MOAIEaseElasticOut__

#include <moaicore/MOAIEaseElasticBase.h>

//================================================================//
// MOAIEaseElasticOut
//================================================================//
/**	@name MOAIEaseElasticOut
 @text A springy ease out.
 */
class MOAIEaseElasticOut : public virtual MOAIEaseElasticBase {
public:
	
	DECL_LUA_FACTORY( MOAIEaseElasticOut );
	
	//----------------------------------------------------------------//
	MOAIEaseElasticOut();
	virtual ~MOAIEaseElasticOut();
	
	//----------------------------------------------------------------//
	float DistortedTime(float inputTime);
	
	//----------------------------------------------------------------//
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif /* defined(__libmoai__MOAIEaseElasticOut__) */