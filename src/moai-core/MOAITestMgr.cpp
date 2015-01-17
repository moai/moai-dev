// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-test/MOAITestKeywords.h>
#include <moai-test/MOAITestMgr.h>

#ifndef va_copy
	#define va_copy(d,s) ((d) = (s))
#endif

//================================================================//
// local
//================================================================//

////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAITestMgr::_beginTest ( lua_State* L ) {
//	MOAILuaState state ( L );
//	
//	cc8* testName = state.GetValue < cc8* >( 1, 0 );
//	if ( testName ) {
//		MOAITestMgr::Get ().BeginTest ( testName );
//	}
//	return 0;
//}
//
////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAITestMgr::_checkFilter ( lua_State* L ) {
//	MOAILuaState state ( L );
//
//	state.Push ( MOAITestMgr::Get ().CheckFilter ());
//	return 1;
//}
//
////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAITestMgr::_comment ( lua_State* L ) {
//	MOAILuaState state ( L );
//	
//	cc8* comment = state.GetValue < cc8* >( 1, 0 );
//	if ( comment ) {
//		MOAITestMgr::Get ().Comment ( comment );
//	}
//	return 0;
//}
//
////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAITestMgr::_endTest ( lua_State* L ) {
//	MOAILuaState state ( L );
//	
//	bool result = state.GetValue < bool >( 1, false );
//	MOAITestMgr::Get ().EndTest ( result );
//	return 0;
//}
//
////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAITestMgr::_failure ( lua_State* L ) {
//	MOAILuaState state ( L );
//	
//	cc8* type = state.GetValue < cc8* >( 1, "" );
//	cc8* detail = state.GetValue < cc8* >( 2, "" );
//
//	MOAITestMgr::Get ().Failure ( type, detail );
//	return 0;
//}
//
////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAITestMgr::_getTestList ( lua_State* L ) {
//	MOAILuaState state ( L );
//	
//	MOAITestMgr::Get ().PushTestList ( state );
//	return 1;
//}
//
////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAITestMgr::_setFilter ( lua_State* L ) {
//	MOAILuaState state ( L );
//	
//	MOAITestMgr::Get ().SetFilter ( state.GetValue < cc8* >( 1, "" ));
//	return 0;
//}
//
////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAITestMgr::_setStagingFunc ( lua_State* L ) {
//	MOAILuaState state ( L );
//	
//	MOAITestMgr::Get ().mStagingFunc.SetRef ( state, 1 );
//	return 0;
//}
//
////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAITestMgr::_setTestFunc ( lua_State* L ) {
//	MOAILuaState state ( L );
//	
//	MOAITestMgr::Get ().mTestFunc.SetRef ( state, 1 );
//	return 0;
//}
//
////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAITestMgr::_staging ( lua_State* L ) {
//	MOAILuaState state ( L );
//	
//	state.Push ( true );
//	return 1;
//}

//================================================================//
// MOAITestMgr
//================================================================//

//----------------------------------------------------------------//
MOAITestMgr::MOAITestMgr () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAITestMgr::~MOAITestMgr () {
}

//----------------------------------------------------------------//
void MOAITestMgr::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
	
//	state.SetField ( -1, "MATH",		MOAI_TEST_MATH );
//	state.SetField ( -1, "SAMPLE",		MOAI_TEST_SAMPLE );
//	state.SetField ( -1, "UTIL",		MOAI_TEST_UTIL );
//	
//	luaL_Reg regTable [] = {
//		{ "beginTest",				_beginTest },
//		{ "checkFilter",			_checkFilter },
//		{ "comment",				_comment },
//		{ "endTest",				_endTest },
//		{ "failure",				_failure },
//		{ "getTestList",			_getTestList },
//		{ "setFilter",				_setFilter },
//		{ "setStagingFunc",			_setStagingFunc },
//		{ "setTestFunc",			_setTestFunc },
//		{ "staging",				_staging },
//		{ NULL, NULL }
//	};
//
//	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITestMgr::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}
