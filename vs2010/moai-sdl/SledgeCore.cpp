#include "SledgeCore.h"

#ifdef WIN32
#include <shlobj.h>
#endif


const char* const SledgeCore::SFSMOAIEnvKeys[SFS_ENV_MOAIMAX+SFS_ENV_ADDITIONAL_KEY_COUNT] = {
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

const char* const SledgeCore::SFSMOAIEnvDefaults[SFS_ENV_MOAIMAX] = {
	"Moai Debug",
	"moai-test-debug",
	"UNKNOWN VERSION",
	"UNKNOWN BUILD",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	""
};
const char* const SledgeCore::SFSMOAIXMLElementNames[SFS_ENV_MOAIMAX] = {
	"app_name",
	"identifier",
	"version",
	"build",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	""
};
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
		{ "killConsole",		_killConsole },
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
#ifdef WIN32
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

int SledgeCore::_killConsole ( lua_State* L )
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

#ifdef WIN32
void SledgeCore::LoadInfoXML( const char* p_xmlfilename, MOAIEnvironment* p_env )
{
	//printf("%s\n", __FUNCTION__);
	//TiXmlDocument* doc = new TiXmlDocument();

	MXMLDocument infoDoc;
	
	// get the current working directory
	char cwdPath[PATH_MAX+1];
	_getcwd(cwdPath, PATH_MAX);

	int xmlfilename_len = strlen(p_xmlfilename);

	char* xmlfilepath_abs = new char[strlen(cwdPath) + xmlfilename_len + 1];

	sprintf(xmlfilepath_abs, "%s\\%s", cwdPath, p_xmlfilename);

	printf("absolute path to xml file: %s\n", xmlfilepath_abs);

	MXMLStreamFile infoFile(xmlfilepath_abs, "rb");

	if (infoFile.IsOpen())
	{
		bool readResult = infoDoc.Load(infoFile);
		if(readResult)
		{
			MXMLElement* node = infoDoc.GetDocumentElement();

			if(node)
			{
				node = node->GetFirstChildElement();
				const char* doctype = node->GetName();
				printf("root:\t%s\n", doctype);

				int count = node->CountChildElements();

				// The following is the most straightforward way of doing
				// things; so we're not going to do that.
				/*
				if(count > 0)
					node = node->GetFirstChildElement();
				for (int i = 0; i < count; ++i)
				{
					// [MASSIVE FUCKING SWITCH STATEMENT USING STRCMP]

					node = node->GetNextSiblingElement();
				}
				*/

				// It's more valuable to be able to easily add keys to our
				// string arrays, so we'll do things in a "slightly" less
				// optimal manner.
				if(count == 0)
					return; // early out
				for (int i = 0; i < SFS_ENV_MOAIMAX; ++i)
				{
					MXMLElement* thisElem = node->FindChild(SFSMOAIXMLElementNames[i]);
					if(thisElem != NULL)
					{
						// we found something!
						p_env->SetValue(
							SFSMOAIEnvKeys[i],
							thisElem->GetText()
						);
					}
				}

			}
		} else {
			printf("Failed to load info.xml (%s)\n", infoDoc.GetBuffer());
		}
	}
}
#endif

/// <summary>	Sets up additional hardware info </summary>
///
/// <remarks>	Jetha, 17/06/2013. </remarks>
void SledgeCore::GetAdditionalHWInfo( MOAIEnvironment* p_env )
{
	// set screen dimensions
	// set screen count
	// set ram amount
	// processor model
	// processor frequency
#ifdef WIN32
	//std::string hwinfo[SFS_ENV_ADDITIONAL_KEY_COUNT];
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
		SFSMOAIEnvKeys[SFS_ENV_desktopRes],
		tempstr
		);
	/*
	sprintf(tempstr, "%d", sei.screenDim[0]);
	p_env->SetValue(
		SFSMOAIEnvKeys[SFS_ENV_horizontalResolution],
		tempstr
	);	
	sprintf(tempstr, "%d", sei.screenDim[1]);	
	p_env->SetValue(
		SFSMOAIEnvKeys[SFS_ENV_verticalResolution],
		tempstr
	);
	*/
	p_env->SetValue(
		SFSMOAIEnvKeys[SFS_ENV_iosRetinaDisplay],
		sei.retina
	);

	//	- # of attached screens
	sprintf(tempstr, "%d", sei.screenCount);
	//hwinfo[SFS_ENV_screenCount - SFS_ENV_MOAIMAX] = tempstr;
	p_env->SetValue(
		SFSMOAIEnvKeys[SFS_ENV_screenCount],
		tempstr
		);

	//	- RAM
	unsigned long long memoryKB = 0;
	GetPhysicallyInstalledSystemMemory(&memoryKB);
	//hwinfo[SFS_ENV_ramAmount - SFS_ENV_MOAIMAX] = tempstr;
	p_env->SetValue(
		SFSMOAIEnvKeys[SFS_ENV_ramAmount],
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
		p_env->SetValue ( SFSMOAIEnvKeys[SFS_ENV_processorModel], nstring );

		procModel = nstring;
		/*
		w32_updateEnvFromRegKeyStr(
			p_env,
			SFSMOAIEnvKeys[SFS_ENV_processorModel],
			hKey,
			L"ProcessorNameString"
		);
		*/

		// use function since we don't need the string for UUID generation
		w32_updateEnvFromRegKeyDword(
			p_env,
			SFSMOAIEnvKeys[SFS_ENV_processorFreq],
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
		p_env->SetValue ( SFSMOAIEnvKeys[SFS_ENV_devUserName], nstring );

		compName = nstring;
	}
	RegCloseKey(hKey);


	// let's try to build a UUID!
	// @todo resolve conflict between kashmir's uuid and windows's uuid
	sprintf(guidseed, "%s %s %d", compName.c_str(), procModel.c_str(), memoryKB);
	//printf("guidseed: [%s]\n", guidseed);

	//kashmir::uuid::uuid_t systemuuid;// = uuid_t(guidseed);


	//p_env->SetValue ( SFSMOAIEnvKeys[SFS_ENV_guid], systemuuid.get() );
	

#endif
}

