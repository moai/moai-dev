//
//  MOAIEaseElasticBase.cpp
//  libmoai
//
//  Created by Aaron Barrett on 1/1/14.
//
//

#include "pch.h"
#include "MOAIEaseElasticBase.h"

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	setPeriod
 @text	Sets the period value for the ease.
 
 @in		MOAIEaseSimpleBase self
 @in		number period
 @out	nil
 */
int MOAIEaseElasticBase::_setPeriod( lua_State *L ){
	MOAI_LUA_SETUP( MOAIEaseElasticBase, "UN" );
	
	float period = state.GetValue < float >( 2, 0 );
	self->SetPeriod(period);
	
	return 0;
}

//================================================================//
// MOAIEaseElasticBase
//================================================================//

//----------------------------------------------------------------//
void MOAIEaseElasticBase::SetPeriod(float period){
	this->mPeriod = period;
}

//----------------------------------------------------------------//
MOAIEaseElasticBase::MOAIEaseElasticBase()
:mPeriod(0.3f)
{
	RTTI_SINGLE(MOAIEase)
}

//----------------------------------------------------------------//
MOAIEaseElasticBase::~MOAIEaseElasticBase(){
	
}

//----------------------------------------------------------------//
void MOAIEaseElasticBase::RegisterLuaClass(MOAILuaState &state){
	MOAIEase::RegisterLuaClass(state);
}

//----------------------------------------------------------------//
void MOAIEaseElasticBase::RegisterLuaFuncs(MOAILuaState &state){
	MOAIEase::RegisterLuaFuncs(state);
	
	luaL_Reg regTable [] = {
		{ "setPeriod",				_setPeriod },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

