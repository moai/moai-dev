//
//  MOAIHusky.h
//  libmoai
//
//  Created by Brendan Ragan on 20/12/13.
//
//

#ifndef __libmoai__MOAIHusky__
#define __libmoai__MOAIHusky__

#ifdef __APPLE__
	#include <dlfcn.h>
#elif WIN32
	
#endif
#include <Husky.h>

bool endsWith(const char *string, const char *ending);

class HuskyLoaderHandle {
public:
	HuskyLoaderHandle();
	HuskyLoaderHandle(void* dllhandle);
	void* dllhandle;
};

typedef std::map<std::string, HuskyLoaderHandle> LoaderHandleMap;
//using LoaderHandleMap = std::map<std::string, HuskyLoaderHandle>;

class MOAIHusky : public MOAIGlobalClass <MOAIHusky, MOAILuaObject>, public HuskyObserver {
private:
	static int _getAvailable							( lua_State *L );
	static int _getCurrent							( lua_State *L );
	static int _setCurrent							( lua_State *L );
	static int _hasLeaderboards						( lua_State *L );
	static int _hasAchievements						( lua_State *L );
	static int _hasCloudSaves						( lua_State *L );
	static int _achievementReset						( lua_State *L );
	static int _achievementSet						( lua_State *L );
	static int _achievementSetCallback				( lua_State *L );
	static int _leaderboardUploadScore				( lua_State *L );
	static int _leaderboardSetScoreCallback			( lua_State *L );
	static int _leaderboardGetScores					( lua_State *L );
	static int _leaderboardSetGetScoresCallback		( lua_State *L );
	static int _cloudDataUpload						( lua_State *L );
	static int _cloudDataSetUploadCallback			( lua_State *L );
	static int _cloudDataDownload					( lua_State *L );
	static int _cloudDataSetDownloadCallback			( lua_State *L );
	static int _doTick								( lua_State *L );
	
	void HuskyObserverAchievementCallback(const char *name, bool success);
	void HuskyObserverLeaderboardScoreSetCallback(const char *name, bool success);
	void HuskyObserverLeaderboardScoreGetCallback(const char *name, HuskyLeaderboardEntry *entries, int number);
	void HuskyObserverCloudDataDownloaded(const char *cloudfilename, void* buffer, int32_t bytes);
	void HuskyObserverCloudDataUploaded(const char *path, bool success);
	
	MOAILuaLocal _achievementCallback;
	MOAILuaLocal _leaderboardScoreSetCallback;
	MOAILuaLocal _leaderboardScoreGetCallback;
	MOAILuaLocal _cloudDataUploadCallback;
	MOAILuaLocal _cloudDataDownloadCallback;

	LoaderHandleMap *_map;
	void* _currentHuskyHandle;
	Husky* _instance;
	HuskyGetName* _fHuskyName;
	HuskyShutdownStaticInstance* _fHuskyShutdown;
	u16 _huskyCapabilities;
	
public:
	DECL_LUA_SINGLETON ( MOAIHusky )
	
							MOAIHusky	(  );
							~MOAIHusky	(  );
	//----------------------------------------------------------------//
	void					RegisterLuaClass		( MOAILuaState& state );
	void					RegisterLuaFuncs		( MOAILuaState& state );
};



#endif /* defined(__libmoai__MOAIHusky__) */
