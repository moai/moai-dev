// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITESTMGR_H
#define	MOAITESTMGR_H

#include <tinyxml.h>
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

	STLSet < STLString > mFilter;

	typedef USFactory < STLString, MOAITest >::iterator Iterator;
	USFactory < STLString, MOAITest > mFactory;

	STLString		mTestName;
	
	STLString		mFilterFilename;

	STLString		mResultsFilename;
	FILE*			mResultsFile;

	STLString		mXmlResultsFilename;
	MOAIXmlWriter*	mXmlWriter;
	
	MOAILuaRef		mStagingFunc;
	MOAILuaRef		mTestFunc;
	
	bool			mStaging;

	//----------------------------------------------------------------//
	static int		_beginTest			( lua_State* L );
	static int		_checkFilter		( lua_State* L );
	static int		_comment			( lua_State* L );
	static int		_endTest			( lua_State* L );
	static int		_failure			( lua_State* L );
	static int		_getTestList		( lua_State* L );
	static int		_setFilter			( lua_State* L );
	static int		_setStagingFunc		( lua_State* L );
	static int		_setTestFunc		( lua_State* L );
	static int		_staging			( lua_State* L );
	static int		_success			( lua_State* L );

	//----------------------------------------------------------------//
	void			ExtendFilter			( cc8* filter );
	void			Finish					();
	static void		ParseKeywords			( USStream& stream, STLSet < STLString >& keywords );
	void			PrintResult				( cc8* format, ... );
	void			WriteFilterFile			();

public:

	DECL_LUA_SINGLETON ( MOAITestMgr )

	//----------------------------------------------------------------//
	void		BeginTest				( cc8* testName );
	bool		CheckFilter				();
	void		Comment					( cc8* comment );
	void		EndTest					( bool result );
	void		Failure					( cc8* type, cc8* detail );
				MOAITestMgr				();
				~MOAITestMgr			();
	void		PushTestList			( MOAILuaState& state );
	void		RegisterLuaClass		( MOAILuaState& state );
	void		RegisterLuaFuncs		( MOAILuaState& state );
	void		RunScript				( cc8* filename );
	void		RunTest					( cc8* testname );
	void		SetFilter				( cc8* filter, cc8* next = 0, ... );
	void		SetFilterFile			( cc8* filename );
	void		SetResultsFile			( cc8* filename );
	void		SetXmlResultsFile		( cc8* filename );
	void		SetStaging				();
	void		Success					( cc8* detail );

	//----------------------------------------------------------------//
	template < class TYPE >
	void RegisterTest () {
		
		TYPE test;
		this->mFactory.Register < TYPE >( test.GetName ());
	};
};

#endif
