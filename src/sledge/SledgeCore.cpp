#include "stdafx.h"
#include "SledgeCore.h"

#ifdef _WIN32
#include <shlobj.h>
#endif

const char* const SledgeCore::SFSMOAIEnvKeys[Sledge::SFS_ENV_MOAIMAX+Sledge::SFS_ENV_ADDITIONAL_KEY_COUNT] = {
	MOAI_ENV_appDisplayName,
	MOAI_ENV_appID,
	MOAI_ENV_appVersion,
	MOAI_ENV_buildNumber,
	MOAI_ENV_cacheDirectory,
	MOAI_ENV_carrierISOCountryCode,
	MOAI_ENV_carrierMobileCountryCode,
	MOAI_ENV_carrierMobileNetworkCode,
	MOAI_ENV_carrierName,
	MOAI_ENV_connectionType,
	MOAI_ENV_countryCode,
	MOAI_ENV_cpuabi,
	MOAI_ENV_devBrand,
	MOAI_ENV_devName,
	MOAI_ENV_devUserName,
	MOAI_ENV_devManufacturer,
	MOAI_ENV_devModel,
	MOAI_ENV_devPlatform,
	MOAI_ENV_devProduct,
	MOAI_ENV_documentDirectory,
	MOAI_ENV_iosRetinaDisplay,
	MOAI_ENV_languageCode,
	MOAI_ENV_numProcessors,
	MOAI_ENV_osBrand,
	MOAI_ENV_osVersion,
	MOAI_ENV_resourceDirectory,
	MOAI_ENV_screenDpi,
	MOAI_ENV_verticalResolution,
	MOAI_ENV_horizontalResolution,
	MOAI_ENV_udid,
	MOAI_ENV_openUdid,
	MOAI_ENV_screenCount,
	MOAI_ENV_ramAmount,
	MOAI_ENV_processorModel,
	MOAI_ENV_processorFreq,
	MOAI_ENV_desktopRes
};

SledgeCore::SledgeCore(){
	RTTI_BEGIN
		RTTI_EXTEND(MOAILuaObject)
	RTTI_END
}

SledgeCore::~SledgeCore()
{
}

/**
 * Registers this class's static methods with MOAI.
 *
 * @author	Jetha Chan
 * @date	6/09/2013
 *
 * @param [in,out]	state	MOAI's current Lua state.
 */
void SledgeCore::RegisterLuaClass(MOAILuaState& state) {
	luaL_Reg regTable[] = {
		{ "setupDirectories",	DoSetupDirectories },
		{ "quitGame",			DoQuitGame },
		{ "killConsole",		DoKillConsole },
		{ NULL, NULL }
	};
	luaL_register(state, 0, regTable);
}

/**
 * Registers this class's member methods with MOAI.
 *
 * @author	Jetha Chan
 * @date	6/09/2013
 *
 * @param [in,out]	state	MOAI's current Lua state.
 */
void SledgeCore::RegisterLuaFuncs(MOAILuaState& state) {
	luaL_Reg regTable[] = {
		{ NULL, NULL}
	};
	luaL_register(state, 0, regTable);
}

/**
 * Attempts to create the specified directory if it doesn't exist.
 *
 * @author	Jetha Chan
 * @date	6/09/2013
 *
 * @param	absolutePath	Absolute path of the desired directory.
 */
void SledgeCore::createDir( const char* absolutePath )
{
	// create if not found
	// @todo do something with that -1 signal

	if(dirExists(absolutePath) == 0)
	{
#ifdef WIN32
		mkdir(absolutePath);
#endif
	}
}

/**
 * Checks whether the specified directory exists.
 *
 * @author	Jetha Chan
 * @date	6/09/2013
 *
 * @param	absolutePath	Absolute path of the directory to check for.
 *
 * @return	1 if the directory exists, 0 if it doesn't, -1 on unexpected
 * 			error.
 */
s32 SledgeCore::dirExists( const char* absolutePath )
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

s32 SledgeCore::DoKillConsole ( lua_State* L )
{
	MOAI_LUA_SETUP (SledgeCore, "U");
#if defined( MOAI_OS_WINDOWS )
	FreeConsole();
#endif

	return 0;

}

//----------------------------------------------------------------//
/**	@name	setupDirectories
	@text	Sets up the MOAIEnvironment directories on Windows,
			given a name for the app.

	@in		SledgeCore self
	@in		string appName				App name.
	@out	nil
*/
s32 SledgeCore::DoSetupDirectories( lua_State* L )
{
	MOAI_LUA_SETUP (SledgeCore, "US");

#if defined( MOAI_OS_WINDOWS )

	cc8* appName = state.GetValue< cc8* >(2, "");

	// get base documents directory
	char docuPath[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, docuPath);
	s32 len_docuPath = strlen(docuPath);

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

s32 SledgeCore::DoQuitGame( lua_State* L )
{
	MOAI_LUA_SETUP (SledgeCore, "U");

	// send quit signal
	SDL_Event ev;
	ev.type = SDL_QUIT;
	SDL_PushEvent(&ev);

	return 0;
}


#if defined(_WIN32) || defined(_WIN64)
/**
 * Loads game information from XML.
 *
 * @author	Jetha Chan
 * @date	6/09/2013
 *
 * @param	p_xmlfilename	The filename of the XML document to load.
 * @param [in,out]	p_env	If non-null, the current MOAI environment.
 */
void SledgeCore::LoadInfoXML( const char* p_xmlfilename, MOAIEnvironment& p_env )
{
	
	// get the current working directory
	char cwdPath[PATH_MAX+1];
	_getcwd(cwdPath, PATH_MAX);

	int xmlfilename_len = strlen(p_xmlfilename);

	char* xmlfilepath_abs = (char*)calloc(
		strlen(cwdPath) +
		strlen(p_xmlfilename) +
		5,
		sizeof(char)
	);

	sprintf(xmlfilepath_abs, "%s\\%s", cwdPath, p_xmlfilename);

	printf("absolute path to xml file: '%s'\n", xmlfilepath_abs);
	///*

	FILE *file = fopen(xmlfilepath_abs, "rb");  
	if(file)
	{
		//TiXmlDocument *document = new TiXmlDocument();
		//
		//if (document->LoadFile(file)) {

		//}

		fclose(file);
	}
	/*
	bool parsed = false;
	TiXmlDocument *document = new TiXmlDocument();
	if (file) {
		if (document->LoadFile(file)) {
			TiXmlElement* root = document->FirstChildElement( "info" );
			if (root) {
				TiXmlNode *child;
				TiXmlElement *element;
				for (int i = 0; i < Sledge::SFS_ENV_MOAIMAX; ++i) {
					child = (root->FirstChild(SFSMOAIXMLElementNames[i]));
					if (child) {
						if (child->Type() == TiXmlNode::TINYXML_ELEMENT) {
							element = child->ToElement();
							p_env.SetValue(SFSMOAIEnvKeys[i], element->GetText());
						}
					}
				}
				parsed = true;
			}
		}
	} 
	if (!parsed) {
		printf("Failed to load XML file\n");
	}
	*/
}
#endif


#if defined(_WIN32) || defined(_WIN64)
/**
 * Sets up additional hardware info for MOAIEnvironment. Windows-specific.
 *
 * @author	Jetha Chan
 * @date	6/09/2013
 */
void SledgeCore::GetAdditionalHWInfo( MOAIEnvironment* p_env )
{
	char tempstr[200];
	char guidseed[400];

	HKEY hKey;
	LONG lRes;
	WCHAR szBuffer[512];
	DWORD dwBufferSize = sizeof(szBuffer);

	// Read HW info:
	//	- screen res, retina flag
	ScreenEnvInfo sei = SledgeGraphicsHandler::GetScreenEnvInfo();
	sprintf(tempstr, "%dx%d", sei.screenDim[0], sei.screenDim[1]);
	p_env->SetValue(
		SFSMOAIEnvKeys[Sledge::SFS_ENV_desktopRes],
		tempstr
		);
	p_env->SetValue(
		SFSMOAIEnvKeys[Sledge::SFS_ENV_iosRetinaDisplay],
		sei.retina
	);

	//	- # of attached screens
	sprintf(tempstr, "%d", sei.screenCount);
	//hwinfo[SFS_ENV_screenCount - SFS_ENV_MOAIMAX] = tempstr;
	p_env->SetValue(
		SFSMOAIEnvKeys[Sledge::SFS_ENV_screenCount],
		tempstr
		);

	//	- RAM
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx (&statex);
	unsigned long long memoryKB = statex.ullAvailPhys;
	p_env->SetValue(
		SFSMOAIEnvKeys[Sledge::SFS_ENV_ramAmount],
		memoryKB / 1024
	);

	//	- processor model
	//	- processor frequency
	std::string procModel = "";

	lRes = RegOpenKeyExW(
		HKEY_LOCAL_MACHINE,
		L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
		0,
		KEY_READ,
		&hKey
		);
	if (lRes == ERROR_SUCCESS)
	{
		ULONG nError = RegQueryValueExW(
			hKey,
			L"ProcessorNameString",
			0,
			NULL,
			(LPBYTE)szBuffer,
			&dwBufferSize
			);

		size_t origsize = wcslen(szBuffer) + 1;
		const size_t newsize = 513;
		size_t convertedChars = 0;
		char nstring[newsize];
		wcstombs_s(&convertedChars, nstring, origsize, szBuffer, _TRUNCATE);
		p_env->SetValue ( SFSMOAIEnvKeys[Sledge::SFS_ENV_processorModel], nstring );

		procModel = nstring;
		// use function since we don't need the string for UUID generation
		w32_updateEnvFromRegKeyDword(
			p_env,
			SFSMOAIEnvKeys[Sledge::SFS_ENV_processorFreq],
			hKey,
			L"~Mhz"
			);
	}
	RegCloseKey(hKey);

	//  - computer name
	std::string compName = "";

	lRes = RegOpenKeyExW(
		HKEY_LOCAL_MACHINE,
		L"SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ComputerName",
		0,
		KEY_READ,
		&hKey
		);
	if (lRes == ERROR_SUCCESS)
	{
		ULONG nError = RegQueryValueExW(
			hKey,
			L"ComputerName",
			0,
			NULL,
			(LPBYTE)szBuffer,
			&dwBufferSize
			);

		size_t origsize = wcslen(szBuffer) + 1;
		const size_t newsize = 513;
		size_t convertedChars = 0;
		char nstring[newsize];
		wcstombs_s(&convertedChars, nstring, origsize, szBuffer, _TRUNCATE);
		p_env->SetValue ( SFSMOAIEnvKeys[Sledge::SFS_ENV_devUserName], nstring );

		compName = nstring;
	}
	RegCloseKey(hKey);


	// let's try to build a UUID!
	// @todo resolve conflict between kashmir's uuid and windows's uuid
	sprintf(guidseed, "%s %s %d", compName.c_str(), procModel.c_str(), memoryKB);
}
#endif

