// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDeviceInfo.h>
#include <moaicore/MOAILogMgr.h>
#include <aku/AKU.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	_generateUUID
	@text	Generates a uninversally unique identifier

	@out	string UUID
*/
int MOAIDeviceInfo::_generateUUID ( lua_State* L ) {

	lua_pushstring ( L, /*Get UUID */ "");
	return 1;
}

//----------------------------------------------------------------//
int MOAIDeviceInfo::_getAppDisplayName ( lua_State* L  ) {

	lua_pushstring ( L, MOAIDeviceInfo::Get ().mAppDisplayName.c_str ());
	return 1;
}

//----------------------------------------------------------------//
int MOAIDeviceInfo::_getAppID ( lua_State* L  ) {

	lua_pushstring ( L, MOAIDeviceInfo::Get ().mAppID.c_str ());
	return 1;
}

//----------------------------------------------------------------//
int MOAIDeviceInfo::_getAppVersion ( lua_State* L  ) {

	lua_pushstring ( L, MOAIDeviceInfo::Get ().mAppVersion.c_str ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	_getConnectionType
	@text	Gets whether the device is connected to WIFI, WWAN or nothing

	@out	string connectionType
*/
int MOAIDeviceInfo::_getConnectionType ( lua_State* L ) {

	lua_pushstring ( L, /*Get connection type */ "");
	return 1;
}

//----------------------------------------------------------------//
int MOAIDeviceInfo::_getCPUABI ( lua_State* L  ) {

	lua_pushstring ( L, MOAIDeviceInfo::Get ().mCPUABI.c_str ());
	return 1;
}

//----------------------------------------------------------------//
int MOAIDeviceInfo::_getDevBrand ( lua_State* L  ) {

	lua_pushstring ( L, MOAIDeviceInfo::Get ().mDevBrand.c_str ());
	return 1;
}

//----------------------------------------------------------------//
int MOAIDeviceInfo::_getDevName ( lua_State* L  ) {

	lua_pushstring ( L, MOAIDeviceInfo::Get ().mDevName.c_str ());
	return 1;
}

//----------------------------------------------------------------//
int MOAIDeviceInfo::_getDevManufacturer ( lua_State* L  ) {

	lua_pushstring ( L, MOAIDeviceInfo::Get ().mDevManufacturer.c_str ());
	return 1;
}

//----------------------------------------------------------------//
int MOAIDeviceInfo::_getDevModel ( lua_State* L  ) {

	lua_pushstring ( L, MOAIDeviceInfo::Get ().mDevModel.c_str ());
	return 1;
}

//----------------------------------------------------------------//
int MOAIDeviceInfo::_getDevProduct ( lua_State* L  ) {

	lua_pushstring ( L, MOAIDeviceInfo::Get ().mDevProduct.c_str ());
	return 1;
}

//----------------------------------------------------------------//
int MOAIDeviceInfo::_getOSBrand ( lua_State* L  ) {

	lua_pushstring ( L, MOAIDeviceInfo::Get ().mOSBrand.c_str ());
	return 1;
}

//----------------------------------------------------------------//
int MOAIDeviceInfo::_getOSVersion ( lua_State* L  ) {

	lua_pushstring ( L, MOAIDeviceInfo::Get ().mOSVersion.c_str ());
	return 1;
}

//----------------------------------------------------------------//
int MOAIDeviceInfo::_getUDID ( lua_State* L  ) {

	lua_pushstring ( L, MOAIDeviceInfo::Get ().mUDID.c_str ());
	return 1;
}

//================================================================//
// MOAIDeviceInfo
//================================================================//

//----------------------------------------------------------------//
MOAIDeviceInfo::MOAIDeviceInfo () :
	mAppDisplayName ( "UNKNOWN" ),
	mAppID ( "UNKNOWN" ),
	mAppVersion ( "UNKNOWN" ),
	mCPUABI ( "UNKNOWN" ),
	mDevBrand ( "UNKNOWN" ),
	mDevName ( "UNKNOWN" ),
	mDevManufacturer ( "UNKNOWN" ),
	mDevModel ( "UNKNOWN" ),
	mDevProduct ( "UNKNOWN" ),
	mOSBrand ( "UNKNOWN" ),
	mOSVersion ( "UNKNOWN" ),
	mUDID ( "UNKNOWN" ) {

	RTTI_SINGLE ( USLuaObject )
}

//----------------------------------------------------------------//
MOAIDeviceInfo::~MOAIDeviceInfo () {
}

//----------------------------------------------------------------//
void MOAIDeviceInfo::RegisterLuaClass ( USLuaState& state ) {

	luaL_Reg regTable [] = {
		{ "generateUUID",			_generateUUID		 },
		{ "getAppDisplayName",		_getAppDisplayName	 },
		{ "getAppID",				_getAppID			 },
		{ "getAppVersion",			_getAppVersion		 },
		{ "getConnectionType",		_getConnectionType 	 },
		{ "getCPUABI",				_getCPUABI			 },
		{ "getDevBrand",			_getDevBrand		 },
		{ "getDevName",				_getDevName			 },
		{ "getDevManufacturer",		_getDevManufacturer	 },
		{ "getDevModel",			_getDevModel		 },
		{ "getDevProduct",			_getDevProduct		 },
		{ "getOSBrand",				_getOSBrand			 },
		{ "getOSVersion",			_getOSVersion		 },
		{ "getUDID",				_getUDID			 },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIDeviceInfo::SetAppDisplayName ( cc8* appName ) {
	mAppDisplayName = appName;
}

//----------------------------------------------------------------//
void MOAIDeviceInfo::SetAppID ( cc8* appID ) {
	mAppID = appID;
}

//----------------------------------------------------------------//
void MOAIDeviceInfo::SetAppVersion ( cc8* appVer ) {
	mAppVersion = appVer;
}

//----------------------------------------------------------------//
void MOAIDeviceInfo::SetCPUABI ( cc8* abi ) {
	mCPUABI = abi;
}

//----------------------------------------------------------------//
void MOAIDeviceInfo::SetDevBrand( cc8* devBrand ) {
	mDevBrand = devBrand;
}

//----------------------------------------------------------------//
void MOAIDeviceInfo::SetDevName ( cc8* devName ) {
	mDevName = devName;
}

//----------------------------------------------------------------//
void MOAIDeviceInfo::SetDevManufacturer ( cc8* devMan ) {
	mDevManufacturer = devMan;
}

//----------------------------------------------------------------//
void MOAIDeviceInfo::SetDevModel ( cc8* devModel ) {
	mDevModel = devModel;
}

//----------------------------------------------------------------//
void MOAIDeviceInfo::SetDevProduct ( cc8* devProduct ) {
	mDevProduct = devProduct;
}

//----------------------------------------------------------------//
void MOAIDeviceInfo::SetOSBrand ( cc8* osBrand ) {
	mOSBrand = osBrand;
}

//----------------------------------------------------------------//
void MOAIDeviceInfo::SetOSVersion ( cc8* osVer ) {
	mOSVersion = osVer;
}

//----------------------------------------------------------------//
void MOAIDeviceInfo::SetUDID ( cc8* udid ) {
	mUDID = udid;
}

//----------------------------------------------------------------//
STLString MOAIDeviceInfo::ToString () {

	STLString info = "";
	info += mAppDisplayName += "\n";
	info += mAppID += "\n";
	info += mAppVersion += "\n";
	info += mCPUABI += "\n";
	info += mDevBrand += "\n";
	info += mDevName += "\n";
	info += mDevManufacturer += "\n";
	info += mDevModel += "\n";
	info += mDevProduct += "\n";
	info += mOSBrand += "\n";
	info += mOSVersion += "\n";
	info += mUDID += "\n";

	return info;
}
