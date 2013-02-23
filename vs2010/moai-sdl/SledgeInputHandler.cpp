//#include "pch.h"
#include <uslscore/uslscore.h>
#include <moaicore-config.h>
#include "SledgeInputHandler.h"

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	classHello
	@text	Class (a.k.a. static) method. Prints the string 'SledgeInputHandler class foo!' to the console.

	@out	nil
*/
int SledgeInputHandler::_classHello ( lua_State* L ) {
	UNUSED ( L );
	
	printf ( "SledgeInputHandler class foo!\n" );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	instanceHello
	@text	Prints the string 'SledgeInputHandler instance foo!' to the console.

	@out	nil
*/
int SledgeInputHandler::_instanceHello ( lua_State* L ) {
	MOAI_LUA_SETUP ( SledgeInputHandler, "U" ) // this macro initializes the 'self' variable and type checks arguments
	
	printf ( "SledgeInputHandler instance foo!\n" );
	
	return 0;
}

int SledgeInputHandler::_setDeadzones( lua_State* L ) {
	MOAI_LUA_SETUP ( SledgeInputHandler, "UNNN" )

	printf ( "SledgeInputHandler setDeadzones!\n" );
	float left = state.GetValue < float >( 2, 0.0f );
	float right = state.GetValue < float >( 3, 0.0f );
	float triggers = state.GetValue < float >( 4, 0.0f );

	printf("%0.2f\t%0.2f\t%0.2f\t\n", left, right, triggers);

	if(_manager != NULL)
		_manager->setDeadzones(
			left,
			right,
			triggers
		);

	return 0;
}

//================================================================//
// SledgeInputHandler
//================================================================//

//----------------------------------------------------------------//
SledgeInputHandler::SledgeInputHandler () {
	
	// register all classes SledgeInputHandler derives from
	// we need this for custom RTTI implementation
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
		
		// and any other objects from multiple inheritance...
		// RTTI_EXTEND ( SledgeInputHandlerBase )
	RTTI_END
}

//----------------------------------------------------------------//
SledgeInputHandler::~SledgeInputHandler () {
}

//----------------------------------------------------------------//
void SledgeInputHandler::RegisterLuaClass ( MOAILuaState& state ) {

	// call any initializers for base classes here:
	// SledgeInputHandlerBase::RegisterLuaClass ( state );

	// also register constants:
	// state.SetField ( -1, "FOO_CONST", ( u32 )FOO_CONST );

	// here are the class methods:
	luaL_Reg regTable [] = {
		{ "classHello",		_classHello },
		{ "setDeadzones", _setDeadzones },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
	printf ( "SledgeInputHandler::RegisterLuaClass\n" );

	//doAKUInit();
}

//----------------------------------------------------------------//
void SledgeInputHandler::RegisterLuaFuncs ( MOAILuaState& state ) {

	// call any initializers for base classes here:
	// SledgeInputHandlerBase::RegisterLuaFuncs ( state );

	// here are the instance methods:
	luaL_Reg regTable [] = {
		{ "instanceHello",	_instanceHello },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

SledgeInputManager* SledgeInputHandler::_manager = NULL;

//----------------------------------------------------------------//
void SledgeInputHandler::SetManager( SledgeInputManager* p_manager )
{
	SledgeInputHandler::_manager = p_manager;
}