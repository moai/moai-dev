//
//  MOAIHusky.h
//  libmoai
//
//  Created by Brendan Ragan on 20/12/13.
//
//

#ifndef __libmoai__MOAIHusky__
#define __libmoai__MOAIHusky__

#include <dlfcn.h>
#include "Husky.h"

bool endsWith(const char *string, const char *ending);

class HuskyLoaderHandle {
public:
	HuskyLoaderHandle();
	HuskyLoaderHandle(void* dllhandle);
	void* dllhandle;
};

using LoaderHandleMap = std::map<std::string, HuskyLoaderHandle>;

class MOAIHusky : public MOAIGlobalClass <MOAIHusky, MOAILuaObject>, public HuskyObserver {
private:
	static int _getAvailable							( lua_State *L );
	static int _getCurrent							( lua_State *L );
	static int _setCurrent							( lua_State *L );
	static int _achievementReset						( lua_State *L );
	static int _achievementSet						( lua_State *L );
	static int _achievementSetCallback				( lua_State *L );
	static int _leaderboardUploadScore				( lua_State *L );
	static int _leaderboardSetScoreCallback			( lua_State *L );
	static int _leaderboardGetScores					( lua_State *L );
	static int _leaderboardSetGetScoresCallback		( lua_State *L );
	static int _doTick								( lua_State *L );
	
	void HuskyObserverAchievementCallback(const char *name, bool success);
	void HuskyObserverLeaderboardScoreSetCallback(const char *name, bool success);
	void HuskyObserverLeaderboardScoreGetCallback(const char *name, HuskyLeaderboardEntry *entries, int number);

	MOAILuaLocal _achievementCallback;
	MOAILuaLocal _leaderboardScoreSetCallback;
	MOAILuaLocal _leaderboardScoreGetCallback;
public:
	DECL_LUA_SINGLETON ( MOAIHusky )
	
							MOAIHusky	(  );
							~MOAIHusky	(  );
	//----------------------------------------------------------------//
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
	
	LoaderHandleMap *_map;
	void* _currentHuskyHandle;
	Husky* _instance;
	HuskyGetName* _fHuskyName;
	HuskyShutdownStaticInstance* _fHuskyShutdown;
};



#endif /* defined(__libmoai__MOAIHusky__) */
