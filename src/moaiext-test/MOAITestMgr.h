// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITESTMGR_H
#define	MOAITESTMGR_H

#include <moaicore/moaicore.h>
#include <moaiext-test/MOAITest.h>

#define REGISTER_MOAI_TEST(test) MOAITestMgr::Get ().RegisterTest < test >();

//================================================================//
// MOAITestMgr
//================================================================//
// TODO: doxygen
class MOAITestMgr :
	public MOAIGlobalClass < MOAITestMgr, MOAILuaObject > {
private:

	typedef USFactory < STLString, MOAITest >::iterator Iterator;
	USFactory < STLString, MOAITest > mFactory;

	STLString		mTestName;

	STLString		mResultsFilename;
	FILE*			mResultsFile;

	MOAILuaRef		mStagingFunc;
	MOAILuaRef		mTestFunc;
	
	bool			mStaging;

	//----------------------------------------------------------------//
	static int		_beginTest			( lua_State* L );
	static int		_comment			( lua_State* L );
	static int		_endTest			( lua_State* L );
	static int		_getTestList		( lua_State* L );
	static int		_setKeywords		( lua_State* L );
	static int		_setStagingFunc		( lua_State* L );
	static int		_setTestFunc		( lua_State* L );
	static int		_staging			( lua_State* L );

	//----------------------------------------------------------------//
	void		Finish					();
	void		PrintResult				( cc8* format, ... );

public:

	DECL_LUA_SINGLETON ( MOAITestMgr )

	//----------------------------------------------------------------//
	void		BeginTest				( cc8* testName );
	void		Comment					( cc8* comment );
	void		EndTest					( bool result );
				MOAITestMgr				();
				~MOAITestMgr			();
	void		PushTestList			( MOAILuaState& state );
	void		RegisterLuaClass		( MOAILuaState& state );
	void		RegisterLuaFuncs		( MOAILuaState& state );
	void		RunScript				( cc8* filename );
	void		RunTest					( cc8* testname );
	void		SetResultsFile			( cc8* filename );
	void		SetStaging				();

	//----------------------------------------------------------------//
	template < class TYPE >
	void RegisterTest () {
		
		TYPE test;
		this->mFactory.Register < TYPE >( test.GetName ());
	};
};

#endif
