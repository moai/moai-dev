//----------------------------------------------------------------//
// Copyright (c) 2010-2017 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#include <moai-sim/headers.h>
#include "moai_gtest.h"
#include "moai_gtest_MOAICmd.lua.h"

#include <moai-core/MOAIAbstractCmdAPI.h>
#include <moai-core/MOAIAbstractCmdHandler.h>
#include <moai-core/MOAIAbstractCmdStream.h>

#include <moai-core/MOAIAbstractCmdAPI.h>
#include <moai-core/MOAIAbstractCmdHandler.h>
#include <moai-core/MOAICmdMedium.h>
#include <moai-core/MOAIImmediateCmdStream.h>
#include <moai-core/MOAIRetainedCmdStream.h>

//================================================================//
// MOAITestCmdAPI
//================================================================//
// TODO: doxygen
class MOAITestCmdAPI :
	public virtual MOAIAbstractCmdAPI {
protected:

	friend class MOAISharedTestCmdHandler;
	friend class MOAITestCmdHandler;

	enum {
		CMD_FOO,
		CMD_BAR,
		CMD_BAZ,
	};

	//----------------------------------------------------------------//
	static int _dsl_bar ( lua_State* L ) {
		MOAI_LUA_CMD_SETUP ( MOAITestCmdAPI )

		self->Bar ();
		lua_pushvalue ( state, 1 );
		return 1;
	}

	//----------------------------------------------------------------//
	static int _dsl_baz ( lua_State* L ) {
		MOAI_LUA_CMD_SETUP ( MOAITestCmdAPI )

		self->Baz ();
		lua_pushvalue ( state, 1 );
		return 1;
	}

	//----------------------------------------------------------------//
	static int _dsl_foo ( lua_State* L ) {
		MOAI_LUA_CMD_SETUP ( MOAITestCmdAPI )

		self->Foo ();
		lua_pushvalue ( state, 1 );
		return 1;
	}

	//----------------------------------------------------------------//
	void MOAIAbstractCmdAPI_RegisterLuaAPI ( MOAILuaState& state ) {
	
		luaL_Reg regTable [] = {
			{ "bar",						_dsl_bar },
			{ "baz",						_dsl_baz },
			{ "foo",						_dsl_foo },
			{ NULL, NULL }
		};

		luaL_register ( state, 0, regTable );
	}

public:

	//----------------------------------------------------------------//
	MOAIConcreteCmdMedium < MOAITestCmdAPI >& Bar () {

		this->SubmitCommand ( CMD_BAR );
		
		return *this->API < MOAITestCmdAPI >();
	}

	//----------------------------------------------------------------//
	MOAIConcreteCmdMedium < MOAITestCmdAPI >& Baz () {

		this->SubmitCommand ( CMD_BAZ );
		
		return *this->API < MOAITestCmdAPI >();
	}

	//----------------------------------------------------------------//
	MOAIConcreteCmdMedium < MOAITestCmdAPI >& Foo () {

		this->SubmitCommand ( CMD_FOO );
		
		return *this->API < MOAITestCmdAPI >();
	}
};

//================================================================//
// MOAISharedTestCmdHandler
//================================================================//
// TODO: doxygen
class MOAISharedTestCmdHandler :
	public virtual MOAIAbstractCmdHandlerWithAPI < MOAITestCmdAPI >,
	public virtual ZLContextClass < MOAISharedTestCmdHandler > {
protected:

	//----------------------------------------------------------------//
	void MOAIAbstractCmdHandler_HandleCommand ( u32 cmd, const void* param ) {
		UNUSED ( param );

		switch ( cmd ) {
			case MOAITestCmdAPI::CMD_FOO:
				printf ( "FOO!\n" );
				break;
			case MOAITestCmdAPI::CMD_BAR:
				printf ( "BAR!\n" );
				break;
			case MOAITestCmdAPI::CMD_BAZ:
				printf ( "BAZ!\n" );
				break;
		}
	}

public:

	DECL_LUA_SINGLETON ( MOAISharedTestCmdHandler )

	//----------------------------------------------------------------//
	MOAISharedTestCmdHandler () {
	
		RTTI_BEGIN ( MOAISharedTestCmdHandler )
			RTTI_EXTEND ( MOAIAbstractCmdHandler )
		RTTI_END
	}
	
	//----------------------------------------------------------------//
	~MOAISharedTestCmdHandler () {
	}
};

//================================================================//
// MOAITestCmdHandler
//================================================================//
// TODO: doxygen
class MOAITestCmdHandler :
	public virtual MOAIAbstractCmdHandler {
protected:

	//----------------------------------------------------------------//
	void MOAIAbstractCmdHandler_HandleCommand ( u32 cmd, const void* param ) {
		UNUSED ( param );

		switch ( cmd ) {
			case MOAITestCmdAPI::CMD_FOO:
				printf ( "%p: FOO!\n", this );
				break;
			case MOAITestCmdAPI::CMD_BAR:
				printf ( "%p: BAR!\n", this );
				break;
			case MOAITestCmdAPI::CMD_BAZ:
				printf ( "%p: BAZ!\n", this );
				break;
		}
	}
	
	//----------------------------------------------------------------//
	void MOAIAbstractCmdHandler_InitializeMedium ( MOAIAbstractCmdMedium& medium ) {
	
		this->InitializeMedium < MOAITestCmdAPI >( medium );
	}

public:

	DECL_LUA_FACTORY ( MOAITestCmdHandler )

	//----------------------------------------------------------------//
	MOAITestCmdHandler () {
	
		RTTI_BEGIN ( MOAITestCmdHandler )
			RTTI_EXTEND ( MOAIAbstractCmdHandler )
		RTTI_END
	}
	
	//----------------------------------------------------------------//
	~MOAITestCmdHandler () {
	}
};

//----------------------------------------------------------------//
TEST_F ( GTESTMoaiContext, MOAICmd ) {
	ASSERT_TRUE ( this->mContext != 0 );
	
	printf ( "HELLO FROM MOAICmd UNIT TESTS!\n" );
	
	MOAISharedTestCmdHandler::Affirm ();
	
	REGISTER_LUA_CLASS ( MOAISharedTestCmdHandler )
	REGISTER_LUA_CLASS ( MOAITestCmdHandler )
	
	MOAISharedTestCmdHandler& handler0 = MOAISharedTestCmdHandler::Get ();
	MOAITestCmdHandler handler1;

	printf ( "IMMEDIATE from C++\n" );
	MOAIImmediateCmdStream immediate;
	immediate
		._ < MOAITestCmdAPI >( handler0 )
		.Foo ()
		.Bar ()
		.Baz ()
		._ < MOAITestCmdAPI >( handler1 )
		.Foo ()
		.Bar ()
		.Baz ()
		;
	
	MOAIRetainedCmdStream retained;
	retained
		._ < MOAITestCmdAPI >( handler0 )
		.Foo ()
		.Bar ()
		.Baz ()
		._ < MOAITestCmdAPI >( handler1 )
		.Foo ()
		.Bar ()
		.Baz ()
		;
	
	printf ( "RETAINED from C++\n" );
	retained.ExecuteBytecode ();
	
	
	printf ( "IMMEDIAGE from C++ shared\n" );
	MOAISharedTestCmdHandler::Get ()
		.Foo ()
		.Bar ()
		.Baz ()
		;
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	luaL_loadstring ( state, MOAICmd_lua );
	state.DebugCall ( 0, 0 );
}
