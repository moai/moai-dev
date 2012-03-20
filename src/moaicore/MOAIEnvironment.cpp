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
// TODO: doxygen
int MOAIEnvironment::_setListener ( lua_State* L ) {
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );

	if ( idx < TOTAL ) {
		MOAIEnvironment::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}
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

	if ( !MOAIEnvironment::Get ().mAppDisplayName.empty ()) {
		lua_pushstring ( L, MOAIEnvironment::Get ().mAppDisplayName.c_str ());
	} else {
		lua_pushnil ( L );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getAppID
	@text	Returns the app ID

	@out	string appID
*/
int MOAIEnvironment::_getAppID ( lua_State* L  ) {

	if ( !MOAIEnvironment::Get ().mAppID.empty ()) {
		lua_pushstring ( L, MOAIEnvironment::Get ().mAppID.c_str ());
	} else {
		lua_pushnil ( L );
	}

	return 1;
}

//----------------------------------------------------------------//
/**	@name	getAppVersion
	@text	Returns the app version

	@out	string appVersion
*/
int MOAIEnvironment::_getAppVersion ( lua_State* L  ) {

	if ( !MOAIEnvironment::Get ().mAppVersion.empty ()) {
		lua_pushstring ( L, MOAIEnvironment::Get ().mAppVersion.c_str ());
	} else {
		lua_pushnil ( L );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getCacheDirectory
	@text	Returns cache directory

	@out	string cacheDirectory
*/
int MOAIEnvironment::_getCacheDirectory ( lua_State* L  ) {

	if ( !MOAIEnvironment::Get ().mCacheDirectory.empty ()) {
		lua_pushstring ( L, MOAIEnvironment::Get ().mCacheDirectory.c_str ());
	} else {
		lua_pushnil ( L );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getCarrierISOCountryCode
	@text	Returns the ISO country code for the device cellular service provider

	@out	string isoCode
*/
int MOAIEnvironment::_getCarrierISOCountryCode ( lua_State* L  ) {

	if ( !MOAIEnvironment::Get ().mCarrierISOCountryCode.empty ()) {
		lua_pushstring ( L, MOAIEnvironment::Get ().mCarrierISOCountryCode.c_str ());
	} else {
		lua_pushnil ( L );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getCarrierMobileCountryCode
	@text	Returns the mobile country code for the device cellular service provider

	@out	string mobileCountryCode
*/
int MOAIEnvironment::_getCarrierMobileCountryCode ( lua_State* L  ) {

	if ( !MOAIEnvironment::Get ().mCarrierMobileCountryCode.empty ()) {
		lua_pushstring ( L, MOAIEnvironment::Get ().mCarrierMobileCountryCode.c_str ());
	} else {
		lua_pushnil ( L );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getCarrierMobileNetworkCode
	@text	Returns the mobile network code for the device cellular service provider

	@out	string mobileNetworkCode
*/
int MOAIEnvironment::_getCarrierMobileNetworkCode ( lua_State* L  ) {

	if ( !MOAIEnvironment::Get ().mCarrierMobileNetworkCode.empty ()) {
		lua_pushstring ( L, MOAIEnvironment::Get ().mCarrierMobileNetworkCode.c_str ());
	} else {
		lua_pushnil ( L );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getCarrierName
	@text	Returns the name of the device cellular service provider

	@out	string name
*/
int MOAIEnvironment::_getCarrierName ( lua_State* L  ) {

	if ( !MOAIEnvironment::Get ().mCarrierName.empty ()) {
		lua_pushstring ( L, MOAIEnvironment::Get ().mCarrierName.c_str ());
	} else {
		lua_pushnil ( L );
	}
	
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

	if ( !MOAIEnvironment::Get ().mCountryCode.empty ()) {
		lua_pushstring ( L, MOAIEnvironment::Get ().mCountryCode.c_str ());
	} else {
		lua_pushnil ( L );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getCPUABI
	@text	Returns the CPU ABI

	@out	string ABI
*/
int MOAIEnvironment::_getCPUABI ( lua_State* L  ) {

	if ( !MOAIEnvironment::Get ().mCPUABI.empty ()) {
		lua_pushstring ( L, MOAIEnvironment::Get ().mCPUABI.c_str ());
	} else {
		lua_pushnil ( L );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getDevBrand
	@text	Returns the device brand

	@out	string brand
*/
int MOAIEnvironment::_getDevBrand ( lua_State* L  ) {

	if ( !MOAIEnvironment::Get ().mDevBrand.empty ()) {
		lua_pushstring ( L, MOAIEnvironment::Get ().mDevBrand.c_str ());
	} else {
		lua_pushnil ( L );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getDevName
	@text	Returns the device name

	@out	string name
*/
int MOAIEnvironment::_getDevName ( lua_State* L  ) {

	if ( !MOAIEnvironment::Get ().mDevName.empty ()) {
		lua_pushstring ( L, MOAIEnvironment::Get ().mDevName.c_str ());
	} else {
		lua_pushnil ( L );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getDevManufacturer
	@text	Returns the device manufacturer

	@out	string manufacturer
*/
int MOAIEnvironment::_getDevManufacturer ( lua_State* L  ) {

	if ( !MOAIEnvironment::Get ().mDevManufacturer.empty ()) {
		lua_pushstring ( L, MOAIEnvironment::Get ().mDevManufacturer.c_str ());
	} else {
		lua_pushnil ( L );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getDevModel
	@text	Returns the device model

	@out	string model
*/
int MOAIEnvironment::_getDevModel ( lua_State* L  ) {

	if ( !MOAIEnvironment::Get ().mDevModel.empty ()) {
		lua_pushstring ( L, MOAIEnvironment::Get ().mDevModel.c_str ());
	} else {
		lua_pushnil ( L );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getDevProduct
	@text	Returns the device design name

	@out	string product
*/
int MOAIEnvironment::_getDevProduct ( lua_State* L  ) {

	if ( !MOAIEnvironment::Get ().mDevProduct.empty ()) {
		lua_pushstring ( L, MOAIEnvironment::Get ().mDevProduct.c_str ());
	} else {
		lua_pushnil ( L );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getDocumentDirectory
	@text	Returns document directory

	@out	string documentDirectory
*/
int MOAIEnvironment::_getDocumentDirectory ( lua_State* L  ) {

	if ( !MOAIEnvironment::Get ().mDocumentDirectory.empty ()) {
		lua_pushstring ( L, MOAIEnvironment::Get ().mDocumentDirectory.c_str ());
	} else {
		lua_pushnil ( L );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getLanguageCode
	@text	Returns the language code

	@out	string languageCode
*/
int MOAIEnvironment::_getLanguageCode ( lua_State* L  ) {

	if ( !MOAIEnvironment::Get ().mLanguageCode.empty ()) {
		lua_pushstring ( L, MOAIEnvironment::Get ().mLanguageCode.c_str ());
	} else {
		lua_pushnil ( L );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getOSBrand
	@text	Returns the operating system brand

	@out	string brand
*/
int MOAIEnvironment::_getOSBrand ( lua_State* L  ) {

	u32 brand = OS_BRAND_UNAVAILABLE;
	if ( !MOAIEnvironment::Get ().mOSBrand.empty ()) {
		cc8* brandStr = MOAIEnvironment::Get ().mOSBrand.c_str ();
		if ( !strcmp ( brandStr, "Android" ))
			brand = OS_BRAND_ANDROID;
		else if ( !strcmp ( brandStr, "iOS" ))
			brand = OS_BRAND_IOS;
	}

	lua_pushinteger ( L, brand );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getOSVersion
	@text	Returns the operating system version

	@out	string version
*/
int MOAIEnvironment::_getOSVersion ( lua_State* L  ) {

	if ( !MOAIEnvironment::Get ().mOSVersion.empty ()) {
		lua_pushstring ( L, MOAIEnvironment::Get ().mOSVersion.c_str ());
	} else {
		lua_pushnil ( L );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getResourceDirectory
	@text	Returns resource directory

	@out	string resourceDirectory
*/
int MOAIEnvironment::_getResourceDirectory ( lua_State* L  ) {

	if ( !MOAIEnvironment::Get ().mResourceDirectory.empty ()) {
		lua_pushstring ( L, MOAIEnvironment::Get ().mResourceDirectory.c_str ());
	} else {
		lua_pushnil ( L );
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getScreenSize
	@text	Gets the dimensions of the device screen as two return values (width, height).

	@out	number width		The width of the device screen.
	@out	number height		The height of the device screen.
*/
int MOAIEnvironment::_getScreenSize ( lua_State* L ) {

	MOAIEnvironment& env = MOAIEnvironment::Get ();
	
	lua_pushnumber ( L, env.mScreenWidth );
	lua_pushnumber ( L, env.mScreenHeight );

	return 2;
}

//----------------------------------------------------------------//
/**	@name	getUDID
	@text	Returns the device unique identifier

	@out	string UDID
*/
int MOAIEnvironment::_getUDID ( lua_State* L  ) {

	if ( !MOAIEnvironment::Get ().mUDID.empty ()) {
		lua_pushstring ( L, MOAIEnvironment::Get ().mUDID.c_str ());
	} else {
		lua_pushnil ( L );
	}
	
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
	mConnectionType ( CONNECTION_TYPE_NONE ),
	mIsRetinaDisplay ( false ),
	mScreenWidth ( 0 ),
	mScreenHeight ( 0 ) {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIEnvironment::~MOAIEnvironment () {
}

//----------------------------------------------------------------//
void MOAIEnvironment::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "CONNECTIVITY_CHANGED", ( u32 )CONNECTIVITY_CHANGED );

	state.SetField ( -1, "CONNECTION_TYPE_NONE", ( u32 )CONNECTION_TYPE_NONE );
	state.SetField ( -1, "CONNECTION_TYPE_WIFI", ( u32 )CONNECTION_TYPE_WIFI );
	state.SetField ( -1, "CONNECTION_TYPE_WWAN", ( u32 )CONNECTION_TYPE_WWAN );
	
	state.SetField ( -1, "OS_BRAND_ANDROID", ( u32 )OS_BRAND_ANDROID );
	state.SetField ( -1, "OS_BRAND_IOS", ( u32 )OS_BRAND_IOS );
	state.SetField ( -1, "OS_BRAND_UNAVAILABLE", ( u32 )OS_BRAND_UNAVAILABLE );

	luaL_Reg regTable [] = {
		{ "setListener",					_setListener 					},
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
		{ "getScreenSize",					_getScreenSize					},
		{ "getUDID",						_getUDID						},
		{ "getViewSize",					_getViewSize					},
		{ "isRetinaDisplay",				_isRetinaDisplay				},
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetAppDisplayName ( cc8* appName ) {

	if ( appName != NULL ) {
		mAppDisplayName = appName;
	} else {
		mAppDisplayName.clear ();
	}
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetAppID ( cc8* appID ) {

	if ( appID != NULL ) {
		mAppID = appID;
	} else {
		mAppID.clear ();
	}
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetAppVersion ( cc8* appVer ) {

	if ( appVer != NULL ) {
		mAppVersion = appVer;
	} else {
		mAppVersion.clear ();
	}
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetCacheDirectory ( cc8* cacheDir ) {

	if ( cacheDir != NULL ) {
		mCacheDirectory = cacheDir;
	} else {
		mCacheDirectory.clear ();
	}
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetCarrierISOCountryCode ( cc8* isoCode ) {

	if ( isoCode != NULL ) {
		mCarrierISOCountryCode = isoCode;
	} else {
		mCarrierISOCountryCode.clear ();
	}
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetCarrierMobileCountryCode ( cc8* mobCountryCode ) {

	if ( mobCountryCode != NULL ) {
		mCarrierMobileCountryCode = mobCountryCode;
	} else {
		mCarrierMobileCountryCode.clear ();
	}
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetCarrierMobileNetworkCode ( cc8* mobNetworkCode ) {

	if ( mobNetworkCode != NULL ) {
		mCarrierMobileNetworkCode = mobNetworkCode;
	} else {
		mCarrierMobileNetworkCode.clear ();
	}
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetCarrierName ( cc8* name ) {

	if ( name != NULL ) {
		mCarrierName = name;
	} else {
		mCarrierName.clear ();
	}
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetConnectionType ( long connType ) {

	mConnectionType = connType;
	
	MOAILuaRef& callback = this->mListeners [ CONNECTIVITY_CHANGED ];

	if ( callback ) {
		MOAILuaStateHandle state = callback.GetSelf ();

		lua_pushnumber ( state, connType );

		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetCountryCode ( cc8* countryCode ) {

	if ( countryCode != NULL ) {
		mCountryCode = countryCode;
	} else {
		mCountryCode.clear ();
	}
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetCPUABI ( cc8* abi ) {

	if ( abi != NULL ) {
		mCPUABI = abi;
	} else {
		mCPUABI.clear ();
	}
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetDevBrand( cc8* devBrand ) {

	if ( devBrand != NULL ) {
		mDevBrand = devBrand;
	} else {
		mDevBrand.clear ();
	}
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetDevName ( cc8* devName ) {

	if ( devName != NULL ) {
		mDevName = devName;
	} else {
		mDevName.clear ();
	}
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetDevManufacturer ( cc8* devMan ) {

	if ( devMan != NULL ) {
		mDevManufacturer = devMan;
	} else {
		mDevManufacturer.clear ();
	}
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetDevModel ( cc8* devModel ) {

	if ( devModel != NULL ) {
		mDevModel = devModel;
	} else {
		mDevModel.clear ();
	}
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetDevProduct ( cc8* devProduct ) {

	if ( devProduct != NULL ) {
		mDevProduct = devProduct;
	} else {
		mDevProduct.clear ();
	}
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetDocumentDirectory ( cc8* docDir ) {

	if ( docDir != NULL ) {
		mDocumentDirectory = docDir;
	} else {
		mDocumentDirectory.clear ();
	}
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

	if ( langCode != NULL ) {
		mLanguageCode = langCode;
	} else {
		mLanguageCode.clear ();
	}
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetOSBrand ( cc8* osBrand ) {

	if ( osBrand != NULL ) {
		mOSBrand = osBrand;
	} else {
		mOSBrand.clear ();
	}
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetOSVersion ( cc8* osVer ) {

	if ( osVer != NULL ) {
		mOSVersion = osVer;
	} else {
		mOSVersion.clear ();
	}
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetResourceDirectory ( cc8* resDir ) {		

	if ( resDir != NULL ) {
		mResourceDirectory = resDir;
	} else {
		mResourceDirectory.clear ();
	}
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetUDID ( cc8* udid ) {

	if ( udid != NULL ) {
		mUDID = udid;
	} else {
		mUDID.clear ();
	}
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetScreenSize ( long width, long height ) {

	mScreenWidth = width;
	mScreenHeight = height;
}
