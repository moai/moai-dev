//
//  MOAIHusky.cpp
//  libmoai
//
//  Created by Brendan Ragan on 20/12/13.
//
//

#include "pch.h"
#include "MOAIHusky.h"

#include <dirent.h>
#include <iterator>

#import <Foundation/Foundation.h>

HuskyLoaderHandle::HuskyLoaderHandle() {
	this->dllhandle = NULL;
}

HuskyLoaderHandle::HuskyLoaderHandle(void *dllhandle) {
	this->dllhandle = dllhandle;
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
					if (fHuskyName && fHuskyInstance) {
						/** Got Husky Entry points? great, now record this handle so we can use it later **/
						HuskyLoaderHandle *handleObj = new HuskyLoaderHandle(dll_handle);
						std::string *name = new std::string(fHuskyName());
						if (_currentHuskyHandle == NULL) {
							_currentHuskyHandle = handleObj->dllhandle;
							_instance = fHuskyInstance();
							_fHuskyName = fHuskyName;
							_instance->setObserver(this);
						}
						_map->insert(LoaderHandleMap::value_type(*name, *handleObj));
					}
				}
			}
		}
	}
}

MOAIHusky::~MOAIHusky() {
	
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

	if (self->_instance != nil) {
		lua_pushstring(L, self->_fHuskyName());
		return 1;
	}

	return 0;
}

int MOAIHusky::_setCurrent( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHusky, "US" )
	
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

//----------------------------------------------------------------//
void MOAIHusky::RegisterLuaClass ( MOAILuaState& state ) {
	
	// call any initializers for base classes here:
	// SledgeInputHandlerBase::RegisterLuaClass ( state );
	
	// also register constants:
	// state.SetField ( -1, "FOO_CONST", ( u32 )FOO_CONST );
	
	// here are the class methods:
	luaL_Reg regTable [] = {
		{ "getAvailable",	_getAvailable },
		{ "getCurrent",	_getCurrent },
		{ "setCurrent",	_setCurrent },
		{ "achievementSet",	_achievementSet },
		{ "achievementSetCallback",	_achievementSetCallback },
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

