// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIEnvironment.h>
#include <moaicore/MOAIFileSystem.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAILogMgr.h>
#include <aku/AKU.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	generateGUID
	@text	Generates a globally unique identifier

	@out	string GUID
*/
int MOAIEnvironment::_generateGUID ( lua_State* L ) {

	lua_pushstring ( L, MOAIEnvironment::Get ().getGUIDfunc ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getAppDisplayName
	@text	Returns the app display name

	@out	string diaplyName
*/
int MOAIEnvironment::_getAppDisplayName ( lua_State* L  ) {

	lua_pushstring ( L, MOAIEnvironment::Get ().mAppDisplayName.c_str ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getAppID
	@text	Returns the app ID

	@out	string appID
*/
int MOAIEnvironment::_getAppID ( lua_State* L  ) {

	lua_pushstring ( L, MOAIEnvironment::Get ().mAppID.c_str ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getAppVersion
	@text	Returns the app version

	@out	string appVersion
*/
int MOAIEnvironment::_getAppVersion ( lua_State* L  ) {

	lua_pushstring ( L, MOAIEnvironment::Get ().mAppVersion.c_str ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getCacheDirectory
	@text	Returns cache directory

	@out	string cacheDirectory
*/
int MOAIEnvironment::_getCacheDirectory ( lua_State* L  ) {

	lua_pushstring ( L, MOAIEnvironment::Get ().mCacheDirectory.c_str ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getCarrierISOCountryCode
	@text	Returns the ISO country code for the device cellular service provider

	@out	string isoCode
*/
int MOAIEnvironment::_getCarrierISOCountryCode ( lua_State* L  ) {

	lua_pushstring ( L, MOAIEnvironment::Get ().mCarrierISOCountryCode.c_str ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getCarrierMobileCountryCode
	@text	Returns the mobile country code for the device cellular service provider

	@out	string mobileCountryCode
*/
int MOAIEnvironment::_getCarrierMobileCountryCode ( lua_State* L  ) {

	lua_pushstring ( L, MOAIEnvironment::Get ().mCarrierMobileCountryCode.c_str ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getCarrierMobileNetworkCode
	@text	Returns the mobile network code for the device cellular service provider

	@out	string mobileNetworkCode
*/
int MOAIEnvironment::_getCarrierMobileNetworkCode ( lua_State* L  ) {

	lua_pushstring ( L, MOAIEnvironment::Get ().mCarrierMobileNetworkCode.c_str ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getCarrierName
	@text	Returns the name of the device cellular service provider

	@out	string name
*/
int MOAIEnvironment::_getCarrierName ( lua_State* L  ) {

	lua_pushstring ( L, MOAIEnvironment::Get ().mCarrierName.c_str ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getConnectionType
	@text	Gets whether the device is connected to WIFI, WWAN or nothing

	@out	string connectionType
*/
int MOAIEnvironment::_getConnectionType ( lua_State* L ) {

	lua_pushinteger ( L, MOAIEnvironment::Get ().mConnectionType );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getCountryCode
	@text	Gets the country code of the current location
	
	@out	string countryCode
*/
int MOAIEnvironment::_getCountryCode ( lua_State* L ) {

	lua_pushstring ( L, MOAIEnvironment::Get ().mCountryCode.c_str ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getCPUABI
	@text	Returns the CPU ABI

	@out	string ABI
*/
int MOAIEnvironment::_getCPUABI ( lua_State* L  ) {

	lua_pushstring ( L, MOAIEnvironment::Get ().mCPUABI.c_str ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getDevBrand
	@text	Returns the device brand

	@out	string brand
*/
int MOAIEnvironment::_getDevBrand ( lua_State* L  ) {

	lua_pushstring ( L, MOAIEnvironment::Get ().mDevBrand.c_str ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getDevName
	@text	Returns the device name

	@out	string name
*/
int MOAIEnvironment::_getDevName ( lua_State* L  ) {

	lua_pushstring ( L, MOAIEnvironment::Get ().mDevName.c_str ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getDevManufacturer
	@text	Returns the device manufacturer

	@out	string manufacturer
*/
int MOAIEnvironment::_getDevManufacturer ( lua_State* L  ) {

	lua_pushstring ( L, MOAIEnvironment::Get ().mDevManufacturer.c_str ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getDevModel
	@text	Returns the device model

	@out	string model
*/
int MOAIEnvironment::_getDevModel ( lua_State* L  ) {

	lua_pushstring ( L, MOAIEnvironment::Get ().mDevModel.c_str ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getDevProduct
	@text	Returns the device design name

	@out	string product
*/
int MOAIEnvironment::_getDevProduct ( lua_State* L  ) {

	lua_pushstring ( L, MOAIEnvironment::Get ().mDevProduct.c_str ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getDocumentDirectory
	@text	Returns document directory

	@out	string documentDirectory
*/
int MOAIEnvironment::_getDocumentDirectory ( lua_State* L  ) {

	lua_pushstring ( L, MOAIEnvironment::Get ().mDocumentDirectory.c_str ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getLanguageCode
	@text	Returns the language code

	@out	string languageCode
*/
int MOAIEnvironment::_getLanguageCode ( lua_State* L  ) {

	lua_pushstring ( L, MOAIEnvironment::Get ().mLanguageCode.c_str ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getOSBrand
	@text	Returns the operating system brand

	@out	string brand
*/
int MOAIEnvironment::_getOSBrand ( lua_State* L  ) {

	cc8* brandStr = MOAIEnvironment::Get ().mOSBrand.c_str ();
	u32 brand;
	if ( !strcmp ( brandStr, "Android" ))
		brand = OS_BRAND_ANDROID;
	else if ( !strcmp ( brandStr, "iOS" ))
		brand = OS_BRAND_IOS;
	else
		brand = OS_BRAND_UNAVAILABLE;

	lua_pushinteger ( L, brand );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getOSVersion
	@text	Returns the operating system version

	@out	string version
*/
int MOAIEnvironment::_getOSVersion ( lua_State* L  ) {

	lua_pushstring ( L, MOAIEnvironment::Get ().mOSVersion.c_str ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getResourceDirectory
	@text	Returns resource directory

	@out	string resourceDirectory
*/
int MOAIEnvironment::_getResourceDirectory ( lua_State* L  ) {

	lua_pushstring ( L, MOAIEnvironment::Get ().mResourceDirectory.c_str ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getUDID
	@text	Returns the device unique identifier

	@out	string UDID
*/
int MOAIEnvironment::_getUDID ( lua_State* L  ) {

	lua_pushstring ( L, MOAIEnvironment::Get ().mUDID.c_str ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getViewSize
	@text	Returns the width and height of the view
	
	@out	int width
	@out	int height
*/
int MOAIEnvironment::_getViewSize ( lua_State* L  ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	lua_pushnumber ( L, gfxDevice.GetWidth ());
	lua_pushnumber ( L, gfxDevice.GetHeight ());
	
	return 2;
}

//----------------------------------------------------------------//
/**	@name	getDeviceSize
	@text	Gets the dimensions of the device screen as two return values (width, height).

	@out	number width		The width of the device screen.
	@out	number height		The height of the device screen.
*/
int MOAIEnvironment::_getDeviceSize ( lua_State* L ) {

	MOAIEnvironment& env = MOAIEnvironment::Get ();
	
	lua_pushnumber ( L, env.mScreenWidth );
	lua_pushnumber ( L, env.mScreenHeight );

	return 2;
}

//----------------------------------------------------------------//
/**	@name	isRetinaDisplay
	@text	Returns whether or not the device has a Retina Display 
	
	@out	bool isRetinaDisplay
*/
int MOAIEnvironment::_isRetinaDisplay ( lua_State* L  ) {

	lua_pushboolean ( L, MOAIEnvironment::Get ().mIsRetinaDisplay );
	return 1;
}

//================================================================//
// MOAIEnvironment
//================================================================//

//----------------------------------------------------------------//
long MOAIEnvironment::GetConnectivity () {
		
	return mConnectionType;
}

//----------------------------------------------------------------//
STLString MOAIEnvironment::GetResourceDirectory () {
		
	return mResourceDirectory;
}

//----------------------------------------------------------------//
MOAIEnvironment::MOAIEnvironment () :
	mAppDisplayName ( "UNKNOWN" ),
	mAppID ( "UNKNOWN" ),
	mAppVersion ( "UNKNOWN" ),
	mCacheDirectory ( "UNKNOWN" ),
	mCarrierISOCountryCode ( "UNKNOWN" ),
	mCarrierMobileCountryCode ( "UNKNOWN" ),
	mCarrierMobileNetworkCode ( "UNKNOWN" ),
	mCarrierName ( "UNKNOWN" ),
	mConnectionType ( CONNECTION_TYPE_NONE ),
	mCountryCode ( "UNKNOWN" ),
	mCPUABI ( "UNKNOWN" ),
	mDevBrand ( "UNKNOWN" ),
	mDevName ( "UNKNOWN" ),
	mDevManufacturer ( "UNKNOWN" ),
	mDevModel ( "UNKNOWN" ),
	mDevProduct ( "UNKNOWN" ),
	mDocumentDirectory ( "UNKNOWN" ),
	mIsRetinaDisplay ( false ),
	mLanguageCode ( "UNKNOWN" ),
	mOSBrand ( "UNKNOWN" ),
	mOSVersion ( "UNKNOWN" ),
	mResourceDirectory ( "UNKNOWN" ),
	mUDID ( "UNKNOWN" ),
	mScreenWidth ( 0 ),
	mScreenHeight ( 0 ) {

	RTTI_SINGLE ( USLuaObject )
}

//----------------------------------------------------------------//
MOAIEnvironment::~MOAIEnvironment () {
}

//----------------------------------------------------------------//
void MOAIEnvironment::RegisterLuaClass ( USLuaState& state ) {

	state.SetField ( -1, "CONNECTION_TYPE_NONE", ( u32 )CONNECTION_TYPE_NONE );
	state.SetField ( -1, "CONNECTION_TYPE_WIFI", ( u32 )CONNECTION_TYPE_WIFI );
	state.SetField ( -1, "CONNECTION_TYPE_WWAN", ( u32 )CONNECTION_TYPE_WWAN );
	
	state.SetField ( -1, "OS_BRAND_ANDROID", ( u32 )OS_BRAND_ANDROID );
	state.SetField ( -1, "OS_BRAND_IOS", ( u32 )OS_BRAND_IOS );
	state.SetField ( -1, "OS_BRAND_UNAVAILABLE", ( u32 )OS_BRAND_UNAVAILABLE );

	luaL_Reg regTable [] = {
		{ "generateGUID",					_generateGUID					},
		{ "getAppDisplayName",				_getAppDisplayName				},
		{ "getAppID",						_getAppID						},
		{ "getAppVersion",					_getAppVersion					},
		{ "getCacheDirectory",				_getCacheDirectory				},
		{ "getCarrierISOCountryCode",		_getCarrierISOCountryCode		},
		{ "getCarrierMobileCountryCode",	_getCarrierMobileCountryCode	},
		{ "getCarrierMobileNetworkCode",	_getCarrierMobileNetworkCode	},
		{ "getCarrierName",					_getCarrierName					},
		{ "getConnectionType",				_getConnectionType				},
		{ "getCountryCode",					_getCountryCode					},
		{ "getCPUABI",						_getCPUABI						},
		{ "getDevBrand",					_getDevBrand					},
		{ "getDevName",						_getDevName						},
		{ "getDevManufacturer",				_getDevManufacturer				},
		{ "getDevModel",					_getDevModel					},
		{ "getDevProduct",					_getDevProduct					},
		{ "getDocumentDirectory",			_getDocumentDirectory			},
		{ "getLanguageCode",				_getLanguageCode				},
		{ "getOSBrand",						_getOSBrand						},
		{ "getOSVersion",					_getOSVersion					},
		{ "getResourceDirectory",			_getResourceDirectory			},
		{ "getUDID",						_getUDID						},
		{ "getViewSize",					_getViewSize					},
		{ "getDeviceSize",					_getDeviceSize					},
		{ "isRetinaDisplay",				_isRetinaDisplay				},
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetAppDisplayName ( cc8* appName ) {
	mAppDisplayName = appName;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetAppID ( cc8* appID ) {
	mAppID = appID;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetAppVersion ( cc8* appVer ) {
	mAppVersion = appVer;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetCacheDirectory ( cc8* cacheDir ) {
	mCacheDirectory = cacheDir;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetCarrierISOCountryCode ( cc8* isoCode ) {
	mCarrierISOCountryCode = isoCode;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetCarrierMobileCountryCode ( cc8* mobCountryCode ) {
	mCarrierMobileCountryCode = mobCountryCode;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetCarrierMobileNetworkCode ( cc8* mobNetworkCode ) {
	mCarrierMobileNetworkCode = mobNetworkCode;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetCarrierName ( cc8* name ) {
	mCarrierName = name;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetConnectivityFunc ( long (*connFunc)(void) ) {
	getConnectivityFunc = connFunc;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetConnectionType ( long connType ) {
	mConnectionType = connType;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetCountryCode ( cc8* countryCode ) {
	mCountryCode = countryCode;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetCPUABI ( cc8* abi ) {
	mCPUABI = abi;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetDevBrand( cc8* devBrand ) {
	mDevBrand = devBrand;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetDevName ( cc8* devName ) {
	mDevName = devName;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetDevManufacturer ( cc8* devMan ) {
	mDevManufacturer = devMan;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetDevModel ( cc8* devModel ) {
	mDevModel = devModel;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetDevProduct ( cc8* devProduct ) {
	mDevProduct = devProduct;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetDocumentDirectory ( cc8* docDir ) {
	mDocumentDirectory = docDir;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetGUIDFunc ( cc8* (*guidFunc)(void) ) {
	getGUIDfunc = guidFunc;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetIsRetinaDisplay ( bool isRetina ) {
	mIsRetinaDisplay = isRetina;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetLanguageCode ( cc8* langCode ) {
	mLanguageCode = langCode;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetOSBrand ( cc8* osBrand ) {
	mOSBrand = osBrand;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetOSVersion ( cc8* osVer ) {
	mOSVersion = osVer;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetResourceDirectory ( cc8* resDir ) {		
	mResourceDirectory = resDir;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetUDID ( cc8* udid ) {
	mUDID = udid;
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetScreenSize ( long width, long height ) {
	mScreenWidth = width;
	mScreenHeight = height;
}
