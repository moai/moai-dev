//
//  MOAIHusky.cpp
//  libmoai
//
//  Created by Brendan Ragan on 20/12/13.
//
//

#include "pch.h"
#include "MOAIHusky.h"
#include <iterator>

#ifdef __APPLE__
	#include <dirent.h>
	#import <Foundation/Foundation.h>
#elif WIN32
	#define strcasecmp(a, b) lstrcmpiA(a,b)
#endif

HuskyLoaderHandle::HuskyLoaderHandle() {
	this->dllhandle = NULL;
}

HuskyLoaderHandle::HuskyLoaderHandle(void *handle) {
	this->dllhandle = handle;
}

bool endsWith(const char *string, const char *ending) {
	unsigned long length1, length2;
	length1 = strlen(string);
	length2 = strlen(ending);
	const char *endofstring = string + (length1 - length2);
	return strcmp(ending, endofstring) == 0;
}

MOAIHusky::MOAIHusky() {
	RTTI_BEGIN
	RTTI_EXTEND(MOAILuaObject)
	RTTI_END

	_map = new LoaderHandleMap();
	_currentHuskyHandle = NULL;
	_instance = NULL;
	
#ifdef __APPLE__ 
	const char* extension = ".dylib";
	
	/** Grab the directory the exectuable is in, by default we want to load Huskies from where the moai exe is **/
	NSString *executablePath = [[[[NSProcessInfo processInfo] arguments] objectAtIndex:0] stringByDeletingLastPathComponent];
	const char *dirname = [executablePath fileSystemRepresentation];
	unsigned long dirlength = strlen(dirname);

	DIR *dir = opendir(dirname);
	
	struct dirent *ent;

	/** Cycle through the entries in the directory **/
	while((ent = readdir(dir)) != NULL) {
		/** Is this entry a regular file? **/
		if (ent->d_type == DT_REG) {
			if (endsWith(ent->d_name, extension)) {
				/** We need a full path to load the dll **/
				unsigned long filenamelength = strlen(ent->d_name);
				char* fullpath = (char*)malloc(sizeof(char) * (filenamelength + dirlength + 2));
				strcpy(fullpath, dirname);
				strcat(fullpath, "/");
				strcat(fullpath, ent->d_name);
				
				void* dll_handle = dlopen(fullpath, RTLD_LOCAL|RTLD_LAZY);
				free(fullpath);
				/** Got a handle? try to get the husky entry points **/
				if (dll_handle) {
					HuskyGetStaticInstance* fHuskyInstance = (HuskyGetStaticInstance*)dlsym(dll_handle, "getHuskyInstance");
					HuskyGetName* fHuskyName = (HuskyGetName*)dlsym(dll_handle, "getHuskyName");
					HuskyShutdownStaticInstance* fHuskyShutdown;
					fHuskyShutdown = (HuskyShutdownStaticInstance*)dlsym(dll_handle, "shutdownHuskyInstance");
					if (fHuskyName && fHuskyInstance && fHuskyShutdown) {
						/** Got Husky Entry points? great, now record this handle so we can use it later **/
						HuskyLoaderHandle *handleObj = new HuskyLoaderHandle(dll_handle);
						std::string *name = new std::string(fHuskyName());
						if (_currentHuskyHandle == NULL) {
							_currentHuskyHandle = handleObj->dllhandle;
							_instance = fHuskyInstance();
							_fHuskyName = fHuskyName;
							_fHuskyShutdown = fHuskyShutdown;
							_instance->setObserver(this);
							_huskyCapabilities = _instance->getCapabilities();
						}
						_map->insert(LoaderHandleMap::value_type(*name, *handleObj));
					}
				}
			}
		}
	}
#elif WIN32
	
#endif
}

MOAIHusky::~MOAIHusky() {
	if (_fHuskyShutdown) {
		_fHuskyShutdown();
	}
}

int MOAIHusky::_getAvailable( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )
	
	int index = 0;
	
	lua_newtable(L);
	LoaderHandleMap *_map = self->_map;
	LoaderHandleMap::iterator i = _map->begin();
	while(i != _map->end()) {
		lua_pushnumber(L, index);
		lua_pushstring(L, i->first.c_str());
		lua_settable(L, -3);
		std::advance(i, 1);
		index++;
	}

	return 1;
}

int MOAIHusky::_getCurrent( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" );

	if (self->_instance != NULL) {
		lua_pushstring(L, self->_fHuskyName());
		return 1;
	}

	return 0;
}

int MOAIHusky::_setCurrent( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "US" )
	
	return 0;
}

int MOAIHusky::_hasLeaderboards( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )

	if (self->_instance != NULL) {
		state.Push(self->_huskyCapabilities && HuskyHasLeaderboards);
		return 1;
	}
		
	return 0;
}

int MOAIHusky::_hasAchievements( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )
	
	if (self->_instance != NULL) {
		state.Push(self->_huskyCapabilities && HuskyHasAchievements);
		return 1;
	}

	
	return 0;
}

int MOAIHusky::_hasCloudSaves( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "U" )

	if (self->_instance != NULL) {
		state.Push(self->_huskyCapabilities && HuskyHasCloudSaves);
		return 1;
	}

	return 0;
}

int MOAIHusky::_achievementReset( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "" )
	
	self->_instance->resetAchievements();
	return 0;
}


int MOAIHusky::_achievementSet( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "US" )

	cc8* name = lua_tostring ( state, 2 );
	self->_instance->setAchievement(name);
	return 0;
}

int MOAIHusky::_achievementSetCallback( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "UF" )

	self->SetLocal(state, 2, self->_achievementCallback);
	
	return 0;
}

int MOAIHusky::_leaderboardUploadScore( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "USNS" )
	
	cc8* name = lua_tostring ( state, 2 );
	int32_t score = lua_tointeger( state, 3 );
	cc8* replacement = lua_tostring ( state, 4 );
	int64_t data = lua_tointeger( state, 5 );
	HuskyLeaderboardScoreToKeep update = HuskyLeaderboardScoreToKeepNone;
	if (strcasecmp(replacement, "best") == 0) {
		update = HuskyLeaderboardScoreToKeepBest;
	} else if (strcasecmp(replacement, "update") == 0) {
		update = HuskyLeaderboardScoreToKeepUpdate;
	}
	self->_instance->uploadLeaderboardScore(name, score, update, data);
	return 0;
}

int MOAIHusky::_leaderboardSetScoreCallback( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "UF" )
	
	self->SetLocal(state, 2, self->_leaderboardScoreSetCallback);
	
	return 0;
}

int MOAIHusky::_leaderboardGetScores( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "USBBSNN" )
	
	cc8* name = state.GetValue<cc8*>(2, 0);
	bool friends = state.GetValue<bool>(3,0);
	bool around = state.GetValue<bool>(4,0);
	cc8* timeframestring = state.GetValue<cc8*>(5,0);
	int32_t offset = state.GetValue<int>(6, 0);
	int32_t number = state.GetValue<int>(7, 0);
	
	HuskyLeaderboardScoreTimeFrame timeframe = HuskyLeaderboardAllScores;
	if (strcasecmp(timeframestring, "week"))
		timeframe = HuskyLeaderboardWeeksScores;
	else if (strcasecmp(timeframestring, "day"))
		timeframe = HuskyLeaderboardTodaysScores;
		
	if (around)
		self->_instance->requestLeaderboardScoresNearPlayer(name, friends, timeframe, offset, number);
	else
		self->_instance->requestLeaderboardScores(name, friends, timeframe, offset, number);
	
	return 0;
}

int MOAIHusky::_leaderboardSetGetScoresCallback( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "UF" )
	
	self->SetLocal(state, 2, self->_leaderboardScoreGetCallback);
	
	return 0;
}

int MOAIHusky::_cloudDataUpload( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "US" )
	
	cc8* cloudpath = state.GetValue<cc8*>(2, 0);
	MOAIStream* moaistream = state.GetLuaObject < MOAIStream >( 3, true );
	ZLStream *stream = moaistream->GetZLStream();
	stream->Seek(0, SEEK_SET);
	size_t bytes = stream->GetLength();
	void *buffer = malloc(bytes);
	stream->ReadBytes(buffer, bytes);
	self->_instance->uploadCloudData(cloudpath, buffer, bytes);
	return 0;
}

int MOAIHusky::_cloudDataSetUploadCallback( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "UF" )
	
	self->SetLocal(state, 2, self->_cloudDataUploadCallback);
	
	return 0;
}

int MOAIHusky::_cloudDataDownload( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "US" )
	
	cc8* cloudpath = state.GetValue<cc8*>(2, 0);
	self->_instance->requestCloudData(cloudpath);
	
	return 0;
}

int MOAIHusky::_cloudDataSetDownloadCallback( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "UF" )
	
	self->SetLocal(state, 2, self->_cloudDataDownloadCallback);
	
	return 0;
}

int MOAIHusky::_doTick(lua_State *L) {
	// TODO: it'd be good if this was run automagically as part of Sledge rather than having to start the callback from lua-land

	MOAI_LUA_SETUP(MOAIHusky, "U");
	
	if (self->_instance != NULL) {
		self->_instance->doTick();
	}
	
	return 0;
}

//----------------------------------------------------------------//
void MOAIHusky::RegisterLuaClass ( MOAILuaState& state ) {
	
	// call any initializers for base classes here:
	// SledgeInputHandlerBase::RegisterLuaClass ( state );
	
	// also register constants:
	// state.SetField ( -1, "FOO_CONST", ( u32 )FOO_CONST );
	
	// here are the class methods:
	luaL_Reg regTable [] = {
		{ "getAvailable",						_getAvailable },
		{ "getCurrent",							_getCurrent },
		{ "setCurrent",							_setCurrent },
		{ "hasLeaderboards",						_hasLeaderboards },
		{ "hasAchievements",						_hasAchievements },
		{ "hasCloudSaves",						_hasCloudSaves },
		{ "achievementReset",					_achievementReset },
		{ "achievementSet",						_achievementSet },
		{ "achievementSetCallback",				_achievementSetCallback },
		{ "leaderboardUploadScore",				_leaderboardUploadScore },
		{ "leaderboardSetUploadScoreCallback",	_leaderboardSetScoreCallback },
		{ "leaderboardGetScores",				_leaderboardGetScores },
		{ "leaderboardSetGetScoresCallback",		_leaderboardSetGetScoresCallback },
		{ "cloudDataUpload",						_cloudDataUpload },
		{ "cloudDataSetUploadCallback",			_cloudDataSetUploadCallback },
		{ "cloudDataDownload",					_cloudDataDownload },
		{ "cloudDataSetDownloadCallback",		_cloudDataSetDownloadCallback },
		{ "doTick", _doTick },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIHusky::RegisterLuaFuncs ( MOAILuaState& state ) {
	luaL_Reg regTable [] = {
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

void MOAIHusky::HuskyObserverAchievementCallback(const char *name, bool success) {
	if (_achievementCallback) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		this->PushLocal ( state, _achievementCallback );
		state.Push(name);
		state.Push(success);
		state.DebugCall ( 2, 0 );
	}
}

void MOAIHusky::HuskyObserverLeaderboardScoreSetCallback(const char *name, bool success) {
	if (_leaderboardScoreSetCallback) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		this->PushLocal ( state, _leaderboardScoreSetCallback );
		state.Push(name);
		state.Push(success);
		state.DebugCall ( 2, 0 );
	}
}

void MOAIHusky::HuskyObserverLeaderboardScoreGetCallback(const char *name, HuskyLeaderboardEntry *entries, int number) {
	if (!_leaderboardScoreGetCallback)
		return;

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	this->PushLocal ( state, _leaderboardScoreGetCallback );
	state.Push(name);
	lua_newtable(state);
	for(int i = 0; i < number; i++) {
		state.Push(i+1);
		lua_newtable(state);
		state.Push("name");
		state.Push(entries[i].name);
		lua_settable(state, -3);
		state.Push("globalrank");
		state.Push(entries[i].globalrank);
		lua_settable(state, -3);
		state.Push("score");
		state.Push(entries[i].score);
		lua_settable(state, -3);
		state.Push("data");
		state.Push((u64)entries[i].data);
		lua_settable(state, -3);
		lua_settable(state, -3);
	}
	state.DebugCall ( 2, 0 );
}

void MOAIHusky::HuskyObserverCloudDataDownloaded(const char *cloudfilename, void* buffer, int32_t bytes) {
	if (!_cloudDataDownloadCallback)
		return;
	MOAIScopedLuaState state = MOAILuaRuntime::Get().State ();
	MOAIDataBuffer *moaibuffer = new MOAIDataBuffer();
	moaibuffer->Load(buffer, bytes);
	this->PushLocal(state, _cloudDataDownloadCallback);
	state.Push(cloudfilename);
	state.Push(moaibuffer);
	state.DebugCall(2, 0);
}

void MOAIHusky::HuskyObserverCloudDataUploaded(const char *path, bool success) {
	if (!_cloudDataUploadCallback)
		return;

	MOAIScopedLuaState state = MOAILuaRuntime::Get().State();
	this->PushLocal(state, _cloudDataUploadCallback);
	state.Push(path);
	state.Push(success);
	state.DebugCall(2, 0);
}

