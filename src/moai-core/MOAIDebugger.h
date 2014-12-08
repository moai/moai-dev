// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHARNESS_H
#define	MOAIHARNESS_H

#include <moai-core/headers.h>

#include <sstream>
#include <string>
#include <vector>

//================================================================//
// MOAIPathDictionary
//================================================================//
class MOAIPathDictionary {
private:

	struct Entry {
		unsigned int hash;
		int next;
		int idRel;
		int idAbs;
	};
	
	int						mCount;
	std::vector < int >		mBuckets;
	std::vector < Entry >	mEntries;
	std::vector < char >	mStringPool;

	//----------------------------------------------------------------//
	int				FindIdentifierAbsolute		( unsigned int hash, const char path []);
	int				FindIdentifierRelative		( unsigned int hash, const char path []);
	static uint		GetHash						( const char path [], size_t* pathLength );
	void			Grow						();
	int				MakeIdentifier				( const char path[], size_t length );
	void			SetIdentifier				( unsigned int hash, int idRel, int idAbs );

public:

	//----------------------------------------------------------------//
	int				GetIdentifier				( const char path []);
					MOAIPathDictionary			();
					~MOAIPathDictionary			();
};

//================================================================//
// MOAIBreakpoint
//================================================================//
struct MOAIBreakpoint {
	int identifier;
	unsigned int line;

	//----------------------------------------------------------------//
	inline MOAIBreakpoint(int i, unsigned int l) {identifier = i;
		line = l;
	}
};

//================================================================//
// MOAIDebugger
//================================================================//
/**	@lua	MOAIDebugger
	@text	Internal debugging and hooking class.
*/
class MOAIDebugger :
	public MOAIGlobalClass< MOAIDebugger, MOAILuaObject > {
private:

	enum {
		RUN,
		STEP_INTO,
		STEP_OVER,
		STEP_OUT
	};

	MOAIPathDictionary				mPathDictionary;
	std::vector<MOAIBreakpoint>		mBreakpoints;

	int				mStepMode;
	lua_State*		mStepState;
	int				mStepDepth;

	bool			mEnginePaused;
	
	//----------------------------------------------------------------//
	static void		_callback				( lua_State* L, lua_Debug* ar );
	static int		_sendMessage			( lua_State* L );

	//----------------------------------------------------------------//
	void			Callback				( lua_State *L, lua_Debug* ar );
	void			Pause					( lua_State *L );

public:

	DECL_LUA_SINGLETON ( MOAIDebugger )

	//----------------------------------------------------------------//
	void			HookLua					( lua_State* L );
					MOAIDebugger			();
					~MOAIDebugger			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
