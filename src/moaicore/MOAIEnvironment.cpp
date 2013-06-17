// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIEnvironment.h>
#include <moaicore/MOAILogMgr.h>
#include <aku/AKU.h>





//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	generateGUID
	@text	Generates a globally unique identifier. This method will be
			moved to MOAIUnique in a future release.

	@out	string GUID
*/
int MOAIEnvironment::_generateGUID ( lua_State* L ) {

	STLString guid = USUnique::GetGUID ();
	lua_pushstring ( L, guid );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getMACAddress
	@text	Finds and returns the primary MAC Address
 
	@out	string MAC
*/

int MOAIEnvironment::_getMACAddress ( lua_State* L ) {
	
	STLString macAddress = USAdapterInfo::GetMACAddress ();
	lua_pushstring ( L, macAddress );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setValue
	@text	Sets an evironment value and also triggers the listener
			callback (if any).

	@in		string key
	@opt	variant value		Default value is nil.
	@out	nil
*/
int MOAIEnvironment::_setValue ( lua_State* L ) {
	MOAILuaState state ( L );
	
	if ( state.IsType ( 1, LUA_TSTRING )) {
	
		MOAIEnvironment& environment = MOAIEnvironment::Get ();
		environment.SetValue ( state );
	}
	return 0;
}

//================================================================//
// MOAIEnvironment
//================================================================//

//----------------------------------------------------------------//
void MOAIEnvironment::DetectEnvironment () {

	RTTI_SINGLE ( MOAIGlobalEventSource )
	
	#if defined( MOAI_OS_WINDOWS )
	
		this->SetValue ( MOAI_ENV_osBrand, "Windows" );
		
		UUID uuid;
		UuidCreateSequential ( &uuid );
		
		// For now, we'll just use the MAC address which is the last 6 bytes of the uuid.
		char buf[13];
		sprintf ( buf, "%02X%02X%02X%02X%02X%02X", uuid.Data4[2], uuid.Data4[3], uuid.Data4[4], uuid.Data4[5], uuid.Data4[6], uuid.Data4[7]);
		this->SetValue ( MOAI_ENV_udid, buf );
		
		
		const int BUFSIZE = 256;
		TCHAR pszOS[BUFSIZE];

		OSVERSIONINFOEX osvi;
		SYSTEM_INFO si;
		PGNSI pGNSI;				

		ZeroMemory(&si, sizeof(SYSTEM_INFO));
		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
		GetVersionEx((OSVERSIONINFO*) &osvi);
		
		pGNSI = (PGNSI) GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetNativeSystemInfo");
		if(NULL != pGNSI) {
			pGNSI(&si);
		}
		else {
			GetSystemInfo(&si);
		}
		
		// MOAI_ENV_documentDirectory
		// --------------------------------------------
		char path[MAX_PATH];
		SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, path);
		this->SetValue ( MOAI_ENV_documentDirectory, path );

		// MOAI_ENV_cacheDirectory
		// --------------------------------------------
		char cachePath[MAX_PATH];
		sprintf(cachePath, "%s/../cache", path);

		// @todo	create dir if doesn't exist

		this->SetValue ( MOAI_ENV_cacheDirectory, path );


		// MOAI_ENV_cpuabi
		// --------------------------------------------
		switch (si.wProcessorArchitecture)
		{
		case PROCESSOR_ARCHITECTURE_AMD64:
			this->SetValue(MOAI_ENV_cpuabi, "x64");
			break;
		case PROCESSOR_ARCHITECTURE_INTEL:
			this->SetValue(MOAI_ENV_cpuabi, "x86");
			break;
		default:
			break;
		}

		// MOAI_ENV_devManufacturer, MOAI_ENV_devModel
		// --------------------------------------------
		HKEY hKey;
		LONG lRes = RegOpenKeyExW(
			HKEY_LOCAL_MACHINE,
			L"HARDWARE\\DESCRIPTION\\System\\BIOS",
			0,
			KEY_READ,
			&hKey
		);
		if (lRes == ERROR_SUCCESS)
		{
			w32_updateEnvFromRegKeyStr(
				this,
				MOAI_ENV_devManufacturer,
				hKey,
				L"SystemManufacturer"
			);
			w32_updateEnvFromRegKeyStr(
				this,
				MOAI_ENV_devModel,
				hKey,
				L"SystemSKU",
				L"SystemProductName"
				);
		}
		RegCloseKey(hKey);

		// MOAI_ENV_devUserName
		// --------------------------------------------
		lRes = RegOpenKeyExW(
			HKEY_LOCAL_MACHINE,
			L"SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ComputerName",
			0,
			KEY_READ,
			&hKey
			);
		if (lRes == ERROR_SUCCESS)
		{
			w32_updateEnvFromRegKeyStr(
				this,
				MOAI_ENV_devUserName,
				hKey,
				L"ComputerName"
				);
		}
		RegCloseKey(hKey);

		// MOAI_ENV_osVersion
		// --------------------------------------------
		if ( VER_PLATFORM_WIN32_NT==osvi.dwPlatformId && osvi.dwMajorVersion > 4 ) {
		
			strcpy ( pszOS, TEXT ( "Win" ));			
			if ( osvi.dwMajorVersion == 6 ) {
				if ( osvi.dwMinorVersion == 1 ) {
					if( osvi.wProductType == VER_NT_WORKSTATION )
						strcat(pszOS, TEXT("7"));
					else strcat(pszOS, TEXT("2008R2" ));
				} else if ( osvi.dwMinorVersion == 2 ) {
					if( osvi.wProductType == VER_NT_WORKSTATION )
						strcat(pszOS, TEXT("8"));
					else strcat(pszOS, TEXT("2012" ));
				}
				else if( osvi.dwMinorVersion == 0 ) {
					if( osvi.wProductType == VER_NT_WORKSTATION )
						strcat(pszOS, TEXT("Vista"));
					else strcat(pszOS, TEXT("Server2008" ));
				}
			}
			else if ( osvi.dwMajorVersion == 5 ) {
				if (osvi.dwMinorVersion == 2) {				
					if( osvi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64) {
						strcat(pszOS, TEXT( "XPx64"));
					}
					else {
						strcat(pszOS, TEXT("Server2003"));
					}
				}
				else if ( osvi.dwMinorVersion == 1 ) {
					strcat(pszOS, TEXT("XP"));					
				}
				else if ( osvi.dwMinorVersion == 0 ) {
					strcat(pszOS, TEXT("2000"));
				}
			}
			
			this->SetValue ( MOAI_ENV_osVersion, pszOS );
		}
		else {
			this->SetValue ( MOAI_ENV_osVersion, "WinUnknown" );
		}
		
	#elif defined( MOAI_OS_LINUX )
	
		this->SetValue ( MOAI_ENV_osBrand, "Linux" );

	#elif defined ( MOAI_OS_OSX )
	
		this->SetValue ( MOAI_ENV_osBrand, "OSX" );
	  #if 0 /* doesn't compile yet */
		// OS Version
		SInt32 majorVersion,minorVersion,bugFixVersion;

		Gestalt(gestaltSystemVersionMajor, &majorVersion);
		Gestalt(gestaltSystemVersionMinor, &minorVersion);
		Gestalt(gestaltSystemVersionBugFix, &bugFixVersion);

		char buffer[256];
		sprintf(buffer, "%d.%d.%d",majorVersion,minorVersion,bugFixVersion);
		this->SetValue ( MOAI_ENV_osVersion, buffer );
	  #endif
	#endif
}

//----------------------------------------------------------------//
MOAIEnvironment::MOAIEnvironment () {
}

//----------------------------------------------------------------//
MOAIEnvironment::~MOAIEnvironment () {
}

//----------------------------------------------------------------//
void MOAIEnvironment::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIGlobalEventSource::RegisterLuaClass ( state );

	state.SetField ( -1, "EVENT_VALUE_CHANGED", ( u32 )EVENT_VALUE_CHANGED );

	state.SetField ( -1, "CONNECTION_TYPE_NONE", ( u32 )CONNECTION_TYPE_NONE );
	state.SetField ( -1, "CONNECTION_TYPE_WIFI", ( u32 )CONNECTION_TYPE_WIFI );
	state.SetField ( -1, "CONNECTION_TYPE_WWAN", ( u32 )CONNECTION_TYPE_WWAN );
	
	state.SetField ( -1, "OS_BRAND_ANDROID", ( u32 )OS_BRAND_ANDROID );
	state.SetField ( -1, "OS_BRAND_IOS", ( u32 )OS_BRAND_IOS );
	state.SetField ( -1, "OS_BRAND_OSX", ( u32 )OS_BRAND_OSX );
	state.SetField ( -1, "OS_BRAND_LINUX", ( u32 )OS_BRAND_LINUX );
	state.SetField ( -1, "OS_BRAND_WINDOWS", ( u32 )OS_BRAND_WINDOWS );
	state.SetField ( -1, "OS_BRAND_UNAVAILABLE", ( u32 )OS_BRAND_UNAVAILABLE );

	luaL_Reg regTable [] = {
		{ "generateGUID",		_generateGUID },
		{ "getMACAddress",		_getMACAddress },
		{ "setListener",		&MOAIGlobalEventSource::_setListener < MOAIEnvironment > },
		{ "setValue",			_setValue },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetValue ( cc8* key ) {

	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	this->PushLuaClassTable ( state );
	state.ClearField ( -1, key );
	state.Pop ( 1 );
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetValue ( lua_State* L ) {

	MOAILuaState state ( L );

	int top = state.GetTop ();

	this->PushLuaClassTable ( state );
		
	state.CopyToTop ( -3 ); // key
	state.CopyToTop ( -3 ); // value
	
	lua_settable ( state, -3 );
	state.Pop ( 1 );
	
	if ( this->PushListener ( EVENT_VALUE_CHANGED, state )) {
	
		state.CopyToTop ( -3 ); // key
		state.CopyToTop ( -3 ); // value
		
		state.DebugCall ( 2, 0 );
	}
	
	top = state.GetTop ();
}

void w32_updateEnvFromRegKeyStr(MOAIEnvironment* p_env, const char* p_moaikey, const HKEY& p_hkey, const WCHAR* p_valname, const WCHAR* p_valname_fallback)
{
	WCHAR szBuffer[512];
	DWORD dwBufferSize = sizeof(szBuffer);
	
	ULONG nError;
	nError = RegQueryValueExW(
		p_hkey,
		p_valname,
		0,
		NULL,
		(LPBYTE)szBuffer,
		&dwBufferSize
		);
	printf("w32_updateEnvFromRegKeyStr[%s]\n", p_moaikey);
	if (ERROR_SUCCESS == nError)
	{
		//strValue = szBuffer;
		size_t origsize = wcslen(szBuffer) + 1;
		const size_t newsize = 513;
		size_t convertedChars = 0;
		char nstring[newsize];
		wcstombs_s(&convertedChars, nstring, origsize, szBuffer, _TRUNCATE);

		printf("nstr[%s]\n", nstring);

		if(strlen(nstring) == 0 && p_valname_fallback != NULL)
		{
			// use the fallback
			w32_updateEnvFromRegKeyStr(p_env, p_moaikey, p_hkey, p_valname_fallback, NULL);
		} else {
			// set the value
			p_env->SetValue ( p_moaikey, nstring );	
			return;// ret;
		}
	}

	return;// ret;
}
void w32_updateEnvFromRegKeyDword(MOAIEnvironment* p_env, const char* p_moaikey, const HKEY& p_hkey, const WCHAR* p_valname)
{
	WCHAR szBuffer[4];
	DWORD dwBufferSize = sizeof(szBuffer);

	ULONG nError;
	nError = RegQueryValueExW(
		p_hkey,
		p_valname,
		0,
		NULL,
		(LPBYTE)&szBuffer,
		&dwBufferSize
		);
	//printf("w32_updateEnvFromRegKeyDword[%s]\n", p_moaikey);
	if (ERROR_SUCCESS == nError)
	{
		char tempstr[100];
		sprintf(tempstr, "%d", (long)szBuffer[0]);
		p_env->SetValue ( p_moaikey, tempstr );
	}

	return; //(long)szBuffer[0];
}