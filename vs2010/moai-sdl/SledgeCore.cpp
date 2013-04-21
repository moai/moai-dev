#include "SledgeCore.h"

#ifdef _WIN32
#include <shlobj.h>
#endif

SledgeCore::SledgeCore(){
	RTTI_BEGIN
		RTTI_EXTEND(MOAILuaObject)
	RTTI_END
}

SledgeCore::~SledgeCore()
{
}

void SledgeCore::RegisterLuaClass(MOAILuaState& state) {
	luaL_Reg regTable[] = {
		{ "setupDirectories",	_setupDirectories },
		{ "quitGame",			_quitGame },
		{ NULL, NULL }
	};
	luaL_register(state, 0, regTable);
}

void SledgeCore::RegisterLuaFuncs(MOAILuaState& state) {
	luaL_Reg regTable[] = {
		{ NULL, NULL}
	};
	luaL_register(state, 0, regTable);
}

void SledgeCore::createDir( const char* absolutePath )
{
	// create if not found
	// @todo do something with that -1 signal

	if(dirExists(absolutePath) == 0)
	{
#ifdef _WIN32
		mkdir(absolutePath);
#endif
	}
}

/** return:
*	1	if dir does exist
 *	0	if dir doesn't exist
 *	-1	if dir doesn't exist and you should maybe stop
 */
int SledgeCore::dirExists( const char* absolutePath )
{
	int returnval = 0;

	struct stat status;
	int result = stat( absolutePath, &status );

	if(result != 0)
	{
		// problems!
		switch (errno)
		{
		case ENOENT:
			// dir doesn't exist
			returnval = 0;
			break;
		case EINVAL:
			// invalid parameter
			break;
		default:
			// unexpected error
			returnval = -1;
			break;
		}
		
	} else {
		returnval = (int)((status.st_mode & S_IFDIR) != 0);
	}
	return returnval;
}


//----------------------------------------------------------------//
/**	@name	setupDirectories
	@text	Sets up the MOAIEnvironment directories on Windows,
			given a name for the app.

	@in		SledgeCore self
	@in		string appName				App name.
	@out	nil
*/
int SledgeCore::_setupDirectories( lua_State* L )
{
	MOAI_LUA_SETUP (SledgeCore, "US");

#if defined( MOAI_OS_WINDOWS )

	cc8* appName = state.GetValue< cc8* >(2, "");

	// get base documents directory
	char docuPath[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, docuPath);
	int len_docuPath = strlen(docuPath);

	// check for /my games subdir
	char mygamesPath[MAX_PATH];
	sprintf(mygamesPath, "%s\\%s", docuPath, "my games");
	createDir(mygamesPath);

	// check for /my games/appName subdir
	char appPath[MAX_PATH];
	sprintf(appPath, "%s\\%s", mygamesPath, appName);
	createDir(appPath);

	// check for /my games/appName/docs and /my games/appName/cache
	char appPath_docs[MAX_PATH];
	char appPath_cache[MAX_PATH];
	sprintf(appPath_docs, "%s\\%s", appPath, "docs");
	sprintf(appPath_cache, "%s\\%s", appPath, "cache");
	createDir(appPath_docs);
	createDir(appPath_cache);

	// set MOAIEnvironment keys
	MOAIEnvironment::Get().SetValue(MOAI_ENV_documentDirectory, appPath_docs);
	MOAIEnvironment::Get().SetValue(MOAI_ENV_cacheDirectory, appPath_cache);

#endif

	return 0;
}

int SledgeCore::_quitGame( lua_State* L )
{
	MOAI_LUA_SETUP (SledgeCore, "U");

	// send quit signal
	SDL_Event ev;
	ev.type = SDL_QUIT;
	SDL_PushEvent(&ev);

	return 0;
}