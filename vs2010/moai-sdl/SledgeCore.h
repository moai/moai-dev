#ifndef SLEDGECORE_H
#define SLEDGECORE_H


#include <uslscore/pch.h>
#include <uslscore/USAccessors.h>
#include <uslscore/USBox.h>
#include <uslscore/USColor.h>
#include <uslscore/USLeanArray.h>
#include <uslscore/USStream.h>
#include <uslscore/USStreamReader.h>
#include <uslscore/USStreamWriter.h>
#include <uslscore/STLList.h>
#include <uslscore/STLSet.h>
#include <uslscore/STLMap.h>
#include <moaicore/MOAIGlobals.h>
#include <moaicore/MOAIRtti.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAIEnvironment.h>

#include <mxmlstream.h>
#include <mxmldom.h>

/**
// Feel free to use TinyXML if you can get it to work.
#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif
#include <tinyxml.h>
*/

//#include <io.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <errno.h>


#include <SDL.h>
#define SDL_main main


#ifndef HAS_AKU
#define HAS_AKU
#include <aku/AKU.h>
#endif

#include "UtilityTypes.h"

class SledgeCore :
	public MOAIGlobalClass <SledgeCore, MOAILuaObject> {
private:
	static int		_killConsole		( lua_State* L );
	static int		_setupDirectories	( lua_State* L );
	static int		_quitGame			( lua_State* L );

	static void		createDir			( const char* absolutePath );
	static int		dirExists			( const char* absolutePath );

public:
	DECL_LUA_SINGLETON (SledgeCore)

	static enum		SFSEnvVarMap {
		SFS_ENV_appDisplayName,
		SFS_ENV_appID,
		SFS_ENV_appVersion,
		SFS_ENV_buildNumber,
		SFS_ENV_cacheDirectory,
		SFS_ENV_carrierISOCountryCode,
		SFS_ENV_carrierMobileCountryCode,
		SFS_ENV_carrierMobileNetworkCode,
		SFS_ENV_carrierName,
		SFS_ENV_connectionType,
		SFS_ENV_countryCode,
		SFS_ENV_cpuabi,
		SFS_ENV_devBrand,
		SFS_ENV_devName,
		SFS_ENV_devUserName,
		SFS_ENV_devManufacturer,
		SFS_ENV_devModel,
		SFS_ENV_devPlatform,
		SFS_ENV_devProduct,
		SFS_ENV_documentDirectory,
		SFS_ENV_iosRetinaDisplay,
		SFS_ENV_languageCode,
		SFS_ENV_numProcessors,
		SFS_ENV_osBrand,
		SFS_ENV_osVersion,
		SFS_ENV_resourceDirectory,
		SFS_ENV_screenDpi,
		SFS_ENV_verticalResolution,
		SFS_ENV_horizontalResolution,
		SFS_ENV_udid,
		SFS_ENV_openUdid,

		SFS_ENV_MAX
	};

	static const char* const SFSMOAIEnvKeys[];
	static const char* const SFSMOAIEnvDefaults[];
	static const char* const SFSMOAIXMLElementNames[];

	//----------------------------------------------------------------//
					SledgeCore			();
					~SledgeCore			();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );

	static void		LoadInfoXML			( const char* p_xmlfilename, MOAIEnvironment* p_env );
};



#endif