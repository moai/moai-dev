//
//  MOAIEaseElasticIn.h
//  libmoai
//
//  Created by Aaron Barrett on 1/1/14.
//
//

#ifndef __libmoai__MOAIEaseElasticIn__
#define __libmoai__MOAIEaseElasticIn__

#include <moaicore/MOAIEaseElasticBase.h>

//================================================================//
// MOAIEaseElasticIn
//================================================================//
/**	@name MOAIEaseElasticIn
 @text A springy ease in.
 */
class MOAIEaseElasticIn : public virtual MOAIEaseElasticBase {
public:
	
	DECL_LUA_FACTORY( MOAIEaseElasticIn );
	
	//----------------------------------------------------------------//
	MOAIEaseElasticIn();
	virtual ~MOAIEaseElasticIn();
	
	//----------------------------------------------------------------//
	float DistortedTime(float inputTime);
	
	//----------------------------------------------------------------//
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif /* defined(__libmoai__MOAIEaseElasticIn__) */
