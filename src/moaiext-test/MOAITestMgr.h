// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITESTMGR_H
#define	MOAITESTMGR_H

#include <moaicore/moaicore.h>

//================================================================//
// MOAITestMgr
//================================================================//
// TODO: doxygen
class MOAITestMgr :
	public MOAIGlobalClass < MOAITestMgr, MOAILuaObject > {
private:

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
	void		RegisterLuaClass		( MOAILuaState& state );
	void		RegisterLuaFuncs		( MOAILuaState& state );
	void		RunScript				( cc8* filename );
	void		SetResultsFile			( cc8* filename );
	void		SetStaging				();
};

#endif
